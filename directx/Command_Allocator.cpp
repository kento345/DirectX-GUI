#include "Command_Allocator.h"
#include<cassert>

//コマンドアロケータの作成
bool Command_Allocator::create(const D3D12_COMMAND_LIST_TYPE type)noexcept {
	//コマンドリストタイプの設定
	type_ = type;
	//コマンドアロケータの作成
	const auto hr = Device::instance().get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(hr)) {
		assert(false && "コマンドアロケータの作成に失敗しました");
		return false;
	}
	return true;
}

//コマンドアロケータのリセット
void Command_Allocator::reset()noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です");
	}

	commandAllocator_->Reset();
}

//コマンドアロケータの取得
ID3D12CommandAllocator* Command_Allocator::get()const noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です");
	}

	return commandAllocator_.Get();
}

//コマンドリストタイプの取得
D3D12_COMMAND_LIST_TYPE Command_Allocator::getType()const noexcept {
	if (!commandAllocator_) {
		assert(false && "コマンドリストタイプが未設定です");
	}

	return type_;
}