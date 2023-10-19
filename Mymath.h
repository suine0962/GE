#pragma once
#include<cassert>
#include<cmath>
#include"Vector3.h"
#include"Matrix4x4.h"

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Matrix4x4 MakeIdenttity4x4();

Matrix4x4 MakeRotateMatrix(const Vector3& rot);

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

Matrix4x4 Inverse(const Matrix4x4& m);

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
