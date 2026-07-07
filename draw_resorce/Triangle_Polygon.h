#pragma once
#include"Shape.h"

class Triangle_Polygon final: public Shape
{
public:
	//コンストラクタ
	Triangle_Polygon() = default;
	//デストラクタ
	~Triangle_Polygon() = default;
private:
	//頂点バッファの生成
	virtual bool createVertexBuffer()noexcept override;

	//インデックスバッファの作成
	virtual bool createIndexBuffer()noexcept override;
};

