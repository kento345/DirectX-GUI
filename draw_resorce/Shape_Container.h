#pragma once

#include"Shape.h"
#include<unordered_map>>
#include<memory>

class Shape_Container final
{
public:
	//インスタンス取得
	static Shape_Container& instance()noexcept {
		static Shape_Container instance;
		return instance;
	}

public:

	//形状の生成登録
	template<class T>
	UINT64 create() noexcept {
		const auto id = id::get<T>();
		if (shapes_.find(id) != shapes_.end()) {
			return id;
		}

		//形状の生成と登録
		auto p = std::make_unique<T>();
		if (!p->create()) {
			assert(false && "形状の生成に失敗しました");
			return 0;
		}
		shapes_.emplace(id, std::move(p));
		return id;
	}

	//ポリゴン描画
	void draw(const Command_List& commandList, UINT64 id)noexcept;

private:
	//コンストラクタ
	Shape_Container() = default;
	//デストラクタ
	~Shape_Container() = default;

	//コピーとムーブの禁止
	Shape_Container(const Shape_Container&) = delete;
	Shape_Container& operator = (const Shape_Container&) = delete;
	Shape_Container(Shape_Container&&) = delete;
	Shape_Container& operator = (Shape_Container&&) = delete;

protected:
	std::unordered_map<UINT64, std::unique_ptr<Shape>> shapes_; //形状コンテナ
};

