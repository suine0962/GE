#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"GraphicsPipeline.h"
#include"imguiManeger.h"
#include"Model.h"
#include"Camera.h"
#include"TextureManeger.h"
#include"Sprite.h"
#include"CreateResource.h"

class Suine
{
public:
	void Initilize(WinApp*winApp,DirectXCommon*directX,GraphicsPipeline*pso_,
		Camera*camera,TextureManeger*texManager,CreateResources*CResource);

	void Finalize();

	//ループする必要がある処理
	void WinMSG(MSG msg);

	void BeginFrame(const int32_t KClientWidth,const int32_t KClientHedth);

	void EndFrame();
private:

	WinApp* winApp_ = nullptr;

	DirectXCommon* directX_ = nullptr;

	GraphicsPipeline* PSO_ = nullptr;

	ImGuiManager* Imgui_ = nullptr;

	Model* model = nullptr;

	Camera* camera_ = nullptr;

	TextureManeger* texManager_ = nullptr;

	Sprite* sprite = nullptr;

	CreateResources* CResource_ = nullptr;
};

