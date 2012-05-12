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

#include <boost/function.hpp>

#include "RenderModule.h"
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/Entity.h"

#include "component/RenderableComponent.h"
#include "component/CameraComponent.h"
#include "component/renderable/HeightmapRenderable.h"
#include "component/renderable/LightAmbientRenderable.h"
#include "component/renderable/LightDirectionalRenderable.h"
#include "component/renderable/LightPointRenderable.h"
#include "component/renderable/ModelRenderable.h"
#include "component/renderable/RenderableHelicalTube.h"
#include "component/renderable/CubeRenderable.h"
#include "component/renderable/TiledMap.h"
#include "component/renderable/PointLine.h"
#include "component/renderable/Quad.h"

#include "Components.h"
#include "component/renderable/Sprite.h"
#include "component/renderable/D3DFont.h"

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	 * For standard, single-parameter setters, provides a quick and easy way to reduce an immense number of lines of code in CreateComponent by centralizing all common operations.
	 */
	template<typename T>
	inline void getAttribute(const std::string type, std::map<std::string, boost::any> &attributes, const std::string key, boost::function<bool(T)> setter) {
		bool status = false;
		
		if (attributes.count(key) > 0) {
			try {
				status = setter(boost::any_cast < T > (attributes[key]));
			}
			catch (const boost::bad_any_cast &) {
				LOG(LOG_PRIORITY::INFO, type + " " + key + " set with invalid data type.");
			}
			
			if (!status) {
				LOG(LOG_PRIORITY::INFO, "Setter for " +  type + " " + key + " failed to set. (returned false)");
			}
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	/**
	 * For non-standard setters, provides a relatively quick and easy way to reduce an immense number of lines of code in CreateComponent by centralizing all common operations.
	 */
	template<typename T>
	inline std::pair<bool, T> getAttributeAlt(const std::string type, std::map<std::string, boost::any> &attributes, const std::string key) {
		bool status = false;
		T result;
		if (attributes.count(key) > 0) {
			try {
				result = boost::any_cast < T > (attributes[key]);
				status = true;
			}
			catch (const boost::bad_any_cast &) {
				LOG(LOG_PRIORITY::INFO, type + " " + key + " set with invalid data type.");
			}
		}
		return std::pair<bool, T>(status, result);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	// Called to create a component of type (type), with the set of attributes (attributes), and with parent entity (e)
	void RenderModule::CreateComponent(std::string type, std::map<std::string, boost::any> &attributes, EntitySPTR ent) {
		EntitySPTR entity(ent);
		
		if (type == CAMERA && attributes.count(CAMERA__NAME)) {
			CameraComponent* cam = new CameraComponent(entity, this);
			
			getAttribute < CAMERA__FOV_TYPE > (
			    type, attributes, CAMERA__FOV, std::bind1st(std::mem_fun(&CameraComponent::SetFov), cam)
			    );
			
			// Have to use the less-easy method for this, as it doesn'tuse a singular setter.
			if (attributes.count(CAMERA__NEAR_CLIP)) {
				bool status = false;
				std::pair<bool, CAMERA__NEAR_CLIP_TYPE> data;
				
				if ((data = getAttributeAlt < CAMERA__NEAR_CLIP_TYPE > (type, attributes, CAMERA__NEAR_CLIP)).first) {
					status = cam->SetClipRanges(data.second, cam->GetFarClipRange());
				}
				
				if (!status) {
					std::string errMsg("Setter for ");
					LOG(LOG_PRIORITY::ERR, errMsg.append(type).append(" ").append(CAMERA__NEAR_CLIP).append(" failed to set. (returned false)"));
				}
			}

			// Have to use the less-easy method for this, as it doesn'tuse a singular setter.
			if (attributes.count(CAMERA__FAR_CLIP)) {
				bool status = false;
				std::pair<bool, CAMERA__FAR_CLIP_TYPE> data;
				
				if ((data = getAttributeAlt < CAMERA__FAR_CLIP_TYPE > (type, attributes, CAMERA__FAR_CLIP)).first) {
					status = cam->SetClipRanges(cam->GetNearClipRange(), data.second);
				}
				
				if (!status) {
					std::string errMsg("Setter for ");
					LOG(LOG_PRIORITY::ERR, errMsg.append(type).append(" ").append(CAMERA__NEAR_CLIP).append(" failed to set. (returned false)"));
				}
			}

			getAttribute < CAMERA__OFFSET_TYPE > (
			    type, attributes, CAMERA__OFFSET, std::bind1st(std::mem_fun(&CameraComponent::SetOffset), cam)
			    );
			
			getAttribute < CAMERA__ROTATION_TYPE > (
			    type, attributes, CAMERA__ROTATION, std::bind1st(std::mem_fun(&CameraComponent::SetRotation), cam)
			    );
			
			std::string camera_name("");
			try {
				camera_name = boost::any_cast < std::string > (attributes[CAMERA__NAME]);
			}
			catch (const boost::bad_any_cast &) {
				std::string errStr("Invalid data type sent for Camera creation.  Camera name was '");
				errStr.append(CAMERA__NAME).append("'.");
				LOG(LOG_PRIORITY::ERR, errStr);
			}
			
			Threading::UpgradeLock up_lock(this->cameraComponentsMutex);
			
			cameramap::left_const_iterator cam_it = this->cameraComponents.left.find(camera_name);
			
			if (camera_name.length() >= 0 && cam_it == this->cameraComponents.left.end()) {
				{
					Threading::UpgradeToUniqueLock unique_lock(up_lock);
					
					this->cameraComponents.insert(namedcamera(camera_name, cam));
				}
				// Register the first created camera as the active camera
				if (this->cameraComponents.size() <= 1) {
					this->SetCurrentCamera(camera_name);
				}
			}
			else {
				std::string errStr("Invalid name sent for Camera creation.  Camera name was '");
				errStr.append(CAMERA__NAME).append("' and was either already in-use or blank.");
				LOG(LOG_PRIORITY::ERR, errStr);
			}
		}
		else if (type == LIGHT_AMBIENT) {
			LightAmbientRenderable* light = new LightAmbientRenderable(entity, this);
			
			// Pull in color information from attributes
			getAttribute < LIGHT__AMBIENT_COLOR_TYPE > (
			    type, attributes, LIGHT__AMBIENT_COLOR, std::bind1st(std::mem_fun(&LightAmbientRenderable::SetAmbientColor), light)
			    );
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(light);
		}
		else if (type == LIGHT_DIRECTIONAL) {
			LightDirectionalRenderable* light = new LightDirectionalRenderable(entity, this);
			
			// Pull in color information and range from attributes
			getAttribute < LIGHT__AMBIENT_COLOR_TYPE > (
			    type, attributes, LIGHT__AMBIENT_COLOR, std::bind1st(std::mem_fun(&LightDirectionalRenderable::SetAmbientColor), light)
			    );
			
			getAttribute < LIGHT__DIFFUSE_COLOR_TYPE > (
			    type, attributes, LIGHT__DIFFUSE_COLOR, std::bind1st(std::mem_fun(&LightDirectionalRenderable::SetDiffuseColor), light)
			    );
			
			getAttribute < LIGHT__SPECULAR_COLOR_TYPE > (
			    type, attributes, LIGHT__SPECULAR_COLOR, std::bind1st(std::mem_fun(&LightDirectionalRenderable::SetSpecularColor), light)
			    );
			
			getAttribute < LIGHT__RANGE_TYPE > (
			    type, attributes, LIGHT__RANGE, std::bind1st(std::mem_fun(&LightDirectionalRenderable::SetRange), light)
			    );
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(light);
		}
		else if (type == LIGHT_POINT) {
			LightPointRenderable* light = new LightPointRenderable(entity, this);
			
			// Pull in color information and range from attributes
			getAttribute < LIGHT__AMBIENT_COLOR_TYPE > (
			    type, attributes, LIGHT__AMBIENT_COLOR, std::bind1st(std::mem_fun(&LightPointRenderable::SetAmbientColor), light)
			    );
			
			getAttribute < LIGHT__DIFFUSE_COLOR_TYPE > (
			    type, attributes, LIGHT__DIFFUSE_COLOR, std::bind1st(std::mem_fun(&LightPointRenderable::SetDiffuseColor), light)
			    );
			
			getAttribute < LIGHT__SPECULAR_COLOR_TYPE > (
			    type, attributes, LIGHT__SPECULAR_COLOR, std::bind1st(std::mem_fun(&LightPointRenderable::SetSpecularColor), light)
			    );
			
			getAttribute < LIGHT__RANGE_TYPE > (
			    type, attributes, LIGHT__RANGE, std::bind1st(std::mem_fun(&LightPointRenderable::SetRange), light)
			    );
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(light);
		}
		else if (type == HEIGHTMAP) {
			HeightmapRenderable* heightmap = new HeightmapRenderable(entity, this, pd3dDevice);
			
			// Pull in terrain heightmap and texture from attributes
			getAttribute < HEIGHTMAP__FILE_TYPE > (
			    type, attributes, HEIGHTMAP__FILE, boost::bind(&HeightmapRenderable::LoadHeightmapFromFile, boost::ref(heightmap), _1)
			    );
			
			getAttribute < HEIGHTMAP__TEXTURE_FILE_TYPE > (
			    type, attributes, HEIGHTMAP__TEXTURE_FILE, boost::bind(&HeightmapRenderable::LoadTextureFromFile, boost::ref(heightmap), _1)
			    );
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(heightmap);
		}
		else if (type == MESH) {
			ModelComponent* model = new ModelComponent(entity, this);
			
			// Pull in the mesh file
			getAttribute < MESH__FILE_TYPE > (
			    type, attributes, MESH__FILE, boost::bind(&ModelComponent::LoadMeshFromFile, boost::ref(model), _1)
			    );
			
			getAttribute < RENDER__SPACE_TYPE > (
			    type, attributes, RENDER__SPACE, boost::bind(&ModelComponent::SetCoordSpace, boost::ref(model), _1)
			    );
			
			// Go fetch any incoming materials
			{
				std::pair<bool, MATERIALS__DATA_TYPE> data;
				if ((data = getAttributeAlt < MATERIALS__DATA_TYPE > (type, attributes, MATERIALS__DATA)).first) {
					if (!this->CreateMaterial(data.second, model)) {
						LOG(LOG_PRIORITY::INFO, type + " " + MATERIALS__DATA + " set with invalid data type.");
					}
				}
			}
			
			// Go fetch any incoming textures
			{
				std::pair<bool, TEXTURES__DATA_TYPE> data;
				if ((data = getAttributeAlt < TEXTURES__DATA_TYPE > (type, attributes, TEXTURES__DATA)).first) {
					if (!this->CreateTexture(data.second, model)) {
						LOG(LOG_PRIORITY::INFO, type + " " + TEXTURES__DATA + " set with invalid data type.");
					}
				}
			}
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(model);
		}
		else if (type == HELICAL_TUBE) {
			RenderableHelicalTube* model = new RenderableHelicalTube(entity, this, pd3dDevice);
			
			// Loadup the attributes
			getAttribute < HELICAL_TUBE__RADIUS_TYPE > (
			    type, attributes, HELICAL_TUBE__RADIUS, std::bind1st(std::mem_fun(&RenderableHelicalTube::SetRadius), model)
			    );
			
			getAttribute < HELICAL_TUBE__LENGTH_TYPE > (
			    type, attributes, HELICAL_TUBE__LENGTH, std::bind1st(std::mem_fun(&RenderableHelicalTube::SetLength), model)
			    );
			
			getAttribute < HELICAL_TUBE__PITCH_TYPE > (
			    type, attributes, HELICAL_TUBE__PITCH, std::bind1st(std::mem_fun(&RenderableHelicalTube::SetPitch), model)
			    );
			
			getAttribute < HELICAL_TUBE__SEGMENTS_TYPE > (
			    type, attributes, HELICAL_TUBE__SEGMENTS, std::bind1st(std::mem_fun(&RenderableHelicalTube::SetSegmentCount), model)
			    );
			
			// Go fetch any incoming materials
			{
				std::pair<bool, MATERIALS__DATA_TYPE> data;
				if ((data = getAttributeAlt < MATERIALS__DATA_TYPE > (type, attributes, MATERIALS__DATA)).first) {
					if (!this->CreateMaterial(data.second, model)) {
						LOG(LOG_PRIORITY::INFO, type + " " + MATERIALS__DATA + " set with invalid data type.");
					}
				}
			}
			
			// Go fetch any incoming textures
			{
				std::pair<bool, TEXTURES__DATA_TYPE> data;
				if ((data = getAttributeAlt < TEXTURES__DATA_TYPE > (type, attributes, TEXTURES__DATA)).first) {
					if (!this->CreateTexture(data.second, model)) {
						LOG(LOG_PRIORITY::INFO, type + " " + TEXTURES__DATA + " set with invalid data type.");
					}
				}
			}
			
			model->UpdateGeometry();
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(model);
		}
		else if (type == TILED_MAP) {
			TiledMap* tmap = new TiledMap(entity, this, pd3dDevice);

			getAttribute < EnvelopeSPTR > (
				type, attributes, "mapdata", std::bind1st(std::mem_fun(&TiledMap::LoadMapData), tmap)
				);

			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(tmap);
		}
		else if (type == POINTLINE) {
			PointLine* pline = new PointLine(entity, this, pd3dDevice);
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(pline);
		}
		else if (type == SPRITE) {
			Sprite* spr = new Sprite(entity, this, pd3dDevice);

			spr->SetSpriteParams(
					boost::any_cast<float>(attributes["offsetX"]),
					boost::any_cast<float>(attributes["offsetY"]),
					boost::any_cast<std::string>(attributes["texfilename"]),
					boost::any_cast<unsigned int>(attributes["srcX"]),
					boost::any_cast<unsigned int>(attributes["srcY"]),
					boost::any_cast<unsigned int>(attributes["width"]),
					boost::any_cast<unsigned int>(attributes["height"])
				);

			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(spr);
		}
		else if (type == D3DFONT) {
			D3DFont* font = new D3DFont(entity, this, pd3dDevice, this->msgrouter, boost::any_cast<unsigned int>(attributes["width"]), boost::any_cast<unsigned int>(attributes["height"]), boost::any_cast<unsigned int>(attributes["weight"]),  boost::any_cast<bool>(attributes["italic"]));
			font->SetDialogRect(boost::any_cast<unsigned int>(attributes["rectX"]), boost::any_cast<unsigned int>(attributes["rectY"]), boost::any_cast<unsigned int>(attributes["rectW"]), boost::any_cast<unsigned int>(attributes["rectH"]));

			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);

			this->myRenderableComponents.insert(font);
		}
		else if (type == QUAD) {
			Quad* model = new Quad(entity, this, pd3dDevice);
			
			getAttribute < RENDER__SPACE_TYPE > (
			    type, attributes, RENDER__SPACE, std::bind1st(std::mem_fun(&Quad::SetCoordSpace), model)
			    );
			
			getAttribute < QUAD__WIDTH_TYPE > (
			    type, attributes, QUAD__WIDTH, std::bind1st(std::mem_fun(&Quad::SetWidth), model)
			    );
			
			getAttribute < QUAD__HEIGHT_TYPE > (
			    type, attributes, QUAD__HEIGHT, std::bind1st(std::mem_fun(&Quad::SetHeight), model)
			    );
			
			// *HACK: As there is no nice method for attaching the component's enums to script, I'm implementing a uint to enum converter that will be a simple stopgap.
			if (attributes.count(QUAD__ORIGIN)) {
				bool status = false;
				std::pair<bool, QUAD__ORIGIN_TYPE> data;
				
				if ((data = getAttributeAlt < QUAD__ORIGIN_TYPE > (type, attributes, QUAD__ORIGIN)).first) {
					switch (data.second) {
						case 1:
							status = model->SetOrigin(ORIGIN::UPPER_LEFT);
						break;
						case 2:
							status = model->SetOrigin(ORIGIN::CENTER);
						break;
						default:
							LOG(LOG_PRIORITY::ERR, "Setter for " + type + " " + QUAD__ORIGIN + " was not called: Invalid value " + boost::lexical_cast<std::string>(data.second) + " was passed.  Valid values are: 1 = upper-left, 2 = center.");
						break;
					}
					
				}
				
				if (!status) {
					LOG(LOG_PRIORITY::ERR, "Setter for " + type + " " + QUAD__ORIGIN + " failed to set. (returned false)");
				}
			}
			
			// *HACK: As there is no nice method for attaching the component's enums to script, I'm implementing a uint to enum converter that will be a simple stopgap.
			if (attributes.count(QUAD__ORIENT)) {
				bool status = false;
				std::pair<bool, QUAD__ORIENT_TYPE> data;
				
				if ((data = getAttributeAlt < QUAD__ORIENT_TYPE > (type, attributes, QUAD__ORIENT)).first) {
					switch (data.second) {
						case 1:
							status = model->SetOrientation(ORIENTATION::WORLD);
						break;
						case 2:
							status = model->SetOrientation(ORIENTATION::CAMERA);
						break;
						default:
							LOG(LOG_PRIORITY::ERR, "Setter for " + type + " " + QUAD__ORIENT + " was not called: Invalid value " + boost::lexical_cast<std::string>(data.second) + " was passed.  Valid values are: 1 = world, 2 = face camera.");
						break;
					}
					
				}
				
				if (!status) {
					LOG(LOG_PRIORITY::ERR, "Setter for " + type + " " + QUAD__ORIENT + " failed to set. (returned false)");
				}
			}
			
			// Go fetch any incoming materials
			{
				std::pair<bool, MATERIALS__DATA_TYPE> data;
				if ((data = getAttributeAlt < MATERIALS__DATA_TYPE > (type, attributes, MATERIALS__DATA)).first) {
					if (!this->CreateMaterial(data.second, model)) {
						LOG(LOG_PRIORITY::WARN, type + " " + MATERIALS__DATA + " set with invalid data type.");
					}
				}
			}
			
			// Go fetch any incoming textures
			{
				std::pair<bool, TEXTURES__DATA_TYPE> data;
				if ((data = getAttributeAlt < TEXTURES__DATA_TYPE > (type, attributes, TEXTURES__DATA)).first) {
					if (!this->CreateTexture(data.second, model)) {
						LOG(LOG_PRIORITY::WARN, type + " " + TEXTURES__DATA + " set with invalid data type.");
					}
				}
			}
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(model);
		}
		else if (type == "Cube") {
			CubeRenderable* model = new CubeRenderable(entity, this, pd3dDevice);
			
			getAttribute < RENDER__SPACE_TYPE > (
			    type, attributes, RENDER__SPACE, std::bind1st(std::mem_fun(&CubeRenderable::SetCoordSpace), model)
			    );
			
			// Go fetch any incoming materials
			{
				std::pair<bool, MATERIALS__DATA_TYPE> data;
				if ((data = getAttributeAlt < MATERIALS__DATA_TYPE > (type, attributes, MATERIALS__DATA)).first) {
					if (!this->CreateMaterial(data.second, model)) {
						LOG(LOG_PRIORITY::INFO, type + " " + MATERIALS__DATA + " set with invalid data type.");
					}
				}
			}
			
			Threading::WriteLock w_lock(this->myRenderableComponentsMutex);
			
			this->myRenderableComponents.insert(model);
		}
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	WHO_DELETES::TYPE RenderModule::RemoveComponent(ComponentInterface* component) {
		// Determine if this is a camera and remove it from this->cameraComponents.
		{
			CameraComponent* camera = static_cast<CameraComponent*>(component);
			
			Threading::UpgradeLock up_lock(this->cameraComponentsMutex);
			
			cameramap::right_const_iterator it = this->cameraComponents.right.find(camera);
			if (it != this->cameraComponents.right.end()) {
				{
					Threading::UpgradeToUniqueLock unique_lock(up_lock);
					this->cameraComponents.right.erase(camera);
				}
				
				Threading::UpgradeLock cam_up_lock(this->currentCameraMutex);
				if (this->currentCamera == camera) {
					Threading::UpgradeToUniqueLock unique_lock(cam_up_lock);
					this->currentCamera = nullptr;
				}
				
				return WHO_DELETES::CALLER; // upstream delete
			}
		}
		
		// Determine if this is a RenderableComponent and remove it from the lists.
		{
			RenderableComponent* renderable = static_cast<RenderableComponent*>(component);
			
			{
				Threading::UpgradeLock up_lock(this->renderableComponentsMutex);
				
				std::set<RenderableComponent*>::iterator it = this->renderableComponents.find(renderable);
				if (it != this->renderableComponents.end()) {
					Threading::UpgradeToUniqueLock unique_lock(up_lock);
					this->renderableComponents.erase(it);
				}
			}
			
			{
				Threading::UpgradeLock up_lock(this->myRenderableComponentsMutex);
				
				std::set<RenderableComponent*>::iterator it_mine = this->myRenderableComponents.find(renderable);
				if (it_mine != this->myRenderableComponents.end()) {
					Threading::UpgradeToUniqueLock unique_lock(up_lock);
					this->myRenderableComponents.erase(it_mine);
					
					return WHO_DELETES::CALLER; // upstream delete
				}
			}
			
			return WHO_DELETES::CALLEE; // no delete needed
		}
	}
}
