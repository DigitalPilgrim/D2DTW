#include "GO_Rectangle.h"
#include "SafeRelease.h"
#include "MouseEventManager.h"

void GO_Rectangle::Update()
{
	SetClipArea();
}

void GO_Rectangle::Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	/*m_rectangle.left = (m_size.x / 2.0f) * (- 1.0f);
	m_rectangle.top = (m_size.y / 2.0f) * (- 1.0f);
	m_rectangle.right = m_size.x / 2.0f;
	m_rectangle.bottom = m_size.y / 2.0f;*/
	m_rectangle.left = 0;
	m_rectangle.top = 0;
	m_rectangle.right = m_size.x;
	m_rectangle.bottom = m_size.y;

	scb->SetColor(m_color);
	render->FillRectangle(&m_rectangle, scb);
	scb->SetColor(m_colorStroke);
	render->DrawRectangle(&m_rectangle, scb);
}

HRESULT GO_Rectangle::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text)
{
	m_factory = factory;
	return CreateGraphicObjectResources(render, factory, text);
}

void GO_Rectangle::DiscardResources()
{
	DiscardGraphicObjectResources();
}

bool GO_Rectangle::HitTest(float x, float y)
{
	ID2D1RectangleGeometry* geo = NULL;
	if (m_factory != NULL)
	{
		if (IsHitTestVisible(x, y))
		{
			if (SUCCEEDED(m_factory->CreateRectangleGeometry(m_rectangle, &geo)))
			{
				BOOL hit;
				D2D1_MATRIX_3X2_F m = D2D1::Matrix3x2F::Identity();
				if (m_parent != nullptr)
					m = m_transport * m_mxRot * m_mxPos * m_mxScale * m_mxSkew;
				else m = m_mxRot * m_mxPos * m_mxScale * m_mxSkew;
				if (SUCCEEDED(geo->FillContainsPoint(D2D1::Point2F(x, y), m, &hit)))
				{
					SafeRelease(&geo);
					return HitTestG(hit, x, y, true);
				}
			}
		}
	}

	return false;
}

void GO_Rectangle::OnMouseOver(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_Rectangle::OnMouseEnter(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_Rectangle::OnMouseLeave(meaof::obj_args& args)
{
	m_RenderClipArea = false;
}