#pragma once
#include<Windows.h>
#include <string>

class Window final
{
public:
	//コンストラクタ
	Window() = default;
	//デストラクタ
	~Window() = default;

	//ウィンドウの生成
	HRESULT create(HINSTANCE instance, int width, int height, std::string_view name)noexcept;

	//メッセージループ
	bool messageLoop() const noexcept;

private:
	HWND handle_{}; //ウィンドウハンドル
};