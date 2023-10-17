#pragma once
#include"DirectXCommon.h"
#include<dxcapi.h>


#pragma comment(lib,"dxcompiler.lib")


class TextureManeger
{
public:
	void Initilize(WinApp*winApp,DirectXCommon* directX,int32_t backBufferWidth = WinApp::kWindowWidth,
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

	ID3D12Resource* CreateBufferResource();
	ID3D12Resource* CreateMateialResource();

	void CreateDxcCommpiler();
	void CreateRootSignature();
	void CreateInputLayout();
	void CreateBlendState();
	void CreateRasterizerState();
	void CreateShader();
	
	void CreatePipeLineStateObject();
	void CreateViewPort();

	void Draw();
	void CreateRelease();
	
private:
	WinApp* winApp_ = nullptr;
	DirectXCommon* directX_ = nullptr;


	int32_t backBufferWidth_ = 0;
	int32_t backBufferHedth_ = 0;

	IDxcUtils* dxcUtils_ = nullptr;
	IDxcCompiler3* dxcCompiler_ = nullptr;
	IDxcIncludeHandler* includeHandler_ = nullptr;

	ID3D10Blob* signatureBlob_ = nullptr;
	ID3D10Blob* errorBlob_ = nullptr;
	IDxcBlob* shaderBlob_ = nullptr;

	IDxcBlobUtf8* shaderError = nullptr;

	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResouceDesc{};
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	D3D12_ROOT_PARAMETER rootParameters[1] = {};

	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	IDxcBlobEncoding* shaderSource_ = nullptr;
	IDxcResult* shaderResult_ = nullptr;

	//vertexBufferViewの生成
	ID3D12Resource* vertexResouce_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* materialResource_ = nullptr;

	D3D12_INPUT_ELEMENT_DESC inputElementDesc_[1]{};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};

	ID3D12RootSignature* rootSignature_ = nullptr;
	ID3D12PipelineState* graphicsPipelineState_ = nullptr;

	IDxcBlob* vertexShaderBlob_ = nullptr;
	IDxcBlob* pixelShaderBlob_ = nullptr;

	D3D12_VIEWPORT viewport_{};
	D3D12_RECT scissorRect_{};
};
