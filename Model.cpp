#include "Model.h"

void Model::Initilize(DirectXCommon* directX,CreateResources* CResource,Vector4 pos, float size, WorldTransform worldTransform, texResourceProperty tex, SlectModel select)
{
	CResource_ = CResource;
	directX_ = directX;

	switch (select)
	{
	case Cube:
		break;
	case sphere:

		state_ = new Sphere;
		break;
	}
	state_->Initilize(directX_, CResource_, pos, size, worldTransform, tex);
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
