#pragma once

#include"Device.h"
#include"Command_Allocator.h"	

class Command_List final	
{
public:
	//コンストラクタ
	Command_List() = default;
	//デストラクタ
	~Command_List() = default;

	//コマンドリスト作成
	bool create(const Command_Allocator& commanAllocator)noexcept;

	//コマンドリストリセット
	void reset(const Command_Allocator& commandAllocator)noexcept;

	ID3D12GraphicsCommandList* get()const noexcept;
	
private:
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_{};
};

