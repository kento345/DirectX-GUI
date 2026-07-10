#pragma once

#include"Game_Object.h"
#include<functional>
#include<string_view>
#include<string>

namespace game {


	class Game_Object_Manager final
	{
	public:
		//インスタンスの取得
		static Game_Object_Manager& instance()noexcept {
			static Game_Object_Manager instance;
			return instance;
		}

	public:
		//管理オブジェクトの更新
		void update()noexcept;

		//管理オブジェクトの後更新
		void	 postUpdate()noexcept;

		//管理オブジェクトの描画
		void draw(const Command_List& commandList)noexcept;

		//管理オブジェクトのクリア
		void clear()noexcept;

		//オブジェクト取得
		std::optional<Game_Object*> gameObject(UINT64 handle)noexcept;

		//衝突判定オブジェクトを登録
		void refisterHit(UINT64 handle)noexcept;

		//オブジェクト削除登録
		void registerDelete(UINT64 handle)noexcept;

		//アクティブ名全ゲームオブジェクトに対してコールバックを呼ぶ
		void forEach(std::function<void(Game_Object&)> callback)noexcept;

	private:

		//オブジェクトを登録する
		void registerGameObject(std::unique_ptr<Game_Object>&& object)noexcept;

		//登録済みの削除オブジェクトを削除待ちリストに反映する
		void registerWaitDelete()noexcept;

	private:
		//コンストラクタ
		Game_Object_Manager() = default;

		//デストラクタ
		~Game_Object_Manager() = default;


	};

}