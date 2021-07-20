#pragma once

template <class T> 
void SafeRelease(T** pT)
{
	if (*pT)
	{
		(*pT)->Release();
		*pT = NULL;
	}
}