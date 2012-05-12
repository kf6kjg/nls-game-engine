#pragma once

/**
* \file
* \author Adam Martin
* \date 2011-010-28
* \brief  Message routing class to allow advanced subscription based messaging.
*
* 
* 
*/

// Standard Includes
#include <map>
#include <queue>
#include <set>

// Library Includes
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <threading.h>

// Local Includes
#include "Envelope_fwd.h"

// Forward Declarations

// Typedefs
typedef boost::function<void (EnvelopeSPTR)> Subscriber;
typedef boost::function<EnvelopeSPTR (EnvelopeSPTR)> DirectedSubscriber;
typedef std::shared_ptr<Subscriber> SubscriberSPTR;
typedef std::set<SubscriberSPTR> SubscriberCollectionType;
typedef std::map<int, SubscriberCollectionType> SubscriptionsType;
typedef std::map<int, std::shared_ptr<DirectedSubscriber>> DirectedSubscriptionsType;

enum CORE_MESSAGE : unsigned int {STARTUP = 0x0000, QUIT = 0x0099, SHUTDOWN = 0x0100, CREATE = 0x0001, LOADLIBRARY = 0x0002, MODULESTARTED = 0x0003};

class MessageRouter {
public:
	MessageRouter(void);
	~MessageRouter(void);
	
	void Subscribe(int id, SubscriberSPTR& s); // Subscribe to message id, with subscriber function s
	void Subscribe(std::vector<int>& ids, SubscriberSPTR& s); // Subscribe to all messages ids, with subscriber function s
	void SubscribeDirected(int id, std::shared_ptr<DirectedSubscriber>& s); // Subscribe to a Directed Subscription with subscription id
	void Unsubscribe(int id, SubscriberSPTR& s);
	void Unsubscribe(std::vector<int>& ids, SubscriberSPTR& s);
	void SendSP(EnvelopeSPTR, bool async = true); // Add a new envelope to the backlog, Envelopes must be dynamic memory to allow the dtor to free all unrouted messages safely. If sync is set to true the message is sent synchronously
	EnvelopeSPTR SendDirected(EnvelopeSPTR e, int id) const; // Sends a message to the specified id, and returns another Envelope
	void Route(void); // Thread to handle the backlog
	void Shutdown(EnvelopeSPTR e); // Used to shutdown this module after quit has been initiated
	void Quit(EnvelopeSPTR e); // Used when the application is quiting.
	
private:
	mutable Threading::ReadWriteMutex subscriptionsMutex;
	SubscriptionsType subscriptions; // a mapping of message ids to subscribers
	
	mutable Threading::ReadWriteMutex directedSubscriptionMutex;
	DirectedSubscriptionsType directedSubscription; // a mapping of message ids to subscribers
	
	mutable Threading::ReadWriteMutex backlogMutex;
	std::queue<EnvelopeSPTR> backlog;
	
	bool routing;
};
