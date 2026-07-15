#pragma once

#include"DXGI.h"	
#include<d3d12.h>
#include<wrl/client.h>
#include "../util.h"

//デバイスクラス
//簡易シングルトンパターンで作成

class Device final
{
public:
	//インスタンスの取得
	static Device& instance() noexcept {
		static Device instance;
		return instance;
	}

	//デバイスの作成
	bool create() noexcept;

	//デバイスの取得
	ID3D12Device* get() const noexcept;

	const DXGI& dxgi() const noexcept;

private:
	//コンストラクタ
	Device() = default;
	//デストラクタ	
	~Device() = default;

	//コピ-とムーブの禁止
	//データのコピー,代入,移動の禁止
	Device(const Device& r) = delete;
	Device& operator = (const Device& r) = delete;
	Device(Device&& r) = delete;
	Device& operator = (Device&& r) = delete;

private:
	DXGI dxgiInstance_{};                           //DXGIインスタンス
	Microsoft::WRL::ComPtr<ID3D12Device> device_{}; //デバイス
};
