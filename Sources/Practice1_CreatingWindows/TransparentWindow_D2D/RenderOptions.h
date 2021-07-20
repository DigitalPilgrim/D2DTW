#pragma once

enum class RenderOptions
{
	D2D
	, D2DWIC
	, D3DDXGI
};

enum class RenderMultithreading
{
	SINGLE, MULTI
};

struct RenderOptionsManager
{
	RenderOptions Type;
	RenderMultithreading Multithreading;

	RenderOptionsManager()
	{
		Type = RenderOptions::D2D;
		Multithreading = RenderMultithreading::SINGLE;
	}

	RenderOptionsManager(RenderOptions ro, RenderMultithreading rm = RenderMultithreading::SINGLE)
	{
		Type = ro;
		Multithreading = rm;
	}
};