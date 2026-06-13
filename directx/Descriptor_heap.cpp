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

	}





private:Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{};
};
