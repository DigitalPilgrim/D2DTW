#include "ControlCanvas.h"

ControlCanvas::ControlCanvas()
{
	m_optimized = true;
}

void ControlCanvas::Update()
{
	SetClipArea();
}

void ControlCanvas::Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	// EMPTY
}

void ControlCanvas::DrawObject(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
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
				DrawChildrens(m_oRender.Render(), scb);
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
		DrawChildrens(render, scb);
	}

	DrawHelpers(render, scb);

	FinishTransform(render);
}

HRESULT ControlCanvas::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text)
{
	m_result = CreateGraphicObjectResources(render, facotry, text);
	
	if (SUCCEEDED(m_result))
	{
		CreateResourcesChildrens(render, facotry, text);
		return m_result;
	}
	else return m_result;
}

void ControlCanvas::DiscardResources()
{
	DiscardGraphicObjectResources();
}
