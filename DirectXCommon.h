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
	ID3D12Device* Getdevice(){ return device_; };
	ID3D12GraphicsCommandList* GetcommandList() { return commandList_; };
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc_() { return rtvDesc_; };
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc_() { return swapChainDesc; };
	ID3D12DescriptorHeap* GetSrvDescriptorHeap_() { return  srvDescriptorHeap_; };

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
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	int32_t backBufferWidth_ = 0;
	int32_t backBufferHedth_ = 0;


	//ディスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};
	ID3D12DescriptorHeap* rtvDescriptorHeap_ = nullptr;
	ID3D12DescriptorHeap* srvDescriptorHeap_ = nullptr;
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
	ID3D12DescriptorHeap* CreateDescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible);

	void CreatePreDraw();

	void CreateFactory();

	void CreateAdapter();

	void InitilizeDevice();

	void Log(const std::string& message);

	void InitializeCommand();

	void CreateDebugLayer();

	void CreateSwapChain();

	void CreateRTVDescriptorHeap();

	void CreateFence();

	void CreateReportLive();

	void CreateRelease();

	void PostDraw();

	
};
