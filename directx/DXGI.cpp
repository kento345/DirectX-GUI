#include "DXGI.h"
#include<cassert>

#pragma comment(lib,"dxgi.lib")

bool DXGI::setDisplayAdapter()noexcept
{
#if _DEBUG
	//ファクトリーの作成
	ID3D12Debug* debug;
	if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
	}
#endif
	//ファクトリーの作成
	{
		UINT createFactoryFlags = 0;
#if _DEBUG
		createFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
		const auto hr = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory_));
		if(FAILED(hr))
		{
			assert(false && "DXGIファクトリーの作成に失敗");
			return false;
		}
	}
	//ディスプレイアダプタの取得
	{
		//アダプタの列挙
		auto select = 0;
		Microsoft::WRL::ComPtr<IDXGIAdapter1> dxgiAdapter{};

		//適切なアダプタを選ぶ
		while (dxgiFactory_->EnumAdapters1(select, &dxgiAdapter) != DXGI_ERROR_NOT_FOUND) {
			DXGI_ADAPTER_DESC1 desc{};
			dxgiAdapter->GetDesc1(&desc);

			select++;

			//ソフトウェアアダプタは除外
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) {
				dxgiAdapter->Release();
				continue;
			}

			//DirectX3D12が動かない場合も除外
			if (FAILED(D3D12CreateDevice(dxgiAdapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) {
				dxgiAdapter->Release();
				continue;
			}

			//適切なアダプタが見つかった
			dxgiAdapter_ = dxgiAdapter;
			break;
		}
		if (!dxgiAdapter_) {
			assert(false && "アダプタの取得に失敗");
			return false;
		}
	}
	
	return true;
}

//ファクトリの取得
IDXGIFactory4* DXGI::factory()const noexcept
{
	if (!dxgiFactory_) {
		assert(false && "DXGIファクトリーが未作成です");
	}
	return dxgiFactory_.Get();
}

//ディスプレイアダプターの取得
IDXGIAdapter1* DXGI::displayAdapter()const noexcept
{
	if (!dxgiAdapter_) {
		assert(false && "ディスプレイアダプタが未作成です");
	}
	return dxgiAdapter_.Get();
}


