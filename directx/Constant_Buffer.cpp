#include "Constant_Buffer.h"
#include<cassert>

namespace {
	constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
}


//デストラクタ
Constant_Buffer:: ~Constant_Buffer() {
	//ディストラクタヒープの開放も必要
	DescriptorHeapContainer::instance().releaseDescriptor(heapType_, descriptorIndex_);
}

//コンスタントバッファの作成
bool Constant_Buffer::create(UINT bufferSize)noexcept {
	//アライメント済みサイズ計算
	const auto size = (sizeof(bufferSize) + 255) & ~255;

	//バッファリソースの作成
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Width = size;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	const auto res = Device::instance().get()->CreateCommittedResource(
		&heapProps,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constantBuffer_));
	if (FAILED(res)) {
		assert(false && "コンスタントバッファの作成に失敗しました");
		return false;
	}

	const auto descriptorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType_);
	if (!descriptorIndex.has_value()) {
		assert(false && "コンスタントバッファのディスクリプタ確保に失敗しました");
		return false;
	}
	//ディスクリプタインデックスを保存
	descriptorIndex_ = descriptorIndex.value();
	//ヒープ取得
	auto heap = DescriptorHeapContainer::instance().get(heapType_);

	//コンスタントバッファビューの設定
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc{};
	cbvDesc.BufferLocation = constantBuffer_->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;

	//ディスクリプタのサイズを取得
	UINT cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType_);

	//ディスクリプタヒープの開始ハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
	//指定されたインデックス分ハンドルを進める
	cpuHandle.ptr += descriptorIndex_ * cbvDescriptorSize;

	return  true;
}

//コンスタントバッファを取得する
ID3D12Resource* Constant_Buffer::constantBuffer()const noexcept{
	assert(constantBuffer_ && "コンスタントバッファが未作成です\0");
	return constantBuffer_.Get();
}

//GPU用のディスクリプタハンドルを取得する
D3D12_GPU_DESCRIPTOR_HANDLE Constant_Buffer::getGpuDescriptorHandle()const noexcept {
	assert(constantBuffer_ && "コンスタントバッファが未作成です\0");
	return gpuHandle_;
}