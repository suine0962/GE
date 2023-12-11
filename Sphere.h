#pragma once
#include"Mymath.h"
#include"DirectXCommon.h"
#include"TextureManeger.h"
#include"Camera.h"
#include"CreateResource.h"
#include"imguiManeger.h"
#include<numbers>
#include"GraphicsPipeline.h"
#include"IModelState.h"
class Sphere :public IModelState
{
public:

	Sphere();
	~Sphere();

	void Initilize(DirectXCommon*directX, CreateResources* CResource,GraphicsPipeline*PSO,
		Vector4 pos,float size,WorldTransform worldTransform,texResourceProperty tex)override;

	void Draw()override;

	void Release()override;

	void TransferMatrix(Matrix4x4 m)override;

	void SetTexPropety(texResourceProperty NewTex)override;

	Matrix4x4 GetWorldTransform()override;

	

private:

	void DrawColl();

	struct VertexData{
		Vector4 position;
		Vector2 texcoord;
	};

	Vector4* vertexData = nullptr;

	ResourcePropety resource_ = {};

	WorldTransform worldTransform = {};

	texResourceProperty tex_ = {};

	Vector4 color_ = { 1,1,1,1 };

	CreateResources* CResource_;

	DirectXCommon* directX_;

	GraphicsPipeline* PSO_ = nullptr;

	Vector4 centerPos_ = { 0,0,0,0 };

	float size_ = 0;

	const int VertexNum=16;
	
};

