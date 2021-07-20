#pragma once

#include "GraphicObject.h"

class GO_Ellipse : public GraphicObject
{
	D2D1_ELLIPSE m_ellipse;

public:
	GO_Ellipse()
		//: m_colorBrush(NULL)
	{
		m_ellipse = { m_pos, m_size.x, m_size.y };
	}

	void Update() override;
	void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text = NULL) override;
	void DiscardResources() override;

	bool HitTest(float x, float y) override;

	void OnMouseOver(meaof::obj_args& args) override;
	void OnMouseEnter(meaof::obj_args& args) override;
	void OnMouseLeave(meaof::obj_args& args) override;
};