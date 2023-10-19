#include<Windows.h>
#include "WinApp.h"
#include"DirectXCommon.h"
#include"TextureManager.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字入力
	WinApp* winApp = new WinApp();
	DirectXCommon* directXCommon = new DirectXCommon();
	TextureManeger* textureManeger = new TextureManeger();

	winApp->CreateGameWindow();
	directXCommon->Initilize(winApp);
	textureManeger->Initilize(winApp,directXCommon);
	
	


	//ゲームシーン
	while (true)
	{
		directXCommon->CreatePreDraw();
		textureManeger->Draw();
		directXCommon->PostDraw();
		textureManeger->RenewalCBuffer();
		if (winApp->ProcessMessage() == true)
		{

			break;
		}
		
	}
	directXCommon->CreateRelease();
	textureManeger->CreateRelease();
	directXCommon->CreateReportLive();
	return 0;
}