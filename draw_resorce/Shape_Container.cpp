#include "Shape_Container.h"

//ポリゴン描画
void Shape_Container::draw(const Command_List& commandList, UINT64 id)noexcept {
	auto it = shapes_.find(id);
	if (it == shapes_.end()) {
		//指定された形状が存在しない場合何もしない
		return;
	}

	it->second->draw(commandList);
}