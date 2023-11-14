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
	ID3D12Resource* Vertex;
	ID3D12Resource* Material;
	ID3D12Resource* wvpResource;

};



class CreateResource
{
public:

	ResourcePropety GetVertexDataCreateResource(size_t SizeInBytes) { return VertexDataCreateResource(num); };

	ResourcePropety Vector4CreateResource(const int Num);

	ResourcePropety VertexDataCreateResource(const int Num);

	ID3D12Resource* CreateBufferResource(ID3D12Device* device, size_t SizeInBytes);

	D3D12_VERTEX_BUFFER_VIEW CreateBufferView(ID3D12Resource*Resource,size_t strideInBytes,int size);

	void Release(ID3D12Resource* resource);


private:
	DirectXCommon* directX_;
	const int num;
};

