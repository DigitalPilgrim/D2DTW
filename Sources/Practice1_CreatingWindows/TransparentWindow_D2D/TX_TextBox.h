#pragma once

#include "GraphicObject.h"
#include <dwrite.h>
#include <string>

class TX_TextBox : public GraphicObject
{
	IDWriteTextFormat* m_TextFormat;
	std::wstring m_text;
	UINT32 m_text_size;
	float m_font_size;
	D2D1_RECT_F m_layout_rect;

public:
	TX_TextBox();
	TX_TextBox(std::wstring text);
	~TX_TextBox();

	void Text(std::wstring text);
	std::wstring Text() const;

	void FontSize(float size);
	float FontSize() const;

	void Update() override;
	void Draw(ID2D1RenderTarget* render, ID2D1SolidColorBrush* scb = NULL) override;
	HRESULT CreateResources(ID2D1RenderTarget* render, ID2D1Factory* facotry, IDWriteFactory* text = NULL) override;
	void DiscardResources() override;

	/*void Size(D2D1_POINT_2F size) override;
	D2D1_POINT_2F Size() const override;
	void Position(D2D1_POINT_2F position) override;
	D2D1_POINT_2F Position() const override;
	void Color(D2D1_COLOR_F color) override;
	D2D1_COLOR_F Color() const override;


	void Rotate(float angle, D2D_POINT_2F origin = D2D1::Point2F()) override;
	void Translate(D2D1_POINT_2F value) override;
	void Scale(D2D1_POINT_2F value, D2D_POINT_2F origin = D2D1::Point2F()) override;
	D2D1_SIZE_F Scale() override;
	float Rotate(bool ReturnAngle = true) override;
	D2D1_POINT_2F Translate() override;*/
};