/**
* \file
* \author Ricky Curtice
* \date 2012-01-09
* \brief Provides a common interface for all objects that need to implment materials.
*
* *TODO: A full description of the purpose of the file.
*
*/

#include "MaterialsSystemInterface.h"

// Standard Includes
#include <cassert>
#include <set>

// Library Includes
#include <boost/lexical_cast.hpp>

// Local Includes
#include "../../sharedbase/Envelope.h"
#include "../Materials.h"

#include "MaterialInterface.h"
#include "BasicMaterial.h"

// Forward Declarations

// Typedefs
namespace GraphicsCore {
	typedef std::set<MaterialInterface*> MatSet;
}

// Local Constants

// Class Methods
namespace GraphicsCore {
	static MatSet gMaterials;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	MaterialInterface* MaterialsSystemInterface::CreateMaterial(EnvelopeSPTR envelope) {
		MaterialInterface* material = nullptr;
		
		switch (envelope->msgid) {
			case ID_MATERIAL_BASIC: {
				Color ambient, diffuse, specular, emissive;
				float power = 0.0f;
				
				ambient = diffuse = specular = Color(1.0f, 1.0f, 1.0f, 1.0f);
				emissive = Color(0.0f, 0.0f, 0.0f, 1.0f);
				
				{
					unsigned int colorType;
					float red, green, blue, alpha;
					
					bool safe = true;
					
					unsigned int count = envelope->GetCount();
					for (unsigned int index = 0; index < count && safe; ++index) {
						colorType = envelope->GetDataUInt(index);
						switch (colorType) {
							case ID_MATERIAL_COLOR_AMBIENT:
							case ID_MATERIAL_COLOR_DIFFUSE:
							case ID_MATERIAL_COLOR_SPECULAR:
							case ID_MATERIAL_COLOR_EMISSIVE: {
								// Get the values
								red   = envelope->GetDataFloat(index + 1);
								green = envelope->GetDataFloat(index + 2);
								blue  = envelope->GetDataFloat(index + 3);
								alpha = envelope->GetDataFloat(index + 4);
								
								// Consume the positions used.
								index += 4;
								
								// Go do the unique processing.
								switch (colorType) {
									case ID_MATERIAL_COLOR_AMBIENT: {
										ambient = Color(red, green, blue, alpha);
									}
									break;
									case ID_MATERIAL_COLOR_DIFFUSE: {
										diffuse = Color(red, green, blue, alpha);
									}
									break;
									case ID_MATERIAL_COLOR_SPECULAR: {
										specular = Color(red, green, blue, alpha);
									}
									break;
									case ID_MATERIAL_COLOR_EMISSIVE: {
										emissive = Color(red, green, blue, alpha);
									}
									break;
									// No default case needed as that has already been taken care of in the parent switch.
								}
							}
							break;
							case ID_MATERIAL_COLOR_POWER: {
								// Get the values
								power = envelope->GetDataFloat(index + 1);
								
								// Consume the positions used.
								index += 1;
							}
							break;
							default: {
								LOG(LOG_PRIORITY::INFO, "Unknown material color type sent: " + boost::lexical_cast < std::string > (colorType));
								safe = false; // Stop processing the loop: to proceed would be just garbage due to unknown types.
							}
						}
					}
				}
				
				material = new BasicMaterial(diffuse, ambient, specular, emissive, power);
			}
			break;
			case ID_MATERIAL_BASIC_USING_MAT: {
				try {
					material = new BasicMaterial(boost::any_cast<Material>(envelope->GetData(0)));
				}
				catch (const boost::bad_any_cast &) {
					LOG(LOG_PRIORITY::INFO, "Bad data type passed for new BasicMaterial using a source Material.");
				}
			}
			break;
			default: {
				LOG(LOG_PRIORITY::INFO, "Unknown material type: " + boost::lexical_cast < std::string > (envelope->msgid));
			}
		}
		
		// Make sure that no nulls ever get put in the materials list.
		if (material != nullptr) {
			gMaterials.insert(material);
		}
		
		return material;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void MaterialsSystemInterface::DestroyMaterial(MaterialInterface* material) {
		// Only delete if it had been created and has not already been deleted.
		MatSet::iterator matit = gMaterials.find(material);
		if (matit != gMaterials.end()) {
			assert(material != nullptr); // Just be very sure.  It should be absolutely impossible to have a nullptr stored in the materials list.
			
			delete material;
			
			gMaterials.erase(matit);
		}
	}
	
}
