/**
 * \file
 * \author Ricky Curtice
 * \date 20120129
 * \brief Provides a list of the message IDs used and constant handles to reference them by.
 *
 * <p>
 * </p>
 */
#pragma once

namespace Physics {
	/* * * * * * * * * * * * * * * * * * * * * */
	// Messages
	/* * * * * * * * * * * * * * * * * * * * * */
	const unsigned int ACK_MODULE_LOADED = 3001; ///< Message sent from the module to indicate that the it has loaded
	
	const unsigned int MSG_MODULE_START = 3002; ///< Message sent to the module to start it up
	const unsigned int ACK_MODULE_START = 3003; ///< Message sent from the module with a status indicator bool that indicate whether the module sucessfully loaded
	
	const unsigned int MSG_ENTITY_COLLISION = 3100; ///< Message sent from the module to indicate a collision between the two referenced entities, with the direction and depth of the collision as a Vector3D.
	
	const unsigned int MSG_RAY_CAST = 3500; ///< Directed message sent to the module to request a ray to be cast and the result to be the names of the collided entities.  Sent format is uint raytype, Vector3D source_location, Vector3D direction.  Direction doesn't have to be normalized.  Ray type is one of the RAY_CAST_* values.
	const unsigned int MSG_SCREEN_PICK = 3501; ///< Directed message sent to the module to request a ray to be cast from screen space and the result to be the names of the collided entities.  Sent format is uint raytype, Vector3D source_location, Vector3D direction.  Direction doesn't have to be normalized.  Ray type is one of the RAY_CAST_* values.
		const unsigned int RAY_CAST_ALL = 0; ///< Return all collisions.
		const unsigned int RAY_CAST_NEAREST = 1; ///< Only return the nearest collision.
}
