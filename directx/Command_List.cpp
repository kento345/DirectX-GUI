#include "Command_List.h"
#include<cassert>

//コマンドリスト生成
bool Command_List::create(const Command_Allocator& command_Allocator)noexcept {
	//コマンドリストの作成
	const auto hr = Device::instance().get()->CreateCommandList(
		0, command_Allocator.getType(), command_Allocator.get(), nullptr, IID_PPV_ARGS(&commandList_)
	);
	if (FAILED(hr)) {
		assert(false && "コマンドリストの作成に失敗しました");
		return false;
	}
	//コマンドリストの初期化状態設定
	commandList_->Close();
	return true;
}

//コマンドリストのリセット
void Command_List::reset(const Command_Allocator& command_Allocator)noexcept {
	if (!commandList_) {
		assert(false && "コマンドリストが未作成です");
	}
	//コマンドリストをリセット
	commandList_->Reset(command_Allocator.get(), nullptr);
}

//コマンドリストを取得する
ID3D12GraphicsCommandList* Command_List::get() const noexcept {
	if (!commandList_) {
		assert(false && "コマンドリストが未作成です");
	}
	return commandList_.Get();
}