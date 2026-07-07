#include "Screen_Polygon.h"
#include<cassert>

//頂点バッファの生成
bool Screen_Polygon::createVertexBuffer()noexcept {
	ScreenVertex::Vertex vertices[] = {
		{{-1.0f,  1.0f}, {0.0f, 0.0f}}, // 左上頂点（画面左上）
		{{ 1.0f,  1.0f}, {1.0f, 0.0f}}, // 右上頂点（画面右上）
		{{-1.0f, -1.0f}, {0.0f, 1.0f}}, // 左下頂点（画面左下）
		{{ 1.0f, -1.0f}, {1.0f, 1.0f}}, // 右下頂点（画面右下）
	};

	//頂点データサイズ
	const auto vertexBufferSize = sizeof(vertices);

	//ヒープの設定を指定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask  = 1;

	//どんなリソースを作成するのか設定
	D3D12_RESOURCE_DESC resorceDesc{};
	resorceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resorceDesc.Alignment = 0;
	resorceDesc.Width = vertexBufferSize;
	resorceDesc.Height = 1;
	resorceDesc.DepthOrArraySize = 1;
	resorceDesc.MipLevels = 1;
	resorceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resorceDesc.SampleDesc.Count = 1;
	resorceDesc.SampleDesc.Quality = 0;
	resorceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resorceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//頂点バッファの生成
	auto res = Device::instance().get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resorceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBuffer_));
	if (FAILED(res)) {
		assert(false && "頂点バッファの作成に失敗");
		return false;
	}

	//頂点バッファにデータを転送する
	//CPUからアクセス可能なアドレス取得
	ScreenVertex::Vertex* data{};

	//バッファをマップ(CPUからのアクセス可)
	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗");
		return false;
	}

	//頂点データをコピー
	memcpy_s(data, vertexBufferSize, vertices, vertexBufferSize);

	vertexBuffer_->Unmap(0, nullptr);

	//頂点バッファビューの設定
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress(); //頂点バッファのアドレス
	vertexBufferView_.SizeInBytes = vertexBufferSize;                         //頂点バッファのサイズ
	vertexBufferView_.StrideInBytes = sizeof(ScreenVertex::Vertex);           //1頂点当たりのサイズ

	//トポロジーの設定
	topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;

	return true;
}

//インデックスバッファの生成
bool Screen_Polygon::createIndexBuffer()noexcept {
	uint16_t indices[] = {
		0,1,2,3//頂点インデックス(TRIANGLESTRIPなのでZオーダー(順番)
	};

	const auto indexBufferSize = sizeof(indices);


	//ヒープの設定を指定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//どんなリソースを作成するのか設定
	D3D12_RESOURCE_DESC resorceDesc{};
	resorceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resorceDesc.Alignment = 0;
	resorceDesc.Width = indexBufferSize;
	resorceDesc.Height = 1;
	resorceDesc.DepthOrArraySize = 1;
	resorceDesc.MipLevels = 1;
	resorceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resorceDesc.SampleDesc.Count = 1;
	resorceDesc.SampleDesc.Quality = 0;
	resorceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resorceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	//頂点バッファの生成
	auto res = Device::instance().get()->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&resorceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuffer_));
	if (FAILED(res)) {
		assert(false && "インデックスバッファの作成に失敗");
		return false;
	}

	//インデックスバッファにデータ転送
	uint16_t* data{};
	res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "インデックスバッファのマップに失敗");
		return false;
	}

	memcpy_s(data, indexBufferSize, indices, indexBufferSize);

	indexBuffer_->Unmap(0, nullptr);

	//インデックスバッファビュー作成
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT;

	//インデックス数の設定
	indexCount_ = _countof(indices);
	
	return true;
}