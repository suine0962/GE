#include "Suine.h"

void Suine::Initilize(WinApp*winApp,DirectXCommon* directX,
	GraphicsPipeline*pso,Camera* camera,TextureManeger*texManager,
	CreateResources* CResource)
{

	winApp_ = winApp;
	directX_ = directX;
	PSO_ = pso;
	camera_ = camera;
	texManager_ = texManager;
	CResource_ = CResource;

	//WinAPP
	winApp_->Initilize();
	
	//DirectXCommon

	directX_->Initilize(winApp_);

	directX_->CreateFactory();

	directX_->CreateAdapter();

	directX_->CreateDebugLayer();

	directX_->InitilizeDevice();

	directX_->InitializeCommand();

	directX_->CreateSwapChain();

	directX_->CreateRTVDescriptorHeap();

	directX_->CreateSettingRTV();

	directX_->CreateVDescriptorHeap();

	directX_->CreateFence();
	
	//ImGuiManager
	Imgui_->Initialize(winApp_, directX_);

	//PipeLine
	PSO_->Initilize(directX_);
	PSO_->ShaderCompile();
	PSO_->PSOCreate();

	//Camera
	camera_->Initialize(winApp_);

	//TextureManager
	texManager_->Initilize(directX_);

	//CreateResource
	CResource_->Initilize(directX_);
	//sprite

}




void Suine::WinMSG(MSG msg)
{
	winApp_->ProcessMessage();
}

void Suine::BeginFrame(const int32_t KClientWidth, const int32_t KClientHedth)
{
	directX_->CreatePreDraw();
	directX_->PostDraw();

	Imgui_->BeginFlame(directX_);
}

void Suine::EndFrame()
{
	Imgui_->EndFlame(directX_);
}


void Suine::Finalize()
{
	directX_->Finalize();

}