#pragma once

#include "GraphicObject.h"

class GO_GeometryEllipse : public GraphicObject
{
	ID2D1Factory* m_factory;
	ID2D1BitmapRenderTarget* m_bRT;
	ID2D1EllipseGeometry* m_ellipseGeometry;
	D2D1_ELLIPSE m_ellipse;
public:
	GO_GeometryEllipse() 
		: m_ellipseGeometry(NULL)
		, m_factory(NULL)
		, m_ellipse(D2D1::Ellipse(D2D1::Point2F(), m_size.x, m_size.y)) {}

	void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL) override; // , ID2D1Factory * facotry
	void DiscardResources() override;

	bool HitTest(float x, float y) override;

	void OnMouseOver(meaof::obj_args& args) override;
	void OnMouseEnter(meaof::obj_args& args) override;
	void OnMouseLeave(meaof::obj_args& args) override;
};