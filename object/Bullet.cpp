#include "Bullet.h"
#include"Enemy.h"

#include"../draw_resorce/Shape_Container.h"
#include"../draw_resorce/Quad_Polygon.h"

#include"../object/Game_Object_Manager.h"

namespace game {

	static RegisterCreator<Bullet> registerCreator("弾ベース");

	//オブジェクトの初期化
	void Bullet::initialize()noexcept {
		Game_Object::initialize();

		auto quadId = Shape_Container::instance().create<Quad_Polygon>();

		DirectX::XMFLOAT3 parentPos{};
		if (auto parent = Game_Object_Manager::instance().gameObject(parent_)) {
			DirectX::XMStoreFloat3(&parentPos, parent.value()->world().r[3]);
		}
		set(parentPos, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }, { 0.0f,0.0f,1.0f,0.3f }, quadId);
	}

	//オブジェクトの更新
	void Bullet::update()noexcept {
		Game_Object::update();

		constexpr float moveSpeed = 0.3f;
		//入力処理
		DirectX::XMFLOAT3 pos{};
		pos.z += moveSpeed;
		//ワールド行列の更新
		world_ = DirectX::XMMatrixMultiply(world_, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));

		Game_Object_Manager::instance().registerHit(handle());
	}

	//ヒットしたときの処理
	void Bullet::onHit()noexcept {
		Game_Object_Manager::instance().registerDelete(handle());
	}

	//ヒットする対象
	UINT64 Bullet::hitTargetTypeId()noexcept {
		return id::get<Enemy>();
	}
}