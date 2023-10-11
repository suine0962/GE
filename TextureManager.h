#pragma once
#include"DirectXCommon.h"
#include<dxcapi.h>

#pragma comment(lib,"dxcompiler.lib")


class TextureManeger
{
public:
	void Initilize(WinApp* win, DirectXCommon* directX ,int32_t backBufferWidth = WinApp::kWindowWidth,
		int32_t backBufferHeight = WinApp::kWindowHeight);
public:
	void Log(const std::string& message);

	IDxcBlob* CreateCompileShader(//compileするshaderファイルへのパス
		const std::wstring &filePath,
		//compilerに使用するプロファイル
		const wchar_t *profile,
		//初期化で生成したものを3つ
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* IncludeHandler);

	
	void CreateDxcCommpiler();
	void CreateRootSignature();
	void CreateInputLayout();
	void CreateBlendState();
	void CreateRasterizerState();
	void CreateShader();
	void CreatePipeLineStateObject();
	void CreateViewPort();
	void PreDraw();
	void CreateReportLive();
	
private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* directXCommon_=nullptr;
	int32_t backBufferWidth_ = 0;
	int32_t backBufferHedth_ = 0;
	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;
	ID3D10Blob* signatureBlob_ = nullptr;
	ID3D10Blob* errorBlob_ = nullptr;
	IDxcBlob* shaderBlob_ = nullptr;
	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12PipelineState* graphicsPipelineState_ = nullptr;
	IDxcBlob* vertexShaderBlob_ = nullptr;
	IDxcBlob* pixelShaderBlob_ = nullptr;
	ID3D12GraphicsCommandList* commandList_ = nullptr;
	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
};
