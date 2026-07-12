#pragma once

#include"Game_Object.h"

namespace game {

	class Bullet : public Game_Object
	{
	public:
		//コンストラクタ
		Bullet() = default;

		//コンストラクタ継承
		using Game_Object::Game_Object;

		//デストラクタ
		virtual ~Bullet() = default;
	public:

		//オブジェクトの初期化
		virtual void initialize()noexcept override;

		//オブジェクト更新
		virtual void update()noexcept override;
	public:

		//ヒット時の処理
		virtual void onHit()noexcept override;

		//ヒットする対象
		virtual UINT64 hitTargetTypeId()noexcept override;
	};
}
