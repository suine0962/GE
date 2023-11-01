#pragma once
#include"Engine.h"
#include"DirectXCommon.h"
#include"Mymath.h"
struct VertexData {

	Vector4 position;
	Vector2 texcoord;

};

struct ResourcePropety
{
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ID3D12Resource* Resource;
	ID3D12Resource* Material;
	ID3D12Resource* wvpResource;
};



class CreateResource
{
public:

	ResourcePropety Vector4CreateResource(DirectXCommon* directX,const int Num);

	ResourcePropety VertexDataCreateResource(const int Num);

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t SizeInBytes);

	D3D12_VERTEX_BUFFER_VIEW CreateBufferView(ID3D12Resource*Resource,size_t SizeInBeyes,int size);

private:
	DirectXCommon* directX_;

};

