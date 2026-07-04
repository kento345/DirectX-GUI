#pragma once
#include"Command_Queue.h"
class Swap_Chain final
{
public:
	//コンストラクタ
	Swap_Chain() = default;
	//デストラクター
	~Swap_Chain() = default;
	//スワップチェーンを作成
	bool create(const Command_Queue& commandQueue) noexcept;
	//スワップチェーンを取得
	IDXGISwapChain3* get() const noexcept;
	
	//スワップチェインの設定を取得する
	const DXGI_SWAP_CHAIN_DESC1& getDesc() const noexcept;

private:
	Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain_{}; //スワップチェーン
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_{}; //スワップチェーンの設定
};

