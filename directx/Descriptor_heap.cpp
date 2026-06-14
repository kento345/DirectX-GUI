#include "Descriptor_heap.h"
#include"Device.h"
#include<cassert>
#include<wrl/client.h>

//ディスクリプタヒープ制御クラス

class DescriptorHeap final {
public:
	//コンストラクタ
	DescriptorHeap() = default;
	//デストラクタ
	~DescriptorHeap() = default;

public:
	//ディスクリプタヒープを生成する
	bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false)noexcept {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = type;
		heapDesc.NumDescriptors = numDescriptors;
		heapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		type_ = type; //ヒープのタイプを保存

		//ディスクリプタヒープの生成
		HRESULT hr = Device::instance().get()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap_));
		if (FAILED(hr)) {
			assert(false && "ディスクリプタヒープの生成に失敗しました");
			return false;
		}

		//全ディスクリプタをフリーリストに登録
		maxDescriptorCount_ = numDescriptors;
		for (UINT i = 0l; i < maxDescriptorCount_; i++) {
			freeIndices_.push_back(i);
		}

		return true;
	}

	//解放予約されているディスクリプタを開放する
	void applyPendingFree()noexcept {
		if (pendingFreeIndices_.empty()) {
			return;
		}

		for (auto i : pendingFreeIndices_) {
			freeIndices_.push_back(i);
		}
		pendingFreeIndices_.clear();
	}

	//ディスクリプタヒープを取得
	ID3D12DescriptorHeap* get()const noexcept {
		if (!heap_) {
			assert(false && "ディスクリプタヒープが未作成です");
		}
		return heap_.Get();
	}

	//ディスクリプタヒープのタイプを取得する
	D3D12_DESCRIPTOR_HEAP_TYPE getType()const noexcept {
		if (!heap_) {
			assert(false && "ディスクリプタヒープが未作成です");
		}
		return type_;
	}

	//ディスクリプタを確保する
	std::optional<UINT> allocatorDescriptor()noexcept {
		if (freeIndices_.empty()) {
			assert(false && "ディスクリプタの空きがありません");
			return std::nullopt;
		}
		const auto index = freeIndices_.back();
		freeIndices_.pop_back();
		return index;
	}

	//解放予定のディスクリプタを登録する
	void releaseDescriptor(UINT descriptorIndex)noexcept {
		//10 フレーム後に解放するよう登録
		pendingFreeIndices_.push_back(descriptorIndex);
	}

private:
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{}; //ディスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_TYPE type_{}; //ヒープのタイプ
	UINT maxDescriptorCount_{}; //最大ディスクリプタインデックスのリスト
	std::vector<UINT> freeIndices_{}; //空きディスクリプタインデックスのリスト
	std::vector<UINT> pendingFreeIndices_{};//解放待ちディスクリプタインデックスのリスト
};

//コンストラクタ

DescriptorHeapContainer::DescriptorHeapContainer() = default;

//デストラクタ
DescriptorHeapContainer::~DescriptorHeapContainer() {
	map_.clear();
}

//ディスクリプタヒープを生成する
bool DescriptorHeapContainer::create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible)noexcept {
	if (map_.find(type) != map_.end()) {
		//既に作成済み
		return false;
	}

	auto p = std::make_unique<DescriptorHeap>();
	if (p->create(type, numDescriptors, shaderVisible)) {
		map_.emplace(type, std::move(p));
	}

	return true;
}

//解放予約されているディスクリプタを解放する
void DescriptorHeapContainer::applyPendingFree()noexcept {
	for (auto& [key, p] : map_) {
		p->applyPendingFree();
	}
}

//ディスクリプタヒープを取得する
ID3D12DescriptorHeap* DescriptorHeapContainer::get(D3D12_DESCRIPTOR_HEAP_TYPE type)const noexcept {
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません");
		return nullptr;
	}

	//return it->second->get();
}

//ディスクリプタを確保する
std::optional<UINT> DescriptorHeapContainer::allocatorDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type)noexcept {
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません");
		return std::nullopt;
	}

	//return it->second->allocatorDescriptor();
}

//解放予定のディスクリプタを登録

void DescriptorHeapContainer::releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex)noexcept {
	const auto it = map_.find(type);
	if (it == map_.end()) {
		assert(false && "ディスクリプタヒープがありません");
	}

	//return it->second->releaseDescriptor(descriptorIndex);
}