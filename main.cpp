#include<Windows.h>
#include "WinApp.h"
#include"DirectXCommon.h"
#include"imguiManeger.h"
#include"TextureManeger.h"
#include"Sphere.h"
#include"suine.h"
#include"CreateResource.h"
#include"Model.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字入力

	const int32_t	KCientwidth = 1280;
	const int32_t	KCientHeight = 720;

	MSG msg{};

	Suine* suine = new Suine();
	WinApp* winApp = new WinApp();
	DirectXCommon* directXCommon = new DirectXCommon();
	GraphicsPipeline* pso = new GraphicsPipeline();
	ImGuiManager* imguiManeger = new ImGuiManager();
	TextureManeger* texManeger = new TextureManeger();
	Camera* camera = new Camera();
	CreateResources* CResource = new CreateResources();

	suine->Initilize(winApp, directXCommon, pso, camera, texManeger,CResource);

	Transform cameraTransform
	{
		cameraTransform.Scale = {1,1,1},
		cameraTransform.rotate = {0,0,-0.5},
		cameraTransform.translate = {0,0,0}
	};

	WorldTransform worldTransform[3];

	worldTransform[0].Initilize();
	worldTransform[1].Initilize();
	worldTransform[2].Initilize();

	Model* model = new Model();

	Vector4 pos = { 0,0,0,1 };

	float size = 1;

	texResourceProperty UVtex =
		texManeger->LoadTexture("DefaultResources/uvChecker.png");
	texResourceProperty Monstertex =
		texManeger->LoadTexture("DefaultResources/monsterBall.png");

	model->Initilize(directXCommon, CResource, pso, pos, size, worldTransform[0], UVtex, sphere);

	Sprite* sprite = new Sprite();

	sprite->Initilize(CResource,pso,directXCommon,{ 0,0 }, 320, worldTransform[1], UVtex, Box);

	bool texFlag = false;


	//ゲームシーン
	while (true)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			suine->WinMSG(msg);

		}
		suine->BeginFrame(KCientwidth, KCientHeight);

		ImGui::Begin("sphere");
		ImGui::SliderFloat4("Trans", &worldTransform[0].transform_.x, -10.0f, 10.0f);
		ImGui::SliderFloat4("rotate", &worldTransform[0].rotation_.x, -10.0f, 10.0f);
		ImGui::Checkbox("tex", &texFlag);
		ImGui::End();

		if (texFlag)
		{
			model->SetTexPropety(Monstertex);

		}
		else
		{
			model->SetTexPropety(UVtex);
		}

		Matrix4x4 SphereWorldMatrix = MakeAffineMatrix(
			worldTransform[0].scale_,
			worldTransform[0].rotation_,
			worldTransform[0].transform_);

		SphereWorldMatrix = camera->worldViewProjectionMatrix(SphereWorldMatrix);

		model->TransferMatrix(SphereWorldMatrix);

		model->Draw();


		ImGui::Begin("Camera");
		ImGui::SliderFloat3("trans", &cameraTransform.translate.x, -10.0f, 10.0f);
		ImGui::End();

		camera->SetPosition(cameraTransform);

		ImGui::Begin("sprite");
		ImGui::SliderFloat4("Trans", &worldTransform[1].transform_.x, -10.0f, 10.0f);
		ImGui::SliderFloat4("rotate", &worldTransform[1].rotation_.x, -10.0f, 10.0f);
		ImGui::Checkbox("tex", &texFlag);
		ImGui::End();


		Matrix4x4 SpriteWorldMatrix = MakeAffineMatrix(
			worldTransform[1].scale_,
			worldTransform[1].rotation_,
			worldTransform[1].transform_);

		SpriteWorldMatrix = camera->worldViewProjectionMatrix(SpriteWorldMatrix);

		sprite->TransforMatrix(SpriteWorldMatrix);

		sprite->Draw();

		suine->EndFrame();
	}

	UVtex = texManeger->Release(UVtex);
	Monstertex = texManeger->Release(Monstertex);
	
	sprite->Release();
	model->Release();

	suine->Finalize();

	return 0;
}