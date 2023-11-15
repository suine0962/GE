#include "Sprite.h"
	
void Sprite::Initilize(
	Vector2 leftpos, float size, WorldTransform worldTransform,
	texResourceProperty texResource, const SpriteMode mode)
{
	worldTransform_ = worldTransform;
	tex = texResource;

	const int TriangleNumVertex = 3;
	const int BoxNumVertex = 6;

	pos.LeftTop = { leftpos.x,leftpos.y,0.0f,1.0f };
	pos.RightTop = { leftpos.x + size,leftpos.y,0.0f,1.0f };
	pos.LeftBottom = { leftpos.x,leftpos.y + size,0.0f,1.0f };
	pos.RightBottom = { leftpos.x + size,leftpos.y + size,0.0f,1.0f };

	switch (mode)
	{
	case Triangle:


		mode_ = mode;
		resource_ = CResource_->GetVertexDataCreateResource(TriangleNumVertex);

		break;

	case Box:

		resource_ = CResource_->GetVertexDataCreateResource(BoxNumVertex);
		mode_ = mode;

		break;
	}
}

void Sprite::TransforMatrix(Matrix4x4 m)
{
	worldTransform_.matWorld_ = m;
}

void Sprite::SetTexPropety(texResourceProperty NewTex)
{
	tex = NewTex;
}

void Sprite::Draw()
{

	const int TriangleNum = 3;
	const int BoxNum = 6;

	switch (mode_)
	{
	case Triangle:
		//書き込むためのアドレスを取得
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

		//左下
		vertexData[0].position = { pos.LeftBottom };
		vertexData[0].texcoord = { 0.0f,1.0f };
		//上
		vertexData[1].position = { pos.LeftTop };
		vertexData[1].texcoord = { 0.5f,0.0f };
		//右下
		vertexData[2].position = { pos.RightBottom };
		vertexData[2].texcoord = { 1.0f,1.0f };

		//マテリアル
		*MaterialData = color_;
	
		//行列の変換
		worldTransform_.matWorld_ = camera_->GetworldOthographicMatrix(worldTransform_.matWorld_);//ここにカメラのデータを入れる

		*wvpData = worldTransform_.matWorld_;

		CommandCall(TriangleNum);

		break;
	
	case Box:
		resource_.Vertex->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		resource_.Material->Map(0, nullptr, reinterpret_cast<void**>(&MaterialData));
		resource_.wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));


		vertexData[0].position = { pos.LeftBottom };
		vertexData[0].texcoord = { 0.0f,1.0f };
		//上
		vertexData[1].position = { pos.LeftTop };
		vertexData[1].texcoord = { 0.5f,0.0f };
		//右下
		vertexData[2].position = { pos.RightBottom };
		vertexData[2].texcoord = { 1.0f,1.0f };


		//左下
		vertexData[3].position = { pos.LeftTop };
		vertexData[3].texcoord = { 0.0f,1.0f };
		//上
		vertexData[4].position = { pos.RightTop };
		vertexData[4].texcoord = { 0.5f,0.0f };
		//右下
		vertexData[5].position = { pos.RightBottom };
		vertexData[5].texcoord = { 1.0f,1.0f };


		//マテリアル
		*MaterialData = color_;

		//行列の変換
		worldTransform_.matWorld_= camera_->GetworldOthographicMatrix(worldTransform_.matWorld_);

		*wvpData = worldTransform_.matWorld_;

		CommandCall(BoxNum);

		break;
	}

}

void Sprite::Release()
{
	Sprite::Releace(resource_.Material);
	Sprite::Releace(resource_.Vertex);
	Sprite::Releace(resource_.wvpResource);
}


void Sprite::Releace(ID3D12Resource* resource)
{
	resource->Release();
}

void Sprite::CommandCall(const int Num)
{
	PSOProperty PSO = pso_->GetPSO().sprite;

	directX_->GetcommandList()->SetGraphicsRootSignature(PSO.rootSignature);
	directX_->GetcommandList()->SetPipelineState(PSO.GrahicsPipeLineState);
	//形状を設定
	directX_->GetcommandList()->IASetVertexBuffers(0, 1, &resource_.vertexBufferView_);

	//wvp用のCBufferの場所を設定
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(1, resource_.wvpResource->GetGPUVirtualAddress());

	//マテリアルCBufferの場所を設定
	directX_->GetcommandList()->SetGraphicsRootConstantBufferView(0, resource_.Material->GetGPUVirtualAddress());

	//
	directX_->GetcommandList()->SetGraphicsRootDescriptorTable(2, tex.SrvHandleGPU);

	//描画(ドローコール)
	directX_->GetcommandList()->DrawInstanced(Num, 1, 0, 0);

}


ResourcePropety Sprite::CreateResourceSprite(const int Num)
{
	ResourcePropety result;

	result.Vertex = CreatBufferResourceSprite(sizeof(VertexData) * Num);
	result.Material = CreatBufferResourceSprite(sizeof(Vector4));
	result.wvpResource = CreatBufferResourceSprite(sizeof(Matrix4x4));
	result.vertexBufferView_ = CreateBufferViewSprite(sizeof(VertexData) * Num, result.Vertex, Num);

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
	//実際に頂点リソースを作る
	ID3D12Resource* Resource = nullptr;
	HRESULT hr = directX_->Getdevice()->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&ResouceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&Resource));
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