#include "Window.h"
#include"Input.h"

// ImGui の Win32 メッセージハンドラ（imgui_impl_win32.h 内で宣言済み）
//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace {
	//ウィンドウプロシージャ
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		/*if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
			return true;
		}*/

		switch (msg) {
		case WM_DESTROY:  // ウィンドウが閉じられたとき
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

//ウィンドウの生成
HRESULT Window::create(HINSTANCE instance, int width, int height, std::string_view name) noexcept {
	// ウィンドウの定義
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = instance;
	wc.lpszClassName = name.data();
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	// ウィンドウクラスの登録
	RegisterClass(&wc);

	// ウィンドウの作成
	handle_ = CreateWindow(wc.lpszClassName, wc.lpszClassName,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		nullptr, nullptr, instance, nullptr);
	if (!handle_) {
		return E_FAIL;
	}

	// ウインドウの表示
	ShowWindow(handle_, SW_SHOW);

	// ウィンドウを更新
	UpdateWindow(handle_);

	// ウィンドウのサイズを保存
	witdh_ = width;
	height_ = height;

	// 成功を返す
	return S_OK;
}

//メッセージループ
bool Window::messageLoop() const noexcept {
	MSG msg{};

	Input::instance().updatePrevKeyState();

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return false;  // WM_QUITメッセージが来たらループを抜ける
		}

		// メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		// キー情報の取得
		static byte keyState[256]{};
		if (GetKeyboardState(keyState)) {
			// キー情報取得に成功したら、Input クラスに情報を渡す
			Input::instance().updateKeyState(keyState);
		}
	}

	return true;
}

//ウィンドウサイズ取得
std::pair<int, int> Window::size() const noexcept {
	return { witdh_, height_ };
}