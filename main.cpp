#include<Windows.h>
#include "WinApp.h"

//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//出力ウィンドウへの文字入力
	WinApp* winApp = new WinApp();

	winApp->CreateGameWindow();




	//ゲームシーン
	while (true)
	{
		if (winApp->ProcessMessage() == true)
		{
			break;
		}
		
	}
	return 0;
}