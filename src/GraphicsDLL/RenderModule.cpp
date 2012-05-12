/**
 * \file
 * \author Ricky Curtice
 * \date 20110717
 * \brief Defines the class methods for the render engine core
 *
 * <p>
 * </p>
 *
 */

#include <cassert>

#include <windows.h>
#include <d3d9.h>
#include <boost/foreach.hpp>

#include "RenderModule.h"
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/PropertyMap.h"
#include "../sharedbase/Entity.h"
#include "../sharedbase/MessageRouter.h"
#include "../sharedbase/Envelope.h"

#include "component/RenderableComponent.h"
#include "component/CameraComponent.h"

#include "InterfaceUpdateable.h"

#include "GlobalPropertyNames.h"
#include "Messages.h"
#include "Types.h"
#include "../sharedbase/QuatMath.h"

#ifndef _WIN32
#define sprintf_s sprintf
#endif

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Create the static data
	/* * * * * * * * * * * * * * * * * * * * * */
	RenderModule* RenderModule::currentRenderModule;
	
	/* * * * * * * * * * * * * * * * * * * * * */
	// Create the local consts and globals
	/* * * * * * * * * * * * * * * * * * * * * */
	const D3DXVECTOR3 UP_DIRECTION = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	const D3DFILLMODE DEFAULT_FILLMODE = D3DFILL_SOLID;
	const float DEFAULT_RENDER_DELAY = 1.0f / 30.0f;
	
	const Color BLACK = Color(0.0f, 0.0f, 0.0f, 1.0f);
	const Color WHITE = Color(1.0f, 1.0f, 1.0f, 1.0f);
	
	float AspectRatio, AspectRatioInv;

	RenderModule::RenderModule(PropertyMap* gprops, MessageRouter* msgrouter) :
			ModuleInterface(gprops, msgrouter) {
		assert(this->gprops != nullptr);
		assert(this->msgrouter != nullptr);
		
		assert(RenderModule::GetRenderModule() == nullptr); // Enforce the one-instance-only rule of a singleton, but leave the class publicly initializable.
		RenderModule::SetRenderModule(this);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Create the global properties
		/* * * * * * * * * * * * * * * * * * * * * */
		// Screen size
		this->gprops->SetProperty(GP_WINDOW_WIDTH, GP_WINDOW_WIDTH_DEF);
		this->gprops->SetProperty(GP_WINDOW_HEIGHT, GP_WINDOW_HEIGHT_DEF);
		
		this->gprops->SetProperty(GP_CLIENT_WIDTH, GP_CLIENT_WIDTH_DEF);
		this->gprops->SetProperty(GP_CLIENT_HEIGHT, GP_CLIENT_HEIGHT_DEF);
		this->gprops->SetProperty(GP_CLIENT_ASPECT, GP_CLIENT_ASPECT_DEF);
		this->gprops->SetProperty(GP_CLIENT_ASPECT_INV, 1.0f / this->gprops->GetPropertyValue<float>(GP_CLIENT_ASPECT));
		
		// Clear color
		this->gprops->SetProperty(GP_GRAPHICS_PAUSE, GP_GRAPHICS_PAUSE_DEF);
		
		// Clear color
		this->gprops->SetProperty(GP_CLEAR_COLOR, GP_CLEAR_COLOR_DEF);
		
		// Anisotropic filtering
		this->gprops->SetProperty(GP_ANISOTROPIC_FILTERING, GP_ANISOTROPIC_FILTERING_DEF);
		
		// Fog
		this->gprops->SetProperty(GP_FOG_ENABLED, GP_FOG_ENABLED_DEF);
		this->gprops->SetProperty(GP_FOG_COLOR, GP_FOG_COLOR_DEF);
		this->gprops->SetProperty(GP_FOG_MODE, GP_FOG_MODE_DEF);
		this->gprops->SetProperty(GP_FOG_RANGE_NEAR, GP_FOG_RANGE_NEAR_DEF);
		this->gprops->SetProperty(GP_FOG_RANGE_FAR, GP_FOG_RANGE_FAR_DEF);
		this->gprops->SetProperty(GP_FOG_RANGE_DENSITY, GP_FOG_RANGE_DENSITY_DEF);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Define the local globals
		/* * * * * * * * * * * * * * * * * * * * * */
		AspectRatio = this->gprops->GetPropertyValue<float>(GP_CLIENT_ASPECT);
		AspectRatioInv = this->gprops->GetPropertyValue<float>(GP_CLIENT_ASPECT_INV);
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Define the instance properties
		/* * * * * * * * * * * * * * * * * * * * * */
		// DX
		this->pD3D = nullptr;
		this->pd3dDevice = nullptr;
		
		// Camera
		this->currentCamera = nullptr;
		this->currentCameraName.assign("");
		
		// Render
		this->fillMode = DEFAULT_FILLMODE;
		this->renderDelay = DEFAULT_RENDER_DELAY;
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Register the event handlers
		/* * * * * * * * * * * * * * * * * * * * * */
		this->RegisterListeners();
		
		
		/* * * * * * * * * * * * * * * * * * * * * */
		// Notify loading success
		/* * * * * * * * * * * * * * * * * * * * * */
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_LOAD_GRAPHICS;
			this->msgrouter->SendSP(e);
		}
	}

	RenderModule::~RenderModule(void) {
		assert(this->myRenderableComponents.empty());
		assert(this->cameraComponents.empty());
		assert(this->renderableComponents.empty()); // At this stage, all renderable components should have been cleared via their destructors.
		// If this last assert fired, then some other core did not destroy the RenderableComponents it instantiated before this destructor was called.
		
		// The below should be null if this object is being destroyed, via the Shutdown method.
		assert(this->pD3D == nullptr);
		assert(this->pd3dDevice == nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::RegisterComponent(RenderableComponent* renderableComponent) {
		std::pair<std::set<RenderableComponent*>::iterator, bool> result;
		
		assert(renderableComponent != nullptr);
		
		Threading::WriteLock w_lock(this->renderableComponentsMutex);
		
		result = this->renderableComponents.insert(renderableComponent);

		return result.second; // the second value is true if, and only if, the element was inserted.
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::UnregisterComponent(RenderableComponent* renderableComponent) {
		int num_removed;

		assert(renderableComponent != nullptr);
		
		Threading::WriteLock w_lock(this->renderableComponentsMutex);
		
		num_removed = this->renderableComponents.erase(renderableComponent); // This will fail on RenderModule shutdown, but by then it doesn't make a difference.

		return (num_removed > 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::RegisterUpdateable(InterfaceUpdateable* updateableObject) {
		std::pair<std::set<InterfaceUpdateable*>::iterator, bool> result;
		
		assert(updateableObject != nullptr);
		
		Threading::WriteLock w_lock(this->updateableObjectsMutex);
		
		result = this->updateableObjects.insert(updateableObject);
		
		return result.second; // the second value is true if, and only if, the element was inserted.
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::UnregisterUpdateable(InterfaceUpdateable* updateableObject) {
		int num_removed;
		
		assert(updateableObject != nullptr);
		
		Threading::WriteLock w_lock(this->updateableObjectsMutex);
		
		num_removed = this->updateableObjects.erase(updateableObject); // This will fail on RenderModule shutdown, but by then it doesn't make a difference.
		
		return (num_removed > 0);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void RenderModule::Render() {
		static D3DXMATRIX matrixProj, matrixView, matrixOrtho, matrix;
		static D3DXVECTOR3 cameraLookAt, cameraPosition;
		
		if (this->pd3dDevice == nullptr) { // Did you make sure Startup was called?
			return;
		}

		// Set fill mode
		this->pd3dDevice->SetRenderState(D3DRS_FILLMODE, this->fillMode);
		
		// Enable lighting
		this->pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
		this->pd3dDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		
		// Activate depth buffer
		this->pd3dDevice->SetRenderState(D3DRS_ZENABLE, true);
		this->pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
		
		// Activate texture filtering on texture 0
		// *TODO: Make the texture levels configurable via script - probably via some form of advanced script-controllable shader system.
		{
			unsigned int aniso_level = this->gprops->GetPropertyValue<unsigned int>(GP_ANISOTROPIC_FILTERING);
			if (aniso_level <= 0) {
				aniso_level = 1; // 1 means disabled, 0 is invalid.
			}
			
			this->pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
			this->pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
			this->pd3dDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, aniso_level);
		}
		
		// Activate alpha-blending
		{
			this->pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			//D3DTSS_COLORARG1 is set to D3DTA_TEXTURE which means that colour1 is
			//entirely taken from the texture and nothing else.
			this->pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			
			//The next 2 RenderStates determine how the blending is done.
			//The source is our object, destination is the drawing surface.
			//SRCBLEND specifies that the final color should be 50% (because
			//that's what we set our source alpha to) from our source and 50%
			//(INVSRCALPHA, 100%-SRCALPHA(50% in this case))
			this->pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			this->pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//This enables Alpha Blending
			this->pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		}
		
		// Set up fog
		{
			bool fog_enabled = this->gprops->GetPropertyValue<bool>(GP_FOG_ENABLED);
			
			if (fog_enabled) {
				unsigned long fog_color = (unsigned long) this->gprops->GetPropertyValue<Color>(GP_FOG_COLOR);
				unsigned int fog_mode = this->gprops->GetPropertyValue<unsigned int>(GP_FOG_MODE);
				float fog_range_near = this->gprops->GetPropertyValue<float>(GP_FOG_RANGE_NEAR);
				float fog_range_far = this->gprops->GetPropertyValue<float>(GP_FOG_RANGE_FAR);
				float fog_range_density = this->gprops->GetPropertyValue<float>(GP_FOG_RANGE_DENSITY);
				
				// Enable fog blending.
				this->pd3dDevice->SetRenderState(D3DRS_FOGENABLE, true);
		 
				// Set the fog color.
				this->pd3dDevice->SetRenderState(D3DRS_FOGCOLOR, fog_color);
				
				// Set fog parameters.
				this->pd3dDevice->SetRenderState(D3DRS_FOGTABLEMODE, fog_mode);
				
				if (fog_mode == D3DFOG_LINEAR) {
					this->pd3dDevice->SetRenderState(D3DRS_FOGSTART, *(unsigned long *)(&fog_range_near));
					this->pd3dDevice->SetRenderState(D3DRS_FOGEND,   *(unsigned long *)(&fog_range_far));
				}
				else {
					this->pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(unsigned long *)(&fog_range_density));
				}
			}
		}
		
		{ // Clear the backbuffer to the specified color
			Color clear_color = this->gprops->GetPropertyValue<Color>(GP_CLEAR_COLOR);
			
			this->pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_color, 1.0f, 0);
		}
		
		// Open a scene for rendering
		if (SUCCEEDED(this->pd3dDevice->BeginScene())) {
			Threading::ReadLock r_lock(this->currentCameraMutex);
			
			// Verify active camera, no rendering perspective-space objects otherwise.
			if (this->currentCamera) {
				/* * * * * * * * * * * * * * * * * */
				// Set the perspective camera in the 3D world
				/* * * * * * * * * * * * * * * * * */

				// Clear any ambient light left-overs.
				this->pd3dDevice->SetRenderState(D3DRS_AMBIENT, BLACK);
				
				float fovy = this->currentCamera->GetFov() * AspectRatioInv; // calculate the vertical FOV from the horizontal FOV stored in the camera.

				// Set the projection transformation
				D3DXMatrixIdentity(&matrixProj);
				D3DXMatrixPerspectiveFovLH(&matrixProj, fovy, AspectRatio, this->currentCamera->GetNearClipRange(), this->currentCamera->GetFarClipRange());
				this->pd3dDevice->SetTransform(D3DTS_PROJECTION, &matrixProj);
				
				// Set the view transformation
				D3DXMatrixIdentity(&matrixView);
				
				// Calculate the camera's world position
				cameraPosition = this->currentCamera->GetOffset(); // Get and rotate the offset
				RotateVectorByQuaternion(&cameraPosition, &cameraPosition, &(this->currentCamera->GetOwner()->GetWorldRotation()));
				cameraPosition += this->currentCamera->GetOwner()->GetWorldPosition();
				
				// Calculate a lookat based on rotation of camera and a unit vector
				cameraLookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
				RotateVectorByQuaternion(&cameraLookAt, &cameraLookAt, &(this->currentCamera->GetRotation() * this->currentCamera->GetOwner()->GetWorldRotation()));
				cameraLookAt += cameraPosition; // the look at position is naturally relative to the camera, so add the camera's position to adjust to world coordinates.

				D3DXMatrixLookAtLH(
					&matrixView,
					&cameraPosition, // Camera Position
					&cameraLookAt, // Look At Position
					&UP_DIRECTION // Up Direction
				);
				this->pd3dDevice->SetTransform(D3DTS_VIEW, &matrixView);
				
				// Render the 3D world space
				
				// Clear the matrices
				D3DXMatrixIdentity(&matrix);
				
				// Set the world transformation
				this->pd3dDevice->SetTransform(D3DTS_WORLD, &matrix);
				
				this->RenderObjectInSpace(CS_WORLD);
			}

			{
				/* * * * * * * * * * * * * * * * * */
				// Set the orthographic camera for the 2D world of the HUD
				/* * * * * * * * * * * * * * * * * */
				
				// Set up the ambient light for the HUD that makes everything visible without having to resort to emissive surfaces.
				this->pd3dDevice->SetRenderState(D3DRS_AMBIENT, WHITE);
				
				// Clear the fog
				this->pd3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
				
				// Clear the Z Buffer so that world objects don't clip through screen objects.
				this->pd3dDevice->Clear(0, nullptr, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
				
				// Clear the matrices
				D3DXMatrixIdentity(&matrixOrtho);
				D3DXMatrixIdentity(&matrixView);
				D3DXMatrixIdentity(&matrix);
				
				// Camera is facing the X-Y plane, such that 
				//  <0,0,0> is in the upper left of the window and
				//  X=1 is at the right-most position of the screen.
				D3DXMatrixOrthoOffCenterLH(&matrixOrtho, 0.0f, 1.0f, -AspectRatioInv, 0.0f, -100.0f, 100.0f);
				this->pd3dDevice->SetTransform(D3DTS_PROJECTION, &matrixOrtho);
				
				// Clear the view matrix
				this->pd3dDevice->SetTransform(D3DTS_VIEW, &matrixView);
				
				// Set the world transformation
				this->pd3dDevice->SetTransform(D3DTS_WORLD, &matrix);
				
				// Render the 2D HUD space
				this->RenderObjectInSpace(CS_SCREEN);
			}
			
			this->pd3dDevice->EndScene();
		}
		
		// Present the backbuffer contents to the display
		this->pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void RenderModule::RenderObjectInSpace(ECoordinateSpace space) {
		static D3DXMATRIX matrix;
		static const D3DMATRIX UNIT_MATRIX = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};

		float scale;
		
		Threading::ReadLock r_lock(this->renderableComponentsMutex);
		
		BOOST_FOREACH(RenderableComponent * component, this->renderableComponents) {
			if (component->GetCoordSpace() == space) {
				EntitySPTR owner(component->GetOwner());
				assert(owner.get() != nullptr); // There MUST be an owning entity for any component.
				
				scale = owner->GetWorldScale();
				if (fabs(scale) <= 0.000001f) { // 0 (or way too close to it) is not a valid scale.  Negative is fine though!
					LOG(LOG_PRIORITY::ERR, "Entity '" + owner->GetName() + "' has an invalid calculated 'world' scale: scale must be non-zero and positive!");
					continue;
				}
				
				D3DXMatrixTransformation(&matrix, nullptr, nullptr, &D3DXVECTOR3(scale, scale, scale), nullptr, &(owner->GetWorldRotation()), &(owner->GetWorldPosition()));
				
				this->pd3dDevice->SetTransform(D3DTS_WORLD, &matrix);
				
				this->pd3dDevice->SetMaterial(&BLANK_MATERIAL);
				this->pd3dDevice->SetTexture(0, nullptr);
				this->pd3dDevice->SetTransform(D3DTS_TEXTURE0, &UNIT_MATRIX);
				
				component->Render(this->pd3dDevice);
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Update is treated as a render call in this class
	// Called each game update with change in time (dt) in seconds since last update
	void RenderModule::Update(double dt) {
		this->deltaAccumulator += dt;
		
		if (this->deltaAccumulator >= this->renderDelay) {
			if (!this->gprops->GetPropertyValue<bool>(GP_GRAPHICS_PAUSE)) {
				Threading::WriteLock w_lock(this->updateableObjectsMutex);
				
				BOOST_FOREACH(InterfaceUpdateable* updateable, this->updateableObjects) {
					updateable->Update(this->deltaAccumulator);
				}
			}

			this->Render();
			this->deltaAccumulator = 0.0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::Resize() {
		// Function contents in part copied from provided code from previous coursework.
		
		unsigned int clientWidth, clientHeight;
		HWND hwnd;
		
		// Get hwnd
		if (!this->gprops->PropertyExists(GP_HWND)) {
			LOG(LOG_PRIORITY::ERR, "No hwnd global property found!");
			return false;
		}
		try {
			hwnd = boost::any_cast < HWND > (this->gprops->GetProperty(GP_HWND));
		}
		catch (const boost::bad_any_cast & e) {
			e; // ignore the "unreferenced local variable" warning by faking its use here.
			LOG(LOG_PRIORITY::CONFIG, "hwnd global property found, but of wrong type!");
			return false;
		}

		// Get screen width and height from the hwnd, and store in gprops
		RECT clientArea;
		GetClientRect(hwnd, &clientArea);
		
		clientWidth = clientArea.right - clientArea.left;
		clientHeight = clientArea.bottom - clientArea.top;
		
		this->gprops->SetProperty(GP_CLIENT_WIDTH, clientWidth);
		this->gprops->SetProperty(GP_CLIENT_HEIGHT, clientHeight);
		
		// Set the presentation parameters
		ZeroMemory(&this->d3dpp, sizeof(this->d3dpp));
		this->d3dpp.Windowed = TRUE;
		this->d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		this->d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		this->d3dpp.BackBufferCount = 1;
		this->d3dpp.BackBufferHeight = clientHeight;
		this->d3dpp.BackBufferWidth = clientWidth;
		this->d3dpp.hDeviceWindow = hwnd;
		this->d3dpp.EnableAutoDepthStencil = TRUE;
		this->d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		// Initialize the D3D device
		if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &this->d3dpp, &pd3dDevice))) {
			LOG(LOG_PRIORITY::SYSERR, "Unable to create the Direct3D device!");
			return false;
		}

		// Disable lighting
		pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
		
		return true;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Called to initialize the core
	void RenderModule::Startup(EnvelopeSPTR envelope) {
		bool status = false;
		
		// Init DX
		if (nullptr == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) {
			LOG(LOG_PRIORITY::SYSERR, "Unable to create Direct3D object!");
			return;
		}

		// Set the buffer size
		status = this->Resize();
		
		// Zero the accumulator
		this->deltaAccumulator = 0.0f;
		
		// Un-register from the startup message, as there's no need to start again.
		{
			std::shared_ptr<Subscriber> handler(new Subscriber(boost::bind(&RenderModule::Startup, this, _1)));
			this->msgrouter->Unsubscribe(MSG_START_GRAPHICS, handler);
		}
		
		// Send notification that the module is started.
		{
			EnvelopeSPTR e(new Envelope);
			e->msgid = ACK_START_GRAPHICS;
			e->AddData(status);
			this->msgrouter->SendSP(e);
		}
		
		LOG(LOG_PRIORITY::INFO, "GraphicsDX9 started.");
		
		return;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Called to shutdown the core
	void RenderModule::Shutdown(EnvelopeSPTR envelope) {
		{
			// Delete the cameras
			Threading::WriteLock w_lock(this->cameraComponentsMutex);
			
			for (
			cameramap::left_const_iterator it = this->cameraComponents.left.begin();
					it != this->cameraComponents.left.end();
					++it
					) {
				CameraComponent* cam = (*it).second;
				delete cam;
			}
			this->cameraComponents.clear(); // note that this doesn't delete pointer-based objects on its own, so the above does the job.
		}
		
		{
			// Delete the renderable components that were instantiated locally.
			// After all, if I didn't instantiate it, then deleting it would be a violation of RAII.
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			BOOST_FOREACH(RenderableComponent * comp, this->myRenderableComponents) {
				delete comp;
			}
			this->myRenderableComponents.clear();
		}
		
		//this->renderableComponents // now contains only pointers to objects owned by other people. (Mine were cleared via their destructors by the above)
		
		{
			// Invalidate the pointer to the active camera
			Threading::WriteLock w_lock(this->currentCameraMutex);
			
			this->currentCamera = nullptr;
		}
		
		if (pd3dDevice != nullptr) {
			pd3dDevice->Release();
			pd3dDevice = nullptr;
		}
		
		if (pD3D != nullptr) {
			pD3D->Release();
			pD3D = nullptr;
		}
	}
}
