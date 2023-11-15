#pragma once
#include "externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"

#include<vector>
#include"WinApp.h"
#include"DirectXCommon.h"

struct texResourceProperty
{

	D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
	ID3D12Resource* Resource;

};

struct DescriptorSize
{
	uint32_t SRV;
	uint32_t RTV;
	uint32_t DSV;

};

class TextureManeger
{
public:

	void Initilize();
	void Finalize();

	void UploadTexData(ID3D12Resource* tex, const DirectX::ScratchImage& mipImage);

	texResourceProperty Release(texResourceProperty tex);

	/*void LoadTexture(const std::string& filePath, DirectXCommon*directX_);*/


	DirectX::ScratchImage CreateMipImage(const std::string& filePath);
	D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	D3D12_HEAP_PROPERTIES SettingHeap();
	ID3D12Resource* CreateTexResource(const DirectX::TexMetadata& metadata);

	texResourceProperty LoadTexture(const std::string& filePath);
	
	 const texResourceProperty& Gettex() { return tex; };

	 D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);

	 D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index);


private:

	texResourceProperty tex{};

	DirectXCommon* directX_ = nullptr;
};


