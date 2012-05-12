/**
 * \file
 * \author Adam Martin
 * \date 20120120
 * \brief Basic A* Pathfinding component
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
#include "../../sharedbase/ComponentInterface.h"
#include "../../sharedbase/Envelope_fwd.h"

// Forward Declarations
class MessageRouter;

// Typedefs

// Constants

// Classes
namespace Physics {

	enum STATE {SOLID, PICKUP, FREE};

	struct navPoint {
		int f, g;
		float h;
		Point pos;
		navPoint* parent;
	};

	typedef std::map< Point, STATE > STATEGrid;
	typedef std::map< Point, navPoint > NavGrid;

	class ASTARMap : public ComponentInterface {
	public:
		ASTARMap(EntitySPTR owner, ModuleInterface* module, MessageRouter* msgrtr);

		~ASTARMap();

		// Load the map from a TMX file.
		bool LoadMapData(EnvelopeSPTR data);
		bool LoadObjectData(EnvelopeSPTR data);

		// Obstacle placement attempt.
		EnvelopeSPTR MouseClickonPoint(EnvelopeSPTR env);

		// Retrieves the node at x,y
		STATE GetState(Point c);

		// Sets the specified node at x,y to n
		void SetState(Point c, STATE n);

		// Shortcut to set the node at x,y to an empty,free node
		void RemoveState(Point c);

		// Retrieves the begin iterator
		STATEGrid::iterator begin();

		// Retrieves the end iterator
		STATEGrid::iterator end();

		// Finds the path to the target node starting at x,y
		void FindPath(Point player);

		// Updates the path if anything has changed.
		// It will use the currentPoint in points for the starting location.
		void UpdatePath();
		EnvelopeSPTR UpdateIfPathBlocked(EnvelopeSPTR env); // Checks if the next point in the list is blocked and calls UpdatePath if it is

		void SetGoal(int x, int y);

		// Retrieves the PointList for use by other components or scripting
		PointList* GetPoints();

		Point start;
	private:
		STATEGrid grid;
		PointList points;
		Point goal;
		NavGrid openList;
		NavGrid closedList;
		MessageRouter* msgrouter;
	};
}
