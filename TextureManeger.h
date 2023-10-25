#pragma once
#include "externals/DirectXTex/DirectXTex.h"
#include"externals/DirectXTex/d3dx12.h"
//#include"../externals/DirectXTex/d3dx12.h"
//#include"../externals/DirectXTex/DirectXTex.h"

#include<vector>
#include"WinApp.h"
#include"DirectXCommon.h"

struct texResourceProperty
{
	D3D12_GPU_DESCRIPTOR_HANDLE SrvHandleGPU;
	ID3D12Resource* Resource;

};

class TextureManeger
{
public:

	void Initilize();
	void Finalize();

	void UploadTexData(ID3D12Resource* tex, const DirectX::ScratchImage& mipImage);
	/*void LoadTexture(const std::string& filePath, DirectXCommon*directX_);*/


	DirectX::ScratchImage CreateMipImage(const std::string& filePath);
	D3D12_RESOURCE_DESC SettingResource(const DirectX::TexMetadata& metadata);

	D3D12_HEAP_PROPERTIES SettingHeap();
	ID3D12Resource* CreateTexResource(const DirectX::TexMetadata& metadata, DirectXCommon* directX_);

	
	texResourceProperty LoadTexture(const std::string& filePath, DirectXCommon*directX_);
	
private:
};


