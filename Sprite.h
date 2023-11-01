#pragma once
#include "TextureManeger.h"
#include"Mymath.h"
#include"WorldTransform.h"
#include"DirectXCommon.h"
#include"Engine.h"

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



class Sprite
{
public:

	Sprite();
	~Sprite();

	void Initilize(DirectXCommon*directX,Engine*engine, Vector2 leftpos, float size, WorldTransform worldTransform, texResourceProperty texResource, const SpriteMode mode);

	void TransforMatrix(Matrix4x4 m);

	void SetTexPropety(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return worldTransform_.matWorld_; };

	void CreateVertexResourceSprite();


private:


	void CommandCall();

	void Relese();


	DirectXCommon* directX_ = nullptr;
	Engine* engine_ = nullptr;
	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
	};


	D3D12_VERTEX_BUFFER_VIEW CreateBufferViewSprite_{};

	ID3D12Resource* CreateBufferResource = nullptr;

	WorldTransform worldTransform_;

	texResourceProperty tex;

	BoxPosition pos;

	SpriteMode mode;


};

