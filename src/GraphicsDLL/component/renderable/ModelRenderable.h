/**
 * \file
 * \author Justin Avignone
 * \date 20110715
 * \brief Provides the interface class declaration for methods and objects for getting and setting model components
 */
#pragma once

#include "../RenderableComponent.h"

#include <d3d9.h>
#include <d3dx9tex.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

#include "../../material/MaterialsSystemInterface.h"
#include "../../texture/TexturesSystemInterface.h"
#include "../../material/MaterialInterface.h"
#include "../../texture/TextureInterface.h"
#include "../../Types.h"

namespace GraphicsCore {
	struct MeshData {
		MeshData(const LPD3DXMESH& mesh, const unsigned long& materialCount, const LPD3DXBUFFER& materialBuffer) :
			mesh(mesh),
			materialCount(materialCount),
			materialBuffer(materialBuffer)
			{
		}
		
		~MeshData() {
			this->materialBuffer->Release(); // Don't need it any more, so get rid of it.
		}
		
		LPD3DXMESH mesh;
		unsigned long materialCount;
		LPD3DXBUFFER materialBuffer;
	};
	
	typedef std::map<std::string, std::shared_ptr<MeshData>> MeshCacheMap;
	typedef MeshCacheMap::value_type MeshCacheMapValue;
}

namespace GraphicsCore {

	class ModelComponent: public RenderableComponent, public MaterialsSystemInterface, public TexturesSystemInterface {
	public:
		ModelComponent(EntitySPTR, ModuleInterface*);
		virtual ~ModelComponent();

		bool LoadMeshFromFile(const std::string&);

	private: // Overridden/implemented methods of parent classes
		void Render(LPDIRECT3DDEVICE9);

		bool InsertMaterial(unsigned int, EnvelopeSPTR);
		bool RemoveMaterial(unsigned int);

		bool InsertTexture(unsigned int, EnvelopeSPTR);
		bool RemoveTexture(unsigned int);

	private:
		static MeshCacheMap MeshCache;
		
	private:
		void SetMaterials(LPD3DXBUFFER materialBuffer);

		LPD3DXMESH mesh;
		unsigned long materialCount;
		std::vector<MaterialInterface*> meshMaterials;
		std::vector<TextureInterface*> meshTextures;

	};
}

