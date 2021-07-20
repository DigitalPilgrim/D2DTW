#pragma once
#include "GraphicObject.h"

class ControlCanvas : public GraphicObject
{
public:
	ControlCanvas();

	void Update() override;
	void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	void DrawObject(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text = NULL) override;
	void DiscardResources() override;
};