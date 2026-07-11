#pragma once

#if defined(_DEBUG)

#include"../../object/Game_Object.h"

namespace dev::editor {

	class Game_Object_Editor final
	{
	public:

		//コンストラクタ
		Game_Object_Editor() = default;

		//デストラクタ
		~Game_Object_Editor() = default;

	public:

		//オブジェクトのImGuiデバッグウィンドウを構築
		void draw()noexcept;
	};
}
#endif

