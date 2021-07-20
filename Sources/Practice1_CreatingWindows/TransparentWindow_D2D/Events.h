#pragma once

#include <meaof_object.h>
#include <meaof_event.h>

class IEvents
{
public:
	/*IEvents() {}
	IEvents(IEvents&& e) noexcept {}
	IEvents( IEvents& e) {}
	IEvents(const IEvents& e) {}*/
	virtual ~IEvents() {}

	virtual void OnMouseOver(meaof::obj_args& args) {}
	virtual void OnMouseEnter(meaof::obj_args& args) {}
	virtual void OnMouseLeave(meaof::obj_args& args) {}
	virtual void OnMouseButtonUp(meaof::obj_args& args) {}
	virtual void OnMouseButtonDown(meaof::obj_args& args) {}

	/*IEvents& operator=(IEvents& e) { return *this; }
	IEvents& operator=(const IEvents& e) { return *this; }*/

	/*meaof::event_manager event_OnMouseOver;
	meaof::event_manager event_OnMouseEnter;
	meaof::event_manager event_OnMouseLeave;*/
};