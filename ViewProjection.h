#pragma once
#include"Mymath.h"

class ViewProjection {
public:

	Matrix4x4 matProjection;
	Matrix4x4 viewMatrix;

	Matrix4x4 OthographicMatrix;
	Vector3 scale = { 1,1,1 };
	Vector3 translate;
	Vector3 rotate;


	float aspectRatio = 0.0f;
	float fov = 0.0f;
	float nearClip = 0.0f;
	float farClip = 0.0f;
};
