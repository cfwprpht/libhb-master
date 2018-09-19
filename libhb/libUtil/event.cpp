#define LIBRARY_IMPL  (1)
#include "stdafx.h"
#include "event.h"

#pragma comment( lib , "libSceSysmodule_stub_weak.a" )

using namespace common::Service;
namespace sss = sce::SampleUtil::System;

int EventDispatcher::initialize(void) { return SCE_OK; }

int EventDispatcher::finalize(void) { return SCE_OK; }

int EventDispatcher::registerListener(EventListener *listener) {
	printf("[EventDispatcher] REGISTER %p\n", listener);
	_listeners.push_back(listener);
	return SCE_OK;
}

int EventDispatcher::unregisterListener(EventListener *listener) {
	printf("[EventDispatcher] UNREGISTER %p\n", listener);
	std::vector<EventListener*>::iterator it;
	for (it = _listeners.begin(); it != _listeners.end(); it++) {
		if (*it == listener) {
			_listeners.erase(it);
			return SCE_OK;
		}
	}
	return -1;
}

/*static const char* getEventTypeAsString(common::Service::GameEventType eventType) {
	switch (eventType) {
		case kEventClick:					return "kEventClick";
		case kEventUp:				        return "kEventUp";
		case kEventDown:					return "kEventDown";
		case kEventSecret:				    return "kEventSecret";
		case kEventLoginUser:				return "kEventLoginUser";
		case kEventLogoutUser:				return "kEventLogoutUser";
		case kEventInvalid:					return "kEventInvalid";

		default:							return "UNKNOWN EVENT";
	}
}*/

void EventDispatcher::dispatchEvent(const Event *event, const void *userData, int userFlag) {
	//m_debugWindow->debugPrintf("Disptch event : %s\n", getEventTypeAsString((common::Service::GameEventType)event->type));
	for (int i = 0; i < _listeners.size(); i++) {
		_listeners.at(i)->onEventArrived(event, userData, userFlag);
	}
}

void EventDispatcher::dispatchEvent(uint32_t eventType, const void *userData, int userFlag) {
	//m_debugWindow->debugPrintf("Disptch event : %s\n", getEventTypeAsString((common::Service::GameEventType)eventType));
	Event event;
	event.type = eventType;
	for (int i = 0; i < _listeners.size(); i++) {
		EventListener* listener = _listeners.at(i);
		listener->onEventArrived(&event, userData, userFlag);
	}
}