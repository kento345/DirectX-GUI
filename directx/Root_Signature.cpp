#include "Root_Signature.h"
#include<cassert>

//ルートシグネチャを作成
bool Root_Signature::create(const D3D12_ROOT_SIGNATURE_DESC& desc) noexcept {
	ID3D12Device* dev = Device::instance().get();
	if (!dev) {
		assert(false && "デバイスがありません.");
		return false;;
	}

	//ルートシグネチャのシリアライズ
	Microsoft::WRL::ComPtr<ID3DBlob> sigature{};
	Microsoft::WRL::ComPtr<ID3DBlob> error{};

	auto res = D3D12SerializeRootSignature(
		&desc,
		D3D_ROOT_SIGNATURE_VERSION_1,
		&sigature,
		&error
	);

	if (FAILED(res)) {
		if (error) {
			const char* msg = static_cast<const char*>(error->GetBufferPointer());
			OutputDebugStringA(msg);
			OutputDebugStringA("\n");
		}
		assert(false && "ルートシグネチャのシリアライズに失敗しました.");
		return false;
	}

	//ルートシグネチャの生成
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rs;
	res = dev->CreateRootSignature(
		0,
		sigature->GetBufferPointer(),
		sigature->GetBufferSize(),
		IID_PPV_ARGS(&rs)
	);
	if (FAILED(res)) {
		assert(false && "ルートシグネチャの生成に失敗しました.");
		return false;
	}

	rootSignature_ = rs;
	return true;
}

//ルートシグネチャを取得
ID3D12RootSignature* Root_Signature::get() const noexcept {
	if (!rootSignature_) {
		assert(false && "ルートシグネチャが生成されていません.");
		return nullptr;
	}
	return rootSignature_.Get();
}