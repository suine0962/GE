#pragma once
#include"DirectXCommon.h"
#include <dxcapi.h>

struct ShaderMode {
	IDxcBlob* vertexBlob;
	IDxcBlob* pixleBlob;
};

struct Shaders {
	ShaderMode shape;
	ShaderMode sprite;
};

struct PSOProperty {
	ID3D12PipelineState* GrahicsPipeLineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3D10Blob* signatureBlob = nullptr;
	ID3D10Blob* errorBlob = nullptr;
};

struct PSO {

	PSOProperty shape;
	PSOProperty sprite;

};

struct DXCProperty {
	IDxcUtils* dxcUtils;
	IDxcCompiler3* dxcCompiler;
};


class GraphicsPipeline
{
public:
	void Initilize();

	void Finalize();

	void ShaderCompile();

	void PSOCreate();

	PSO GetPSO() { return pso_; };

	void Log(const std::string& message);

private:
	IDxcBlob* CreateCompileShader(//compileするshaderファイルへのパス
		const std::wstring& filePath,
		//compilerに使用するプロファイル
		const wchar_t* profile
	);

	void dxcCreate();//

	void DfIncludeHandlerSetting();

	void ShapePSO();

	void SpritePSO();

	void ShaderRelese(ShaderMode shader);

	void PSORelese(PSOProperty pso);

	DXCProperty dxc_ = {};

	IDxcIncludeHandler* includeHandler_ = nullptr;

	Shaders shader_{ nullptr,nullptr };

	PSO pso_ = {};

	DirectXCommon* directX_ = nullptr;
};

