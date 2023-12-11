#include "Createtriangle.h"

Createtriangle::Createtriangle()
{
}

Createtriangle::~Createtriangle()
{
}



void Createtriangle::Initilize(DirectXCommon*directX,CreateResources* CResource, GraphicsPipeline* PSO,
	Sprite*sprite,WorldTransform worldTransfrom, Vector4 pos, Vector4 color)
{
	directX_ = directX;
	CResource_ = CResource;
	PSO_ = PSO;
	sprite_ = sprite;

	int Num = 3;

	resource_ = CResource_->Vector4CreateResource(Num);

	color_ = color;
	centorPos_ = pos;
	worldTransform_ = worldTransfrom;

}

void Createtriangle::TransforMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld_ = m;
}

void Createtriangle::Draw()
{
	VertexData* vertexData = nullptr;
	Vector4* MaterialData = nullptr;
	Matrix4x4* wvpData = nullptr;

	//書き換えのためのアドレス取得
	resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
	resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	//左下
	vertexData[0] = { centorPos_.x,-size,centorPos_.y,-size,centorPos_.z,centorPos_.w };

	//上
	vertexData[1] = { centorPos_.x,centorPos_.y,size,centorPos_.z,centorPos_.w };

	//右下
	vertexData[2] = { centorPos_.x,+size,centorPos_.y,-size,centorPos_.z,centorPos_.w };


	Vector4 colorData = color_;

	*MaterialData = colorData;

	*wvpData = worldTransform_.matWorld_;

	CommandCall(resource_);
}

void Createtriangle::Release()
{
	CResource_->Release(resource_.Vertex);
	CResource_->Release(resource_.Material);
	CResource_->Release(resource_.wvpResource);

}

void Createtriangle::CommandCall(ResourcePropety resource)
{
	PSOProperty PSO = PSO_->GetPSO().shape;

	//RootSignatureを設定,PSOに設定しているけど別途設定が必要
	directX_->GetcommandList()->SetGraphicsRootSignature(PSO.rootSignature);
	directX_->GetcommandList()->SetPipelineState(PSO.GrahicsPipeLineState);

	//形状を設定
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &resource.vertexBufferView_);

	directX_->GetcommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//マテリアルCBufferの場所を設定
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, resource.Material->GetGPUVirtualAddress());

	//wvp用のCBufferの場所を設定
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, resource.wvpResource->GetGPUVirtualAddress());

	directX_->GetcommandList()->DrawInstanced(3, 1, 0, 0);


}


ResourcePropety Sprite::CreateResourceSprite(const int NumVertex)
{
	ResourcePropety result;

	result.Vertex = CreatBufferResourceSprite(sizeof(VertexData) * NumVertex);
	result.Material = CreatBufferResourceSprite(sizeof(Vector4));
	result.wvpResource = CreatBufferResourceSprite(sizeof(Matrix4x4));
	result.vertexBufferView_ = CreateBufferViewSprite(sizeof(VertexData) * NumVertex, result.Vertex, NumVertex);

	return result;
}


ID3D12Resource* Sprite::CreatBufferResourceSprite(size_t sizeInbytes)
{
	ID3D12Resource* RssultResource = nullptr;
	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	//頂点リソースの設定
	D3D12_RESOURCE_DESC ResouceDesc{};
	//バッファリソース,テクスチャまたは別の設定する
	ResouceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	ResouceDesc.Width = sizeInbytes;

	//バッファの場合はこれらを１にするのがきまり
	ResouceDesc.Height = 1;
	ResouceDesc.DepthOrArraySize = 1;
	ResouceDesc.MipLevels = 1;
	ResouceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにするきまり
	ResouceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT hr = directX_->Getdevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResouceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&RssultResource));
	assert(SUCCEEDED(hr));


	return RssultResource;

}

D3D12_VERTEX_BUFFER_VIEW Sprite::CreateBufferViewSprite(size_t sizeInbytes, ID3D12Resource* Resource, const int size)
{
	D3D12_VERTEX_BUFFER_VIEW resultBufferViewSprite = {};

	resultBufferViewSprite.BufferLocation = Resource->GetGPUVirtualAddress();

	//使用するリソースのサイズは頂点3つ分のサイズ
	resultBufferViewSprite.SizeInBytes = UINT(sizeInbytes);

	//1頂点あたりのサイズ
	resultBufferViewSprite.StrideInBytes = UINT(sizeInbytes / size);

	return resultBufferViewSprite;

}