#include "Game_Object_Manager.h"
#include<queue>
#include<algorithm>
#include<unordered_map>
#include<any>
#include<string_view>
#include<string>

namespace game {

	//ゲームオブジェクトファクトリー
	class GameObjectFactory final
	{
	public://インスタンス
		static GameObjectFactory& instance()noexcept {
			static GameObjectFactory instance;
			return instance;
		}

		//ファクトリー関数の登録
		void registerCreator(UINT64 typeId, std::string_view name, std::function<UINT64()>&& creator)noexcept {
			if (creators_.find(typeId) != creators_.end()) {
				return;
			}
			creators_.emplace(typeId, std::move(creator));
			typeIds_.emplace_back(typeId);
			typeNames_.emplace_back(name);
		}

		//オブジェクト生成
		UINT64 create(UINT64 typeId)noexcept {
			auto it = creators_.find(typeId);
			if (it == creators_.end()) {
				return{};
			}
			return it->second();
		}

	private:

		//コンストラクタ
		GameObjectFactory() = default;

		//コピーとムーブの禁止
		GameObjectFactory(const GameObjectFactory& r) = delete;
		GameObjectFactory& operator=(const GameObjectFactory& r) = delete;
		GameObjectFactory(GameObjectFactory&& r) = delete;
		GameObjectFactory& operator=(GameObjectFactory&& r) = delete;

	public:
		std::unordered_map<UINT64, std::function<UINT64()>> creators_{};//オブジェクト生成関数コンテナ
		std::vector<UINT64> typeIds_{};//登録されているオブジェクトタイプIDのリスト
		std::vector<std::string> typeNames_{};//登録されているオブジェクトタイプ名リスト
	};

	//ゲームオブジェクトコンテナ
	class GameObjectContainer final {
	public:
		//オブジェクトクリア
		void clear()noexcept {
			objects_.clear();
			created_.clear();
			hit_.clear();
			waitDelte_.clear();

			created_.shrink_to_fit();
			hit_.shrink_to_fit();
			waitDelte_.shrink_to_fit();
		}

		//オブジェクト登録
		void registerGameObject(std::unique_ptr<Game_Object>&& o)noexcept {
			created_.emplace_back(std::move(o));
		}

	public:
		std::vector<std::unique_ptr<Game_Object>> created_{}; //生成したオブジェクト
		std::vector<UINT64> hit_{};//衝突判定オブジェクト

		std::vector<std::unique_ptr<Game_Object>> objects_{};//オブジェクト本体コンテナ
		std::unordered_map<UINT64, UINT> objectIndex_{};//オブジェクトインデックスコンテナ
		std::queue<UINT> freeIndex_{};//空きインデックス

		std::vector<std::pair<std::unique_ptr<Game_Object>, UINT>> waitDelte_{};//削除オブジェクト
		std::vector<UINT64> deleteHandle_{};//削除オブジェクトハンドル
	};

	GameObjectContainer container_{}; //ゲームオブジェクトコンテナ

	//管理オブジェクトの更新
	void Game_Object_Manager::update()noexcept {
		//生成オブジェクトの初期化と登録
		if (!container_.created_.empty()) {
			for (auto& p : container_.created_) {
				p->initialize();

				//オブジェクト本体コンテナに登録
				//空きインデックスがあればそこに登録,なければ末尾に追加
				auto index = container_.objects_.size();
				if (!container_.freeIndex_.empty()) {
					index = container_.freeIndex_.front();
					container_.freeIndex_.pop();
				}
				else
				{
					container_.objects_.emplace_back();
				}
				const auto handle = p->handle();
				container_.objects_[index] = std::move(p);
				container_.objectIndex_.emplace(handle, static_cast<UINT>(index));
			}
			container_.created_.clear();
		}

		//オブジェクト更新処理
		for (auto& obj : container_.objects_) {
			if (!obj) {
				continue;
			}
			obj->update();
		}
	}

	//管理オブジェクトの後更新
	void Game_Object_Manager::postUpdate()noexcept {
		//衝突判定処理
		if (!container_.hit_.empty()) {
			//登録されている衝突判定オブジェクトハンドルを順番に処理
			for (auto handle : container_.hit_) {
				auto obj = gameObject(handle);
				if (!obj) {
					continue;
				}
				auto myPos = obj.value()->world().r[3];

				//他のオブジェクトと衝突判定
				for (auto& target : container_.objects_) {
					//自分自身or存在しないオブジェクトor衝突対象外のオブジェクトはスキップ
					if (!target || target->handle() == handle || target->typeId() != obj.value()->hitTargetTypeId()) {
						continue;
					}

					//衝突判定
					auto hitRadius = obj.value()->radius() + target->radius();
					auto targetPos = target->world().r[3];
					auto distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSubtract(myPos, targetPos)));
					if (distance < hitRadius) {
						obj.value()->onHit();
						target->onHit();
					}
				}
			}
			container_.hit_.clear();
		}

		registerWaitDelete();

		//オブジェクト削除処理
		if (!container_.waitDelte_.empty()) {
			for (auto d = container_.waitDelte_.begin(); d != container_.waitDelte_.end();) {
				if ((d->second--) > 0) {
					++d;
					continue;
				}
				d = container_.waitDelte_.erase(d);
			}
		}
	}

	//管理オブジェクトの描画
	void Game_Object_Manager::draw(const Command_List& commandList)noexcept {
		constexpr UINT objectShaderSlot_ = 1;//オブジェクト用シェーダースロット

		//
		std::vector<Game_Object*> drawObjects{};
		for (auto& obj : container_.objects_) {
			if (!obj) {
				continue;
			}
			drawObjects.emplace_back(obj.get());
		}
		std::sort(drawObjects.begin(), drawObjects.end(),
			[](const Game_Object* a, const Game_Object* b) {
				return a->handle() < b->handle();
			});
		for (auto& obj : drawObjects) {
			obj->updateDrawBuffer();
			obj->setDrawCommand(commandList, objectShaderSlot_);
		}
	}

	//管理オブジェクトのクリア
	void Game_Object_Manager::clear()noexcept {
		container_.clear();
	}

	//アクティブな全ゲームオブジェクトに対してコールバックを呼ぶ
	void Game_Object_Manager::forEach(std::function<void(Game_Object&)> callback)noexcept {
		for (auto& obj : container_.objects_) {
			//nullスロット(削除済みまたは空き)はスキップ
			if (!obj) {
				continue;
			}
			callback(*obj);
		}
	}

	//オブジェクト取得
	std::optional<Game_Object*> Game_Object_Manager::gameObject(UINT64 handle)noexcept {
		//オブジェクトインデックスコンテナ～検索
		auto it = container_.objectIndex_.find(handle);

		//見つからなかった場合,生成コンテナを検索
		if (it == container_.objectIndex_.end()) {
			//生成コンテナを検索
			auto find = std::find_if(container_.created_.begin(), container_.created_.end(), [handle](const auto& o) {return o->handle() == handle;});

			//見つからなかった場合
			if (find == container_.created_.end()) {
				return std::nullopt;
			}
			return find->get();
		}

		//オブジェクト本体コンテナ～オブジェクトを返す
		return container_.objects_[it->second].get();
	}

	//衝突判定オブジェクトを登録
	void Game_Object_Manager::refisterHit(UINT64 handle)noexcept {
		container_.hit_.emplace_back(handle);
	}

	//オブジェクト削除登録
	void Game_Object_Manager::registerDelete(UINT64 handle)noexcept {
		//すでに登録されている場合
		if (std::find(container_.deleteHandle_.begin(), container_.deleteHandle_.end(), handle) != container_.deleteHandle_.end()) {
			return;
		}

		//削除オブジェクトハンドルを登録
		container_.deleteHandle_.emplace_back(handle);
	}

	//オブジェクトを登録する
	void Game_Object_Manager::registerGameObject(std::unique_ptr<Game_Object>&& object)noexcept {
		container_.registerGameObject(std::move(object));
	}

	//登録済みの削除オブジェクトを削除待ちリストへ
	void Game_Object_Manager::registerWaitDelete()noexcept {
		//削除登録なければreturn
		if (container_.deleteHandle_.empty()) {
			return;
		}
		//登録されている削除オブジェクトハンドルを順番に処理
		for (auto handle : container_.deleteHandle_) {
			auto it = container_.objectIndex_.find(handle);

			//オブジェクトが見つからなければスキップ
			if (it == container_.objectIndex_.end()) {
				continue;
			}

			//削除登録
			//描画中に削除されないように少し時間を置く
			//本体コンテナはnullに登録
			container_.waitDelte_.emplace_back(std::move(container_.objects_[it->second]), 10);
			//空きインデックスに登録
			container_.freeIndex_.push(it->second);

			//通常インデックスコンテナ～削除
			container_.objectIndex_.erase(it);
		}

		container_.deleteHandle_.clear();
	}

	//デストラクタ
	Game_Object_Manager::~Game_Object_Manager() {
		clear();
	}

	//オブジェクト生成
	UINT64 Game_Object_Manager::create(UINT64 typeId)noexcept {
		return GameObjectFactory::instance().create(typeId);
	}

	//オブジェクトタイプIDのリストを取得
	const std::vector<UINT64>& Game_Object_Manager::typeIds()const noexcept {
		return GameObjectFactory::instance().typeIds_;
	}

	//オブジェクトタイプ名のリスト取得
	const std::vector<std::string>& Game_Object_Manager::typeNames()const noexcept {
		return GameObjectFactory::instance().typeNames_;
	}

	//ファクトリー関数の登録
	void Game_Object_Manager::registerFactory(UINT64 typeId, std::string_view name, std::function<UINT64() > && creator)noexcept {
		GameObjectFactory::instance().registerCreator(typeId, name, std::move(creator));
	}
}