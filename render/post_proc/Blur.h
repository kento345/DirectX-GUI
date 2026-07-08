#pragma once

#include "../../directx/constant_buffer.h"

#if defined(_DEBUG)
namespace dev::editor {
	class Blur_Editor;
}
#endif

namespace graphics {
	//ブラーの設定
	struct BlurSettings {
		float blurStrength;  /// ブラーの強さ（0.0 でぼかし、大きいほど強いぼかし）
		float thereshold;    /// ブラーの閾値（ImGui のスライダーが書き換える）
		float padding[2];    /// float4 アライメント用パディング
	};

	//ブラー描画制御クラス
	class Blur final
	{
#if defined(_DEBUG)
		friend class dev::editor::Blur_Editor; //ブラーエディタークラスをフレンドにする
#endif

	public:
		//コンストラクタ
		Blur() = default;
		//デストラクタ
		~Blur() = default;

		//ブラー設定を作成
		bool create()noexcept;

		//ブラー設定をGPUのコンスタントバッファに書き込む
		void updateSettings()noexcept;

		//コンスタントバッファを取得
		const Constant_Buffer& constantoBuffer()const noexcept;

	private:
		Constant_Buffer blurSettingsCB_{}; //ブラー設定コンスタントバッファ(ImGuiから調整)
		float blurStrength_ = 1.0f;        //ブラーの強さ(ImGuiのスライダが書き換える)
		float thereshold_ = 0.2f;          //ブラーの閾値(ImGuiのスライダが書き換え)
	};
}



