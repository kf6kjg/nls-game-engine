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

#include "GridMover.h"

// System Library Includes

// Application Library Includes

// Local Includes
#include "../../sharedbase/Entity.h"
#include "../../sharedbase/ComponentInterface.h"
#include "../../sharedbase/MessageRouter.h"
#include "../../sharedbase/Envelope.h"

// Forward Declarations

// Typedefs and data structures

// Constants

// Class methods
namespace Physics {
	#define SUBSCRIBE_BASIC(msgid, handler)   {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Subscribe((msgid), handler_sptr);}
	#define UNSUBSCRIBE_BASIC(msgid, handler) {std::shared_ptr<Subscriber> handler_sptr(new Subscriber(boost::bind((&handler), this, _1))); this->msgrouter->Unsubscribe((msgid), handler_sptr);}

	GridMover::GridMover( EntitySPTR owner, ModuleInterface* module, MessageRouter* msgrtr ) : InterfaceUpdateable(owner, module), msgrouter(msgrtr)  {
		preaction = false;
		postaction = false;
		this->scale = 32;
		this->path = nullptr;
		this->currentPoint = Point();
		this->nextPoint = Point();
		this->pauseDelay = 0.0f;
		this->currentDelay = 0.0f;
		SUBSCRIBE_BASIC(3100, GridMover::ActionComplete);
	}

	GridMover::~GridMover() {

	}


	bool GridMover::SetMovementScale( unsigned int size ) {
		this->scale = size;
		return true;
	}

	void GridMover::AddPointActionHandler( unsigned int timing, unsigned int actionhandler ) {
		this->actionHandlers[timing] = actionhandler;
	}

	void GridMover::RemovePointActionHandler( unsigned int timing ) {
		if (this->actionHandlers.find(timing) != this->actionHandlers.end()) {
			this->actionHandlers.erase(timing);
		}
	}
	bool GridMover::LoadObjectData( EnvelopeSPTR data ) {
		for (unsigned int i = 0; i < data.get()->GetCount(); ++i) { // Loop through each object
			EnvelopeSPTR object(data.get()->GetDataEnvelopeSPTR(i));

			HandlerData hd;
			Point p;
			hd.type = object.get()->GetDataString(0);	// 0 - Type
			hd.name = object.get()->GetDataString(1);	// 1 - Name
			hd.data = object.get()->GetDataString(2);	// 2 - Data
			p.x = object.get()->GetDataInt(3) / 32;		// 3 - x
			p.y = object.get()->GetDataInt(4) / 32;		// 4 - y

			this->pointHandlerdata[p] = hd;
		}

		return true;
	}

	bool GridMover::SetPath( PointList* points ) {
		if (points != nullptr) {
			this->path = points;
			this->owner->location.x = (float)this->path->CurrentPoint().x * this->scale; // Stores the current point
			this->owner->location.y = (float)this->path->CurrentPoint().y * this->scale; // Stores the current point
			this->currentPoint = this->path->CurrentPoint(); // Set the currentPoint to the current point in the list
			this->nextPoint = this->path->PeekNextPoint(); // Peek what the next point is after that, and store it in nextPoint
			return true;
		}

		return false;
	}

	void GridMover::Update( double dt ) {
		// Wait for pause if currentDelay was reset upon entering a point
		if ((this->currentDelay + dt) < pauseDelay) {
			this->currentDelay += dt;
			return;
		}

		if (this->postaction) {
			// Check if there is a action for leaving the point
			if (this->actionHandlers.find(TIMING::POINT_LEAVE) != this->actionHandlers.end()) {
				unsigned int leaveActionID = actionHandlers[TIMING::POINT_LEAVE];

				// Send an envelope with the actionID as the msgid
				EnvelopeSPTR actionEnv(new Envelope());
				actionEnv->msgid = leaveActionID;

				// Add the x and y for the current point as 2 separate values as script doesn't yet known about the Point type.
				actionEnv->AddData(this->currentPoint.x);
				actionEnv->AddData(this->currentPoint.y);

				// Add the x and y for the current point as 2 separate values as script doesn't yet known about the Point type.
				if (this->pointHandlerdata.find(this->currentPoint) != this->pointHandlerdata.end()) {
					HandlerData hd = this->pointHandlerdata[this->currentPoint];
					actionEnv->AddData(hd.type);
					actionEnv->AddData(hd.name);
					actionEnv->AddData(hd.data);
					this->pointHandlerdata.erase(this->nextPoint);
				}

				this->msgrouter->SendSP(actionEnv);
			}
		}

		if ((!preaction) && (!postaction)) {
			if (this->owner != nullptr) {
				float x = this->owner->location.x;
				float y = this->owner->location.y;

				// Normalize it
				x /= this->scale;
				y /= this->scale;
				// Direction multipliers
				short leftright = 0; // Moving left to right (1) or right to left (-1), or not in this direction (0)
				short updown = 0; // Moving top to bottom (1) or bottom to top (-1), or not in this direction (0)

				//////////////////////////////////////////////////////////////////////////
				// Left or right
				//////////////////////////////////////////////////////////////////////////

				// Check if owner.location.x the same as nextPoint.x
				if (fabs((this->owner->location.x / this->scale) - this->nextPoint.x) > .001) { // It's not
					if (currentPoint.x < this->nextPoint.x) { // Moving left to right
						if ((x +  dt) < this->nextPoint.x) { // We wont pass it.
							leftright = 1;
						}
					}
					else if (currentPoint.x > this->nextPoint.x) { // Moving right to left
						if ((x + (-1 * dt)) > this->nextPoint.x) {  // We wont pass it.
							leftright = -1;
						}
					}
				}

				//////////////////////////////////////////////////////////////////////////
				// Up or down
				//////////////////////////////////////////////////////////////////////////

				// Check if owner.location.y the same as nextPoint.y
				if (fabs((this->owner->location.y / this->scale) - this->nextPoint.y) > .001) { // It's not
					if (currentPoint.y < this->nextPoint.y) { // Moving up to down
						if ((y +  dt) < this->nextPoint.y) { // We wont pass it.
							updown = 1;
						}
					}
					else if (currentPoint.y > this->nextPoint.y) { // Moving down to up
						if ((y + (-1 * dt)) > this->nextPoint.y) {  // We wont pass it.
							updown = -1;
						}
					}
				}

				// If we can't move leftright or upddown we have each the nextpoint
				if ((leftright == 0) && (updown == 0)) {
					Point prevPoint = this->currentPoint;
					this->currentPoint = this->nextPoint; // We arrived at the next point, update current point to reflected that

					// Path is either null or invalid
					if (this->path == nullptr)  {
						return;
					}
					if (this->path->PeekNextPoint().x == -1) {
						return;
					}

					// Check if the current point we are on is still the next point
					if ((this->currentPoint.x == this->path->PeekNextPoint().x) && (this->currentPoint.y == this->path->PeekNextPoint().y)) {
						this->path->NextPoint(); // Advance the next point
						this->nextPoint = this->path->PeekNextPoint(); // Peek what the next point is after that, and store it in nextPoint
					}
					else { // Our path changed
						Point nextPeek = this->path->PeekNextPoint();
						if ((abs(nextPeek.x - this->currentPoint.x) <= 1) && (abs(nextPeek.y - this->currentPoint.y) <= 1)) { // Movement will be 1 away so still good
							this->path->NextPoint(); // Set the currentPoint to the next point in the list (advances the list)
							this->nextPoint = this->path->PeekNextPoint(); // Peek what the next point is after that, and store it in nextPoint
						}
						else { // Moving to far so move back one
							this->nextPoint = prevPoint;
						}
					}

					// Check if there is an action upon entering the point
					// Reset the delay in ActionComplete
					if (this->actionHandlers.find(TIMING::POINT_ENTER) != this->actionHandlers.end()) {
						this->preaction = true;
						unsigned int enterActionID = this->actionHandlers[TIMING::POINT_ENTER];

						// Send an envelope with the actionID as the msgid
						EnvelopeSPTR actionEnv(new Envelope());
						actionEnv->msgid = enterActionID;

						actionEnv->AddData(this->currentPoint.x);
						actionEnv->AddData(this->currentPoint.y);

						// Add the x and y for the current point as 2 separate values as script doesn't yet known about the Point type.
						if (this->pointHandlerdata.find(this->currentPoint) != this->pointHandlerdata.end()) {
							HandlerData hd = this->pointHandlerdata[this->currentPoint];
							actionEnv->AddData(hd.type);
							actionEnv->AddData(hd.name);
							actionEnv->AddData(hd.data);
							//this->pointHandlerdata.erase(this->nextPoint);
						}

						this->msgrouter->SendSP(actionEnv);
					}
					else { // No actions so reset the delay
						this->currentDelay = 0.0f;
					}
				}
				else { // Move
					if (leftright == 0) {
						this->owner->location.x = (float)this->nextPoint.x * scale;
					}
					else {
						this->owner->location.x += (float)((double)leftright * dt) * scale;
					}

					if (updown == 0) {
						this->owner->location.y = (float)this->nextPoint.y * scale;
					}
					else {
						this->owner->location.y += (float)((double)updown * dt) * scale;
					}
				}

			}
		}
	}

	void GridMover::ActionComplete( EnvelopeSPTR env ) {
		unsigned int timing = env->GetDataUInt(0);
		if (timing == TIMING::POINT_ENTER) {
			this->preaction = false;

			this->postaction = true;
		}

		if (timing == TIMING::POINT_LEAVE) {
			this->postaction = false;

			this->currentDelay = 0.0f;
		}
	}

	bool GridMover::SetPauseDelay( float d ) {
		this->pauseDelay = d;

		return true;
	}
}
