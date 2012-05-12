/**
* \file 
* \author Adam Martin
* \date 2011-10-28
* \brief
*
*
* 
*/

#include "MessageRouter.h"

// Standard Includes
#include <algorithm>

// Library Includes
#include <boost/foreach.hpp>

// Local Includes
#include "Envelope.h"

// Static class member initialization

// Class methods in the order they are defined within the class header

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MessageRouter::MessageRouter( void ) : routing(true) {
	SubscriberSPTR sshutdown(new Subscriber(boost::bind(&MessageRouter::Shutdown, this, _1)));
	Subscribe(CORE_MESSAGE::SHUTDOWN, sshutdown);
	SubscriberSPTR squit(new Subscriber(boost::bind(&MessageRouter::Quit, this, _1)));
	Subscribe(CORE_MESSAGE::QUIT, squit);
}

MessageRouter::~MessageRouter( void ) {
	while (!this->backlog.empty()) {
		this->backlog.pop();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Subscribe(int id, SubscriberSPTR& subscriber) {
	Threading::WriteLock w_lock(this->subscriptionsMutex);
	
	this->subscriptions[id].insert(subscriber);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Subscribe( std::vector<int>& ids, SubscriberSPTR& subscriber) {
	BOOST_FOREACH(int id, ids) {
		this->Subscribe(id, subscriber);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::SubscribeDirected( int id, std::shared_ptr<DirectedSubscriber>& subscriber ) {
	Threading::WriteLock w_lock(this->directedSubscriptionMutex);
	
	this->directedSubscription[id] = subscriber;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Unsubscribe( int id, SubscriberSPTR& subscriber ) {
	// We need to retrieve the obj_ptr from both functors in order to see if we have a match
	Subscriber func1 = (*subscriber.get());
	boost::detail::function::function_buffer functor = func1.functor;
	Subscriber func2;
	boost::detail::function::function_buffer functor2;

	Threading::WriteLock w_lock(this->subscriptionsMutex);
	
	SubscriptionsType::iterator subscriptions_it = this->subscriptions.find(id);
	if (subscriptions_it != this->subscriptions.end()) {
		for (SubscriberCollectionType::iterator subscribers_it = subscriptions_it->second.begin(); subscribers_it != subscriptions_it->second.end();) {
			SubscriberSPTR subscriber = *subscribers_it;
			++subscribers_it;
			
			func2 = *subscriber;
			functor2 = func2.functor;
			if (functor.obj_ptr == functor2.obj_ptr) {
				this->subscriptions.at(id).erase(subscriber);
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Unsubscribe( std::vector<int>& ids, SubscriberSPTR& subscriber ) {
	BOOST_FOREACH(int id, ids) {
		this->Unsubscribe(id, subscriber);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
EnvelopeSPTR MessageRouter::SendDirected( EnvelopeSPTR envelope, int id ) const {
	Threading::ReadLock r_lock(this->directedSubscriptionMutex);
	
	DirectedSubscriptionsType::const_iterator it = this->directedSubscription.find(id);
	if (it != this->directedSubscription.end()) {
		std::shared_ptr<DirectedSubscriber> sub = (*it).second;
		return (*sub.get())(envelope);
	}
	
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Route( void ) {
	EnvelopeSPTR envelope;
	while (routing) {
		while (!this->backlog.empty()) {
			// Get the next Envelope from the queue and pop it from the queue
			{
				Threading::WriteLock w_lock(this->backlogMutex);
				
				envelope = this->backlog.front();
				this->backlog.pop();
			}
			
			// See if anyone is subscribed to the message id, get the functor, and call it
			this->SendSP(envelope, false);
		}
		Sleep(1);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Shutdown( EnvelopeSPTR envelope ) {
	Threading::WriteLock w_lock(this->backlogMutex);
	
	while (!this->backlog.empty()) {
		this->backlog.pop();
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::Quit( EnvelopeSPTR envelope ) {
	this->routing = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MessageRouter::SendSP( EnvelopeSPTR envelope, bool async /*= true*/ ) {
	if (async) {
		Threading::WriteLock w_lock(this->backlogMutex);
		
		this->backlog.push(envelope);
	}
	else {
		SubscriberCollectionType subscribers;
		
		{
			Threading::ReadLock r_lock(this->subscriptionsMutex);
			
			SubscriptionsType::iterator it = this->subscriptions.find(envelope->msgid);
			if (it != this->subscriptions.end()) {
				subscribers = (*it).second;
			}
		}
		
		BOOST_FOREACH(SubscriberSPTR subscriber, subscribers) {
			(*subscriber)(envelope);
		}
	}
}
