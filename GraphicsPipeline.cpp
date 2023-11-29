#include "GraphicsPipeline.h"

void GraphicsPipeline::Initilize(DirectXCommon*directX)
{
	directX_ = directX;

	dxcCreate();
	DfIncludeHandlerSetting();
}

void GraphicsPipeline::Finalize()
{
	PSORelese(pso_.shape);
	PSORelese(pso_.sprite);

	ShaderRelese(shader_.shape);
	ShaderRelese(shader_.sprite);

}

void GraphicsPipeline::ShaderCompile()
{
	ShaderMode shape, sprite{};

	shape.vertexBlob = CreateCompileShader(
		L"./ShapeObject3d.VS.hlsl",
		L"vs_6_0");

	shape.pixleBlob = CreateCompileShader(
		L"./ShapeObject3d.PS.hlsl",
		L"ps_6_0");

	sprite.vertexBlob = CreateCompileShader(
		L"./SpriteObject3d.VS.hlsl",
		L"vs_6_0");

	sprite.pixleBlob = CreateCompileShader(
		L"./SpriteObject3d.PS.hlsl",
		L"ps_6_0");

	shader_.shape = shape;
	shader_.sprite = sprite;

}

void GraphicsPipeline::PSOCreate()
{
	ShapePSO();
	SpritePSO();

}


void GraphicsPipeline::dxcCreate()
{
	HRESULT hr;

	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxc_.Utils));
	assert(SUCCEEDED(hr));

	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxc_.Compiler));
	assert(SUCCEEDED(hr));

}


void GraphicsPipeline::DfIncludeHandlerSetting()
{
	HRESULT hr = S_FALSE;
	dxc_.Utils->CreateDefaultIncludeHandler(&includeHandler_);
	assert(SUCCEEDED(hr));

}

void GraphicsPipeline::ShapePSO()
{
	HRESULT hr = S_FALSE;

	ID3D12Device* device_ = directX_->Getdevice();
	PSOProperty shapePSO{};
	ShaderMode shader = shader_.shape;

	//RootSIgnature
	D3D12_ROOT_SIGNATURE_DESC descriptionRootsignature{};

	descriptionRootsignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	//

	D3D12_ROOT_PARAMETER rootParameters[2] = {};

	//ルートパラメータ
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
	rootParameters[0].Descriptor.ShaderRegister = 0;//レジスタ番号０とバインド

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;//CBVを使う
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //vertexshaderを使う
	rootParameters[1].Descriptor.ShaderRegister = 0; //レジスタ番号0を使う

	descriptionRootsignature.pParameters = rootParameters;//ルートパラメータ配列へのポインタ
	descriptionRootsignature.NumParameters = _countof(rootParameters);//配列の長さ


	//シリアライズしてバイナリする
	hr = D3D12SerializeRootSignature(&descriptionRootsignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &shapePSO.signatureBlob,&shapePSO.errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(shapePSO.signatureBlob->GetBufferPointer()));
		assert(false);
	}

	//バイナリをもとに生成

	hr = directX_->Getdevice()->CreateRootSignature(0, shapePSO.signatureBlob->GetBufferPointer(),
		shapePSO.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&shapePSO.rootSignature));

	assert(SUCCEEDED(hr));


	//InputLayOut

	D3D12_INPUT_ELEMENT_DESC inputElementDesc_[1]{};

	inputElementDesc_[0].SemanticName = "POSITION";
	inputElementDesc_[0].SemanticIndex = 0;
	inputElementDesc_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	inputLayoutDesc.pInputElementDescs = inputElementDesc_;
	inputLayoutDesc.NumElements = _countof(inputElementDesc_);

	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};
	//全ての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;



	//RASTERIZER
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	//裏面を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;

	//depth
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	
	//PSOの生成

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = shapePSO.rootSignature;//rootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//inputLayout
	graphicsPipelineStateDesc.VS = { shader.vertexBlob->GetBufferPointer(),
	shader.vertexBlob->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { shader.pixleBlob->GetBufferPointer(),
	shader.pixleBlob->GetBufferSize() };//pixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc_;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;//RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するポジトリ
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面にに色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	hr = directX_->Getdevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&shapePSO.GrahicsPipeLineState));
	assert(SUCCEEDED(hr));

	pso_.shape = shapePSO;

}

void GraphicsPipeline::SpritePSO()
{
	PSOProperty SpritePSO{};
	ShaderMode shader = shader_.sprite;
	ID3D12Device* device_ = directX_->Getdevice();
	HRESULT hr = S_FALSE;

	//RootSignature作成
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};

	descriptionRootSignature.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;


	D3D12_ROOT_PARAMETER rootParameters[3] = {};

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

	hr = D3D12SerializeRootSignature(&descriptionRootSignature,
		D3D_ROOT_SIGNATURE_VERSION_1, &SpritePSO.signatureBlob, &SpritePSO.errorBlob);
	if (FAILED(hr))
	{
		Log(reinterpret_cast<char*>(SpritePSO.signatureBlob->GetBufferPointer()));
		assert(false);
	}

	hr = directX_->Getdevice()->CreateRootSignature(0, SpritePSO.signatureBlob->GetBufferPointer(),
		SpritePSO.signatureBlob->GetBufferSize(), IID_PPV_ARGS(&SpritePSO.rootSignature));

	assert(SUCCEEDED(hr));

	//InputLayOut

	D3D12_INPUT_ELEMENT_DESC inputElementDesc_[2]{};
	inputElementDesc_[0].SemanticName = "POSITION";
	inputElementDesc_[0].SemanticIndex = 0;
	inputElementDesc_[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDesc_[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputElementDesc_[1].SemanticName = "TEXCOORD";
	inputElementDesc_[1].SemanticIndex = 0;
	inputElementDesc_[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDesc_[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	inputLayoutDesc.pInputElementDescs = inputElementDesc_;
	inputLayoutDesc.NumElements = _countof(inputElementDesc_);




	//BlendStateの設定
	D3D12_BLEND_DESC blendDesc_{};
	//全ての色要素を書き込む
	blendDesc_.RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;


	//RASTERIZER
	D3D12_RASTERIZER_DESC rasterizerDesc_{};
	//裏面を表示しない
	rasterizerDesc_.CullMode = D3D12_CULL_MODE_BACK;
	//三角形の中を塗りつぶす
	rasterizerDesc_.FillMode = D3D12_FILL_MODE_SOLID;


	//
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	//PSOの生成

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	graphicsPipelineStateDesc.pRootSignature = SpritePSO.rootSignature;//rootSignature
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;//inputLayout
	graphicsPipelineStateDesc.VS = { shader.vertexBlob->GetBufferPointer(),
	shader.vertexBlob->GetBufferSize() };//VertexShader
	graphicsPipelineStateDesc.PS = { shader.pixleBlob->GetBufferPointer(),
	shader.pixleBlob->GetBufferSize() };//pixelShader
	graphicsPipelineStateDesc.BlendState = blendDesc_;//BlendState
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc_;//RasterizerState

	//書き込むRTVの情報
	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//利用するポジトリ
	graphicsPipelineStateDesc.PrimitiveTopologyType =
		D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//どのように画面にに色を打ち込むかの設定
	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	//実際に生成
	hr = directX_->Getdevice()->CreateGraphicsPipelineState(&graphicsPipelineStateDesc,
		IID_PPV_ARGS(&SpritePSO.GrahicsPipeLineState));
	assert(SUCCEEDED(hr));

	pso_.sprite = SpritePSO;
}

void GraphicsPipeline::ShaderRelese(ShaderMode shader)
{
	shader.vertexBlob->Release();
	shader.pixleBlob->Release();


}

void GraphicsPipeline::PSORelese(PSOProperty pso)
{
	pso.GrahicsPipeLineState->Release();
	pso.signatureBlob->Release();
	if (pso.errorBlob)
	{
		pso.errorBlob->Release();
	}
	pso.rootSignature->Release();
}


IDxcBlob* GraphicsPipeline::CreateCompileShader(
	const std::wstring& filePath, const wchar_t* profile)
{
	IDxcUtils* dxcUtils = dxc_.Utils;
	IDxcCompiler3* dxcCompiler = dxc_.Compiler;
	IDxcIncludeHandler* includeHandler = includeHandler_;

	//1HLSLファイルを読む
	//1.hlslファイルを読む
	Log(ComvertString(std::format(L"Begin CompileShader,path:{},profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr =
		dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかったら止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	//2.Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(),
		L"-E",L"main",
		L"-T",profile,
		L"-Zi",L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};

	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler->Compile(&shaderSourceBuffer, arguments, _countof(arguments), includeHandler, IID_PPV_ARGS(&shaderResult));
	//コンパイルエラーではなくdxcが起動出来ないなど致命的な状況
	assert(SUCCEEDED(hr));


	//3.警告・エラーが出ていないかを確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log(shaderError->GetStringPointer());
		assert(false);
	}


	//4.Compile結果を受け取って返す
	//BLOB・・・BinaryLargeOBject
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log(ComvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却

	return shaderBlob;

}