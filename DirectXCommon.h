#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <windows.h>
#include <string>
#include <format>



//メンバ関数を宣言してから、CPPに定義を書く
//資料を見て必要なメンバ変数をprivateで宣言する

class DirectXCommon
{
public:
	void Initilize();
private:

	void CreateFactory();

	void CreateAdapter();

	void InitilizeDevice();

	std::wstring ConvertString(const std::string& str);

	std::string ConvertString(const std::wstring& str);

	void Log(const std::string& message);

private:

	IDXGIFactory7* dxgiFactory_ = nullptr;
	IDXGIAdapter4* useAdapter_ = nullptr;
	DXGI_ADAPTER_DESC3 adapterDesc_{};


};
