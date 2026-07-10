#pragma once

#include"../directx/Constant_Buffer.h"
#include"../directx/Command_List.h"
#include"../util.h"

namespace game {
	class Object
	{
	public:
		//コンストラクタ
		Object() = default;
		//デストラクタ
		virtual ~Object() = default;

		//オブジェクトの初期化
		virtual void initialize()noexcept {};

		//オブジェクトの更新
		virtual void update()noexcept {};

		//描画コマンド設定
		virtual void setDrawCommand(const Command_List& commandList, UINT slot)noexcept;

	private:
		//描画バッファの作成
		virtual void createDrawBuffer()noexcept = 0;

		//描画バッファの更新
		virtual void updateDrawBuffer()noexcept = 0;

	public:
		//オブジェクトタイプIDの取得
		UINT64 typeId()const noexcept {
			return typeId_;
		};
		
		//オブジェクトタイプIDの設定
		void setTypeId(UINT64 id)noexcept {
			typeId_ = id;
		};

	protected:
		//コンスタントバッファの更新
		template<class T>
		void updateConstantBuffer(const T& data)noexcept {
			std::byte* dst{};
			constantBuffer_.constantBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&dst));
			memcpy_s(dst, sizeof(T), &data, sizeof(T));
			constantBuffer_.constantBuffer()->Unmap(0, nullptr);
		}

	protected:
		Constant_Buffer constantBuffer_{};  //コンスタントバッファ
		UINT64 typeId_{};                   //オブジェクトタイプID
	};
}



