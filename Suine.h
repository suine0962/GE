#pragma once
#include"WinApp.h"
#include"DirectXCommon.h"
#include"GraphicsPipeline.h"
#include"imguiManeger.h"
#include"Model.h"
#include"Camera.h"
#include"TextureManeger.h"
#include"Sprite.h"


class Suine
{
public:
	void Initilize();

	void Finalize();

	//ループする必要がある処理
	void WinMSG(MSG msg);

	void BeginFrame(const int32_t KClientWidth,const int32_t KClientHedth);

	void EndFrame();
private:

	WinApp* winApp_ = nullptr;

	DirectXCommon* directXCommon = nullptr;

	GraphicsPipeline* PSO = nullptr;

	ImGuiManager* Imgui = nullptr;

	Model* model = nullptr;

	Camera* camera = nullptr;

	TextureManeger* texManager = nullptr;

	Sprite* sprite = nullptr;
};

