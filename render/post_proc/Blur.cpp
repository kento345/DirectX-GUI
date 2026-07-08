#include "Blur.h"
#include<cassert>

namespace graphics {

	//ブラー設定作成
	bool Blur::create()noexcept {
		//ブラー設定コンスタントバッファの作成
		if (!blurSettingsCB_.create(sizeof(BlurSettings))) {
			assert(false && "ブラー設定コンスタントバッファの作成に失敗しました");
			return false;
		}

		updateSettings();

		return true;
	}

	//ブラー設定をGPUのコンスタントバッファに書き込む
	void Blur::updateSettings()noexcept {
		BlurSettings settings{
			blurStrength_,thereshold_,{0.f,0.f}
		};
		BlurSettings* dst{};
		blurSettingsCB_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&dst));
		*dst = settings;
		blurSettingsCB_.constantBuffer()->Unmap(0, nullptr);
	}

	//コンスタントバッファを取得
	const Constant_Buffer& Blur::constantoBuffer()const noexcept {
		return blurSettingsCB_;
	}
}