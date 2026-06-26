#pragma once

#include<Windows.h>
#include<cstdint>
#include<array>

class Input final
{
public:
	//インスタンス取得
	static Input& instance()noexcept {
		static Input instance;
		return instance;
	}

	//キー取得
	//入力中
	bool getKey(uint16_t sKey)const noexcept;

	//キー取得
	//入力された時
	bool	 getTrigger(uint16_t sKey)const noexcept;
	//キー情報更新
	void updateKeyState(void* pState)noexcept;
	//キー情報の更新
	void updatePrevKeyState()noexcept;

private:

	//コンストラクタ
	Input() = default;
	//デストラクタ
	~Input() = default;

	//コピーとムーブの禁止
	Input(const Input& r) = delete;
	Input& operator=(const Input& r) = delete;
	Input(Input&& r) = delete;
	Input& operator=(Input&& r) = delete;

private:
	std::array<byte, 256> keyState_{};
	std::array<byte, 256> prevKeyState_{};
};

