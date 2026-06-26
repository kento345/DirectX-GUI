#pragma once
#include<Windows.h>
#include <string>

class Window final
{
public:
	//インスタンス
	static Window& instance() noexcept {
		static Window instance;
		return instance;
	}

	//ウィンドウの生成
	HRESULT create(HINSTANCE instance, int width, int height, std::string_view name) noexcept;

	//メッセージループ
	bool messageLoop() const noexcept;

	//ウィンドウハンドル取得
	HWND handle() const noexcept;

	//ウィンドウのサイズ取得
	std::pair<int, int> size() const noexcept;

private:
	//コンストラクタ
	Window() = default;
	//デストラクタ
	~Window() = default;
	//コピーとムーブの禁止
	Window(const Window& r) = delete;
	Window& operator=(const Window& r) = delete;
	Window(Window&& r) = delete;
	Window& operator=(Window&& r) = delete;

private:
	HWND handle_{}; //ウィンドウハンドル
	int  witdh_{};   /// ウィンドウの横幅
	int  height_{};  /// ウィンドウの縦幅
};