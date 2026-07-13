#pragma once
#include"Game_Object.h"

namespace game {

	class Enemy : public Game_Object
	{
	public:
		//コンストラクタ
		Enemy() = default;

		//デストラクタ
		virtual ~Enemy() = default;

	public:
		//オブジェクトの初期化
		virtual void initialize()noexcept override;

		//オブジェクト更新
		virtual void update()noexcept override;

	public:

		//ヒットした時の処理
		virtual void onHit()noexcept override;
	};
}