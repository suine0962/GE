#include "Mymath.h"

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result;

	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] +
		m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] +
		m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] +
		m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] +
		m1.m[0][3] * m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] +
		m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] +
		m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] +
		m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] +
		m1.m[1][3] * m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] +
		m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] +
		m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] +
		m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] +
		m1.m[2][3] * m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] +
		m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] +
		m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] +
		m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] +
		m1.m[3][3] * m2.m[3][3];

	return result;
}


Matrix4x4 MakeIdenttity4x4()
{
	Matrix4x4 result;

	result.m[0][0] = 1;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 1;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1;
	result.m[2][3] = 0;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = 0;
	result.m[3][3] = 1;

	return result;
}

Matrix4x4 MakeRotateMatrix(const Vector3& rot) {

	Matrix4x4 rotate[3];

	// X回転
	rotate[0].m[0][0] = 1;
	rotate[0].m[0][1] = 0;
	rotate[0].m[0][2] = 0;
	rotate[0].m[0][3] = 0;

	rotate[0].m[1][0] = 0;
	rotate[0].m[1][1] = std::cos(rot.x);
	rotate[0].m[1][2] = std::sin(rot.x);
	rotate[0].m[1][3] = 0;

	rotate[0].m[2][0] = 0;
	rotate[0].m[2][1] = -std::sin(rot.x);
	rotate[0].m[2][2] = std::cos(rot.x);
	rotate[0].m[2][3] = 0;

	rotate[0].m[3][0] = 0;
	rotate[0].m[3][1] = 0;
	rotate[0].m[3][2] = 0;
	rotate[0].m[3][3] = 1;

	// Y回転
	rotate[1].m[0][0] = std::cos(rot.y);
	rotate[1].m[0][1] = 0;
	rotate[1].m[0][2] = -std::sin(rot.y);
	rotate[1].m[0][3] = 0;

	rotate[1].m[1][0] = 0;
	rotate[1].m[1][1] = 1;
	rotate[1].m[1][2] = 0;
	rotate[1].m[1][3] = 0;

	rotate[1].m[2][0] = std::sin(rot.y);
	rotate[1].m[2][1] = 0;
	rotate[1].m[2][2] = std::cos(rot.y);
	rotate[1].m[2][3] = 0;

	rotate[1].m[3][0] = 0;
	rotate[1].m[3][1] = 0;
	rotate[1].m[3][2] = 0;
	rotate[1].m[3][3] = 1;

	// Z回転
	rotate[2].m[0][0] = std::cos(rot.z);
	rotate[2].m[0][1] = std::sin(rot.z);
	rotate[2].m[0][2] = 0;
	rotate[2].m[0][3] = 0;

	rotate[2].m[1][0] = -std::sin(rot.z);
	rotate[2].m[1][1] = std::cos(rot.z);
	rotate[2].m[1][2] = 0;
	rotate[2].m[1][3] = 0;

	rotate[2].m[2][0] = 0;
	rotate[2].m[2][1] = 0;
	rotate[2].m[2][2] = 1;
	rotate[2].m[2][3] = 0;

	rotate[2].m[3][0] = 0;
	rotate[2].m[3][1] = 0;
	rotate[2].m[3][2] = 0;
	rotate[2].m[3][3] = 1;

	Matrix4x4 rotateMultiply = Multiply(rotate[0], Multiply(rotate[1], rotate[2]));

	return rotateMultiply;
}


Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate) {



	// スケーリング行列
	Matrix4x4 matScale{ 0 };
	matScale.m[0][0] = scale.x;
	matScale.m[1][1] = scale.y;
	matScale.m[2][2] = scale.z;

	matScale.m[3][3] = 1;

	// X軸回転まわりの回転

	Matrix4x4 matRotX = { 0 };

	matRotX.m[0][0] = 1;
	matRotX.m[1][1] = cosf(rot.x);
	matRotX.m[2][1] = -sinf(rot.x);
	matRotX.m[1][2] = sinf(rot.x);
	matRotX.m[2][2] = cosf(rot.x);
	matRotX.m[3][3] = 1;

	// Y軸まわりの回転
	Matrix4x4 matRotY = { 0 };
	matRotY.m[0][0] = cosf(rot.y);
	matRotY.m[1][1] = 1;
	matRotY.m[0][2] = -sinf(rot.y);
	matRotY.m[2][0] = sinf(rot.y);
	matRotY.m[2][2] = cosf(rot.y);
	matRotY.m[3][3] = 1;

	// Z軸まわりの回転
	Matrix4x4 matRotZ = { 0 };
	matRotZ.m[0][0] = cosf(rot.z);
	matRotZ.m[1][0] = sinf(rot.z);
	matRotZ.m[0][1] = -sinf(rot.z);
	matRotZ.m[1][1] = cosf(rot.z);
	matRotZ.m[2][2] = 1;
	matRotZ.m[3][3] = 1;

	Matrix4x4 matRot = Multiply(Multiply(matRotZ, matRotX), matRotY);

	Matrix4x4 matTrans = { 0 };

	matTrans.m[0][0] = 1;
	matTrans.m[1][1] = 1;
	matTrans.m[2][2] = 1;
	matTrans.m[3][3] = 1;
	matTrans.m[3][0] = translate.x;
	matTrans.m[3][1] = translate.y;
	matTrans.m[3][2] = translate.z;


	Matrix4x4 matWorld = Multiply(Multiply(matScale, matRot), matTrans);

	return matWorld;
}


Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result;

	float date = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2] -

		m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2] +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2] -

		m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0] +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	float dateRect = -sqrtf(date * date);

	result.m[0][0] = (
		m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] -
		m.m[1][3] * m.m[2][2] * m.m[3][1] -
		m.m[1][2] * m.m[2][1] * m.m[3][3] -
		m.m[1][1] * m.m[2][3] * m.m[3][2])
		/ dateRect;

	result.m[0][1] = (
		-m.m[0][1] * m.m[2][2] * m.m[3][3] -
		m.m[0][2] * m.m[2][3] * m.m[3][1] -
		m.m[0][3] * m.m[2][1] * m.m[3][2] +
		m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[2][3] * m.m[3][2])
		/ dateRect;

	result.m[0][2] = (
		m.m[0][1] * m.m[1][2] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] -
		m.m[0][3] * m.m[1][2] * m.m[3][1] -
		m.m[0][2] * m.m[1][1] * m.m[3][3] -
		m.m[0][1] * m.m[1][3] * m.m[3][2])
		/ dateRect;

	result.m[0][3] = (
		-m.m[0][1] * m.m[1][2] * m.m[2][3] -
		m.m[0][2] * m.m[1][3] * m.m[2][1] -
		m.m[0][3] * m.m[1][1] * m.m[2][2] +
		m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] +
		m.m[0][1] * m.m[1][3] * m.m[2][2])
		/ dateRect;

	result.m[1][0] = (
		-m.m[1][0] * m.m[2][2] * m.m[3][3] -
		m.m[1][2] * m.m[2][3] * m.m[3][0] -
		m.m[1][3] * m.m[2][0] * m.m[3][2] +
		m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[1][0] * m.m[2][3] * m.m[3][2])
		/ dateRect;

	result.m[1][1] = (
		m.m[0][0] * m.m[2][2] * m.m[3][3] +
		m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] -
		m.m[0][3] * m.m[2][2] * m.m[3][0] -
		m.m[0][2] * m.m[2][0] * m.m[3][3] -
		m.m[0][0] * m.m[2][3] * m.m[3][2])
		/ dateRect;

	result.m[1][2] = (
		-m.m[0][0] * m.m[1][2] * m.m[3][3] -
		m.m[0][2] * m.m[1][3] * m.m[3][0] -
		m.m[0][3] * m.m[1][0] * m.m[3][2] +
		m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] +
		m.m[0][0] * m.m[1][3] * m.m[3][2])
		/ dateRect;

	result.m[1][3] = (
		m.m[0][0] * m.m[1][2] * m.m[2][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] -
		m.m[0][3] * m.m[1][2] * m.m[2][0] -
		m.m[0][2] * m.m[1][0] * m.m[2][3] -
		m.m[0][0] * m.m[1][3] * m.m[2][2])
		/ dateRect;

	result.m[2][0] = (
		m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] -
		m.m[1][3] * m.m[2][1] * m.m[3][0] -
		m.m[1][1] * m.m[2][0] * m.m[3][3] -
		m.m[1][0] * m.m[2][3] * m.m[3][1])
		/ dateRect;

	result.m[2][1] = (
		-m.m[0][0] * m.m[2][1] * m.m[3][3] -
		m.m[0][1] * m.m[2][3] * m.m[3][0] -
		m.m[0][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] +
		m.m[0][0] * m.m[2][3] * m.m[3][1])
		/ dateRect;

	result.m[2][2] = (
		m.m[0][0] * m.m[1][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] -
		m.m[0][3] * m.m[1][1] * m.m[3][0] -
		m.m[0][1] * m.m[1][0] * m.m[3][3] -
		m.m[0][0] * m.m[1][3] * m.m[3][1])
		/ dateRect;

	result.m[2][3] = (
		-m.m[0][0] * m.m[1][1] * m.m[2][3] -
		m.m[0][1] * m.m[1][3] * m.m[2][0] -
		m.m[0][3] * m.m[1][0] * m.m[2][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] +
		m.m[0][0] * m.m[1][3] * m.m[2][1])
		/ dateRect;

	result.m[3][0] = (
		-m.m[1][0] * m.m[2][1] * m.m[3][2] -
		m.m[1][1] * m.m[2][2] * m.m[3][0] -
		m.m[1][2] * m.m[2][0] * m.m[3][1] +
		m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] +
		m.m[1][0] * m.m[2][2] * m.m[3][1])
		/ dateRect;

	result.m[3][1] = (
		m.m[0][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] -
		m.m[0][2] * m.m[2][1] * m.m[3][0] -
		m.m[0][1] * m.m[2][0] * m.m[3][2] -
		m.m[0][0] * m.m[2][2] * m.m[3][1])
		/ dateRect;

	result.m[3][2] = (
		-m.m[0][0] * m.m[1][1] * m.m[3][2] -
		m.m[0][1] * m.m[1][2] * m.m[3][0] -
		m.m[0][2] * m.m[1][0] * m.m[3][1] +
		m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] +
		m.m[0][0] * m.m[1][2] * m.m[3][1])
		/ dateRect;

	result.m[3][3] = (
		m.m[0][0] * m.m[1][1] * m.m[2][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] -
		m.m[0][2] * m.m[1][1] * m.m[2][0] -
		m.m[0][1] * m.m[1][0] * m.m[2][2] -
		m.m[0][0] * m.m[1][2] * m.m[2][1])
		/ dateRect;

	return result;
}


/// 透視投影行列
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip) {
	Matrix4x4 result;
	float tangent = tan(fovY / 2);
	float cot = 1 / tangent;


	result.m[0][0] = 1 / aspectRatio * cot;
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = cot;
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1;

	result.m[3][0] = 0;
	result.m[3][1] = 0;
	result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
	result.m[3][3] = 0;

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result;

	result.m[0][0] = 2 / (right - left);
	result.m[0][1] = 0;
	result.m[0][2] = 0;
	result.m[0][3] = 0;

	result.m[1][0] = 0;
	result.m[1][1] = 2 / (top - bottom);
	result.m[1][2] = 0;
	result.m[1][3] = 0;

	result.m[2][0] = 0;
	result.m[2][1] = 0;
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[2][3] = 0;

	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);
	result.m[3][3] = 1;

	return result;

}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}


