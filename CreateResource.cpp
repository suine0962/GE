#include "CreateResource.h"

CreateResources::~CreateResources() {

}

ResourcePropety CreateResources::Vector4CreateResource(DirectXCommon *directX,const int Num)
{
	directX_ = directX;
	ResourcePropety Result;

	Result.Vertex = CreateBufferResource(directX_->Getdevice(), sizeof(Vector4) * Num);//1頂点あたりのサイズを決める
	Result.Material = CreateBufferResource(directX_->Getdevice(), sizeof(Vector4) * 3);//
	Result.wvpResource = CreateBufferResource(directX_->Getdevice(), sizeof(Matrix4x4) * Num);
	Result.vertexBufferView_ = CreateBufferView(Result.Vertex, sizeof(Vector4) * Num, Num);

	return Result;
}

ResourcePropety CreateResources::VertexDataCreateResource(DirectXCommon *directX,const int Num)
{
	directX_ = directX;
	ResourcePropety Result;

	Result.Vertex = CreateBufferResource(directX_->Getdevice(), sizeof(VertexData) * Num);
	Result.Material = CreateBufferResource(directX_->Getdevice(), sizeof(Vector4) * 3);
	Result.wvpResource = CreateBufferResource(directX_->Getdevice(), sizeof(Matrix4x4) * Num);
	Result.vertexBufferView_ = CreateBufferView(Result.Vertex, sizeof(VertexData) * Num, Num);

	return Result;
}

ID3D12Resource* CreateResources::CreateBufferResource(ID3D12Device* device, size_t SizeInBytes)
{

	ID3D12Resource* RssultResource = nullptr;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResouceDesc{};
	//バッファリソース,テクスチャまたは別の設定する
	ResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResouceDesc.Width = SizeInBytes;

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


	return RssultResource;
}

D3D12_VERTEX_BUFFER_VIEW CreateResources::CreateBufferView(ID3D12Resource* Resource, size_t strideInBytes, int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferView = {};

	resultBufferView.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferView.SizeInBytes = UINT(strideInBytes);

	//1頂点あたりのサイズ
	resultBufferView.StrideInBytes = UINT(strideInBytes / size);

	return resultBufferView;
}

void CreateResources::Release(ID3D12Resource* resource)
{
	resource->Release();
}

