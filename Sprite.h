#pragma once
#include "TextureManeger.h"
#include"Mymath.h"
#include"WorldTransform.h"
#include"DirectXCommon.h"
#include"Engine.h"
#include"CreateResource.h"
#include"Camera.h"
#include"GraphicsPipeline.h"

enum SpriteMode
{
	Triangle,
	Box
};

struct BoxPosition
{
	Vector4 LeftTop;
	Vector4 RightTop;
	Vector4 LeftBottom;
	Vector4 RightBottom;;

};



class Sprite:CreateResource
{
public:

	Sprite();
	~Sprite();

	void Initilize(DirectXCommon*directX,CreateResource*CResource,Engine*engine, Vector2 leftpos, float size,
		WorldTransform worldTransform, texResourceProperty texResource, const SpriteMode mode);

	void TransforMatrix(Matrix4x4 m);

	void SetTexPropety(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return worldTransform_.matWorld_; };

	void Draw();

	void Release();

private:


	void CommandCall(const int Num);

	void Releace(ID3D12Resource* resource);


	DirectXCommon* directX_ = nullptr;
	CreateResource* CResource_ = nullptr;
	GraphicsPipeline* pso_ = nullptr;

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};


	D3D12_VERTEX_BUFFER_VIEW CreateBufferViewSprite(size_t sizeInbytes, ID3D12Resource* Resource, const int size);

	ID3D12Resource* CreatBufferResourceSprite(size_t sizeInbytes);

	WorldTransform worldTransform_;

	ResourcePropety CreateResourceSprite(const int Num);

	ResourcePropety resource_;

	texResourceProperty tex;

	BoxPosition pos;

	SpriteMode mode_;

	Vector4 color_ = { 1,1,1,1 };

	Camera* camera_ = nullptr;

	Engine* engine_ = nullptr;

	VertexData* vertexData;
	Vector4* MaterialData;
	Matrix4x4* wvpData;
};

