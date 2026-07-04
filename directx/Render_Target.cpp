#include "Render_Target.h"
#include<cassert>

//デストラクタ
Render_Target::~Render_Target() {
	renderTargets_.clear();
}

//バックバッファを生成
bool Render_Target::createBackBuffer(const Swap_Chain& swapChain)noexcept {
	//スワップチェインの設定を取得
	const auto& desc = swapChain.getDesc();

	//レンダーターゲットリソースのサイズを設定
	renderTargets_.resize(desc.BufferCount);

	//バックバッファの生成
	for (uint8_t i = 0; i < desc.BufferCount;i++) {
		const auto hr = swapChain.get()->GetBuffer(i, IID_PPV_ARGS(&renderTargets_[i]));
		if (FAILED(hr)) {
			assert(false && "バックバッファの取得に失敗しました");
			return false;
		}
	}
	//ビュー作成
	if (!createRenderTargetView()) {
		return false;
	}
	//ビュー作成
	if (!createShaderResourceView()) {
		return false;
	}
	return true;
}

//レンダーターゲットを作成
bool Render_Target::createrenderTarget(UINT w, UINT h, DXGI_FORMAT fromat, UINT num) noexcept
{
	//desc作成
	D3D12_RESOURCE_DESC desc{};
	desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Width = w;
	desc.Height = h;
	desc.DepthOrArraySize = 1;
	desc.MipLevels = 1;
	desc.Format = fromat;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//レンダーターゲット用ヒーププロパティ
	D3D12_HEAP_PROPERTIES heapProps{};
	heapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
	heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProps.CreationNodeMask = 1;
	heapProps.VisibleNodeMask = 1;

	//レンダーターゲット用クリア値
	D3D12_CLEAR_VALUE clearValue{};
	clearValue.Color[0] = 0.0f;
	clearValue.Color[1] = 0.0f;
	clearValue.Color[2] = 0.0f;
	clearValue.Color[3] = 1.0f;

	for (UINT i = 0;i < num;i++) {
		//レンダーターゲットリソースの生成
		Microsoft::WRL::ComPtr<ID3D12Resource> rendertarget{};

		const auto hr = Device::instance().get()->CreateCommittedResource(
			&heapProps,
			D3D12_HEAP_FLAG_NONE,
			&desc,
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			&clearValue,
			IID_PPV_ARGS(&rendertarget)
		);
		if (FAILED(hr)) {
			assert(false && "レンダーターゲットの作成に失敗しました");
			return false;
		}

		//レンダーターゲットリソースを配列に追加
		renderTargets_.emplace_back(std::move(rendertarget));
	}

	//ビュー作成
	if (!createRenderTargetView()) {
		return false;
	}

	//ビュー作成
	if (!createShaderResourceView()) {
		return false;
	}
	return true;
}

//ビュー(ディスクリプタハンドル)を取得
D3D12_CPU_DESCRIPTOR_HANDLE Render_Target::getCpuDescriptorHandle(UINT index)const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index] || rtvDescriptorIndex_.size() != renderTargets_.size()) {
		assert(false && "不正なレンダーターゲットです");
	}

	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//ディスクリプタヒープの取得
	auto heap = DescriptorHeapContainer::instance().get(heapType);

	//ディスクリプタヒープのハンドルを取得
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();

	//ディスクリプタヒープのハンドルを取得
	const auto rtvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

	//インデックスに応じてハンドルを移動
	handle.ptr += rtvDescriptorIndex_[index] * rtvDescriptorSize;
	return handle;
}

//GPU用のディスクリプタハンドルの取得
D3D12_GPU_DESCRIPTOR_HANDLE Render_Target::getGpuDescriptorHandle(UINT index)const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index] || srvDescriptorIndex_.size() != renderTargets_.size()) {
		assert(false && "不正なレンダーターゲット");
	}

	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	//ディスクリプタヒープの取得
	auto heap = DescriptorHeapContainer::instance().get(heapType);
	//GPU用ディスクリプタハンドルを取得
	auto handle_ = heap->GetGPUDescriptorHandleForHeapStart();
	//ディスクリプタのサイズ取得
	const auto srvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

	//インデックスに応じてハンドル移動
	handle_.ptr += srvDescriptorIndex_[index] * srvDescriptorSize;
	return handle_;
}

//レンダーターゲットを取得
ID3D12Resource* Render_Target::get(UINT index) const noexcept {
	if (index >= renderTargets_.size() || !renderTargets_[index]) {
		assert(false && "不正なレンダーターゲットです");
		return nullptr;
	}
	return renderTargets_[index].Get();
}

//テクスチャサイズを取得
std::pair<UINT, UINT> Render_Target::size()const noexcept {
	if (renderTargets_.empty()) {
		assert(false && "不正なレンダーターゲットです");
		return {};
	}

	//サイズは同じなので最初の要素から取得
	const auto desc = renderTargets_[0]->GetDesc();
	return { static_cast<UINT>(desc.Width),static_cast<UINT>(desc.Height) };
}

//ビュー(ディスクリプタハンドル)を作成する
bool Render_Target::createRenderTargetView()noexcept {
	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

	//レンダーターゲットビューの作成
	for (auto i = 0;i < renderTargets_.size();i++) {
		//ディスクリプタの確保
		const auto descriptorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType);
		if (!descriptorIndex.has_value()) {
			assert(false && "レンダーターゲットのディスクリプタ確保に失敗");
			return false;
		}
		auto index = descriptorIndex.value();

		//ディスクリプタヒープの取得
		auto heap = DescriptorHeapContainer::instance().get(heapType);
	    //ディスクリプタのサイズ取得
		const auto cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);

		//ディスクリプタヒープのハンドル取得
		auto cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += index * cbvDescriptorSize;

		//レンダーターゲットビューを作成してディスクリプタヒープのハンドルと関連付ける
		Device::instance().get()->CreateRenderTargetView(renderTargets_[i].Get(), nullptr, cpuHandle);

		rtvDescriptorIndex_.emplace_back(index);
	}
	return true;
}

//シェーダリソースビューを作成
bool Render_Target::createShaderResourceView()noexcept {
	constexpr auto heapType = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	//シェーダーリソースビューの作成
	for (auto i = 0;i < renderTargets_.size();i++) {
		//ディスクリプタの確保
		const auto descriptorIndex = DescriptorHeapContainer::instance().allocateDescriptor(heapType);
		if (!descriptorIndex.has_value()) {
			assert(false && "シェーダーリソースビューのディスクリプタ確保に失敗しました");
			return false;
		}
		auto index = descriptorIndex.value();

		//シェーダーリソースビューの設定
		D3D12_SHADER_RESOURCE_VIEW_DESC desc{};
		desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		desc.Format = renderTargets_[i]->GetDesc().Format;
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Texture2D.MipLevels = renderTargets_[i]->GetDesc().MipLevels;

		//ディスクリプタヒープの取得
		auto heap = DescriptorHeapContainer::instance().get(heapType);
		//ディスクリプタのサイズ取得s
		const auto cbvDescriptorSize = Device::instance().get()->GetDescriptorHandleIncrementSize(heapType);
		//ディスクリプタヒープのハンドルを取得
		auto cpuHandle = heap->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += index * cbvDescriptorSize;
		//シェーダーリソースビューを作成してディスクリプタヒープのハンドルと関連付ける
		Device::instance().get()->CreateShaderResourceView(renderTargets_[i].Get(), &desc, cpuHandle);

		srvDescriptorIndex_.emplace_back(index);
	}

	return true;
}