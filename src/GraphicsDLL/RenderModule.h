/**
 * \file
 * \author Ricky Curtice
 * \date 20110717
 * \brief Provides the class declaration for the render engine core
 *
 * <p>
 * </p>
 *
 */
#pragma once

#include <d3d9.h>
#include <d3dx9tex.h>
#include <set>
#include <string>
#include <boost/function.hpp>
#include <boost/bimap/bimap.hpp>
#include <threading.h>

#include "../sharedbase/ModuleInterface.h"
#include "../sharedbase/Envelope_fwd.h"

class MessageRouter;
extern "C"
{
	__declspec(dllexport) ModuleInterface* ModuleFactory(PropertyMap*, MessageRouter*, EntityList*, EventLogger*);
}

/// Namespace to contain every item that is related to the rendering engine.  The name itself is a legacy, as is a lot of the code contain by it, from the Saturn V engine that built as part of Adam and Ricky's Engine class.
namespace GraphicsCore {
	class RenderableComponent;
	class CameraComponent;
	class MaterialsSystemInterface;
	class TexturesSystemInterface;
	class InterfaceUpdateable;
	enum ECoordinateSpace;
}

namespace GraphicsCore {
	typedef boost::bimaps::bimap<std::string, CameraComponent*> cameramap;
	typedef cameramap::value_type namedcamera;
}

namespace GraphicsCore {
	class RenderModule: public ModuleInterface {
	public:
		static RenderModule* GetRenderModule() {
			return RenderModule::currentRenderModule;
			// *TODO: remove the need for this from RenderableComponent (the complex auto-registration no longer needs to exist, it can be implmented during the Create event)
		}

		RenderModule(PropertyMap* gprops, MessageRouter* msgrouter);
		~RenderModule(void); // NOTE: Do NOT inherit from: the class is not designed for inheritance.

		bool RegisterComponent(RenderableComponent* renderableComponent);
		bool UnregisterComponent(RenderableComponent* renderableComponent);

		bool RegisterUpdateable(InterfaceUpdateable*);
		bool UnregisterUpdateable(InterfaceUpdateable*);

		void Update(double dt);
		void Startup(EnvelopeSPTR);
		void Shutdown(EnvelopeSPTR);
		void CreateComponent(std::string, std::map<std::string, boost::any>&, EntitySPTR);
		WHO_DELETES::TYPE RemoveComponent(ComponentInterface*);
		
		MessageRouter* GetMessageRouter() {
			return this->msgrouter;
		}

		/**
		 * Loads a texture from the specified file into the texture pointer.  Supported file
		 * types are: .bmp, .jpg, .png, and .tga
		 *
		 * \param file Input: The path and file name for a supported image file
		 * \param texture Output: The resulting texture
		 * \return whether or not the texture loaded successfully
		 */
		HRESULT TextureFactory(const std::string& file, LPDIRECT3DTEXTURE9 *texture);

		/**
		 * Loads a mesh from the specified X-format mesh file.
		 * Parameters are duplicates of the parameters for D3DXCreateMeshFromX(...)
		 *
		 * \param file Input: The path and file name for an X-format mesh
		 * \param options Input:
		 * \param ppAdjacency Output:
		 * \param ppMaterials Output:
		 * \param ppEffectInstances Output:
		 * \param pNumMaterials Output:
		 * \param ppMesh Output:
		 * \return whether or not the mesh loaded successfully
		 */
		HRESULT MeshFactory(const std::string& file, DWORD options, LPD3DXBUFFER* ppAdjacency, LPD3DXBUFFER* ppMaterials, LPD3DXBUFFER* ppEffectInstances, DWORD* pNumMaterials, LPD3DXMESH* ppMesh);

	private: // Member methods
		static void SetRenderModule(RenderModule* engine) {
			RenderModule::currentRenderModule = engine;
		}

		bool Resize();

		void Render();
		void RenderObjectInSpace(ECoordinateSpace);

		void RegisterListeners();

		bool SetCurrentCamera(const std::string&);
		bool SetFillMode(D3DFILLMODE);
		bool SetRenderDelay(float);
		bool SetRenderRate(float);
		
		bool CreateMaterial(EnvelopeSPTR, MaterialsSystemInterface*) const;
		bool CreateTexture(EnvelopeSPTR, TexturesSystemInterface*) const;

		// Message Handlers
		EnvelopeSPTR HandleSetCurrentCamera(EnvelopeSPTR);
		EnvelopeSPTR HandleSetFillMode(EnvelopeSPTR);
		EnvelopeSPTR HandleSetRenderDelay(EnvelopeSPTR);
		EnvelopeSPTR HandleSetRenderRate(EnvelopeSPTR);
		EnvelopeSPTR HandleGetRenderDelay(EnvelopeSPTR) const;
		EnvelopeSPTR HandleGetRenderRate(EnvelopeSPTR) const;


	private: // Member data
		static RenderModule* currentRenderModule;

		double deltaAccumulator; ///< Accumulator for the change in time between each call to update

		mutable Threading::ReadWriteMutex renderableComponentsMutex;
		std::set<RenderableComponent*> renderableComponents;
		
		mutable Threading::ReadWriteMutex myRenderableComponentsMutex;
		std::set<RenderableComponent*> myRenderableComponents;
		
		mutable Threading::ReadWriteMutex cameraComponentsMutex;
		cameramap cameraComponents;
		
		mutable Threading::ReadWriteMutex updateableObjectsMutex;
		std::set<InterfaceUpdateable*> updateableObjects;

		mutable Threading::ReadWriteMutex currentCameraMutex;
		CameraComponent* currentCamera;
		std::string currentCameraName;

		D3DFILLMODE fillMode;
		float renderDelay;

		LPDIRECT3D9 pD3D;
		LPDIRECT3DDEVICE9 pd3dDevice;
		D3DPRESENT_PARAMETERS d3dpp;
	};
}
