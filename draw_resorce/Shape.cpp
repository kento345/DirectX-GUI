#include "Shape.h"
#include<cassert>

//ポリゴンの生成
//成功すればtrueを返す
bool Shape::create()noexcept {
	//頂点バッファの生成
	if (!createVertexBuffer()) {
		return false;
	}
	//インデックスバッファの生成
	if (!createIndexBuffer()) {
		return false;
	}

	return true;
}

//ポリゴン描画

void Shape::draw(const Command_List& commandList) noexcept
{
	//頂点バッファの設定
	commandList.get()->IASetVertexBuffers(0, 1, &vertexBufferView_);
	//インデックスバッファの設定
	commandList.get()->IASetIndexBuffer(&indexBufferView_);
	//プリミティブ形状の設定
	commandList.get()->IASetPrimitiveTopology(topology_);
	//描画コマンド
	commandList.get()->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}