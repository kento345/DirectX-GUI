//ウィンドウアプリケーション

#include"../window/Window.h"
#include"../window/Input.h"

#include

#include<cassert>

namespace {
	constexpr std::wstring_view APP_NAME = L"MyApp"; //アプリ名
}

class Application final {
public: 
	//コンストラクタ
	Application() = default;
	//デストラクタ
	~Application() = default;

	//アプリケーションの初期化
	HRESULT initialize(HINSTANCE instance)noexcept {
		//ウィンドウ生成
		return windowInstance_.create(instance, 1280, 720, APP_NAME);
	}
	
	//アプリケーションループ
	void loop()noexcept {
		while (windowInstance_.messageLoop()) {
			//メッセージループ内での処理をここに記述
		}
		//ループを抜けるとウィンドウを閉じる
	}

private:
	Window windowInstance_; //ウィンドウインスタンス
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	//アプリケーションクラスのインスタンスを生成
	Application app;

	if (app.initialize(hInstance)) {
		assert(false && "ウィンドウの初期化に失敗しました");
	}

	//アプリケーションループを開始
	app.loop();

	return 0;
}