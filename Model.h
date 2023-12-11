#pragma once
#include"WorldTransform.h"
#include"GraphicsPipeline.h"
#include"TextureManeger.h"
#include"Sphere.h"
#include"IModelState.h"

enum SlectModel
{
	Cube,
	sphere
};

class Model
{
public:
	Model();
	~Model();


	void Initilize(DirectXCommon* directX,CreateResources* CResource ,GraphicsPipeline*PSO, Vector4 pos,float size ,
		WorldTransform worldTransform,texResourceProperty tex, SlectModel select);

	void Draw();

	void Release();

	void TransferMatrix(Matrix4x4 m);

	void SetTexPropety(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform() { return GetWorldTransform(); };

private:
	Shaders shader_{ nullptr,nullptr };

	IModelState* state_ = nullptr;

	CreateResources* CResource_ = nullptr;

	DirectXCommon* directX_ = nullptr;

	GraphicsPipeline* PSO_ = nullptr;
};

