#include "Camera.h"

void Camera::Initialize(WinApp*winApp)
{
	assert(winApp);
	winApp_ = winApp;

	CameraTransform= 
	{
	{ 1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} 
	};

	//透視変換行列
	viewProjection.aspectRatio
		= float(winApp_->kWindowWidth 
			/ 
			float(winApp_->kWindowHeight));
	viewProjection.nearClip = 0.1f;
	viewProjection.farClip = 100.0f;
	viewProjection.fov = 0.45f;

	viewProjection.matProjection =
		MakePerspectiveFovMatrix(
			viewProjection.fov,
			viewProjection.aspectRatio,
			viewProjection.nearClip,
			viewProjection.farClip
		);
	//
}

void Camera::Finalize()
{
}



void Camera::SetPosition(Transform Transform)
{

	CameraTransform = Transform;
}

Matrix4x4 Camera::worldViewProjectionMatrix(Matrix4x4 matrix)
{
	Matrix4x4 Matrix = MakeAffineMatrix(
		CameraTransform.Scale,
		CameraTransform.rotate,
		CameraTransform.translate
	);
	Matrix4x4 viewMatrix = Inverse(Matrix);
	Matrix4x4 worldViewProjectionMatrix=
		Multiply(matrix, Multiply(viewMatrix,viewProjection.matProjection));
	return worldViewProjectionMatrix;
}


Matrix4x4 Camera::worldOthographicMatrix(Matrix4x4 matrix)
{
	Matrix4x4 viewMatrix = MakeIdenttity4x4();
	Matrix4x4 OrthographicsMatrix = MakeOrthographicMatrix(
		0, 0, float(winApp_->kWindowWidth), float(winApp_->kWindowHeight), 0.0f, 100.0f
	);
	Matrix4x4 worldViewProjectionMatrix =
		Multiply(matrix, Multiply(viewMatrix,
			OrthographicsMatrix));
	return worldViewProjectionMatrix;
}
