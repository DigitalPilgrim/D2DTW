#include "TX_TextBox.h"
#include "SafeRelease.h"

TX_TextBox::TX_TextBox()
	: m_TextFormat(nullptr)
	, m_text(L"")
	, m_text_size(0)
	, m_font_size(16.0f)
	, m_layout_rect(D2D1::RectF(1.0f, 1.0f))
{

}

TX_TextBox::TX_TextBox(std::wstring text)
	: m_TextFormat(nullptr)
	, m_text(text)
	, m_text_size(static_cast<UINT32>(wcslen(m_text.c_str())))
	, m_font_size(16.0f)
	, m_layout_rect(D2D1::RectF(1.0f, 1.0f))
{
}

TX_TextBox::~TX_TextBox()
{

}

void TX_TextBox::Text(std::wstring text)
{
	m_text = text;
	m_text_size = static_cast<UINT32>(wcslen(m_text.c_str()));
}

std::wstring TX_TextBox::Text() const
{
	return m_text;
}

void TX_TextBox::FontSize(float size)
{
	m_font_size = size;
}

float TX_TextBox::FontSize() const
{
	return m_font_size;
}

void TX_TextBox::Update()
{
}

void TX_TextBox::Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb)
{
	/*if (m_UpdatePos)
	{
		m_UpdatePos = false;
		m_Update = true;
	}
	if (m_UpdateSize)
	{
		m_UpdateSize = false;
		m_Update = true;
	}
	if (m_UpdateColor)
	{
		m_UpdateColor = false;
		m_Update = true;
	}

	if (m_Update)
	{
		m_Update = false;
	}*/

	/*m_layout_rect.top = (m_size.y / 2.0f) * (- 1.0f);
	m_layout_rect.left = (m_size.x / 2.0f) * (-1.0f);
	
	m_layout_rect.right = m_size.x / 2.0f;
	m_layout_rect.bottom = m_size.y / 2.0f;*/

	m_layout_rect.top = 0;
	m_layout_rect.left = 0;

	m_layout_rect.right = m_size.x;
	m_layout_rect.bottom = m_size.y;

	//SetClipArea();

	scb->SetColor(m_color);
	render->DrawRectangle(&m_layout_rect, scb);
	render->DrawTextW(
		m_text.c_str(), m_text_size, m_TextFormat, m_layout_rect, scb
		//, D2D1_DRAW_TEXT_OPTIONS::D2D1_DRAW_TEXT_OPTIONS_CLIP
	);
	
}

HRESULT TX_TextBox::CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text)
{
	HRESULT hr = CreateGraphicObjectResources(render, facotry, text);
	if (SUCCEEDED(hr))
	{
		if (text != NULL)
		{
			hr = text->CreateTextFormat(
				L"Gabriola"
				, NULL
				, DWRITE_FONT_WEIGHT_REGULAR
				, DWRITE_FONT_STYLE_NORMAL
				, DWRITE_FONT_STRETCH_NORMAL
				, m_font_size
				, L"en-us"
				, &m_TextFormat
			);
			if (SUCCEEDED(hr))
			{
				hr = m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER); // DWRITE_TEXT_ALIGNMENT_CENTER

				if (SUCCEEDED(hr))
				{
					hr = m_TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR); // DWRITE_PARAGRAPH_ALIGNMENT_CENTER
				}
			}
		}
	}
	return hr;
}

void TX_TextBox::DiscardResources()
{
	DiscardGraphicObjectResources();
	SafeRelease(&m_TextFormat);
}