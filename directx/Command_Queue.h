#pragma once

#include"Device.h"

class Command_Queue final	
{
public:
	//コンストラクタ
	Command_Queue() = default;
	//デストラクタ
	~Command_Queue() = default;

	//コマンドキューの作成
	bool create()noexcept;
	//コマンドキュー取得
	ID3D12CommandQueue* get()const noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commanQueue_{};//コマンドキュー
};
