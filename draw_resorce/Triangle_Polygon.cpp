#include "Triangle_Polygon.h"
#include<cassert>

//頂点バッファの生成
bool Triangle_Polygon::createVertexBuffer()noexcept {
	//頂点データ
	ModelVertex::Vertex triangleVerteices[] = {
		{{ 0.0f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 上頂点
		{{ 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}, // 右下頂点
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}}  // 左下頂点
	};
	//頂点データのサイズ
	const auto vertexBufferSize = sizeof(triangleVerteices);

	//ヒープの設定を指定
	//CPUからアクセス可能なメモリを利用する為の設定
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

	//頂点バッファにデータの転送
	//CPUからアクセス可能なアドレスを取得
	ModelVertex::Vertex* data{};

	//バッファをマップ(CPUからアクセス可能にする)
	//vertexBuffer_を直接利用するのではなく,dataを介して更新するイメージ
	res = vertexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "頂点バッファのマップに失敗");
		return false;
	}

	//頂点データをコピー
	memcpy_s(data, vertexBufferSize, triangleVerteices, vertexBufferSize);

	//コピーが終わったのでマップ解除(CPUからのアクセス不可に)
	//GPUの利用するメモリ領域(VRAM)にコピー済みなので,verticesは不要
	vertexBuffer_->Unmap(0, nullptr);

	//頂点バッファビューの設定
	vertexBufferView_.BufferLocation = vertexBuffer_->GetGPUVirtualAddress(); //頂点バッファのアドレス
	vertexBufferView_.SizeInBytes = vertexBufferSize;                         //頂点バッファのサイズ
	vertexBufferView_.StrideInBytes = sizeof(ModelVertex::Vertex);            //1頂点当たりのサイズ

	//トポロジーの設定
	topology_ = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; //三角形を描画するのでTRIANGLELIST
	//POINTLIST     :1頂点に1点※点同士は独立(パーティクル)
	//LINELIST      :2頂点で1本線※線同士は独立(当たり判定表示)
	//LINESTRIP     :隣の頂点と自動で線にする(playerの移動軌跡)
	//TRIANGLELIST  :3頂点で1三角形(3Dモデル,ゲームのほぼすべてのメッシュ)
	//TRIANGLESTRIP :最初の3頂点で三角形を作り,その後頂点を増やすごとに三角形を作る
	return true;
}

//インデックスバッファーの生成
bool Triangle_Polygon::createIndexBuffer()noexcept {
	uint16_t triangleIndices[] = {
		0,1,2 //頂点インデックス
	};

	//インデックスデータのサイズ
	const auto indexBufferSize = sizeof(triangleIndices);

	//ヒープの設定を指定
	D3D12_HEAP_PROPERTIES heapProperty{};
	heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperty.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperty.CreationNodeMask = 1;
	heapProperty.VisibleNodeMask = 1;

	//リソースの設定を行う
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

	//インデックスバッファの生成
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

	//インデックスバッファにデータを転送
	uint16_t* data{};
	res = indexBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&data));
	if (FAILED(res)) {
		assert(false && "インデックスバッファのマップに失敗");
		return false;
	}

	memcpy_s(data, indexBufferSize, triangleIndices, indexBufferSize);
	//GPUの利用するメモリ領域(VRAM)にコピー済みなので,indicesは不要
	indexBuffer_->Unmap(0, nullptr);

	//インデックスバッファビューの作成
	indexBufferView_.BufferLocation = indexBuffer_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = indexBufferSize;
	indexBufferView_.Format = DXGI_FORMAT_R16_UINT; //indicesの型が符号なし整数16bitナノでR16_UINT

	//インデックス数の設定
	indexCount_ = _countof(triangleIndices);

	return true;
}