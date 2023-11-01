#include<Windows.h>
#include "WinApp.h"
#include"DirectXCommon.h"
#include"Engine.h"
#include"imguiManeger.h"
#include"TextureManeger.h"

//まだ理解が浅いけど球体やスプライトをクラス化するか



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字入力

	WinApp* winApp = new WinApp();
	DirectXCommon* directXCommon = new DirectXCommon();
	Engine* engine_ = new Engine();
	ImGuiManager* imguiManeger = new ImGuiManager();
	TextureManeger* texManeger = new TextureManeger();

	winApp->CreateGameWindow();
	directXCommon->Initilize(winApp);
	engine_->Initilize(winApp,directXCommon,texManeger);
	imguiManeger->Initialize(winApp,directXCommon);
	
	//ゲームシーン
	while (true)
	{
		imguiManeger->BeginFlame(directXCommon);
		directXCommon->CreatePreDraw();
		engine_->Draw();
		imguiManeger->EndFlame(directXCommon);
		directXCommon->PostDraw();
		engine_->RenewalCBuffer();

		if (winApp->ProcessMessage() == true)
		{
			break;
		}
	}

	directXCommon->CreateRelease();
	engine_->CreateRelease();
	directXCommon->CreateReportLive();
	
	return 0;
}