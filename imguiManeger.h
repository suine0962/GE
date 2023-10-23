#pragma once
#include"DirectXCommon.h"
#include"WinApp.h"
#include"./externals/imgui/imgui.h"
#include"./externals/imgui/imgui_impl_win32.h"
#include"./externals/imgui/imgui_impl_dx12.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


class ImGuiManager
{
public:

	ImGuiManager();

	void Initialize(WinApp* winApp_,DirectXCommon* directX_);


	void BeginFlame(DirectXCommon *directX_);


	void EndFlame(DirectXCommon *directX_);

	void Release();
private:

};
