#pragma once

#include<DirectXMath.h>
#include"Object.h"

#if defined(_DEBUG)
namespace dev::editor {
	class Game_Object_Editor;
}
#endif

namespace game {

	class Game_Object : public Object
	{
#if defined(_DEBUG)
		friend class dev::editor::Game_Object_Editor;
#endif
	public:
		//コンストラクタ
		Game_Object() = default;
		Game_Object(UINT64 parent): parent_(parent) {}
		//デストラクタ
		virtual ~Game_Object() = default;

	public:
		//オブジェクトの初期化
		virtual void initialize()noexcept override;

		//オブジェクトの更新
		virtual void update()noexcept override {};

		//描画コマンド設定
		virtual void setDrawCommand(const Command_List& commandList, UINT slot)noexcept override;

	public:
		//描画バッファの作成
		virtual void createDrawBuffer()noexcept override;

		//描画バッファの更新
		virtual void updateDrawBuffer()noexcept override;

	public:

		//ヒットした時の処理
		virtual void onHit()noexcept {};

		//ヒットする対象
		virtual UINT64 hitTargetTypeId()noexcept { return {}; }

	public:
		//オブジェクトハンドルの設定
		void setHandle(UINT64 handle)noexcept;

		//親オブジェクトハンドルの設定
		void setParent(UINT64 parent)noexcept;

		//オブジェクトハンドルの取得
		UINT64 handle()const noexcept;

		//座標とカラーの設定
		void set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, UINT64 shapeId)noexcept;

		//ワールド行列の取得
		DirectX::XMMATRIX world() const noexcept;

		//カラーの取得
		DirectX::XMFLOAT4 color()const noexcept;

		//半径取得
		float radius()noexcept { return radius_; };

		//ワールド行列の平行移動成分を設定
		void setPosition(DirectX::XMFLOAT3 pos)noexcept;

		//カラーの設定
		void setColor(DirectX::XMFLOAT4 color)noexcept;

	protected:
		DirectX::XMMATRIX world_ = DirectX::XMMatrixIdentity();//ワールド行列
		DirectX::XMFLOAT4 color_ = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);//カラー
		UINT64 shapeId_{};//形状識別
		UINT64 handle_{}; //ゲームオブジェクトハンドル
		UINT64 parent_{}; //親オブジェクトハンドル
		float radius_{};  //あたり判定用半径
	};
}

