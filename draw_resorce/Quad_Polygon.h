#pragma once
//四角形ポリゴン
#include"Shape.h"

class Quad_Polygon final : public Shape
{
public:
	//コンストラクタ
	Quad_Polygon() = default;
	//デストラクタ
	~Quad_Polygon() = default;

private:
	//頂点バッファの生成
	virtual bool createVertexBuffer()noexcept override;

	//インデックスバッファの生成
	bool createIndexBuffer()noexcept override;
};

