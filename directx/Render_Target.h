#pragma once

#include"Device.h"
#include"Swap_Chain.h"
#include"Descriptor_heap.h"
#include<vector>

class Render_Target final
{
public:
	//コンストラクター
	Render_Target() = default;
	//デストラクター
	~Render_Target();

	//バックバッファを生成する
	//生成成否を返す
	bool createBackBuffer(const Swap_Chain& swapChain)noexcept;

	//レンダーターゲットを作成する
	//生成成否を返すs
	bool createrenderTarget(UINT w, UINT h, DXGI_FORMAT format, UINT num)noexcept;

	//ビュー(ディスクリプタハンドル)を取得
	//ディスクリプタハンドルを返す
	D3D12_CPU_DESCRIPTOR_HANDLE getCpuDescriptorHandle(UINT index)const noexcept;

	//GPU用ディスクリプタハンドルを取得
	//GPU用ディスクリプタハンドルを返す
	D3D12_GPU_DESCRIPTOR_HANDLE getGpuDescriptorHandle(UINT insex)const noexcept;

	//レンダーターゲットを取得する
	ID3D12Resource* get(UINT index) const noexcept;

	//テクスチャサイズ取得
	std::pair<UINT, UINT> size()const noexcept;

private:
	//レンダーターゲットビュー作成
	//生成成否を返す
	bool createRenderTargetView()noexcept;

	//シェーダーリソースビューを作成
	//生成成否を返す
	bool createShaderResourceView()noexcept;

private:
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> renderTargets_{};//レンダーターゲットリソースの配列
	std::vector<UINT> rtvDescriptorIndex_{}; //ディスクリプタインデックス
	std::vector <UINT> srvDescriptorIndex_{};//ディスクリプタインデックス
};