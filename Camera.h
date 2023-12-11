#pragma once
#include"Mymath.h"
#include"ViewProjection.h"
#include"WorldTransform.h"
#include"WinApp.h"


enum CameraMode
{
	Orthgraphic,
	ParspectiveFov
};


class Camera
{
public:

	Camera();
	~Camera();

	void Initialize(WinApp*winApp);

	void Finalize();

	void SetPosition(Transform Transform);

	Matrix4x4 GetworldOthographicMatrix(Matrix4x4 matrix) { return worldOthographicMatrix(matrix); };

	Matrix4x4 worldViewProjectionMatrix(Matrix4x4 matrix);
	Matrix4x4 worldOthographicMatrix(Matrix4x4 matrix);
private:
	WinApp* winApp_ = nullptr;
	ViewProjection viewProjection{};
	Transform CameraTransform{};
	Matrix4x4 matrix{};
};

