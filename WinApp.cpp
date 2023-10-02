#include "WinApp.h"



LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	/*if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}*/

	switch (msg)
	{
	case WM_DESTROY:

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);


}
void WinApp::CreateGameWindow(const wchar_t* title, UINT windowStyle, int32_t clientWidth, int32_t clientHeight)
{


	//ウィンドウプローシャ
	wc_.lpfnWndProc = WindowProc;
	//ウィンドウクラス名
	wc_.lpszClassName = L"CG2WindowClass";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);
	//ウィンドウサイズを設定する
	const int32_t KCientwindow = 1280;
	const int32_t KCientHeight = 720;

	RECT wrc = { 0,0,KCientwindow,KCientHeight };

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	HWND hwnd = CreateWindow(
		wc_.lpszClassName,
		L"CG2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wrc.right - wrc.left,
		wrc.bottom - wrc.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr);
	ShowWindow(hwnd, SW_SHOW);

#/*ifdef _DEBUG
		ID3D12Debug1* debugController = nullptr;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
	
			debugController->EnableDebugLayer();
	
			debugController->SetEnableGPUBasedValidation(TRUE);
		}
	#endif*/

}

bool WinApp::ProcessMessage()
{
	MSG msg{};

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (msg.message == WM_QUIT)
	{
		return true;
	}
	return false;
}

