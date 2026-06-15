#pragma once

#include"Device.h"
#include"Descriptor_heap.h"

class Constant_Buffer final
{
public:
	//コンストラクタ
	Constant_Buffer() = default;
	//デストラクタ
	~Constant_Buffer() = default;

	//コンスタントバッファの作成
	bool create(UINT bufferSize)noexcept;
	
	//コンスタントバッファを取得
	ID3D12Resource* constantBuffer()const noexcept;

	//GPU用ディスクリプタハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer_{};//コンスタントバッファ
	UINT	 descriptorIndex_{};//ディスクリプタインデックス
	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle_{};//GPU用ディスクリプタハンドル
};

