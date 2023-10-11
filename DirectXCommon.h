#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <windows.h>
#include <string>
#include <format>
#include"ComvertString.h"
#include"WinApp.h"
#include<dxgidebug.h>


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
#pragma comment(lib,"dxguid.lib")
//メンバ関数を宣言してから、CPPに定義を書く
//資料を見て必要なメンバ変数をprivateで宣言する

class DirectXCommon
{
public:
	void Initilize(WinApp*win,int32_t backBufferWidth=WinApp::kWindowWidth,
		int32_t backBufferHeight=WinApp::kWindowHeight);

#pragma region get

	ID3D12Device* Getdevice() { return device_; };
	ID3D12Resource* GetVertexResouce() { return vertexResouce_; };
	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() { return vertexBufferView_; };

#pragma endregion
	

private:

	WinApp* winApp_;


	IDXGIFactory7* dxgiFactory_ = nullptr;
	IDXGIAdapter4* useAdapter_ = nullptr;
	ID3D12Device* device_ = nullptr;


	

	ID3D12CommandQueue* commandQueue_ = nullptr;
	//コマンドアロケータ生成
	ID3D12CommandAllocator* commandAllocator_ = nullptr;
	//コマンドリストを生成する
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	//スワップチェーンを生成する
	IDXGISwapChain4* swapChain_ = nullptr;

	int32_t backBufferWidth_ = 0;
	int32_t backBufferHedth_ = 0;

	//vertexBufferViewの生成

	ID3D12Resource* vertexResouce_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	//ディスクリプタヒープの生成
	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	ID3D12Resource* swapChainResources_[2] = { nullptr };
	//RTV
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle_;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	//debugRayer
	UINT backBufferIndex_;
	ID3D12Debug1* debugController_ = nullptr;
	ID3D12InfoQueue* infoQueue_ = nullptr;
	//fence
	ID3D12Fence* fence_ = nullptr;
	UINT64 fenceValue_ = 0;
	HANDLE fenceEvent_ ;

	//ReportLiveObjects
	IDXGIDebug1* debug;
public:


	void CreatePreDraw();

	void CreateFactory();

	void CreateAdapter();

	void InitilizeDevice();

	void Log(const std::string& message);

	void InitializeCommand();

	void CreateDebugLayer();

	void CreateSwapChain();

	void CreateDescriptorHeap();

	void CreateFence();

	void CreateReportLive();

	void CreateRelease();

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);
};
