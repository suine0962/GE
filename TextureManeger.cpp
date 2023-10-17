#include"TextureManager.h"


void TextureManeger::Initilize(WinApp*winApp,DirectXCommon* directX, int32_t backBufferWidth,
	int32_t backBufferHeight)
{

	assert(winApp);
	assert(directX);

	assert(4 <= backBufferWidth && backBufferWidth_ <= 4096);
	assert(4 <= backBufferHeight && backBufferHedth_ <= 4096);

	winApp_ = winApp;
	directX_ = directX;
	//winApp_->CreateGameWindow(L"CG2", 1280, 720);

	CreateDxcCommpiler();
	CreateRootSignature();
	CreateInputLayout();
	CreateBlendState();
	CreateShader();
	CreateRasterizerState();
	CreatePipeLineStateObject();
	CreateViewPort();
	CreateBufferResource();
	//CreateMateialResource();

}


void TextureManeger::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());

}

void TextureManeger::CreateDxcCommpiler()
{
	HRESULT hr = S_FALSE;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
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
	//1
	Log(ComvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource_);
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource_->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource_->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8のの文字コードであることを通知

	//2
	LPCWSTR arguments[] = {
	   filePath.c_str(),//コンパイラの対象ファイル名
	   L"-E", L"main",//エントリーポイントの指定,基本main以外にはしない
	   L"-T", profile,//shaderprofileの設定
	   L"-Zi", L"-Qembed_debug",//デバッグ用の情報を埋め込む
	   L"-Od", //最適化を外しておく
	   L"-Zpr",//メモリレイアウトは行優先
	};
	//実際にshaderをコンパイラする

	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだプロファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		IncludeHandler,//includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult_));//コンパイル結果
	assert(SUCCEEDED(hr));

	//3
	shaderResult_->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);

	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		Log(shaderError->GetStringPointer());
		assert(false);
	}

	//4
	//コンパイル結果から実行用のバイナリ部分を取得
	shaderBlob_ = nullptr;
	hr = shaderResult_->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob_), nullptr);
	assert(SUCCEEDED(hr));

	//成功したログを出す
	Log(ComvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを開放
	shaderSource_->Release();
	shaderResult_->Release();
	//実行用のバイナリを返却
	return shaderBlob_;

}

void TextureManeger::CreateRootSignature()
{
	HRESULT hr = S_FALSE;

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	//ルートパラメータ
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号０とバインド
	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//シリアライズしてバイナリにする
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

	hr =directX_->Getdevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(hr));


}

void TextureManeger::CreateInputLayout()
{
	//InputLayOut
	inputElementDesc_[0].SemanticName = "POSITION";
	inputElementDesc_[0].SemanticIndex = 0;
	inputElementDesc_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	/*inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;*/
	inputLayoutDesc.pInputElementDescs = inputElementDesc_;
	inputLayoutDesc.NumElements = _countof(inputElementDesc_);


}

void TextureManeger::CreateBlendState()
{
	//BlendStateの設定
	
	//全ての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void TextureManeger::CreateRasterizerState()
{
	//裏面を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

}

void TextureManeger::CreateShader()
{
	//shaderをコンパイラする
	vertexShaderBlob_ = CreateCompileShader(L"./Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_,includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CreateCompileShader(L"./Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_,includeHandler_);
	assert(pixelShaderBlob_ != nullptr);



}




void TextureManeger::CreatePipeLineStateObject()
{
	HRESULT hr = S_FALSE;
	//PSOの生成
	graphicsPipelineStateDesc.pRootSignature = rootSignature_;//rootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//inputLayout
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
	hr = directX_->Getdevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&graphicsPipelineState_));
	assert(SUCCEEDED(hr));

}


ID3D12Resource* TextureManeger::CreateBufferResource()
{

	struct Vector4
	{
		float x;
		float y;
		float w;
		float z;
	};
	struct Vector2
	{
		float x;
		float y;
	};

	struct VertexData {
		Vector4 position;
		//Vector2 texcoord;
	};


	//頂点リソース用のヒープの設定

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定

	//バッファリソース,テクスチャまたは別の設定する
	ResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResouceDesc.Width = sizeof(Vector4) * 3;

	//バッファの場合はこれらを１にするのがきまり
	ResouceDesc.Height = 1;
	ResouceDesc.DepthOrArraySize = 1;
	ResouceDesc.MipLevels = 1;
	ResouceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにするきまり
	ResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	HRESULT hr = directX_->Getdevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResouceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertexResouce_));
	assert(SUCCEEDED(hr));

	//頂点バッファーをビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 3;
	//１頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	//vertexData[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	//vertexData[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[2].position = { 0.5f, -0.5f, 0.0f, 1.0f };

	

	//vertexData[2].texcoord = { 1.0f,1.0f };
	return vertexResouce_;

}


ID3D12Resource* TextureManeger::CreateMateialResource()
{
	struct Vector4
	{
		float x;
		float y;
		float w;
		float z;
	};

	materialResource_ = CreateBufferResource();
	Vector4* materialData = nullptr;

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData = Vector4(1.0f, 0.0f, 0.0f, 1.0f);

	return materialResource_;
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


void TextureManeger::Draw()
{
	directX_->GetcommandList()->RSSetViewports(1, &viewport_);//ViewProtを設定
	directX_->GetcommandList()->RSSetScissorRects(1, &scissorRect_);//scirssor
	//RootSignatureを設定,PSOに設定しているけど別途設定が必要
	directX_->GetcommandList()->SetGraphicsRootSignature(rootSignature_);
	directX_->GetcommandList()->SetPipelineState(graphicsPipelineState_);///PSOを設定
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//形状を設定。PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	//描画
	directX_->GetcommandList()->DrawInstanced(3, 1, 0, 0);
}

void TextureManeger::CreateRelease()
{
	pixelShaderBlob_->Release();
	vertexShaderBlob_->Release();
	graphicsPipelineState_->Release();
	rootSignature_->Release();
	vertexResouce_->Release();
	shaderResult_->Release();
	shaderSource_->Release();
	shaderError->Release();
	shaderBlob_->Release();
	errorBlob_->Release();
	signatureBlob_->Release();
	if (errorBlob_)
	{
		errorBlob_->Release();
	}
	includeHandler_->Release();
	dxcCompiler_->Release();
	dxcUtils_->Release();
	//materialResource_->Release();



}


	


