#include "WindowsMain.h"
#include <chrono>
#include <thread>

int WINAPI wWinMain(HINSTANCE hInstacne, HINSTANCE, PWSTR, int cmdShow)
{
	using namespace std;
	using namespace std::chrono;
	WindowsMain mw;
	mw.SetOptions(RenderOptionsManager(
		RenderOptions::D2D
		, RenderMultithreading::SINGLE)
	);

	if (mw.Create(L"Transparent Window Test", WindowsStyle::Base, 200, 200, 900, 500) == FALSE)
	{
		MessageBox(NULL, L"Create Window Failed", L"ERROR", MB_OK);
		return 0;
	}

	ShowWindow(mw.Window(), cmdShow);
	mw.Render();
	steady_clock::duration dur = milliseconds(20);
	steady_clock::time_point tp = steady_clock::now() + dur;

	MSG msg = {};
	bool RUN = true;
	BOOL ok;
	while (RUN)
	{
		if (ok = GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else RUN = false;
		this_thread::sleep_for(milliseconds(20));
	}

	if (ok < 0)
	{
		DWORD er = GetLastError();
		std::wstring text = L"Something goes wrong:\nError code : " + std::to_wstring(er);
		MessageBox(NULL, text.c_str(), L"Error", MB_OK);
	}

	return 0;
}