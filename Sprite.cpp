#include "Sprite.h"

void Sprite::Initilize(DirectXCommon*directX, CreateResource* CResource,Vector2 leftpos,
	float size, WorldTransform worldTransform, texResourceProperty texResource, const SpriteMode mode)
{
	assert(directX);
	assert(CResource);

	directX_ = directX;
	CResource_ = CResource;
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
		resource_ = CreateResource(TriangleNumVertex);

		break;

	case Box:

		resource_ = CreateResource(BoxNumVertex);
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
	VertexData* vertexData;
	Vector4* MaterialData;
	Matrix4x4* wvpData;

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
		worldTransform_.matWorld_;//ここにカメラのデータを入れる

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
		worldTransform_.matWorld_;//ここにカメラのデータを入れる

		*wvpData = worldTransform_.matWorld_;

		CommandCall(BoxNum);

		break;
	}

}



//void Sprite::CreateVertexResourceSprite()
//{
//		//sprite用の頂点リソースを作る
//	ID3D12Resource* vertexResourceSprite = Resource_->CreateBufferResource()
//
//		//頂点バッファビューを作る
//
//		//リソースの先頭のアドレスから使う
//		CreateBufferViewSprite_.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
//		//使用するリソースのサイズは頂点６つ分のサイス
//		CreateBufferViewSprite_.SizeInBytes = sizeof(VertexData) * 6;
//		//１頂点あたりのサイズ
//		CreateBufferViewSprite_.StrideInBytes = sizeof(VertexData);
//
//		VertexData* vertexDataSprite = nullptr;
//		vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
//
//		//一枚目の三角形
//		//左下
//		vertexDataSprite[0].position = { 0.0f,360.0f,0.0f,1.0f };
//		vertexDataSprite[0].texcoord = { 0.0f,1.0f };
//		//左上
//		vertexDataSprite[1].position = { 0.0f,0.0f,0.0f,1.0f };
//		vertexDataSprite[1].texcoord = { 0.0f,0.0f };
//		//右下
//		vertexDataSprite[2].position = { 640.0f,360.0f,0.0f,1.0f };
//		vertexDataSprite[2].texcoord = { 1.0f,1.0f };
//
//		//二枚目の三角形
//		//左上
//		vertexDataSprite[3].position = { 0.0f,0.0f,0.0f,1.0f };
//		vertexDataSprite[3].texcoord = { 0.0f,0.0f };
//		//左上
//		vertexDataSprite[4].position = { 640.0f,0.0f,0.0f,1.0f, };
//		vertexDataSprite[4].texcoord = { 1.0f,0.0f };
//		//右下
//		vertexDataSprite[5].position = { 640.0f,360.0f,0.0f,1.0f };
//		vertexDataSprite[5].texcoord = { 1.0f,1.0f };
//}