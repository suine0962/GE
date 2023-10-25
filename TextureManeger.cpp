#include"TextureManeger.h"


void TextureManeger::Initilize()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
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

D3D12_RESOURCE_DESC TextureManeger::SettingResource(const DirectX::TexMetadata& metadata)
{

	//1 metaDataをもとにresourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};

	resourceDesc.Width = UINT(metadata.width);//Textureの幅
	resourceDesc.Height = UINT(metadata.height);//Tectureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);//mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行or配列textureの配列数
	resourceDesc.Format = metadata.format;//texureのformat
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//textureの次元数.普段使っているのは2次元

	return resourceDesc;
}


D3D12_HEAP_PROPERTIES TextureManeger::SettingHeap()
{
	//2 利用するheapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};

	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;//細かい設定を行う
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;//WriteBackポリシーでCPUアクセス可能
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;//プロセッサの近くに配置

	return heapProperties;
}



ID3D12Resource*TextureManeger:: CreateTexResource(const DirectX::TexMetadata& metadata, DirectXCommon* directX_)
{
	//3 Resoureの生成
	ID3D12Resource* Resource_;
	HRESULT hr = S_FALSE;

	D3D12_RESOURCE_DESC resourceDesc{};
	D3D12_HEAP_PROPERTIES heapProperties{};


	//リソースの設定
	resourceDesc= SettingResource(metadata);
	heapProperties = SettingHeap();

	//本体を作る
	 hr = directX_->Getdevice()->CreateCommittedResource(
		&heapProperties,//Heapの設定
		D3D12_HEAP_FLAG_NONE,//Heapの特殊な設定
		&resourceDesc,//Resourveの設定
		D3D12_RESOURCE_STATE_GENERIC_READ,//初回のResourceState.Textureは基本読むだけ
		nullptr,//Clear最適地.使わないでのnullptr
		IID_PPV_ARGS(&Resource_));
	assert(SUCCEEDED(hr));

	return Resource_;

}

void TextureManeger::LoadTexture(const std::string& filePath, DirectXCommon* directX_)
{
	//textureを読んで転送する
	DirectX::ScratchImage mipImages = CreateMipImage(filePath);
	const DirectX::TexMetadata& metadata = mipImages.GetMetadata();

	ID3D12Resource* texResource = CreateTexResource(metadata, directX_);
	UploadTexData(texResource, mipImages);
	//ここまで
	
	//ShaderResourceViewを作る
	//matadataをもとにSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	//SRVを作成するDescriptorHeapの場所を決める
	D3D12_CPU_DESCRIPTOR_HANDLE texSrvHandleCPU = directX_->GetSrvDescriptorHeap_()->GetCPUDescriptorHandleForHeapStart();
	texSrvHandleGPU = directX_->GetSrvDescriptorHeap_()->GetGPUDescriptorHandleForHeapStart();

	texSrvHandleCPU.ptr += directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	texSrvHandleGPU.ptr += directX_->Getdevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//SRVの作成
	directX_->Getdevice()->CreateShaderResourceView(texResource, &srvDesc, texSrvHandleCPU);

}




