#pragma once
#include"Mymath.h"
#include"DirectXCommon.h"
#include"TextureManeger.h"
#include"Camera.h"
#include"CreateResource.h"
#include"imguiManeger.h"
#include<numbers>
#include"GraphicsPipeline.h"
class Sphere
{

public:
	void Initilize(DirectXCommon* directX, CreateResource* createResource,Vector4 pos,float size,texResourceProperty tex);

	void SphereDraw();

	void TransferMatrix(Matrix4x4 m);


	void SetTexPropety(texResourceProperty NewTex);

	Matrix4x4 GetWorldTransform();

	void Relese();

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

	DirectXCommon* directX_ = nullptr;

	CreateResource* createResource_;

	GraphicsPipeline* PSO_ = nullptr;

	Vector4 centerPos_ = { 0,0,0,0 };

	float radius_ = 0;

	const int VertexNum=16;


	
};

