#pragma once
#include <wincodec.h>
#pragma comment(lib, "windowscodecs")

class WICHelper
{
	IWICImagingFactory* pWICFactory;
	IWICBitmap* pBitmap;
	bool factoryInitialized;

	void Initialize()
	{
		factoryInitialized = false;
		HRESULT hr = CoInitialize(NULL);
		if (SUCCEEDED(hr))
		{
			hr = CoCreateInstance(CLSID_WICImagingFactory,
				NULL,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&pWICFactory));
			if (SUCCEEDED(hr))
			{
				factoryInitialized = true;
			}
		}
	}

public:
	WICHelper() : pWICFactory(NULL), pBitmap(NULL)
	{
		Initialize();
	}

	~WICHelper()
	{
		if (factoryInitialized)
		{
			pWICFactory->Release();
		}

		CoUninitialize();
	}

	void CreateBitmap(unsigned int width, unsigned int height)
	{
		if (pBitmap != NULL)
		{
			ReleaseBitmap();
		}
		pWICFactory->CreateBitmap(
			width,
			height,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapCacheOnLoad,
			&pBitmap);
	}

	void ReleaseBitmap()
	{
		if (pBitmap != NULL)
		{
			pBitmap->Release();
			pBitmap = NULL;
		}
	}

	IWICBitmap* GetBitmap() { return pBitmap; }
};