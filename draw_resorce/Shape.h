#pragma once

#include"../directx/Device.h"
#include"../directx/Command_List.h"
#include"../directx/Pipline_State_Object.h"
#include<DirectXMath.h>

class Shape 
{
public:
	//コンストラクタ
	Shape() = default;

	//デスクリプタ
	virtual ~Shape() = default;

	//ポリゴンの生成
    //成功すればtrueを返す
	bool create()noexcept;

	//ポリゴンの描写
	void draw(const Command_List& commandList)noexcept;

protected:
	//頂点バッファの生成
	virtual bool createVertexBuffer()noexcept = 0;
	//インデックスバッファの生成
	virtual bool createIndexBuffer()noexcept = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer_{};  //頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer_{};   //インデックスバッファ
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};            //頂点バッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};              //インデックスバッファビュー
	D3D_PRIMITIVE_TOPOLOGY topology_{};                      //プリミティブトポロジー
	UINT indexCount_{};                                      //インデックス数
};

//頂点レイアウト関連をここまでまとめて定義しておく

//3Dモデル頂点構造体
struct ModelVertex
{
	struct Vertex {
		DirectX::XMFLOAT3 position; //頂点座標
		DirectX::XMFLOAT4 color;    //頂点カラー
	};

	//頂点レイアウト
	static VertexLayout Layout() noexcept {
		static D3D12_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION",0,   DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		};

		return { layout,_countof(layout) };
	}
};

//スクリーン頂点構造体
struct ScreenVertex
{
	struct Vertex {
		DirectX::XMFLOAT2 position; //頂点座標(x,y)
		DirectX::XMFLOAT2 uv;       //uv
	};

	//頂点レイアウト
	static VertexLayout Layout() noexcept {
		static D3D12_INPUT_ELEMENT_DESC layout[] = {
			{"POSITION",0,DXGI_FORMAT_R32G32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
			{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,8,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		};

		return { layout,_countof(layout) };
	}
};