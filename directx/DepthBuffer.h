#pragma once

#include"Device.h"
class DepthBuffer final
{
public:
	//コンストラクタ
	DepthBuffer() = default;

	//デストラクタ
	~DepthBuffer();

	//デプスバッファを生成する
	bool create()noexcept;

	//デプスバッファを取得
	ID3D12Resource* depthBuffer()const noexcept;

	//ディスクリプタハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_; //レンダーターゲットリソースの配列
	D3D12_CPU_DESCRIPTOR_HANDLE handle_{};               //ディスクリプタハンドル
};

