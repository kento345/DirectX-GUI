#pragma once
#include"Device.h"	

class Command_Allocator final	
{
public:
	//コンストラクタ
	Command_Allocator() = default;
	//デストラクタ
	~Command_Allocator() = default;
	//コマンドアロケータの作成
	bool create(const D3D12_COMMAND_LIST_TYPE type)noexcept;

	//コマンドアロケータのリセット
	void reset()noexcept;

	ID3D12CommandAllocator* get()const noexcept;

	//コマンドリストのリストタイプ取得
	D3D12_COMMAND_LIST_TYPE getType()const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_{}; //コマンドアロケータ
	D3D12_COMMAND_LIST_TYPE type_{};//コマンドリストのタイプ
};

