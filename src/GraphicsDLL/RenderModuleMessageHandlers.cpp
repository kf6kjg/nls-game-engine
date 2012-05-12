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

#include <boost/lexical_cast.hpp>
#include <boost/function.hpp>
#include <utility>

#include "RenderModule.h"
#include "../sharedbase/EventLogger.h"
#include "../sharedbase/Entity.h"
#include "../sharedbase/Envelope.h"
#include "../sharedbase/MessageRouter.h"

#include "Messages.h"

namespace GraphicsCore {
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	void RenderModule::RegisterListeners() {
		// General Message Handlers
		{
			std::shared_ptr<Subscriber> handler(new Subscriber(boost::bind(&RenderModule::Shutdown, this, _1)));
			this->msgrouter->Subscribe(CORE_MESSAGE::SHUTDOWN, handler);
		}
		{
			std::shared_ptr<Subscriber> handler(new Subscriber(boost::bind(&RenderModule::Shutdown, this, _1)));
			this->msgrouter->Subscribe(MSG_STOP_GRAPHICS, handler);
		}
		{
			std::shared_ptr<Subscriber> handler(new Subscriber(boost::bind(&RenderModule::Startup, this, _1)));
			this->msgrouter->Subscribe(MSG_START_GRAPHICS, handler);
		}

		// *TODO: sometime when I have enough time to refactor all the code to do what is needed to get this done (a lot of work apparently): WM_STYLECHANGED, WM_SIZE, etc.

		// Directed Message Handlers
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleSetCurrentCamera, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_SET_CURRENT_CAMERA, handler);
		}
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleSetFillMode, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_SET_FILL_MODE, handler);
		}
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleSetRenderDelay, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_SET_RENDER_DELAY, handler);
		}
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleSetRenderRate, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_SET_RENDER_RATE, handler);
		}
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleGetRenderDelay, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_GET_RENDER_DELAY, handler);
		}
		{
			std::shared_ptr<DirectedSubscriber> handler(new DirectedSubscriber(boost::bind(&RenderModule::HandleGetRenderRate, this, _1)));
			this->msgrouter->SubscribeDirected(MSG_GET_RENDER_RATE, handler);
		}
		
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template<typename T>
	inline EnvelopeSPTR HandleSetterMessage(
	    boost::function<bool(T)> setter,
	    const int msgID,
	    const EnvelopeSPTR e,
	    const T errValue,
	    const int ackMsgNumber) {
		
		T value = errValue;
		bool status;
		
		try {
			value = boost::any_cast < T > (e->GetData());
		}
		catch (const boost::bad_any_cast &) {
			std::string errStr("Bad data type passed for message id ");
			errStr.append(boost::lexical_cast < std::string > (msgID));
			LOG(LOG_PRIORITY::INFO, errStr);
		}

		status = setter(value);
		if (!status) {
			value = errValue;
		}

		EnvelopeSPTR confirmation(new Envelope);
		
		confirmation->msgid = ackMsgNumber;
		confirmation->AddData(value);
		confirmation->AddData(status);
		
		return confirmation;
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::SetCurrentCamera(const std::string& cameraName) {
		bool status = false;
		
		Threading::ReadLock r_lock(this->cameraComponentsMutex);
		
		cameramap::left_const_iterator it = this->cameraComponents.left.find(cameraName);
		
		if (it != this->cameraComponents.left.end()) {
			status = true;
			
			Threading::WriteLock w_lock(this->currentCameraMutex);
			
			this->currentCamera = it->second;
			this->currentCameraName = cameraName;
		}
		
		return status;
	}
	
	EnvelopeSPTR RenderModule::HandleSetCurrentCamera(EnvelopeSPTR incomingEnvelope) {
		return HandleSetterMessage < std::string > (
			boost::bind(&RenderModule::SetCurrentCamera, boost::ref(this), _1), // The setter method
			incomingEnvelope->GetDataInt(0), incomingEnvelope,
			this->currentCameraName, // The return value if the setter failed to set
			ACK_SET_CURRENT_CAMERA // The return message ID
		);
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::SetFillMode(D3DFILLMODE mode) {
		this->fillMode = mode;
		
		return true;
	}
	
	EnvelopeSPTR RenderModule::HandleSetFillMode(EnvelopeSPTR incomingEnvelope) {
		return HandleSetterMessage < D3DFILLMODE > (
			std::bind1st(std::mem_fun(&RenderModule::SetFillMode), this), // The setter method
			incomingEnvelope->GetDataInt(0), incomingEnvelope,
			this->fillMode, // The return value if the setter failed to set
			ACK_SET_FILL_MODE // The return message ID
		);
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::SetRenderDelay(float delay) {
		bool status = false;
		
		if (delay > 0.0f) {
			this->renderDelay = delay;
			status = true;
		}
		
		return status;
	}
	
	EnvelopeSPTR RenderModule::HandleSetRenderDelay(EnvelopeSPTR incomingEnvelope) {
		return HandleSetterMessage<float>(
			std::bind1st(std::mem_fun(&RenderModule::SetRenderDelay), this), // The setter method
			incomingEnvelope->GetDataInt(0), incomingEnvelope,
			this->renderDelay, // The return value if the setter failed to set
			ACK_SET_RENDER_DELAY // The return message ID
		);
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	bool RenderModule::SetRenderRate(float rate) {
		return SetRenderDelay(1.0f / rate);
	}
	
	EnvelopeSPTR RenderModule::HandleSetRenderRate(EnvelopeSPTR incomingEnvelope) {
		return HandleSetterMessage<float>(
			std::bind1st(std::mem_fun(&RenderModule::SetRenderRate), this), // The setter method
			incomingEnvelope->GetDataInt(0), incomingEnvelope,
			1.0f / this->renderDelay, // The return value if the setter failed to set
			ACK_SET_RENDER_RATE // The return message ID
		);
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	EnvelopeSPTR RenderModule::HandleGetRenderDelay(EnvelopeSPTR incomingEnvelope) const {
		EnvelopeSPTR outEnvelope(new Envelope);
		
		outEnvelope->msgid = ACK_GET_RENDER_DELAY;
		outEnvelope->AddData(this->renderDelay);
		
		return outEnvelope;
	}
	
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	EnvelopeSPTR RenderModule::HandleGetRenderRate(EnvelopeSPTR incomingEnvelope) const {
		EnvelopeSPTR outEnvelope(new Envelope);
		
		outEnvelope->msgid = ACK_GET_RENDER_RATE;
		outEnvelope->AddData(1.0f / this->renderDelay);
		
		return outEnvelope;
	}
	
}
