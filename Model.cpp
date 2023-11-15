#include "Model.h"

void Model::Initilize(Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex, SlectModel select)
{

	switch (select)
	{
	case Cube:
		break;
	case sphere:

		state_ = new Sphere;
		break;
	}
	state_->Initilize(pos, size, worldTransform, tex);
}

void Model::Draw()
{
	state_->Draw();
}

void Model::Release()
{
	state_->Release();
}

void Model::TransferMatrix(Matrix4x4 m)
{
	state_->TransferMatrix(m);
}

void Model::SetTexPropety(texResourceProperty NewTex)
{
	state_->SetTexPropety(NewTex);

}
