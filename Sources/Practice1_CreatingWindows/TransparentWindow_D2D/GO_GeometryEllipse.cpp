#include "GO_GeometryEllipse.h"
#include "SafeRelease.h"
#include "MouseEventManager.h"

void GO_GeometryEllipse::Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	if (m_UpdateSize)
	{
		m_ellipse.point = D2D1::Point2F(m_size.x / 2.0f, m_size.y / 2.0f);
		m_ellipse.radiusX = m_size.x / 2.0f;
		m_ellipse.radiusY = m_size.y / 2.0f;
		if (m_ellipseGeometry != NULL) SafeRelease(&m_ellipseGeometry);
		m_factory->CreateEllipseGeometry(m_ellipse, &m_ellipseGeometry);
		m_UpdateSize = false;
		//SetClipArea();
	}
	
	if (m_ellipseGeometry != NULL)
	{
		scb->SetColor(m_color);
		render->FillGeometry(m_ellipseGeometry, scb);
		//render->FillEllipse(m_ellipse, scb);
		scb->SetColor(m_colorStroke);
		render->DrawGeometry(m_ellipseGeometry, scb);
		//render->DrawEllipse(m_ellipse, scb);
	}
}

HRESULT GO_GeometryEllipse::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text)
{
	m_factory = factory;
	m_factory->AddRef();

	HRESULT hr = m_factory->CreateEllipseGeometry(m_ellipse, &m_ellipseGeometry);
	if (SUCCEEDED(hr))
	{
		hr = CreateGraphicObjectResources(render, factory, text);
	}
	return hr;
}

void GO_GeometryEllipse::DiscardResources()
{
	SafeRelease(&m_ellipseGeometry);
	SafeRelease(&m_factory);
	DiscardGraphicObjectResources();
}

bool GO_GeometryEllipse::HitTest(float x, float y)
{
	ID2D1EllipseGeometry* geo = NULL;
	bool hitArea = false;
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

void GO_GeometryEllipse::OnMouseOver(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_GeometryEllipse::OnMouseEnter(meaof::obj_args& args)
{
	m_RenderClipArea = true;
}

void GO_GeometryEllipse::OnMouseLeave(meaof::obj_args& args)
{
	m_RenderClipArea = false;
}
