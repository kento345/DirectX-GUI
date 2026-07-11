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
			static int selectedIndex{};   //選択されたオブジェクト
			const auto& types = game::Game_Object_Manager::instance().typeIds();//オブジェクトタイプのリスト取得
			const auto& names = game::Game_Object_Manager::instance().typeNames();//オブジェクトタイプの名前リストを取得

			//char*の配列に変換
			std::vector<const char*> itemPtrs;
			itemPtrs.reserve(names.size());
			for (const auto& s : names) {
				itemPtrs.push_back(s.c_str());
			}

			//コンボボックスでオブジェクトタイプを選択
			ImGui::Combo("##オブジェクトタイプ", &selectedIndex, itemPtrs.data(), static_cast<int>(itemPtrs.size()));
			ImGui::SameLine();
			if (ImGui::Button("生成")) {
				//選択されたオブジェクトタイプIDに対応するオブジェクトを生成する
				if (0 <= selectedIndex && selectedIndex < static_cast<int>(types.size())) {
					game::Game_Object_Manager::instance().create(types[selectedIndex]);
				}
			}

			game::Game_Object_Manager::instance().forEach([&](auto& object){
				//ウィンドウ: ゲームオブジェクト設定
				ImGui::PushID(static_cast<int>(object.handle()));
				const auto text = "オブジェクト:" + std::to_string(object.handle());
				ImGui::SeparatorText(text.c_str());
				//色を変更する項目を追加
				//ImGui::ColorEdit4("色", );
				//位置を変更する項目を追加
				//ImGui::DragFloat3("位置",&object.world_.r[3])
			}
		}
	}
}


#endif