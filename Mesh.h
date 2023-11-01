#pragma once
#include"Mymath.h"
#include"DirectXCommon.h"
class Mesh
{

public:
	void Initilize(DirectXCommon* directX);

	void CreateMesh();

	void MeshDraw();

private:

	struct VertexData{
		Vector4 position;
		Vector2 texcoord;
	};

	Vector4* vertexData = nullptr;

	DirectXCommon* directX_ = nullptr;
};

