#include "MouseEventManager.h"
#include "DPI.h"

// --------------------------------------------------------------------------------------------------

//GraphicObject* MouseEventManager::m_root = nullptr;
GraphicObject* MouseEventManager::m_target = nullptr;
GraphicObject* MouseEventManager::m_target_pre = nullptr;
std::list<GraphicObject*>* MouseEventManager::m_root = nullptr;

D2D1_POINT_2F MouseEventManager::m_mouse_point;
meaof::dispatcher_holder MouseEventManager::m_dispatcher;
bool MouseEventManager::m_mouse_active = false;

// --------------------------------------------------------------------------------------------------

void MouseEventManager::SetMousePosI(meaof::obj_args& args)
{
	float x, y;
	if (args.get_object(x))
	{
		m_mouse_point.x = x;
	}
	if (args.get_object(y))
	{
		m_mouse_point.y = y;
	}
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::SetMousePos(float x, float y)
{
	D2D1_POINT_2F pixels = DPIScale::PixelToDips(x, y);
	meaof::obj_args args;
	args.AddObject(meaof::object(pixels.x));
	args.AddObject(meaof::object(pixels.y));
	m_dispatcher.get()->send_message(meaof::delegate_object(&MouseEventManager::SetMousePosI, args, true));
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::CheckHit()
{
	if (m_mouse_active)
	{
		bool hit = false;
		if (m_root != nullptr)
		{
			std::list< GraphicObject*>::iterator it = m_root->end();
			--it;
			for (;; it--)
			{
				if ((*it)->HitTest(m_mouse_point.x, m_mouse_point.y))
				{
					hit = true;
					break;
				}
				if (it == m_root->begin()) break;
			}
		}
		if (hit)
		{
			MouseEventManager::ResolveHitPre();
		}
		else
		{
			MouseEventManager::ResolveNoHitPre();
		}
	}
	else
	{
		m_mouse_point.x = -1.f;
		m_mouse_point.y = -1.f;

		ResolveNoHitPre();
	}
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::ResolveHit(GraphicObject* go)
{
	meaof::obj_args args;
	// Mouse Enter >> Mouse Over
	if (m_target == nullptr)
	{
		m_target = go;

		if (m_target->m_dispatcher.get() != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(go, &GraphicObject::OnMouseEnter, args));
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(go, &GraphicObject::OnMouseOver, args));
			/*m_target->OnMouseEnter(args);
			m_target->OnMouseOver(args);*/
		}
	}
	// Mouse Over
	else if (m_target == go)
	{
		if (m_target->m_dispatcher.get() != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(go, &GraphicObject::OnMouseOver, args));
			//m_target->OnMouseOver(args);
		}
	}
	// Mouse Leave >> Mouse Enter >> Mouse Over
	else
	{
		if (m_target->m_dispatcher.get() != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseLeave, args));
			//m_target->OnMouseLeave(args);
			m_target = go;
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(go, &GraphicObject::OnMouseEnter, args));
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(go, &GraphicObject::OnMouseOver, args));
			/*m_target->OnMouseEnter(args);
			m_target->OnMouseOver(args);*/
		}
	}
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::ResolveNoHit()
{
	if (m_target != nullptr)
	{
		if (m_target->m_dispatcher.get() != nullptr)
		{
			meaof::obj_args args;
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseLeave, args));
			m_target = nullptr;
		}
	}
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::ResolveHitPre()
{
	meaof::obj_args args;
	// Mouse Enter >> Mouse Over
	if (m_target == nullptr)
	{
		m_target = m_target_pre;

		if (m_target->m_dispatcher.get() != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseEnter, args));
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseOver, args));
		}
	}
	// Mouse Over
	else if (m_target == m_target_pre)
	{
		if (m_target->m_dispatcher.get() != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseOver, args));
		}
	}
	// Mouse Leave >> Mouse Enter >> Mouse Over
	else
	{
		if (m_target->m_dispatcher.get() != nullptr && m_target_pre != nullptr)
		{
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseLeave, args));
			m_target = m_target_pre;
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseEnter, args));
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseOver, args));
		}
	}
}

// --------------------------------------------------------------------------------------------------

void MouseEventManager::ResolveNoHitPre()
{
	if (m_target != nullptr)
	{
		if (m_target->m_dispatcher.get() != nullptr)
		{
			meaof::obj_args args;
			m_target->m_dispatcher.get()->send_message(
				meaof::delegate_object(m_target, &GraphicObject::OnMouseLeave, args));
			m_target = nullptr;
			m_target_pre = nullptr;
		}
	}
}

// --------------------------------------------------------------------------------------------------
