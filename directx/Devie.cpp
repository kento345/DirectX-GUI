#include "Devie.h"
#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxguid.lib")

//デバイスを作成
bool Device::create() noexcept {
	//DXGI作成
	if (!dxgiInstance_.setDisplayAdapter()) {
		assert(false, "DXGIのアダプタ設定に失敗しました");
		return false;
	}
	//デバイス作成
	const auto hr = D3D12CreateDevice(dxgiInstance_.displayAdapter(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device_));
	if (FAILED(hr)) {
		assert(false, "デバイス作成に失敗");
		return false;
	}
	return true;
}

//デバイス取得
ID3D12Device* Device::get()const noexcept {
	if (!device_) {
		assert(false && "デバイスが未作成です");
	}
	return device_.Get();
}

//インスタンス取得
const DXGI& Device::dxgi()const noexcept {
	return dxgiInstance_;
}