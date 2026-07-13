#include "Enemy.h"
#include"../draw_resorce/Shape_Container.h"
#include"../draw_resorce/Triangle_Polygon.h"

#include"../object/Game_Object_Manager.h"

namespace game {
	static RegisterCreator<Enemy> registerCreator("敵ベース");

	//敵の初期化
	void Enemy::initialize() noexcept {
		Game_Object::initialize();

		auto triId = Shape_Container::instance().create<Triangle_Polygon>();
		set({ 0.0f,0.0f,30.0f }, { 0.0f,0.0f,0.0f }, { 10.0f,10.0f,1.0f }, { 1.0f,1.0f,1.0f,1 }, triId);
	}

	//敵更新
	void Enemy::update()noexcept {
	}

	//ヒットした時の処理
	void Enemy::onHit()noexcept {
		color_.y *= 0.95f;
		color_.z += 0.95f;
	}
}
