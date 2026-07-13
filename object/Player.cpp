#include "Player.h"
#include"../window/Input.h"
#include"../draw_resorce/Shape_Container.h"
#include"../draw_resorce/Quad_Polygon.h"

#include"../object/Game_Object_Manager.h"
#include"../object/Bullet.h"

namespace game {
	static RegisterCreator<Player> registerCreatoe("プレイヤーベース");

	//プレイヤー初期化
	void Player::initialize()noexcept {
		Game_Object::initialize();

		auto quadId = Shape_Container::instance().create<Quad_Polygon>();
		set({ -.2f,0.0f,0.1f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { .0f,1,.0f,1 }, quadId);
	}

	//プレイヤー更新
	void Player::update()noexcept {
		Game_Object::update();

		constexpr float moveSpeed = 0.05f;
		//入力処理
		DirectX::XMFLOAT3 pos{};
		if (Input::instance().getKey('W')) {
			pos.z += moveSpeed;
		}if (Input::instance().getKey('S')) {
			pos.z -= moveSpeed;
		}if (Input::instance().getKey('A')) {
			pos.x -= moveSpeed;
		}if (Input::instance().getKey('D')) {
			pos.x += moveSpeed;
		}

		//平行移動
		DirectX::XMVECTOR temp = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
		world_.r[3] = DirectX::XMVectorAdd(world_.r[3], temp);

		if (Input::instance().getTrigger('B')) {
			//弾の生成
			auto bullet = Game_Object_Manager::instance().create(id::get<Bullet>());
			auto obj = Game_Object_Manager::instance().gameObject(bullet);
			obj.value()->setParent(handle());
		}
	}
}