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
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	//heapDesc.Type = 



private:Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_{};
};
