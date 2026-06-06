#pragma once
#include"Devie.h"

//フェンス(CPUとGPUの動機)制御クラス
class Fence final
{
public:
	//コンストラクタ
	Fence() = default;
	//ディストラクタ
	~Fence() = default;
	//フェンスを作成
	bool create()noexcept;
	//同期待ち
	void wait(UINT64 fenceValue) const noexcept;

	//フェンスを取得
	ID3D12Fence* get()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_{}; //フェンス
	HANDLE waitGpuEvent_{};                       //GPUとCPU 同期用のイベントハンドル
};

