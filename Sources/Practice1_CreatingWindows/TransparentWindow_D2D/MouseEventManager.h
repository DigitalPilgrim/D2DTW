#pragma once

#include "GraphicObject.h"
#include "Events.h"
#include <meaof_dispatcher.h>

class MouseEventManager
{
	//static GraphicObject* m_root;
	static std::list< GraphicObject*> * m_root;
	static GraphicObject* m_target;
	static GraphicObject* m_target_pre;
	static D2D1_POINT_2F m_mouse_point;

	static bool m_mouse_active;

	static void SetMousePosI(meaof::obj_args& args);
public:
	static meaof::dispatcher_holder m_dispatcher;

	static void SetMousePos(float x, float y);

	//static void SetRoot(GraphicObject* o) { m_root = o; }
	static void SetRoot(std::list< GraphicObject*>& o) { m_root = &o; }

	static void SetObject(GraphicObject* o) { m_target_pre = o; }

	static void CheckHit();
	//MouseEventManager() : m_target(nullptr) {}
	static void ResolveHit(GraphicObject* go);

	static void ResolveNoHit();

	static void ResolveHitPre();

	static void ResolveNoHitPre();

	static void MouseLeave() { m_mouse_active = false; }
	static void MouseActive() { m_mouse_active = true; }

};
