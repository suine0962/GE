#include "Sprite.h"

void Sprite::Initilize(DirectXCommon*directX,Engine*engine, Vector2 leftpos,
	float size, WorldTransform worldTransform, texResourceProperty texResource, const SpriteMode mode)
{
	assert(directX);
	assert(engine);
	directX_ = directX;
	engine_ = engine;

	worldTransform_ = worldTransform;
	tex = texResource;
	const int TriangleNumVertex = 3;
	const int BoxNumVertex = 6;

	pos.LeftTop = { leftpos.x,leftpos.y,0.0f,1.0f };
	pos.RightTop = { leftpos.x + size,leftpos.y,0.0f,1.0f };
	pos.LeftBottom = { leftpos.x,leftpos.y + size,0.0f,1.0f };
	pos.RightBottom = { leftpos.x + size,leftpos.y + size,0.0f,1.0f };

	CreateVertexResourceSprite();

}

void Sprite::CreateVertexResourceSprite()
{
		//sprite用の頂点リソースを作る
	ID3D12Resource* vertexResourceSprite = engine_->GetCreateBufferResource();

		//頂点バッファビューを作る

		//リソースの先頭のアドレスから使う
		CreateBufferViewSprite_.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
		//使用するリソースのサイズは頂点６つ分のサイス
		CreateBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
		//１頂点あたりのサイズ
		CreateBufferViewSprite_.StrideInBytes = sizeof(VertexData);

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