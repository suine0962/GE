#include"TextureManager.h"


void TextureManeger::Initilize(WinApp* winApp,DirectXCommon*directX, int32_t backBufferWidth,
	int32_t backBufferHeight)
{

	assert(winApp);
	assert(directX);
	assert(4 <= backBufferWidth && backBufferWidth_ <= 4096);
	assert(4 <= backBufferHeight && backBufferHedth_ <= 4096);

	winApp_ = winApp;
	winApp_->CreateGameWindow(L"CG2", 1280, 720);


	CreateDxcCommpiler();
	CreateRootSignature();
	CreateInputLayout();
	CreateBlendState();
	CreateShader();
	CreateRasterizerState();
	CreatePipeLineStateObject();
	CreateViewPort();
}


void TextureManeger::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());

}

void TextureManeger::CreateDxcCommpiler()
{
	dxcUtils_ = nullptr;
	dxcCompiler_ = nullptr;

	HRESULT hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	IDxcIncludeHandler* IncludeHandler = nullptr;
	hr = dxcUtils_->CreateDefaultIncludeHandler(&IncludeHandler);
	assert(SUCCEEDED(hr));

}


IDxcBlob* TextureManeger::CreateCompileShader(
	//compileするshaderファイルへのパス
	const std::wstring &filePath,
	//compilerに使用するプロファイル
	const wchar_t *profile,
	//初期化で生成したものを3つ
	IDxcUtils* dxcUtils,
	IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* IncludeHandler)

{
	HRESULT hr = S_FALSE;

	//これからシェーダをコンパイルする旨をログに出す
	Log(ComvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8のの文字コードであることを通知

	LPCWSTR arguments[] = {
	   filePath.c_str(),//コンパイラの対象ファイル名
	   L"-E", L"main",//エントリーポイントの指定,基本main以外にはしない
	   L"-T", profile,//shaderprofileの設定
	   L"-Zi", L"-Qembed_debug",//デバッグ用の情報を埋め込む
	   L"-Od", //最適化を外しておく
	   L"-Zpr",//メモリレイアウトは行優先
	};
	//実際にshaderをコンパイラする
	IDxcResult* shaderResult = nullptr;

	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだプロファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		IncludeHandler,//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult));//コンパイル結果
	assert(SUCCEEDED(hr));
	IDxcBlobUtf8* shaderError = nullptr;

	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);

	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		assert(false);
	}
	//コンパイル結果から実行用のバイナリ部分を取得
	shaderBlob_ = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob_), nullptr);
	assert(SUCCEEDED(hr));

	//成功したログを出す
	Log(ComvertString(std::format(L"Compile Succeeded,Path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを開放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob_;

}

void TextureManeger::CreateRootSignature()
{
	HRESULT hr = S_FALSE;

	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	signatureBlob_ = nullptr;
	errorBlob_ = nullptr;
	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob_, &errorBlob_);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(errorBlob_->GetBufferPointer()));
		assert(false);
	}
	//バイナリをもとに生成

	hr =directXCommon_->Getdevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(hr));


}

void TextureManeger::CreateInputLayout()
{
	//InputLayOut
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);


}

void TextureManeger::CreateBlendState()
{
	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc{};
	//全ての色要素を書き込む
	blendDesc.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void TextureManeger::CreateRasterizerState()
{
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	//裏面を表示しない
	rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

}

void TextureManeger::CreateShader()
{
	//shaderをコンパイラする
	vertexShaderBlob_ = CreateCompileShader(L"Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_,includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CreateCompileShader(L"Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_,includeHandler_);
	assert(pixelShaderBlob_ != nullptr);



}




void TextureManeger::CreatePipeLineStateObject()
{
	HRESULT hr = S_FALSE;
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{};
	D3D12_BLEND_DESC blendDesc_{};
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	//PSOの生成
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature_;//rootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc_;//inputLayout
	graphicsPipelineStateDesc.VS = { vertexShaderBlob_->GetBufferPointer(),
	vertexShaderBlob_->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { pixelShaderBlob_->GetBufferPointer(),
	pixelShaderBlob_->GetBufferSize() };//pixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc_;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;//RasterizerState
	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するリポジトリ
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面にに色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	graphicsPipelineState_ = nullptr;
	hr = directXCommon_->Getdevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));

}

void TextureManeger::CreateViewPort()	
{
	//ビューボート
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport_.Width = winApp_->kWindowWidth;
	viewport_.Height = winApp_->kWindowHeight;
	viewport_.TopLeftX = 0;
	viewport_.TopLeftY = 0;
	viewport_.MinDepth = 0.0f;
	viewport_.MaxDepth = 1.0f;

	//シザー短形
	//基本的にビューポートと同じ短形が描画されるものとする
	scissorRect_.left = 0;
	scissorRect_.right = winApp_->kWindowWidth;
	scissorRect_.top = 0;
	scissorRect_.bottom = winApp_->kWindowHeight;



}

void TextureManeger::PreDraw()
{
	commandList_->RSSetViewports(1,&viewport_);//ViewProtを設定
	commandList_->RSSetScissorRects(1,&scissorRect_);//scirssor
	//RootSignatureを設定,PSOに設定しているけど別途設定が必要
	commandList_->SetGraphicsRootSignature(rootSignature_);
	commandList_->SetPipelineState(graphicsPipelineState_);///PSOを設定
	//形状を設定。PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	commandList_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//描画
	commandList_->DrawInstanced(3, 1, 0, 0);
}

void TextureManeger::CreateReportLive()
{
	graphicsPipelineState_->Release();
	signatureBlob_->Release();
	if (errorBlob_)
	{
		errorBlob_->Release();
	}
	rootSignature_->Release();
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();


}


	


