#include "GraphicObject.h"
#include "SafeRelease.h"
#include "MouseEventManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

HRESULT GraphicObject::CreateGraphicObjectResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text)
{
	m_factory = factory;
	HRESULT hr = S_OK; 
	CreateResourcesChildrens(render, factory, text);
	hr = m_result;
	m_oRender.CreateResources(render);

	SetClipArea();
	/*if (m_colorBrush == NULL)
	* 
		hr = render->CreateSolidColorBrush(m_color, &m_colorBrush);
	if (SUCCEEDED(hr))
	{
		CreateResourcesChildrens(render, factory, text);
		hr = m_result;
	}*/
	return hr;
}

void GraphicObject::DiscardGraphicObjectResources()
{
	m_oRender.DiscardResources();
	SafeRelease(&m_colorBrush);
	SafeRelease(&m_mouse_detector);
	DiscardResourcesChildrens();
}

void GraphicObject::SetTransform(ID2D1RenderTarget* render)
{
	if (m_parent != nullptr)
	{
		render->GetTransform(&m_transport);
		render->SetTransform(m_transport * m_mxRot * m_mxPos * m_mxScale * m_mxSkew);
	}
	else
	{
		render->SetTransform(m_mxRot * m_mxPos * m_mxScale * m_mxSkew);
	}
}

void GraphicObject::FinishTransform(ID2D1RenderTarget* render)
{
	if (m_parent != nullptr)
	{
		render->SetTransform(m_transport);
	}
}

GraphicObject::GraphicObject() : m_colorBrush(NULL)
	, m_Update(false)
	, m_UpdateSize(false)
	, m_UpdatePos(false)
	, m_UpdateColor(false)
	, m_UpdateColorStroke(false)
	//, m_child(nullptr)
	, m_parent(nullptr)
	, m_result(S_OK)
	, m_factory(NULL)
	, m_mouse_detector(NULL)
	, m_IsHitTestVisible(true)
	, m_RenderAClipArea(false)
	, m_RenderClipArea(false)
	, m_IsClip(false)
	, m_optimized(false)
	, m_cp_area_update(false)
	, m_RenderCPArea(false)
{
	m_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_colorStroke = D2D1::ColorF(D2D1::ColorF::Black);
	m_pos = { 0 };
	m_size = { 10.f, 10.f };
	m_scale = { 1.0f, 1.0f };

	m_cp_area = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_transport = D2D1::Matrix3x2F::Identity();
	m_mxPos = D2D1::Matrix3x2F::Identity();
	m_mxRot = D2D1::Matrix3x2F::Identity();
	m_mxScale = D2D1::Matrix3x2F::Identity();
	m_mxSkew = D2D1::Matrix3x2F::Identity();
	m_angle = 0.0f;
}

//GraphicObject::GraphicObject(const GraphicObject& g)
//{
//}

bool GraphicObject::HitTestG(bool HitTrue, float x, float y, bool HitArea)
{
	
	bool hit = HitTrue;
	if (HitTrue)
	{
		MouseEventManager::SetObject(this);
	}
	if (HitArea)
	{
		if (m_childrens.size() > 0)
		{
			for (std::list<GraphicObject* >::iterator it = m_childrens.end();; )
			{
				--it;
				if ((*it)->HitTest(x, y)) { hit = true; break; }
				if (it == m_childrens.begin()) break;
			}
		}
	}
	

	return hit;
}

void GraphicObject::DrawObject(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	SetTransform(render);

	if (m_optimized)
	{
		if (m_oRender.IsCashed() == true && m_oRender.NeedUpdate() != true)
		{
			m_oRender.Render(m_AClipArea);
		}
		else
		{
			if (m_oRender.CreateRender(D2D1::SizeF(m_AClipArea.right, m_AClipArea.bottom)))
			{
				m_oRender.Render()->BeginDraw();
				m_oRender.Render()->Clear(D2D1::ColorF(D2D1::ColorF::Black, 0.0f));
				Draw(m_oRender.Render(), scb);
				m_oRender.Render()->EndDraw();
				if (m_oRender.CreateBitmap())
				{
					m_oRender.Render(m_AClipArea);
					m_oRender.NeedUpdate(false);
				}
			}
		}
	}
	else
	{
		Draw(render, scb);
	}
	

	DrawHelpers(render, scb);

	DrawChildrens(render, scb);

	FinishTransform(render);
}

void GraphicObject::DrawHelpers(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	if (m_RenderClipArea)
	{
		scb->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow)); // , 0.5f
		render->DrawRectangle(m_area, scb);
	}
	if (m_RenderAClipArea)
	{
		scb->SetColor(D2D1::ColorF(D2D1::ColorF::Yellow));
		D2D1::Matrix3x2F zaloha;
		render->GetTransform(&zaloha);
		render->SetTransform(D2D1::Matrix3x2F::Identity());
		render->DrawRectangle(m_AClipArea, scb);
		render->SetTransform(zaloha);
	}
	if (m_RenderCPArea)
	{
		scb->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		D2D1::Matrix3x2F zaloha;
		render->GetTransform(&zaloha);
		render->SetTransform(D2D1::Matrix3x2F::Identity());
		render->DrawRectangle(m_cp_area, scb);
		render->SetTransform(zaloha);
	}
}

void GraphicObject::Size(D2D1_POINT_2F size)
{
	m_size = size;
	m_UpdateSize = true;
	m_oRender.NeedUpdate(true);
	SetClipArea();
}

D2D1_POINT_2F GraphicObject::Size() const
{
	return m_size;
}

void GraphicObject::Position(D2D1_POINT_2F position)
{
	m_pos = position;
	m_mxPos = D2D1::Matrix3x2F::Translation(D2D1::SizeF(m_pos.x, m_pos.y));
	m_UpdatePos = true;
	SetClipArea();
}

D2D1_POINT_2F GraphicObject::Position() const
{
	return m_pos;
}

void GraphicObject::Color(D2D1_COLOR_F color)
{
	m_color = color;
	m_UpdateColor = true;
}

D2D1_COLOR_F GraphicObject::Color() const
{
	return m_color;
}

void GraphicObject::Rotate(float angle)
{
	// origin preonacit na m_pos.x + m_size.x * percent (z 0 na 1) ...
	m_angle = angle;
	//m_mxRot = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(m_pos.x - m_size.x, m_pos.y - m_size.y));
	//m_mxRot = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(m_pos.x, m_pos.y));
	m_mxRot = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(m_size.x / 2.0f, m_size.y / 2.0f));
	//m_transport = m_transport * m_mxRot;
	//m_mxRot = D2D1::Matrix3x2F::Rotation(angle);
	m_oRender.NeedUpdate(true);
	SetClipArea();
}

void GraphicObject::Rotate(float angle, D2D_POINT_2F origin)
{
	// origin preonacit na m_pos.x + m_size.x * percent (z 0 na 1) ...
	m_angle = angle;
	m_mxRot = D2D1::Matrix3x2F::Rotation(angle, origin);
	m_oRender.NeedUpdate(true);
	SetClipArea();
}

void GraphicObject::Translate(D2D1_POINT_2F value)
{
	m_pos = value;
	m_mxPos = D2D1::Matrix3x2F::Translation(D2D1::SizeF(value.x, value.y));
	SetClipArea();
}

void GraphicObject::Scale(D2D1_POINT_2F value)
{
	// origin preonacit na m_pos.x + m_size.x * percent (z 0 na 1) ...
	m_scale.width = value.x;
	m_scale.height = value.y;
	m_mxScale = D2D1::Matrix3x2F::Scale(m_scale
		, D2D1::Point2F(m_pos.x + m_size.x / 2.f, m_pos.y + m_size.y / 2.f)
	);
	//m_UpdateSize = true;
	m_oRender.NeedUpdate(true);
	SetClipArea();
}

void GraphicObject::Scale(D2D1_POINT_2F value, D2D_POINT_2F origin)
{
	m_scale.width = value.x;
	m_scale.height = value.y;
	m_mxScale = D2D1::Matrix3x2F::Scale(m_scale, origin); 
	//m_UpdateSize = true;
	m_oRender.NeedUpdate(true);
	SetClipArea();
}

D2D1_SIZE_F GraphicObject::Scale()
{
	return m_scale;
}

float GraphicObject::Rotate(bool ReturnAngle)
{
	if (ReturnAngle)
		return m_angle;
	return sinf((m_angle / 360.0f) * static_cast<float>(M_PI) * 2.0f);
}

D2D1_POINT_2F GraphicObject::Translate()
{
	return m_pos;
}

void GraphicObject::AddChild(GraphicObject* o)
{
	o->m_parent = this;
	m_childrens.push_back(o);
}

void GraphicObject::SetHitTestVisible(bool TrueVisible)
{
	m_IsHitTestVisible = TrueVisible;
}

bool GraphicObject::IsHitTestVisible(float x, float y) const
{
	if (m_IsHitTestVisible)
	{
		if (m_IsClip)
		{
			if (m_AClipArea.left <= x && m_AClipArea.top <= y)
			{
				if (m_AClipArea.right >= x && m_AClipArea.bottom >= y)
				{
					return true;
				}
			}
		}
		else return true;
	}
	return false;
}

D2D1_RECT_F GraphicObject::GetAbsoluteClipArea() const
{
	return m_AClipArea;
}

//GraphicObject& GraphicObject::operator=(const GraphicObject& g)
//{
//	if (this != &g)
//	{
//		this->m_AClipArea = g.m_AClipArea;
//		this->m_area = g.m_area;
//		this->m_angle = g.m_angle;
//		this->m_childrens = g.m_childrens;
//		this->m_color = g.m_color;
//		this->m_colorStroke = g.m_colorStroke;
//		this->m_dispatcher = g.m_dispatcher;
//		this->m_factory = g.m_factory;
//		this->m_IsClip = g.m_IsClip;
//		this->m_IsHitTestVisible = g.m_IsHitTestVisible;
//		this->m_mouse_detector = g.m_mouse_detector;
//		this->m_mxPos = g.m_mxPos;
//		this->m_mxRot = g.m_mxRot;
//		this->m_mxScale = g.m_mxScale;
//		this->m_mxSkew = g.m_mxSkew;
//		this->m_parent = g.m_parent;
//		this->m_pos = g.m_pos;
//		this->m_RenderAClipArea = g.m_RenderAClipArea;
//		this->m_RenderClipArea = g.m_RenderClipArea;
//		this->m_scale = g.m_scale;
//		this->m_size = g.m_size;
//		this->m_transport = g.m_transport;
//
//	}
//	return *this;
//}

//void GraphicObject::OnMouseOverG(meaof::obj_args& args)
//{
//
//}

void GraphicObject::AddParent(GraphicObject* o)
{
	m_parent = o;
}

void GraphicObject::DrawChildrens(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	bool end = false;
	if (m_childrens.size() > 0)
	{
		if (m_IsClip)
		{
			render->PushAxisAlignedClip(m_area, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			/*if (m_parent != nullptr)
			{
				render->PushAxisAlignedClip(m_area, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			}
			else
			{
				render->PushAxisAlignedClip(m_area, D2D1_ANTIALIAS_MODE::D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
			}*/
			end = true;
		}

		for (GraphicObject* o : m_childrens)
		{
			o->DrawObject(render, scb);
		}

		if (end)
		{
			render->PopAxisAlignedClip();
		}
	}
}

void GraphicObject::CreateResourcesChildrens(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text)
{
	if (m_childrens.size() > 0)
	{
		for (GraphicObject* o : m_childrens)
		{
			m_result = o->CreateResources(render, factory, text);
		}
	}
}

void GraphicObject::DiscardResourcesChildrens()
{
	if (m_childrens.size() > 0)
	{
		for (GraphicObject* o : m_childrens)
		{
			o->DiscardResources();
		}
	}
}

void GraphicObject::SetClipArea()
{
	m_area.left = 0;
	m_area.top = 0;
	m_area.right = m_size.x;
	m_area.bottom = m_size.y;

	if (m_factory != NULL)
	{
		ID2D1RectangleGeometry* geo;
		if (SUCCEEDED(m_factory->CreateRectangleGeometry(m_area, &geo)))
		{
			D2D1_MATRIX_3X2_F m = D2D1::Matrix3x2F::Identity();
			if (m_parent != nullptr)
				m = m_transport * m_mxRot * m_mxPos * m_mxScale * m_mxSkew;
			else m = m_mxRot * m_mxPos * m_mxScale * m_mxSkew;
			D2D1_RECT_F bounds;
			if (SUCCEEDED(geo->GetBounds(m, &bounds)))
			{
				m_cp_area = m_AClipArea = bounds;
				SafeRelease(&geo);
			}
		}
	}

	if (m_parent != nullptr)
	{
		m_cp_area = m_parent->m_AClipArea;
		if (m_AClipArea.top < m_cp_area.top)
		{
			m_cp_area.top = m_AClipArea.top;
		}
		if (m_AClipArea.left < m_cp_area.left)
		{
			m_cp_area.left = m_AClipArea.left;
		}
		if (m_AClipArea.bottom > m_cp_area.bottom)
		{
			m_cp_area.bottom = m_AClipArea.bottom;
		}
		if (m_AClipArea.right > m_cp_area.right)
		{
			m_cp_area.right = m_AClipArea.right;
		}
		m_parent->m_cp_area_update = true;
	}

	/*if (m_parent != nullptr)
	{
		m_AClipArea = m_parent->GetAbsoluteClipArea();
		m_AClipArea.left += m_pos.x;
		m_AClipArea.top += m_pos.y;
		m_AClipArea.right = m_AClipArea.left + m_size.x;
		m_AClipArea.bottom = m_AClipArea.top + m_size.y;
	}
	else
	{
		m_AClipArea.left = m_pos.x;
		m_AClipArea.top = m_pos.y;
		m_AClipArea.right = m_pos.x + m_size.x;
		m_AClipArea.bottom = m_pos.y + m_size.y;
	}*/

	if (m_childrens.size() > 0)
	{
		for (GraphicObject* o : m_childrens)
		{
			o->SetClipArea();
		}
	}

	if (m_cp_area_update)
	{
		GraphicObject* go = nullptr;
		for (std::list<GraphicObject*>::iterator it = m_childrens.begin(); ;)
		{
			go = (*it);
			if (go->m_cp_area.top < m_AClipArea.top)
			{
				m_cp_area.top = go->m_cp_area.top;
				if (!m_IsClip)
					m_AClipArea.top = go->m_cp_area.top;
			}
			if (go->m_cp_area.left < m_AClipArea.left)
			{
				m_cp_area.left = go->m_cp_area.left;
				if (!m_IsClip)
					m_AClipArea.left = go->m_cp_area.left;
			}
			if (go->m_cp_area.right > m_AClipArea.right)
			{
				m_cp_area.right = go->m_cp_area.right;
				if (!m_IsClip)
					m_AClipArea.right = go->m_cp_area.right;
			}
			if (go->m_cp_area.bottom > m_AClipArea.bottom)
			{
				m_cp_area.bottom = go->m_cp_area.bottom;
				if (!m_IsClip)
					m_AClipArea.bottom = go->m_cp_area.bottom;
			}
			++it;
			if (it == m_childrens.end()) break;
		}
		m_cp_area_update = false;
	}
}

HRESULT GraphicObject::CreateMouseDetector()
{
	if (m_factory != NULL)
	{
		SafeRelease(&m_mouse_detector);
		return m_factory->CreateRectangleGeometry(m_area, &m_mouse_detector);
	}
	else return S_OK;
}
