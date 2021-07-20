#include "GO_Ellipse.h"
#include "SafeRelease.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "MouseEventManager.h"

void GO_Ellipse::Update()
{
}

void GO_Ellipse::Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	if (m_UpdateSize)
	{
		m_ellipse.point = D2D1::Point2F(m_size.x / 2.0f, m_size.y / 2.0f);
		m_ellipse.radiusX = m_size.x / 2.0f;
		m_ellipse.radiusY = m_size.y / 2.0f;
		m_UpdateSize = false;
		SetClipArea();
	}

	scb->SetColor(m_color);
	render->FillEllipse(m_ellipse, scb);
	scb->SetColor(m_colorStroke);
	render->DrawEllipse(m_ellipse, scb);
}

HRESULT GO_Ellipse::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text)
{
	return CreateGraphicObjectResources(render, facotry, text);
}

void GO_Ellipse::DiscardResources()
{
	DiscardGraphicObjectResources();
}

bool GO_Ellipse::HitTest(float x, float y)
{
	ID2D1EllipseGeometry* geo = NULL;
	if (m_factory != NULL)
	{
		if (IsHitTestVisible(x, y))
		{
			if (SUCCEEDED(m_factory->CreateEllipseGeometry(m_ellipse, &geo)))
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


void GO_Ellipse::OnMouseOver(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_Ellipse::OnMouseEnter(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_Ellipse::OnMouseLeave(meaof::obj_args& args)
{
	m_RenderClipArea = false;
}