#pragma once
#include"Mymath.h"
class WorldTransform {
public:
	// ローカルスケール
	Vector3 scale_ = { 1, 1, 1 };
	// X,Y,Z軸回りのローカル回転角
	Vector3 rotation_ = { 0, 0, 0 };
	// ローカル座標
	Vector3 transform_ = { 0, 0, 0 };
	// ローカル → ワールド変換行列
	Matrix4x4 matWorld_;
	
	void Initilize();

	void UpdataMatrix();


};
