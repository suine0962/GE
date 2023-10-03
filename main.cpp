#include<Windows.h>
#include "WinApp.h"
#include"DirectXCommon.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字入力
	WinApp* winApp = new WinApp();
	DirectXCommon* directXCommon = new DirectXCommon();
	

	winApp->CreateGameWindow();
	directXCommon->Initilize(winApp);
	
	


	//ゲームシーン
	while (true)
	{
		directXCommon->CreatePreDraw();
		directXCommon->CreateDebugLayer();
		if (winApp->ProcessMessage() == true)
		{

			break;
		}
		
	}
	return 0;
}