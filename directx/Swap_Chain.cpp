#include "Swap_Chain.h"
#include"DXGI.h"
#include"../window/Window.h"

#include<cassert>

//スワップチェーンを作成
bool Swap_Chain::create(const Command_Queue& commandQueue) noexcept {
	swapChainDesc_ = {};
	swapChainDesc_.BufferCount = 2; //バックバッファの数(ダブルバッファ)
	swapChainDesc_.Width = 0; //ウィンドウ幅(0で自動)
	swapChainDesc_.Height = 0; //ウィンドウ高さ(0で自動)
	swapChainDesc_.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //バックバッファのフォーマット
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //レンダーターゲットとして使用
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; //毎フレーム画面更新するので描写が終わったらbackバッファを破棄
	swapChainDesc_.SampleDesc.Count = 1; //マルチサンプリングしない

	//一時的なスワップチェインの作成
	//スワップチェインのアップグレードが必要になる
	Microsoft::WRL::ComPtr<IDXGISwapChain1> tempSwapChain{};
	{
		const auto hr = Device::instance().dxgi().factory()->CreateSwapChainForHwnd(
			commandQueue.get(),
			Window::instance().handle(),
			&swapChainDesc_,
			nullptr,
			nullptr,
			tempSwapChain.GetAddressOf());
		if (FAILED(hr)) {
			assert(false && "スワップチェーンの作成に失敗");
			return false;
		}
	}
	//スワップチェインをバージョンをアップグレード
	{
		//一時的なスワップチェインをIDXGISwapChain3に変換
		const auto hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapChain_));
		if (FAILED(hr)) {
			assert(false && "スワップチェインのアップグレートに失敗");
			return false;
		}
	}
	return true;
}

//スワップチェインを取得する
//スワップチェインのポインタを返す
IDXGISwapChain3* Swap_Chain::get()const noexcept {
	if (!swapChain_) {
		assert(false && "スワップチェインが未作成です");
	}
	return swapChain_.Get();
}

//スワップチェインの設定を取得
//スワップチェインの設定を返す
const DXGI_SWAP_CHAIN_DESC1& Swap_Chain::getDesc()const noexcept {
	if (!swapChain_) {
		assert(false && "スワップチェインが未作成です");
	}
	return swapChainDesc_;
}
