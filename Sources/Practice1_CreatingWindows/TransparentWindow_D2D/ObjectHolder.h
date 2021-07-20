#pragma once
#include <d2d1.h>

class ObjectRenderHolder
{
protected:
	std::list<GraphicObject* > m_graphics;
public:
	virtual ~ObjectRenderHolder() {}

	virtual void Render(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) = 0;
	virtual HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text = NULL) = 0;
	virtual void DiscardResources() = 0;

	const std::list<GraphicObject* >* GetListP() const { return &m_graphics; }
	std::list<GraphicObject* >& GetList() { return m_graphics; }
};