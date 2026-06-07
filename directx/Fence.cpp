#include "Fence.h"
#include<cassert>	

bool Fence::create() noexcept {
	//フェンスの作成
	HRESULT  hr = Device::instance().get()->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
	if (FAILED(hr)) {
		assert(false && "フェンスの作成に失敗しました");
		return false;
	}
	//GPU　同期用のイベントハンドルを作成
	// --------------------ここがエラー----------------------
	waitGpuEvent_ = CreateEvent(nullptr, false, false,L"WAIT_GPU");
	//-------------------------------------------------------
	if (!waitGpuEvent_) {
		assert(false && "GPU 同期用イベントハンドルの作成に失敗しました");
		return false;
	}
	return true;
}

//同期待ち

void Fence::wait(UINT64 fenceValue) const noexcept {
	if (!fence_) {
		assert(false && "フェンスが未作成です");
		return ;
	}
	//フェンスの値が指定されました値に達するまで待機
	if (fence_->GetCompletedValue() < fenceValue) {
		//GPUがフェンス値に到達するまで待つ
		fence_->SetEventOnCompletion(fenceValue, waitGpuEvent_);
		WaitForSingleObject(waitGpuEvent_, INFINITE);
	}
}

//フェンスを取得
ID3D12Fence* Fence::get()const noexcept {
	if (!fence_) {
		assert(false && "フェンスが未作成です");
	}
	return fence_.Get();
}