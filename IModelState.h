#pragma once
#include"WorldTransform.h"
#include"TextureManeger.h"

class Model;

class IModelState
{
public:

	virtual void Initilize(Vector4 pos, float size,
		WorldTransform worldTransform, texResourceProperty tex) = 0;

	virtual void Draw() = 0;

	virtual void Release() = 0;

	virtual void TransferMatrix(Matrix4x4 m) = 0;

	virtual void SetTexPropety(texResourceProperty NewTex) = 0;

	virtual Matrix4x4 GetWorldTransform() = 0;

private:
};

