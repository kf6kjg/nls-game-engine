/**
 * \file
 * \author Ricky Curtice
 * \date 20120112
 * \brief A dynamic helical tube mesh
 *
 * <p>
 * </p>
 *
 */

#include "ASTARMap.h"

// System Library Includes

// Application Library Includes
#include <EngineConfig.h>

// Local Includes
#include "../../sharedbase/ComponentInterface.h"
#include "../../sharedbase/Entity.h"
#include "../../sharedbase/MessageRouter.h"
#include "../../sharedbase/Envelope.h"

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods
namespace Physics {
#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Subscribe((msgid), handler_sptr);}
#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->c->Unsubscribe((msgid), handler_sptr);}
#define SUBSCRIBE_DIRECTED(msgid, handler)   {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->SubscribeDirected((msgid), handler_sptr);}
#define UNSUBSCRIBE_DIRECTED(msgid, handler) {std::shared_ptr<DirectedSubscriber> handler_sptr(new DirectedSubscriber(boost::bind((&handler), this, _1))); this->msgrouter->UnsubscribeDirected((msgid), handler_sptr);}

	ASTARMap::ASTARMap( EntitySPTR owner, ModuleInterface* module, MessageRouter* msgrtr ) : ComponentInterface(owner, module), msgrouter(msgrtr) {
		SUBSCRIBE_DIRECTED(3103, ASTARMap::MouseClickonPoint);
		SUBSCRIBE_DIRECTED(3104, ASTARMap::UpdateIfPathBlocked);
	}

	ASTARMap::~ASTARMap()	{

	}

	bool ASTARMap::LoadMapData( EnvelopeSPTR data ) {
		EnvelopeSPTR layersEnv(data.get()->GetDataEnvelopeSPTR(0)); // First envelope is layers
		for (unsigned int i = 0; i < layersEnv.get()->GetCount(); ++i) { // Loop through each layer
			EnvelopeSPTR layer(layersEnv.get()->GetDataEnvelopeSPTR(i)); // Get each layer
			for (unsigned int n = 0; n < layer.get()->GetCount(); n = n + 3) { // Loop through each tile
				unsigned int r = layer.get()->GetDataUInt(n);			// 0 - row
				unsigned int c = layer.get()->GetDataUInt(n + 1);		// 1 - column
				unsigned int tileid = layer.get()->GetDataUInt(n + 2);	// 2 - tileid
				if (tileid != 0) {
					Point cor;
					cor.x = c;
					cor.y = r;
					this->grid[cor] = STATE::FREE;
				}
			}
		}
		
		return true;
	}
	bool ASTARMap::LoadObjectData( EnvelopeSPTR data ) {
		for (unsigned int n = 0; n < data.get()->GetCount(); ++n) { // Loop through each object
			EnvelopeSPTR objectEnv(data.get()->GetDataEnvelopeSPTR(n)); // Get each layer
			std::string name = objectEnv.get()->GetDataString(0);	// 0 - object name
			int x = objectEnv.get()->GetDataInt(1);					// 1 - x
			int y = objectEnv.get()->GetDataInt(2);					// 2 - y
			int width = objectEnv.get()->GetDataInt(3);				// 3 - width
			int height = objectEnv.get()->GetDataInt(4);			// 4 - height

			if (name == "start") {
				Point c;
				c.x = x / 32;
				c.y = y / 32;
				this->start = c;
			} else  if (name == "goal") {
				Point c;
				c.x = x / 32;
				c.y = y / 32;
				this->goal = c;
			}
		}

		return true;
	}



	EnvelopeSPTR ASTARMap::MouseClickonPoint( EnvelopeSPTR env ) {
		Point p;
		p.x = env->GetDataUInt(0);
		p.y = env->GetDataUInt(1);
		EntitySPTR obstacle(env->GetDataEntityP(2));

		EnvelopeSPTR retenv(new Envelope());
		retenv->msgid = 3103;
		if (GetState(p) == STATE::FREE) {
			SetState(p, STATE::SOLID);
			obstacle->location.x = (float)p.x * 32;
			obstacle->location.y = (float)p.y * 32;

			// Send an ACK about placing the obstacle
			retenv->AddData<bool>(true);
			//UpdatePath();
		}
		else {
			retenv->AddData<bool>(false);
		}
		return retenv;
	}


	STATE ASTARMap::GetState( Point c ) {
		return this->grid[c];
	}

	void ASTARMap::SetState(Point c, STATE n) {
		this->grid[c] = n;
	}

	void ASTARMap::RemoveState(Point c) {
		SetState(c, FREE);
	}

	STATEGrid::iterator ASTARMap::begin() {
		return this->grid.begin();
	}

	STATEGrid::iterator ASTARMap::end() {
		return this->grid.begin();
	}

	void ASTARMap::FindPath(Point player) {
		this->openList.clear(); // Clear the old lists
		this->closedList.clear(); // Clear the old lists
		navPoint playerNode = {0,0,0, player, nullptr};
		navPoint targetNode;
		this->openList[player] = playerNode;

		while (this->openList.size() > 0) {
			// Find the lowest f cost in the this->openList,
			// set it as our loc node,
			// and move it to the this->closedList
			NavGrid::iterator lowest = this->openList.begin();
			for (NavGrid::iterator itr = this->openList.begin(); itr != this->openList.end(); ++itr) {
				if (itr->second.f < lowest->second.f) {
					lowest = itr;
				}
			}
			if (lowest == this->openList.end()) {
				return;
			}
			this->closedList.insert(*lowest);
			navPoint* curNode = &this->closedList[lowest->second.pos]; // grab the pointer to the closed list item as the one in open list will be removed
			this->openList.erase(lowest);
			if ((curNode->pos.x == goal.x) && (curNode->pos.y == goal.y)) {
				targetNode = *curNode;
				// We have reach out goal so lets store the path
				navPoint* parent = &targetNode;
				
				// Store the points in a container for reversing them into the point list
				std::vector<Point> reverseVector;
				while (parent != nullptr) {
					reverseVector.push_back(parent->pos);
					parent = parent->parent;
				}

				// Now we do the reversing
				for (std::vector<Point>::reverse_iterator itr = reverseVector.rbegin(); itr != reverseVector.rend(); ++itr) {
					this->points.AddPoint((*itr));
				}

				this->owner->properties.SetProperty("pointlist", &this->points);
				return;
			}


			// Get the surrounding nodes 
			navPoint temp;
			int cost;
			Point loc = player;
			for (int x = -1; x < 2; ++x) {
				for (int y = -1; y < 2; ++y) {

					// Diagonal check. Remove to allow diagonal movement.
					if ((x != 0) && (y != 0)) {
						continue;
					}
					if ((x == 0) && (y == 0)) { // We don't need to check the current node
						continue;
					} else if ((x == 0) || (y == 0)) { // Straight movement
						cost = 10;
					} else { // Diagonal
						cost = 14;
					}
					loc.x = curNode->pos.x + x;
					loc.y = curNode->pos.y + y;
					if (this->grid.find(loc) != this->grid.end()) { // Node at coords exists
						if ((this->openList.find(loc) == this->openList.end()) && (this->closedList.find(loc) == this->closedList.end())) { // Not on open or closed list
							if (this->grid[loc] == FREE) {
								temp.g = cost + curNode->g;
								temp.h = cost * (abs(curNode->pos.x - this->goal.x) + abs(curNode->pos.y - this->goal.y));
								temp.f = (int)((float)temp.g + temp.h);
								temp.pos = loc;
								temp.parent = curNode;
								this->openList[loc] = temp;
							}
						} else if (this->openList.find(loc) != this->openList.end()) {
							if (this->openList[loc].g > (curNode->g + cost)) {
								this->openList[loc].parent = curNode;
								this->openList[loc].g = (curNode->g + cost);
								this->openList[loc].f = (int)((float)this->openList[loc].g + this->openList[loc].h);
							}
						}
					}
				}
			}
		}
	}

	void ASTARMap::UpdatePath() {
		// Do some housework to find the new path
		Point curr = this->points.CurrentPoint(); // Save the current point
		this->points.Clear(); // Clear the points
		FindPath(curr); // Find the new path starting at curr
		this->points.GotoPoint(curr); // Go back to the current point in the list
	}


	void ASTARMap::SetGoal(int x, int y) {
		this->goal.x = x;
		this->goal.y = y;
	}

	PointList* ASTARMap::GetPoints() {
		return &this->points;
	}

	EnvelopeSPTR ASTARMap::UpdateIfPathBlocked(EnvelopeSPTR env) {
		EnvelopeSPTR retenv(new Envelope());
		retenv->msgid = 3104;

		Point p = this->points.PeekNextPoint();
		if (GetState(p) == STATE::SOLID) {
			UpdatePath();

			retenv->AddData<bool>(true);
		}

		retenv->AddData<bool>(false);

		return retenv;
	}

}
