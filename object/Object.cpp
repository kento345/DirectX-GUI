#include "Object.h"

namespace game {

	//描画コマンド設定
	void Object::setDrawCommand(const Command_List& commandList, UINT slot) noexcept
	{
		//コンスタントバッファの設定
		commandList.get()->SetGraphicsRootDescriptorTable(
			slot,
			constantBuffer_.getGpuDescriptorHandle());
	}

}