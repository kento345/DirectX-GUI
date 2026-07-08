#pragma once

#if defined(_DEBUG)
#include"../../render/post_proc/Blur.h"

namespace dev::editor {
	//ブラーエディタークラス
    class Blur_Editor final {
    public:
        //コンストラクタ
        Blur_Editor() = default;

        //デストラクタ
        ~Blur_Editor() = default;

    public:
        //ブラーの ImGui デバッグウィンドウを構築する
        void draw(graphics::Blur& blur) noexcept;
    };
}
#endif