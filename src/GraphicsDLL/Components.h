/**
 * \file
 * \author Ricky Curtice
 * \date 20110724
 * \brief Provides a list of component name strings for creating components
 *
 * <p>
 * Provides a list of component name strings for creating components
 * </p>
 *
 */
#pragma once

#include <string>

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Components
	/* * * * * * * * * * * * * * * * * * * * * */
	const std::string CAMERA = "camera";
		const std::string CAMERA__NAME           = "name";
		#define           CAMERA__NAME_TYPE      std::string
		const std::string CAMERA__FOV            = "fov";      // (0, PI] - the horizontal field of view in radians
		#define           CAMERA__FOV_TYPE       float
		const std::string CAMERA__NEAR_CLIP      = "nearclip"; // [0, farclip)
		#define           CAMERA__NEAR_CLIP_TYPE float
		const std::string CAMERA__FAR_CLIP       = "farclip";  // (nearclip, inf)
		#define           CAMERA__FAR_CLIP_TYPE  float
		const std::string CAMERA__OFFSET         = "offset";
		#define           CAMERA__OFFSET_TYPE    D3DXVECTOR3
		const std::string CAMERA__ROTATION       = "rotation";
		#define           CAMERA__ROTATION_TYPE  D3DXQUATERNION
	
	/* * * * * * * * * * * * * * * * * * * * * */
	// Renderable Components
	/* * * * * * * * * * * * * * * * * * * * * */
	const std::string RENDER__SPACE      = "space";
	#define           RENDER__SPACE_TYPE ECoordinateSpace
	
	const std::string MATERIALS__DATA      = "materials";
	#define           MATERIALS__DATA_TYPE EnvelopeSPTR
	
	const std::string TEXTURES__DATA      = "textures";
	#define           TEXTURES__DATA_TYPE EnvelopeSPTR
	
	const std::string   LIGHT_AMBIENT = "amblight";
	const std::string   LIGHT_DIRECTIONAL = "dirlight";
	const std::string   LIGHT_POINT = "pointlight";
		const std::string LIGHT__AMBIENT_COLOR       = "ambient";
		#define           LIGHT__AMBIENT_COLOR_TYPE  D3DXCOLOR
		const std::string LIGHT__DIFFUSE_COLOR       = "diffuse";
		#define           LIGHT__DIFFUSE_COLOR_TYPE  D3DXCOLOR
		const std::string LIGHT__SPECULAR_COLOR      = "specular";
		#define           LIGHT__SPECULAR_COLOR_TYPE D3DXCOLOR
		const std::string LIGHT__RANGE               = "range"; // [0, inf]
		#define           LIGHT__RANGE_TYPE          float
	
	const std::string   HEIGHTMAP = "heightmap";
		const std::string HEIGHTMAP__FILE              = "heightmap";
		#define           HEIGHTMAP__FILE_TYPE         std::string
		const std::string HEIGHTMAP__TEXTURE_FILE      = "texture";
		#define           HEIGHTMAP__TEXTURE_FILE_TYPE std::string
	
	const std::string   MESH = "mesh";
		const std::string MESH__FILE           = "file";
		#define           MESH__FILE_TYPE      std::string
	
	const std::string   HELICAL_TUBE = "helicaltube";
		const std::string HELICAL_TUBE__RADIUS           = "radius";
		#define           HELICAL_TUBE__RADIUS_TYPE      float
		const std::string HELICAL_TUBE__LENGTH           = "length";
		#define           HELICAL_TUBE__LENGTH_TYPE      float
		const std::string HELICAL_TUBE__PITCH            = "pitch";
		#define           HELICAL_TUBE__PITCH_TYPE       float
		const std::string HELICAL_TUBE__SEGMENTS         = "segments";
		#define           HELICAL_TUBE__SEGMENTS_TYPE    unsigned int
	
	const std::string   TILED_MAP = "tiledmap";
	const std::string   POINTLINE = "pointline";
	const std::string   SPRITE = "sprite";
	const std::string   D3DFONT = "d3dfont";
	
	const std::string   QUAD = "quad";
		const std::string QUAD__WIDTH           = "width";
		#define           QUAD__WIDTH_TYPE      float
		const std::string QUAD__HEIGHT          = "height";
		#define           QUAD__HEIGHT_TYPE     float
		const std::string QUAD__ORIGIN          = "origin";
		#define           QUAD__ORIGIN_TYPE     unsigned int
		const std::string QUAD__ORIENT          = "orient";
		#define           QUAD__ORIENT_TYPE     unsigned int
}
