#include "DPI.h"
#pragma warning(disable : 4996)
float DPIScale::scaleX = 1.0f;
float DPIScale::scaleY = 1.0f;

void DPIScale::Initialize(ID2D1Factory* f)
{
	FLOAT x, y;
	f->GetDesktopDpi(&x, &y); // #pragma warning(disable : 4996)
	scaleX = x / 96.0f;
	scaleY = y / 96.0f;
}

void DPIScale::Initialize(HWND* hwnd)
{
	/*
	WINDOWS 10
	UINT d = GetDpiForWindow(*hwnd);
	scaleX = (float)d / 96.0f;
	scaleY = (float)d / 96.0f;*/
}

float DPIScale::PixelsToDipsX(float x)
{
	return x / scaleX;
}

float DPIScale::PixelsToDipsY(float y)
{
	return y / scaleY;
}