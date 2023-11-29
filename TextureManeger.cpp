#include"TextureManeger.h"



DescriptorSize descriptorSize_;

uint32_t texIndex;

void TextureManeger::Initilize(DirectXCommon*directX)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);

	directX_ = directX;

	descriptorSize_.SRV = directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	descriptorSize_.RTV = directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	descriptorSize_.DSV = directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	texIndex = 0;
}

void TextureManeger::Finalize()
{
	CoUninitialize();


}


DirectX::ScratchImage TextureManeger::CreateMipImage(const std::string& filePath)
{
	HRESULT hr = S_FALSE;

	//テクスチャファイルを読み込んでプログラムで使えるようにする
	DirectX::ScratchImage image{};
	std::wstring filePathw = ComvertString(filePath);
	hr = DirectX::LoadFromWICFile(filePathw.c_str(), DirectX::WIC_FLAGS_FORCE_SRGB, nullptr, image);
	assert(SUCCEEDED(hr));

	

	//ミップマップの生成
	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), DirectX::TEX_FILTER_SRGB, 0, mipImages);
	assert(SUCCEEDED(hr));
	//ミップマップ付きのデータを返す
	return mipImages;

}

D3D12_CPU_DESCRIPTOR_HANDLE TextureManeger::GetCPUDescriptorHandle(ID3D12DescriptorHeap*descriptorHeap,uint32_t descriptorSize,uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize *index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManeger::GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;

}


D3D12_RESOURCE_DESC TextureManeger::SettingResource(const DirectX::TexMetadata& metadata)
{

	//1 metaDataをもとにresourceの設定
	D3D12_RESOURCE_DESC resourceDesc_{};

	resourceDesc_.Width = UINT(metadata.width);//Textureの幅
	resourceDesc_.Height = UINT(metadata.height);//Tectureの高さ
	resourceDesc_.MipLevels = UINT16(metadata.mipLevels);//mipmapの数
	resourceDesc_.DepthOrArraySize = UINT16(metadata.arraySize);//奥行or配列textureの配列数
	resourceDesc_.Format = metadata.format;//texureのformat
	resourceDesc_.SampleDesc.Count = 1;//サンプリングカウント1固定
	resourceDesc_.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//textureの次元数.普段使っているのは2次元

	return resourceDesc_;
}


D3D12_HEAP_PROPERTIES TextureManeger::SettingHeap()
{
	//2 利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties_{};

	heapProperties_.Type = D3D12_HEAP_TYPE_CUSTOM;//細かい設定を行う
	heapProperties_.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperties_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置

	return heapProperties_;
}






ID3D12Resource*TextureManeger:: CreateTexResource(const DirectX::TexMetadata& metadata)
{
	//3 Resoureの生成
	HRESULT hr = S_FALSE;
	ID3D12Resource* Resource_;
	D3D12_RESOURCE_DESC resourceDesc_{};
	D3D12_HEAP_PROPERTIES heapProperties_{};
	
	//リソースの設定
	resourceDesc_= SettingResource(metadata);
	heapProperties_ = SettingHeap();

	//本体を作る
	 hr = directX_->Getdevice()->CreateCommittedResource(
		&heapProperties_,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定
		&resourceDesc_,//Resourveの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,//初回のResourceState.Textureは基本読むだけ
		nullptr,//Clear最適地.使わないでのnullptr
		IID_PPV_ARGS(&Resource_));
	assert(SUCCEEDED(hr));

	return Resource_;

}

void TextureManeger::UploadTexData(ID3D12Resource* tex, const DirectX::ScratchImage& mipImages)
{
	//meta情報を取得
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//全mipmapについて
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; ++mipLevel)
	{
		//miplevelを指定して各imageを取得
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		//textureに転送
		HRESULT hr = tex->WriteToSubresource(
			UINT(mipLevel),
			nullptr,//全領域へコピー
			img->pixels,//元データアドレス
			UINT(img->rowPitch),//１ラインサイズ
			UINT(img->slicePitch)//1枚サイズ
		);
		assert(SUCCEEDED(hr));
	}


}



texResourceProperty TextureManeger::LoadTexture(const std::string& filePath)
{
	

	//textureを読んで転送する
	DirectX::ScratchImage mipImages = CreateMipImage(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	ID3D12Resource* texResource = CreateTexResource(metadata);
	UploadTexData(texResource, mipImages);
	//ここまで

	//ShaderResourceViewを作る
	//matadataをもとにSRVの設定

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc_{};
	srvDesc_.Format = metadata.format;
	srvDesc_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc_.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc_.Texture2D.MipLevels = UINT(metadata.mipLevels);

	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE texSrvHandleCPU = GetCPUDescriptorHandle(directX_->GetSrvDescriptorHeap_(), descriptorSize_.SRV, texIndex);

	D3D12_GPU_DESCRIPTOR_HANDLE texSrvHandleGPU = GetGPUDescriptorHandle(directX_->GetSrvDescriptorHeap_(), descriptorSize_.SRV, texIndex);

	texSrvHandleCPU.ptr += directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	texSrvHandleGPU.ptr += directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	directX_->Getdevice()->CreateShaderResourceView(texResource, &srvDesc_, texSrvHandleCPU);

	

	tex.Resource = texResource;
	tex.SrvHandleGPU = texSrvHandleGPU;

	return tex;

}



texResourceProperty TextureManeger::Release(texResourceProperty tex)
{

	tex.Resource->Release();

	return tex;
}
