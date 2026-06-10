#include "Command_Queue.h"
#include <cassert>	

//コマンドキュー作成
bool Command_Queue::create() noexcept {
	//コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC desc{};
	desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;//ダイレクトコマンドキュー
	desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;//通常優先度
	desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;//特別フラグなし
	desc.NodeMask = 0;//GPUは一つのみ使用する

	//コマンドキュー作成
	const auto hr = Device::instance().get()->CreateCommandQueue(&desc, IID_PPV_ARGS(&commanQueue_));
	if (FAILED(hr)) {
		assert(false && "コマンドキューの作成に失敗");
		return false;
	}
	return true;
}

//コマンドキューの取得
ID3D12CommandQueue* Command_Queue::get() const noexcept {
	if (!commanQueue_) {
		assert(false && "コマンドキューが未作成です");
	}
	return commanQueue_.Get();
}