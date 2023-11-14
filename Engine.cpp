#include"Engine.h"


void Engine::Initilize(WinApp* winApp, DirectXCommon* directX,TextureManeger*texManeger,Sphere*sphere,int32_t backBufferWidth,
	int32_t backBufferHeight)
{
	assert(winApp);
	assert(directX);
	assert(texManeger);
	assert(sphere);

	assert(4 <= backBufferWidth && backBufferWidth_ <= 4096);
	assert(4 <= backBufferHeight && backBufferHedth_ <= 4096);

	winApp_ = winApp;
	directX_ = directX;
	texManeger_ = texManeger;
	sphere_ = sphere;
	//winApp_->CreateGameWindow(L"CG2", 1280, 720);

	CreateDxcCommpiler();
	CreateRootSignature();
	CreateInputLayout();
	CreateBlendState();
	CreateShader();
	CreateRasterizerState();
	CreatePipeLineStateObject();
	CreateViewPort();
	vertexResouce_ = CreateBufferResource(directX_->Getdevice(), sizeof(VertexData) * 6);
	CreateVertexResource();
	CreateMateialResource();
	CreateTransformationMatrix();
	CreateWorldViewProjectionMatrix();
	CreateVertexResourceSprite();
	CreateTransformationMatrixResourceSprite();
	TextureLoad("DefaultResources/uvChecker.png", directX_);
	sphereDraw();
}


void Engine::Log(const std::string& message)
{
	OutputDebugStringA(message.c_str());

}

void Engine::CreateDxcCommpiler()
{
	HRESULT hr = S_FALSE;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils_));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler_));
	assert(SUCCEEDED(hr));

	hr = dxcUtils_->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));
}


IDxcBlob* Engine::CreateCompileShader(
	//compileするshaderファイルへのパス
	const std::wstring& filePath,
	//compilerに使用するプロファイル
	const wchar_t* profile,
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

void Engine::CreateRootSignature()
{
	HRESULT hr = S_FALSE;

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	


	//ルートパラメータ
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号０とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //vertexshaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0; //レジスタ番号0を使う


	D3D12_DESCRIPTOR_RANGE descriptorRange[1]{};
	//DescriptorRange
	descriptorRange[0].BaseShaderRegister = 0;//0から始まる
	descriptorRange[0].NumDescriptors = 1;//数は1つ
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//offsetを自動計算

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//descriptortableを使う
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootSignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootSignature.NumParameters = _countof(rootParameters);//配列の長さ

	//Samplerの設定
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;

	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	descriptionRootSignature.pStaticSamplers = staticSamplers;
	descriptionRootSignature.NumStaticSamplers = _countof(staticSamplers);



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

	hr = directX_->Getdevice()->CreateRootSignature(0, signatureBlob_->GetBufferPointer(),
		signatureBlob_->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(hr));


}

void Engine::CreateInputLayout()
{

	//InputLayOut
	inputElementDesc_[0].SemanticName = "POSITION";
	inputElementDesc_[0].SemanticIndex = 0;
	inputElementDesc_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDesc_[1].SemanticName = "TEXCOORD";
	inputElementDesc_[1].SemanticIndex = 0;
	inputElementDesc_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDesc_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDesc_;
	inputLayoutDesc.NumElements = _countof(inputElementDesc_);


}

void Engine::CreateBlendState()
{
	//BlendStateの設定

	//全ての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
}

void Engine::CreateRasterizerState()
{
	//裏面を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;
}

void Engine::CreateShader()
{
	//shaderをコンパイラする
	vertexShaderBlob_ = CreateCompileShader(L"./Object3d.VS.hlsl",
		L"vs_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(vertexShaderBlob_ != nullptr);

	pixelShaderBlob_ = CreateCompileShader(L"./Object3d.PS.hlsl",
		L"ps_6_0", dxcUtils_, dxcCompiler_, includeHandler_);
	assert(pixelShaderBlob_ != nullptr);



}

void Engine::CreateDepthStencilState()
{
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

}


void Engine::CreatePipeLineStateObject()
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


ID3D12Resource* Engine::CreateBufferResource(ID3D12Device* device, size_t sizeInBytes)
{
	//頂点リソース用のヒープの設定

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定

	//バッファリソース,テクスチャまたは別の設定する
	ResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResouceDesc.Width = sizeInBytes;

	//バッファの場合はこれらを１にするのがきまり
	ResouceDesc.Height = 1;
	ResouceDesc.DepthOrArraySize = 1;
	ResouceDesc.MipLevels = 1;
	ResouceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにするきまり
	ResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//実際に頂点リソースを作る
	ID3D12Resource* Resource = nullptr;
	HRESULT hr = directX_->Getdevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResouceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}


ID3D12Resource* Engine::CreateMateialResource()
{


	struct VertexData {
		Vector4 position;
		//Vector2 texcoord;
	};
	materialResource_ = CreateBufferResource(directX_->Getdevice(), sizeof(Vector4));
	Vector4* materialData = nullptr;

	materialResource_->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	*materialData = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	return materialResource_;
}



void Engine::CreateViewPort()
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


void Engine::Draw()
{
	const texResourceProperty& texLoad = texManeger_->Gettex();

	directX_->GetcommandList()->RSSetViewports(1, &viewport_);//ViewProtを設定
	directX_->GetcommandList()->RSSetScissorRects(1, &scissorRect_);//scirssor
	//RootSignatureを設定,PSOに設定しているけど別途設定が必要
	directX_->GetcommandList()->SetGraphicsRootSignature(rootSignature_);
	directX_->GetcommandList()->SetPipelineState(graphicsPipelineState_);///PSOを設定
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite_);
	//形状を設定。PSOに設定しているものとはまた別、同じものを設定すると考えておけばいい
	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, materialResource_->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, wvpResource->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2,texLoad.SrvHandleGPU);
	//描画
	directX_->GetcommandList()->DrawInstanced(6, 1, 0, 0);
}

void Engine::CreateRelease()
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
	materialResource_->Release();
	//tex.Resource->Release();


}

void Engine::CreateVertexResource()
{

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};

	//頂点バッファーをビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexResouce_->GetGPUVirtualAddress();
	//使用するリソースは頂点３つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点当たりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	//頂点リソースにデータを書き込む
	VertexData* vertexData = nullptr;
	//書き込むためのアドレスを取得
	vertexResouce_->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//左下
	vertexData[0].position = { -0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[0].texcoord = { 0.0f,1.0f };
	//上
	vertexData[1].position = { 0.0f,0.5f,0.0f,1.0f };
	vertexData[1].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[2].position = { 0.5f, -0.5f, 0.0f, 1.0f };
	vertexData[2].texcoord = { 1.0f,1.0f };


	//左下
	vertexData[3].position = { -0.5f,-0.5f,0.5f,1.0f };
	vertexData[3].texcoord = { 0.0f,1.0f };
	//上
	vertexData[4].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexData[4].texcoord = { 0.5f,0.0f };
	//右下
	vertexData[5].position = { 0.5f,-0.5f,-0.5f,1.0f };
	vertexData[5].texcoord = { 1.0f,1.0f };
}

void Engine::CreateVertexResourceSprite()
{
	//sprite用の頂点リソースを作る
	ID3D12Resource* vertexResourceSprite = CreateBufferResource(directX_->Getdevice(), sizeof(VertexData) * 6);

	//頂点バッファビューを作る
	
	//リソースの先頭のアドレスから使う
	vertexBufferViewSprite_.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点６つ分のサイス
	vertexBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
	//１頂点あたりのサイズ
	vertexBufferViewSprite_.StrideInBytes = sizeof(VertexData);

	VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));

	//一枚目の三角形
	//左下
	vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[0].texcoord = { 0.0f,1.0f };
	//左上
	vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[1].texcoord = { 0.0f,0.0f };
	//右下
	vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[2].texcoord = { 1.0f,1.0f };

	//二枚目の三角形
	//左上
	vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };
	vertexDataSprite[3].texcoord = { 0.0f,0.0f };
	//左上
	vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f, };
	vertexDataSprite[4].texcoord = { 1.0f,0.0f };
	//右下
	vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };
	vertexDataSprite[5].texcoord = { 1.0f,1.0f };


}

void Engine::TextureLoad(const std::string& filePath, DirectXCommon* directX_)
{

	texManeger_->LoadTexture(filePath,directX_);

}

void Engine::sphereDraw()
{
	sphere_->SphereDraw();

}

void Engine::CreateTransformationMatrix()
{
	//wvp用のリソースを作る,matrix4x4一つ分サイズを用意する
	wvpResource = CreateBufferResource(directX_->Getdevice(), sizeof(Matrix4x4));
	//データを書き込む
	wvpData = nullptr;
	//書き込むためのアドレス取得
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));
	//単位行列を書き込んでおく
	*wvpData = MakeIdenttity4x4();
}

void Engine::CreateTransformationMatrixResourceSprite()
{
	//sprite用のTransformaitonMatrix用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixResourceSprite = CreateBufferResource(directX_->Getdevice(), sizeof(Matrix4x4));
	//データを書き込む
	Matrix4x4* trasformationMatrixDataSprite = nullptr;
	//書き込むためのアドレス取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&trasformationMatrixDataSprite));
	//単位行列を書き込んでおく
	*trasformationMatrixDataSprite = MakeIdenttity4x4();

	//sprite用のworldViewProjectionMatrixを作る
	Matrix4x4 worldMatrixSprite = MakeAffineMatrix(transformSprite.scale, transformSprite.rotate, transformSprite.translate);
	Matrix4x4 viewMatrixSprite = MakeIdenttity4x4();
	Matrix4x4 projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(winApp_->kWindowWidth), float(winApp_->kWindowHeight),
		0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = Multiply(worldMatrixSprite, Multiply(viewMatrixSprite, projectionMatrixSprite));
	*trasformationMatrixDataSprite = worldViewProjectionMatrixSprite;
}

void Engine::RenewalCBuffer()
{
	CreateWorldViewProjectionMatrix();
	transform.rotate.y += 0.005f;
}

void Engine::CreateWorldViewProjectionMatrix()
{

	Matrix4x4 worldMatrix = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	Matrix4x4 cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	Matrix4x4 viewMatrix = Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(winApp_->kWindowWidth) / float(winApp_->kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*wvpData = worldMatrix;

	
}




