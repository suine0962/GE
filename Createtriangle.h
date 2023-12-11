#pragma once
#include"WorldTransform.h"
#include"CreateResource.h"
#include"GraphicsPipeline.h"
#include"TextureManeger.h"
#include"Sprite.h"

class Createtriangle
{
public:

	Createtriangle();
	~Createtriangle();

	void Initilize(DirectXCommon*directX ,CreateResources*CResource,GraphicsPipeline*PSO,
		Sprite*sprite,WorldTransform worldTransfrom,Vector4 pos,Vector4 color);

	void TransforMatrix(Matrix4x4 m);

	void Draw();

	void Release();

private:

	void CommandCall(ResourcePropety resource);

	DirectXCommon* directX_ = nullptr;

	GraphicsPipeline* PSO_ = nullptr;

	CreateResources* CResource_ = nullptr;

	Sprite* sprite_ = nullptr;

	ResourcePropety resource_;

	WorldTransform worldTransform_;

	Vector4 centorPos_ ={ 0,0,0,0 };

	Vector4 color_ = { 1,1,1,1 };

	float size = 0.1f;
};

