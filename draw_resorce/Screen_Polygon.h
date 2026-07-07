#pragma once
#include"Shape.h"

class Screen_Polygon final :public Shape
{
public:
	//コンストラクタ
	Screen_Polygon() = default;
	//デストラクタ
	~Screen_Polygon() = default;

private:
	//頂点バッファの生成
	virtual bool createVertexBuffer()noexcept override;

	//インデックスバッファの生成
	bool createIndexBuffer()noexcept override;
};

