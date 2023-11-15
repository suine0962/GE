#include "Suine.h"

void Suine::Initilize()
{
	//WinAPP
	winApp_->CreateGameWindow();
	
	//DirectXCommon
	directXCommon->CreateFactory();
	directXCommon->CreateAdapter();
	directXCommon->InitilizeDevice();
	directXCommon->InitializeCommand();
	directXCommon->CreateDebugLayer();
	directXCommon->CreateSwapChain();
	directXCommon->CreateRTVDescriptorHeap();
	directXCommon->CreateDepthStencilView();
	directXCommon->CreateFence();
	
	//ImGuiManager
	Imgui->Initialize(winApp_, directXCommon);

	//PipeLine
	PSO->Initilize();
	PSO->ShaderCompile();
	PSO->PSOCreate();

	//Camera
	camera->Initialize(winApp_);

	//TextureManager
	texManager->Initilize();

	//sprite

}




void Suine::WinMSG(MSG msg)
{
	winApp_->ProcessMessage();
}

void Suine::BeginFrame(const int32_t KClientWidth, const int32_t KClientHedth)
{
	directXCommon->CreatePreDraw();
	directXCommon->PostDraw();

	Imgui->BeginFlame(directXCommon);
}

void Suine::EndFrame()
{
	Imgui->EndFlame(directXCommon);
}


void Suine::Finalize()
{
	

}