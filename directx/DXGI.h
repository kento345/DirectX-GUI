#pragma once

#include<d3d12.h>
#include <dxgi1_4.h>
#include<wrl/client.h>

class DXGI final
{
public:
	//コンストラクタ
	DXGI() = default;
	//デストラクタ	
	~DXGI() = default;

	//ディスプレイアダプタの設定
	//ディスプレイの情報が正しく取得出来たらtueをreturn
	bool setDisplayAdapter()noexcept;

	//ファクトリーの取得
	//ファクトリのポインタをreturn	
	IDXGIFactory4* factory()const noexcept;	

	//ディスプレイアダプターの取得
	//ディスプレイアダプタのポインタをreturn
	IDXGIAdapter1* displayAdapter()const noexcept;

private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory_;     //DXGIを作成するファクトリー
	Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter_;  //ディスプレイモード取得用アダプター
};

