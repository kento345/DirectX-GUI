#pragma once

#include"Object.h"
#include<DirectXMath.h>

namespace game {

	class Camera :public Object
	{
	public:
		//コンストラクタ
		Camera() = default;

		//デストラクタ
		virtual ~Camera() = default;

	public:
		//カメラ初期化
		virtual void initialize() noexcept override;
		//カメラ更新
		virtual void update()noexcept override;

	public:
		//描画バッファの作成
		virtual void createDrawBuffer()noexcept override;
		//描画バッファの更新
		virtual void updateDrawBuffer()noexcept override;

	public:
		//カメラのビュー行列取得
		DirectX::XMMATRIX XM_CALLCONV viewMatrix()const noexcept;
		//プロジェクション行列取得
		DirectX::XMMATRIX XM_CALLCONV projection()const noexcept;

	private:
		DirectX::XMMATRIX view_{};      //ビュー行列
		DirectX::XMMATRIX projection_{};//射影行列
	
		//カメラパレメータ				
		DirectX::XMFLOAT3 position_{};	//カメラの位置
		DirectX::XMFLOAT3 target_{};	//カメラの注視点
		DirectX::XMFLOAT3 up_{};		//カメラの上方向
	};
}