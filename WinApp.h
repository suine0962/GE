#pragma once
#include<Windows.h>
#include<cstdint>
#include <d3d12.h>

//ウィンドウズアプリケーション
class WinApp
{
public:
	//ウィンドウサイズ
	static const int kWindowWidth = 1280;//横幅
	static const int kWindowHeight = 720;//縦幅
	//ウィンドウクラス名
	static const wchar_t kWindowClassName[];

	enum class SizeChangeMode {
		kNone,//サイズ変更不可
		kNormal,//サイズ自由変更
		kFixedAspect,//アスペクト比一定
	};

public://静的メンバ関数
	//シングルトンインスタンスの取得
	//<returns>シングルトンインスタンス</returns>
	static WinApp* GetInstance();
	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	static LRESULT WindowProc(HWND hwnd, UINT msg,
		WPARAM wparam, LPARAM lparam);


public://メンバ関数
	//ウィンドウの生成
	void CreateGameWindow(
		const wchar_t* title = L"CG2", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kWindowHeight);

	/*/// ゲームウィンドウの破棄
	void TerminateGameWindow();*/

	//メッセージの処理
	bool ProcessMessage();

	//ウィンドウハンドルの取得
	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHIntstance()const { return wndClass_.hInstance; }



private://メンバ関数
	/*WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator = (const WinApp&) = delete;*/


private: // メンバ変数
	// Window関連
	HWND hwnd_ = nullptr;   // ウィンドウハンドル
	WNDCLASSEX wndClass_{}; // ウィンドウクラス
	UINT windowStyle_;
	bool isFullscreen_ = false;
	RECT windowRect_;
	SizeChangeMode sizeChangeMode_ = SizeChangeMode::kNormal;
	float aspectRatio_;
	WNDCLASS wc_  {};

};

