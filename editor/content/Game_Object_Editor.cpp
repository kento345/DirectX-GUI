#if defined(_DEBUG)

#include "Game_Object_Editor.h"
#include"../../editor/ImGuiManager.h"
#include"../../object/Game_Object_Manager.h"

namespace dev::editor {
	//オブジェクトのImGuiデバッグウィンドウを構築
	void Game_Object_Editor::draw()noexcept {
		//ウィンドウ:ゲームオブジェクト管理設定
		ImGui::Begin("ゲームオブジェクト管理設定");
		{
			static int selectedIndex{};
			//const auto& types = game::
		}
	}
}


#endif