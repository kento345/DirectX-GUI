#if defined(_DEBUG)

#include"Blur_Editor.h"
#include"../../editor/ImGuiManager.h"

namespace dev::editor {

    //ImGui デバッグウィンドウを構築する
    //毎フレーム newFrame() の直後に呼ぶ
    void Blur_Editor::draw(graphics::Blur& blur) noexcept
    {
        //ウィンドウ:ブラー設定
        ImGui::Begin("ブラー設定\0");
        {
            bool update{};

            // スライダーが変化したときだけ GPU バッファを更新する
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
            update |= ImGui::SliderFloat("ブラー強度", &blur.blurStrength_, 0.0f, 2.5f);
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.4f);
            update |= ImGui::SliderFloat("ブラー閾値（輝度）\0", &blur.thereshold_, 0.0f, 1.0f);

            if (update)
            {
                blur.updateSettings();
            }
        }
        ImGui::End();
    }
}

#endif