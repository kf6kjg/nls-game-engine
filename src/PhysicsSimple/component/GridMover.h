/**
 * \file
 * \author Adam Martin
 * \date 20120120
 * \brief Waypoint-based mover in a regular grid.
 * Different actions can be assigned at each grid location.
 *
 * 
 *
 */
#pragma once

// Standard Includes
#include <map>
#include <string>
#include <vector>

// Library Includes

// Local Includes
#include "../../sharedbase/PointList.h"
#include "../InterfaceUpdateable.h"
#include "../../sharedbase/Envelope_fwd.h"

// Forward Declarations
class InterfaceUpdateable;
class ModuleInterface;
class Entity;
class MessageRouter;

// Typedefs

// Constants

// Classes
namespace Physics {
	namespace TIMING {
		const unsigned int POINT_ENTER = 0;
		const unsigned int POINT_LEAVE = 1;	
	}

	struct HandlerData {
		std::string name;
		std::string type;
		std::string data;
	};

	class GridMover : public InterfaceUpdateable {
	public:
		GridMover(EntitySPTR owner, ModuleInterface* module, MessageRouter* msgrtr);

		~GridMover();

		bool SetMovementScale(unsigned int size); // Sets the scale applied to the owner's location each update
		void AddPointActionHandler(unsigned int timing, unsigned int actionhandler); // Register the point action handler for a given timing
		void RemovePointActionHandler(unsigned int timing); // Removes the point action handler for a given timing
		bool LoadObjectData(EnvelopeSPTR data);
		bool SetPath(PointList* points); // Sets the current path to follow.
		void Update(double dt);
		
		void ActionComplete(EnvelopeSPTR env);

		bool SetPauseDelay(float d);

	private:
		unsigned int scale;

		Point currentPoint;
		Point nextPoint;
		PointList* path; // Stores the current path. We are storing a reference to it, so any change to the original list, is also reflected here.
		std::map< unsigned int, unsigned int > actionHandlers; // Point action handlers for a given timing (timing, actionahandler)
		std::map< Point, HandlerData > pointHandlerdata; // Stores the extra handler data for a given point

		bool preaction; // Waiting on the preaction to complete
		bool postaction; // Waiting on the postaction to complete

		MessageRouter* msgrouter;

		float pauseDelay; // Stores the time to pause on each point
		float currentDelay;  // Counter for the current delay
	};
}
