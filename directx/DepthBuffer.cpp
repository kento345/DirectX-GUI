#include "DepthBuffer.h"
#include "Descriptor_heap.h"
#include"../window/Window.h"
#include<cassert>

namespace {
	constexpr auto heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
}

//デストラクタ
DepthBuffer::~DepthBuffer() {
}

//デプスバッファ生成
bool  DepthBuffer::create()noexcept {
	//ウィンドウサイズを取得
	
}
