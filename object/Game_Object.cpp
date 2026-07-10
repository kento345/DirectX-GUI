#include "Game_Object.h"
#include"../draw_resorce/Shape_Container.h"
#include<cmath>

namespace {
	//ゲームオブジェクトコンスタントバッファ用データ構造体
	struct ConstantBufferData
	{
		DirectX::XMMATRIX world_{}; //ワールド行列
		DirectX::XMFLOAT4 color_{}; //カラー(RGBA)
	};
}

namespace game {
	//オブジェクトの初期化
	void Game_Object::initialize()noexcept {
		createDrawBuffer();
	}

	//描画コマンド設定
	void Game_Object::setDrawCommand(const Command_List& commandList, UINT slot)noexcept {
		Object::setDrawCommand(commandList, slot);
		Shape_Container::instance().draw(commandList, shapeId_);
	}

	//描画用バッファの作成
	void Game_Object::createDrawBuffer()noexcept {
		if (!constantBuffer_.create(sizeof(ConstantBufferData))) {
			assert(false && "GameObject　コンスタントバッファの作成に失敗しました");
		}
	}

	//描画用バッファの更新
	void Game_Object::updateDrawBuffer()noexcept {
		Object::updateConstantBuffer(ConstantBufferData{ DirectX::XMMatrixTranspose(world_),color_ });
	}

	//オブジェクトハンドルの設定
	void Game_Object::setHandle(UINT64 handle)noexcept {
		handle_ = handle;
	}

	//親オブジェクトハンドルの設定
	void Game_Object::setParent(UINT64 parent)noexcept {
		parent_ = parent;
	}

	//オブジェクトハンドルの取得
	UINT64 Game_Object::handle()const noexcept {
		return handle_;
	}

	//座標とカラーの設定
	void Game_Object::set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, UINT64 shapeId) noexcept
	{
		DirectX::XMMATRIX matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		world_ = matScale * rotation * translation;

		//カラーの設定
		color_ = color;
		//形状識別子の設定
		shapeId_ = shapeId;

		//衝突判定用の半径設定
		radius_ = (scale.x + scale.y + scale.z) / (3.0f * 2.0f);
	}

	//ワールド行列の取得
	DirectX::XMMATRIX Game_Object::world()const noexcept {
		return world_;
	}

	//カラーの取得
	DirectX::XMFLOAT4 Game_Object::color()const noexcept {
		return color_;
	}

	//ワールド行列の平行移動成分を設定
	void Game_Object::setPosition(DirectX::XMFLOAT3 pos)noexcept {
		//world_.r[3]が平行移動ベクトル(w成分は1.0fを維持する)
		world_.r[3] = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 1.0f);
	}

	//カラーを設定
	void Game_Object::setColor(DirectX::XMFLOAT4 color)noexcept {
		color_ = color;
	}
}