#include"DirectXCommon.h"

void DirectXCommon::Initilize(WinApp* winApp, int32_t backBufferWidth,
	int32_t backBufferHeight) {
	//nullptrチェック
	assert(winApp);
	assert(4 <= backBufferWidth && backBufferWidth_<=4096);
	assert(4 <= backBufferHeight && backBufferHedth_ <= 4096);

	winApp_ = winApp;
	backBufferWidth_ = backBufferWidth;
	backBufferHedth_ = backBufferHeight;
	
	CreateFactory();
	CreateAdapter();
	InitilizeDevice();
	InitializeCommand();
	CreateSwapChain();
	CreateDescriptorHeap();
}



void DirectXCommon:: Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());

}





void DirectXCommon::CreateFactory()
{
	dxgiFactory_ = nullptr;

	HRESULT	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_));

	assert(SUCCEEDED(hr));
}

void DirectXCommon::CreateAdapter()
{
	useAdapter_ = nullptr;
	DXGI_ADAPTER_DESC3 adapterDesc_{};

	//いい順にアダプタを頼む
	for(UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter_)) !=
		DXGI_ERROR_NOT_FOUND; ++i)
	{
		//アダプターの情報を取得する
		HRESULT hr = useAdapter_->GetDesc3(&adapterDesc_);
		assert(SUCCEEDED(hr));
		//ソフトウェアアダプタでなければ採用
		if (!(adapterDesc_.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			//採用したアダプタの情報をログに出力,wstringのほうなので注意
			Log(ComvertString(std::format(L"Use Adaptaer:{}\n", adapterDesc_.Description)));
			break;
		}
		//ソフトウェアアダプタの場合は見なかったことにする
		useAdapter_ = nullptr;
	}

	assert(useAdapter_ != nullptr);

}

void DirectXCommon::InitilizeDevice()
{
	device_ = nullptr;

	D3D_FEATURE_LEVEL featureLevels_[] = { D3D_FEATURE_LEVEL_12_2,D3D_FEATURE_LEVEL_12_1,D3D_FEATURE_LEVEL_12_0 };
	const char* featureLevelStrings_[3] = { "12.2,12.1,12.0" };
	//高い順に生成できるか試していく
	for (size_t i = 0; i < _countof(featureLevels_); ++i)
	{
		HRESULT hr = D3D12CreateDevice(useAdapter_, featureLevels_[i], IID_PPV_ARGS(&device_));
		//指定した機能レベルでデバイスが生成できたかを確認
		if (SUCCEEDED(hr))
		{
			//生成できたのでログ出力を行ってループを抜ける
			Log(std::format("FeatureLevel:{}\n", featureLevelStrings_[i]));
			break;
			
		}
	}
	//デバイスの生成がうまくいかなったので起動できない
	assert(device_ != nullptr);
	Log("Complete create D3D12Device!!!\n");//初期化完了のログを出す
}


void DirectXCommon::InitializeCommand()
{
	HRESULT result = S_FALSE;
	// コマンドアロケータを生成
	result = device_->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator_));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = device_->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator_, nullptr,
		IID_PPV_ARGS(&commandList_));
	assert(SUCCEEDED(result));

	// 標準設定でコマンドキューを生成

	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device_->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue_));
	assert(SUCCEEDED(result));


}



void DirectXCommon::CreateDebugLayer()
{
#ifdef _DEBUG
	debugController = nullptr;
	infoQueue = nullptr;
	
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		//デバッグレイヤーを有効かする
		debugController->EnableDebugLayer();
		//さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif _DEBUG

	
	


#ifdef _DEBUG

	D3D12_MESSAGE_ID denyIds[] = {
		//Dx12デバッグプレイヤーの相互作用バグによるエラーメッセージ
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
	};

	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		//抑制するレベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		//指定したメッセージの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		//やばいエラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION,true);
		//エラー時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		//警告時に止まる
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		//開放
		infoQueue->Release();

	}
#endif _DEBUG



}



void DirectXCommon::CreateSwapChain()
{
	swapChain_ = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.Width = backBufferWidth_;//画面の幅
	swapChainDesc.Height = backBufferHedth_;//画面の幅
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色の形式
	swapChainDesc.SampleDesc.Count = 1;//マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//描画のターゲットとして利用する
	swapChainDesc.BufferCount = 2;//ダブルブァッファ
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//モニタに移したら中身を破壊
	//コマンドキュー,ウィンドウハンドル,設定を渡して生成する
	HRESULT hr = dxgiFactory_->CreateSwapChainForHwnd(commandQueue_,winApp_->GetHwnd(), &swapChainDesc, nullptr, nullptr,
		reinterpret_cast<IDXGISwapChain1**>(&swapChain_));
	assert(SUCCEEDED(hr));


}




void DirectXCommon::CreateDescriptorHeap()
{
	rtvDescriptorHeap_ = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc_{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_{};


	HRESULT result = S_FALSE;
	rtvDescriptorHeapDesc_.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー用
	rtvDescriptorHeapDesc_.NumDescriptors = 2;//ダブルバッファ用に２つ
	result = device_->CreateDescriptorHeap(&rtvDescriptorHeapDesc_,
		IID_PPV_ARGS(&rtvDescriptorHeap_));
	//ディスクリぷたーヒープが作れなかったので起動できない
	assert(SUCCEEDED(result));
	//swapChainからResourcesを引っ張ってくる
	result = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[0]));
	//うまく取得できなければ起動できない
	assert(SUCCEEDED(result));

	result = swapChain_->GetBuffer(0, IID_PPV_ARGS(&swapChainResources_[1]));
	assert(SUCCEEDED(result));
	//RTVの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;//出力結果をSRGBに変換して書き込む
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;//2dテクスチャとして書き込む
	//ディスクリプタの先頭を取得する
	rtvStartHandle_ = rtvDescriptorHeap_->GetCPUDescriptorHandleForHeapStart();
	//RTVを2つ作るのでディスクリプタを二つ用意する
	//D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles_[2];
	
	//まず一つ目をつくる、作る場所をこちらで設定してあげる必要がある
	rtvHandles_[0] = rtvStartHandle_;
	device_->CreateRenderTargetView(swapChainResources_[0], &rtvDesc_, rtvHandles_[0]);
	//二つ目のディスクリプタハンドルを得る(自力で)
	rtvHandles_[1].ptr = rtvHandles_[0].ptr + device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//二つ目を作る
	device_->CreateRenderTargetView(swapChainResources_[1], &rtvDesc_, rtvHandles_[1]);


}


void DirectXCommon::CreatePreDraw()
{
	HRESULT result = S_FALSE;
	//バックバッファインデックスを取得
	backBufferIndex = swapChain_->GetCurrentBackBufferIndex();
	//描画先のRTVを設定する
	commandList_->OMSetRenderTargets(1, &rtvHandles_[backBufferIndex], false, nullptr);
	//指定した色で画面全体をクリアする
	float ClearColor_[] = { 0.1f,0.25f,0.5f,1.0f };
	commandList_->ClearRenderTargetView(rtvHandles_[backBufferIndex], ClearColor_, 0, nullptr);
	//コマンドリストの内容を確定させる.全てのコマンドを積んでからcloseすること
	result = commandList_->Close();
	assert(SUCCEEDED(result));
	//GPUにコマンドリストの実行を行わさせる
	ID3D12CommandList* commandLists_[] = { commandList_ };
	commandQueue_->ExecuteCommandLists(1, commandLists_);
	//GPUとOSの画面の交換を行うように通知する
	swapChain_->Present(1, 0);
	//次のフレーム用コマンドリストの準備
	result = commandAllocator_->Reset();
	assert(SUCCEEDED(result));
	result = commandList_->Reset(commandAllocator_, nullptr);
	assert(SUCCEEDED(result));
}







