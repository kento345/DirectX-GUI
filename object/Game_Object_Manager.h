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
		void postUpdate()noexcept;

		//管理オブジェクトの描画
		void draw(const Command_List& commandList)noexcept;

		//管理オブジェクトのクリア
		void clear()noexcept;

		//オブジェクト取得
		std::optional<Game_Object*> gameObject(UINT64 handle)noexcept;

		//衝突判定オブジェクトを登録
		void registerHit(UINT64 handle)noexcept;

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
		~Game_Object_Manager();

		//コピーとムーブの禁止
		Game_Object_Manager(const Game_Object_Manager& r) = delete;
		Game_Object_Manager& operator=(const Game_Object_Manager& r) = delete;
		Game_Object_Manager(Game_Object_Manager&& r) = delete;
		Game_Object_Manager& operator=(Game_Object_Manager&& r) = delete;

	private:
		UINT64 counter_{};

	public:

		//オブジェクト生成
		UINT64 create(UINT64 typeId)noexcept;

		//オブジェクトタイプIDのリストを取得
		const std::vector<UINT64>& typeIds() const noexcept;

		//オブジェクトタイプ名のリストを取得
		const std::vector<std::string>& typeNames()const noexcept;

	public:
		//ファクトリー関数の登録
		void registerFactory(UINT64 typeId, std::string_view name, std::function<UINT64()>&& creator)noexcept;

		//オブジェクト生成
		template<typename T,typename... Args>
		UINT64 createObject(Args&&... args)noexcept {
			static_assert(std::is_base_of<Game_Object, T>::value, "GameObjectではないものを作ろうとしています\0");
			const auto handle = ++counter_;

			//パラメータパックを完全転送で展開してコンストラクタ引数にしてTを生成
			auto p = std::make_unique<T>(std::forward<decltype(args)>(args)...);

			//オブジェクトのタイプIDとハンドルを設定
			p->setTypeId(id::get<T>());
			p->setHandle(handle);

			//オブジェクトを登録
			registerGameObject(std::move(p));

			return handle;
		}
	};

	//ゲームオブジェクト登録クラス
	template<class T>
	class RegisterCreator final {
		friend class Game_Object_Manager;

	public:
		RegisterCreator() = delete;
		RegisterCreator(std::string_view name) {
			Game_Object_Manager::instance().registerFactory(
				id::get<T>(),
				name,
				//一旦引数なし
				[]() {return Game_Object_Manager::instance().createObject<T>();});
		}
	};
}