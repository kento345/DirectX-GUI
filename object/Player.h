#pragma once

#include"Game_Object.h"

namespace game {


	class Player : public Game_Object
	{
	public:
		//コンストラクタ
		Player() = default;

		//デストラクタ
		virtual ~Player() = default;

	public:
		//オブジェクト初期化
		virtual void initialize()noexcept override;
		//オブジェクト更新
		virtual void update()noexcept override;
	};
}