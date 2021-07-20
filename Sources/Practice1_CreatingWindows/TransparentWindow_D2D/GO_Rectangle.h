#pragma once
#include "GraphicObject.h"

class GO_Rectangle : public GraphicObject
{
	D2D1_RECT_F m_rectangle;
public:
	GO_Rectangle() : m_rectangle(D2D1::RectF(0.0f, 0.0f, m_size.x, m_size.y)) {}

	void Update() override;
	void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* factory, IDWriteFactory* text = NULL) override;
	void DiscardResources() override;

	bool HitTest(float x, float y) override;

	void OnMouseOver(meaof::obj_args& args) override;
	void OnMouseEnter(meaof::obj_args& args) override;
	void OnMouseLeave(meaof::obj_args& args) override;
};