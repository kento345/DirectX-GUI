//ウィンドウアプリケーション

#include"../window/Window.h"
#include"../window/Input.h"

#include"../directx/Device.h"
#include"../directx/DXGI.h"
#include"../directx/Command_Allocator.h"
#include"../directx/Command_List.h"
#include"../directx/Command_Queue.h"
#include"../directx/Swap_Chain.h"
#include"../directx/Descriptor_heap.h"
#include"../directx/Render_Target.h"
#include"../directx/Fence.h"
#include"../directx/Root_Signature.h"
#include"../directx/Shader.h"
#include"../directx/Pipline_State_Object.h"
#include"../directx/Constant_Buffer.h"
#include"../directx/DepthBuffer.h"

#include"../draw_resorce/Triangle_Polygon.h"
#include"../draw_resorce/Quad_Polygon.h"
#include"../draw_resorce/Screen_Polygon.h"
#include"../draw_resorce/Shape_Container.h"

#include"../object/Object.h"
#include"../object/Camera.h"
#include"../object/Game_Object_Manager.h"
#include"../object/Player.h"
#include"../object/Enemy.h"

#include"../render/post_proc/Blur.h"

#include"../editor/ImGuiManager.h"
#include"../editor/content/Blur_Editor.h"
#include"../editor/content/Game_Object_Editor.h"

#include<memory>
#include<vector>
#include<cassert>

namespace {
	constexpr UINT sceneShaderSlot_ = 0; //シーン共通用シェーダースロット
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
		if (S_OK != Window::instance().create(instance, 1280, 720, "MyApp")) {
			assert(false && "ウィンドウの生成に失敗しました");
			return false;
		}

		//デバイス生成
		if (!Device::instance().create()) {
			assert(false && "デバイスの作成に失敗しました");
			return false;
		}

		//コマンドキュー
		if (!commandQueue_.create()) {
			assert(false && "コマンドキューの作成に失敗しました");
			return false;
		}

		//スワップチェインの作成
		if (!swapChain_.create(commandQueue_)) {
			assert(false && "スワップてぇいんの作成に失敗しました");
			return false;
		}

		//ディスクリプタヒープの生成
		if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_RTV,10)) {
			assert(false && "ディスクリプタヒープの作成に失敗しました");
			return false;
		}

		//定数バッファ用ディスクリプタヒープ生成
		if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 64, true))
		{
			assert(false && "定数バッファ用ディスクリプタヒープの作成に失敗しました");
			return false;
		}

		//デプスバッファ用ディスクリプタヒープの作成
		if (!DescriptorHeapContainer::instance().create(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1)) {
			assert(false && "デプスバッファ用ディスクリプタヒープの作成に失敗しました");
			return false;
		}

		//レンダーターゲットの生成
		if (!renderTarget_.createBackBuffer(swapChain_)) {
			assert(false && "レンダーターゲットの作成に失敗しました");
			return false;
		}

		//縮小バッファレンダーターゲットを二枚作成
		if (!tempRenderTarget_.createrenderTarget(1280 / 4, 720 / 4, DXGI_FORMAT_R8G8B8A8_UNORM, 2)) {
			assert(false && "レンダーターゲットの作成に失敗しました");
			return false;
		}

		//デプスバッファの生成
		if (!depthBuffer_.create()) {
			assert(false && "デプスバッファの作成に失敗しました");
			return false;
		}

		//コマンドアロケータの生成
		if (!commandAllocator_[0].create(D3D12_COMMAND_LIST_TYPE_DIRECT)){
			assert(false && "コマンドアロケータの作成に失敗しました");
			return false;
		}
		if (!commandAllocator_[1].create(D3D12_COMMAND_LIST_TYPE_DIRECT)){
			assert(false && "コマンドアロケータの作成に失敗しました");
			return false;
		}

		//コマンドリストの生成
		if (!commandList_.create(commandAllocator_[0])) {
			assert(false && "コマンドリストの作成に失敗しました");
			return false;
		}

		//フェンスの生成
		if (!fence_.create()) {
			assert(false && "フェンスの作成に失敗しました");
			return false;
		}

		//ルートシグネチャの生成
		if (!draw3DRS_.createFromProvider<Draw3DRootParameters>()) {
			assert(false && "ルートシグネチャーの作成に失敗しました");
			return false;
		}

		//ルートシグネチャの生成
		if (!drawScreenTextureRS_.createFromProvider<DrawScreenTextureRootParameters>()) {
			assert(false && "ルートシグネチャーの作成に失敗しました");
			return false;
		}

		//3Dモデルシェーダーの生成
		if (!draw3DShader_.create("asset/shader.hlsl")) {
			assert(false && "シェーダーの作成に失敗しました");
			return false;
		}

		//縮小バッファ作成シェーダーの生成
		if (!downSampleShader_.create("asset/down_sample.hlsl")) {
			assert(false && "シェーダーの作成に失敗しました");
			return false;
		}

		//ブラーシェーダーの生成
		if (!blueShader_.create("asset / blur.hlsl")) {
			assert(false && "シェーダーの作成に失敗しました");
			return false;
		}

		//スクリーンテクスチャ描画シェーダーの生成
		if (!drawScreenTextureShader_.create("asset/screen_texture.hlsl")) {
			assert(false && "シェーダーの作成に失敗しました");
			return false;
		}

		//3Dモデル描画パイプラインステートオブジェクトの生成
		if (!draw3DPSO_.create(draw3DShader_, draw3DRS_, ModelVertex::Layout, 2, true)) {
			assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
			return false;
		}

		//縮小バッファ作成パイプラインステートオブジェクトの生成
		if (!downSamplePSO_.create(downSampleShader_, drawScreenTextureRS_, ScreenVertex::Layout, 0, false)) {
			assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
			return false;
		}

		//ブラー専用ルートシグネチャの生成(CBV スロット付き)
		if (!blurPSO_.create(blueShader_, blurRS_, ScreenVertex::Layout, 0, false)) {
			assert(false && "ブラー用ルートシグネチャの作成に失敗しました");
			return false;
		}

		//合成パイプラインステートオブジェクトの生成
		if (!compositePSO_.create(drawScreenTextureShader_, drawScreenTextureRS_, ScreenVertex::Layout, 1, false)) {
			assert(false && "パイプラインステートオブジェクトの作成に失敗しました");
			return false;
		}

		//ブラーの生成
		if (!blur_.create()) {
			assert(false && "ブラーの作成に失敗しました");
			return false;
		}

		//カメラの作成
		camera_ = std::make_unique<game::Camera>();
		camera_->initialize();

		//ゲームオブジェクトの生成
		game::Game_Object_Manager::instance().create(id::get<game::Player>());
		game::Game_Object_Manager::instance().create(id::get<game::Enemy>());

		//ImGuiの初期化(Device・CommandQueueの生成後に呼ぶ)
		//numFrames = 2はダブルバッファリングの枚数
		if (!ImGuiManager::instance().initialize(
			Window::instance().handle(),
			Device::instance().get(),
			commandQueue_.get(),
			2,
			DXGI_FORMAT_R8G8B8A8_UNORM)) {
			assert(false && "ImGuiの初期化に失敗しました");
			return false;
		}

		return true;
	}

	//アプリケーションループ
	void loop()noexcept {
		while (Window::instance().messageLoop()) {
			//メッセージループ内での処理をここに記述
			//----------更新処理----------

			//カメラ更新
			camera_->update();

			//ゲームオブジェクトの更新
			game::Game_Object_Manager::instance().update();

			//ゲームオブジェクトの後更新
			game::Game_Object_Manager::instance().postUpdate();

			//デバッグ
			//ImGuiの新フレーム開始とUI構築
			//ImGui::NewFrame()以降でないとImGui::Begin()などは呼べない
			ImGuiManager::instance().newFrame();
#if defined(_DEBUG)
			blurEditor_.draw(blur_);
			gameObjectEditor_.draw();
#endif
			//----------描画処理----------
			//現在のバッグバッファインデックスを取得
			const auto backBufferIndex = swapChain_.get()->GetCurrentBackBufferIndex();

			//以前のフレームのGPUの処理が完了しているか確認して待機
			if (frameFrnceValue_[backBufferIndex] != 0) {
				fence_.wait(frameFrnceValue_[backBufferIndex]);
			}

			//ディスクリプタヒープの解放予約分を開放
			DescriptorHeapContainer::instance().applyPendingFree();

			//コマンドアロケータリセット
			commandAllocator_[backBufferIndex].reset();
			//コマンドリストリセット
			commandList_.reset(commandAllocator_[backBufferIndex]);

			//コンスタントバッファ用ディスクリプタヒープの設定
			ID3D12DescriptorHeap* p[] = { DescriptorHeapContainer::instance().get(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) };
			commandList_.get()->SetDescriptorHeaps(1, p);

			// 3Dシーン描画(描画パス 1)
			{
				//リソースバリアでレンダーターゲットをPresentからRenderTargetへ変更
				auto pToRt = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &pToRt);

				//レンダーターゲットの設定
				D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTarget_.getCpuDescriptorHandle(backBufferIndex) };
				//デプスバッファのハンドルも設定
				D3D12_CPU_DESCRIPTOR_HANDLE depthHandle = depthBuffer_.getCpuDescriptorHandle();
				commandList_.get()->OMSetRenderTargets(1, handles, false, &depthHandle);

				//ビューポートの設定
				const auto [w, h] = renderTarget_.size();
				D3D12_VIEWPORT viewport{
					0.0f,0.0f,
					static_cast<float>(w),static_cast<float>(h),
					0.0f,1.0f };
				commandList_.get()->RSSetViewports(1, &viewport);

				//シザー短形の設定
				D3D12_RECT scissorRect{ 0,0,static_cast<INT64>(w),static_cast<INT64>(h)};
				commandList_.get()->RSSetScissorRects(1, &scissorRect);

				//レンダーターゲットのクリア
				const float clearColor[] = { 0.4f,0.4f,0.4f,1.0f }; //クリア
				commandList_.get()->ClearRenderTargetView(handles[0], clearColor, 0, nullptr);
				//デプスバッファのクリア
				commandList_.get()->ClearDepthStencilView(depthHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

				//ルートシグネチャの設定
				commandList_.get()->SetGraphicsRootSignature(draw3DRS_.get());
				//パイプラインステートの設定
				commandList_.get()->SetPipelineState(draw3DPSO_.get());

				//カメラのコンスタントバッファへデータ転送
				camera_->updateDrawBuffer();
				camera_->setDrawCommand(commandList_, sceneShaderSlot_);

				//ゲームオブジェクトの描画
				game::Game_Object_Manager::instance().draw(commandList_);
			}
			
			//縮小バッファの作成(描画パス 2)
			{
				//レンダーターゲットの設定
				D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { tempRenderTarget_.getCpuDescriptorHandle(0) };
				commandList_.get()->OMSetRenderTargets(1, handles, false, nullptr);

				//ビューポートの設定
				const auto [w, h] = tempRenderTarget_.size();
				D3D12_VIEWPORT viewport{
					0.0f,0.0f,
					static_cast<float>(w),static_cast<float>(h),
					0.0f,1.0f };
				commandList_.get()->RSSetViewports(1, &viewport);

				//シザー短形の設定
				D3D12_RECT scissorRect{ 0,0,static_cast<INT64>(w),static_cast<INT64>(h) };
				commandList_.get()->RSSetScissorRects(1, &scissorRect);

				//レンダーターゲットを別のシェーダで読み取れるように理🅂－スバリアに変更
				auto rtToSr = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
				commandList_.get()->ResourceBarrier(1, &rtToSr);

				//ルートシグネチャの設定
				commandList_.get()->SetGraphicsRootSignature(drawScreenTextureRS_.get());
				//パイプラインステートの設定
				commandList_.get()->SetPipelineState(downSamplePSO_.get());

				//テクスチャを設定して描画
				commandList_.get()->SetGraphicsRootDescriptorTable(0, renderTarget_.getGpuDescriptorHandle(backBufferIndex));
				auto id = Shape_Container::instance().create<Screen_Polygon>();
				Shape_Container::instance().draw(commandList_,id);
			}

			//ブラー処理(描画パス 3)
			{
				//ビューポートとシザーは縮小バッファ作成時と同じなので省略
			    //レンダーターゲットを別のシェーダで読み取れるように理🅂－スバリアに変更
				auto rtToSr = resourceBarrier(tempRenderTarget_.get(0), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
				commandList_.get()->ResourceBarrier(1, &rtToSr);

				//レンダーターゲットの設定
				D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { tempRenderTarget_.getCpuDescriptorHandle(1) };
				commandList_.get()->OMSetRenderTargets(1, handles, false, nullptr);

				//ブラー専用ルートシグネチャの設定(CBV スロット付き)
				commandList_.get()->SetGraphicsRootSignature(blurRS_.get());
				//パイプラインステートの設定
				commandList_.get()->SetPipelineState(blurPSO_.get());

				//スロット 0:　テクスチャを設定
				commandList_.get()->SetGraphicsRootDescriptorTable(0, tempRenderTarget_.getGpuDescriptorHandle(0));
				//スロット 1:　ブラー設定コンスタンントバッファを設定
				commandList_.get()->SetGraphicsRootDescriptorTable(1, blur_.constantoBuffer().getGpuDescriptorHandle());

				auto id = Shape_Container::instance().create<Screen_Polygon>();
				Shape_Container::instance().draw(commandList_, id);

				//レンダーターゲットを別のシェーダで読み取れるように理🅂－スバリアに変更
				auto srToRt = resourceBarrier(tempRenderTarget_.get(0), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &srToRt);
			}

			//ブラー処理2回目(描画パス 4)
			{
				// ビューポートとシザーは縮小バッファ作成時と同じなので省略
				// ルートシグネチャとパイプラインステートは描画パス 3 から引き継がれる

				//レンダーターゲットを別のシェーダで読み取れるように理🅂－スバリアに変更
				auto rtToSr = resourceBarrier(tempRenderTarget_.get(1), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
				commandList_.get()->ResourceBarrier(1, &rtToSr);

				//レンダーターゲットの設定
				D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { tempRenderTarget_.getCpuDescriptorHandle(0) };
				commandList_.get()->OMSetRenderTargets(1, handles, false, nullptr);

				//スロット 0:　テクスチャを設定
				commandList_.get()->SetGraphicsRootDescriptorTable(0, tempRenderTarget_.getGpuDescriptorHandle(1));
				//スロット 1:　ブラー設定コンスタンントバッファを設定
				commandList_.get()->SetGraphicsRootDescriptorTable(1, blur_.constantoBuffer().getGpuDescriptorHandle());

				auto id = Shape_Container::instance().create<Screen_Polygon>();
				Shape_Container::instance().draw(commandList_, id);

				//レンダーターゲットを別のシェーダで読み取れるように理🅂－スバリアに変更
				auto srToRt = resourceBarrier(tempRenderTarget_.get(1), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &srToRt);
			}

			//合成描画(描画パス　5)
			{
				//リソースバリアでレンダーターゲットをRenderTargetへ変更
				auto srToRt = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &srToRt);
				//リソースバリアでレンダーターゲットをRenderTargetへ変更
				auto rtToSr = resourceBarrier(tempRenderTarget_.get(0), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
				commandList_.get()->ResourceBarrier(1, &rtToSr);

				//レンダーターゲットの設定
				D3D12_CPU_DESCRIPTOR_HANDLE handles[] = { renderTarget_.getCpuDescriptorHandle(backBufferIndex) };
				commandList_.get()->OMSetRenderTargets(1, handles, false, nullptr);

				//ビューポートの設定
				const auto [w, h] = renderTarget_.size();
				D3D12_VIEWPORT viewport{
					0.0f,0.0f,
					static_cast<float>(w),static_cast<float>(h),
					0.0f,1.0f };
				commandList_.get()->RSSetViewports(1, &viewport);

				//シザー短形の設定
				D3D12_RECT scissorRect{ 0,0,static_cast<INT64>(w),static_cast<INT64>(h) };
				commandList_.get()->RSSetScissorRects(1, &scissorRect);

				//ルートシグネチャの設定
				commandList_.get()->SetGraphicsRootSignature(drawScreenTextureRS_.get());
				//パイプラインステートの設定
				commandList_.get()->SetPipelineState(compositePSO_.get());

				//テクスチャを設定して描画
				commandList_.get()->SetGraphicsRootDescriptorTable(0, tempRenderTarget_.getGpuDescriptorHandle(0));
				auto id = Shape_Container::instance().create<Screen_Polygon>();
				Shape_Container::instance().draw(commandList_, id);

				//リソースバリアでレンダーターゲットをRenderTargetへ変更
				auto srToRt2 = resourceBarrier(tempRenderTarget_.get(0),D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE , D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &srToRt);

				//リソースバリアでレンダーターゲットをRenderTargetからPresentへ変更
				auto rtToP = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET , D3D12_RESOURCE_STATE_PRESENT);
				commandList_.get()->ResourceBarrier(1, &rtToP);
			}

			//ImGui描画(描画パス　6)
			//Pass5がバックバッファをPRESENT状態にしたので
			//ImGuiを書くためにRenderTarget状態に戻してから描画
			{
				auto pToRt = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
				commandList_.get()->ResourceBarrier(1, &pToRt);

				//ImGuoの描画先レンダーターゲットを設定
				D3D12_CPU_DESCRIPTOR_HANDLE rtHandle = renderTarget_.getCpuDescriptorHandle(backBufferIndex) ;
				commandList_.get()->OMSetRenderTargets(1, &rtHandle, false, nullptr);

				//ImGuiの描画コマンドを積む(内部でヒープ切り替えも行う)
				ImGuiManager::instance().render(commandList_.get());

				//再びPresent状態に
				auto rtToP = resourceBarrier(renderTarget_.get(backBufferIndex), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
				commandList_.get()->ResourceBarrier(1, &rtToP);
			}

			//コマンドリストをクローズ
			commandList_.get()->Close();

			//コマンドキューにコマンドリストを送信
			ID3D12CommandList* ppCommandLists[] = { commandList_.get() };
			commandQueue_.get()->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

			//プレゼント
			swapChain_.get()->Present(1, 0);

			//フェンスにフェンス値wp設定
			commandQueue_.get()->Signal(fence_.get(), nextFenceValue_);
			frameFrnceValue_[backBufferIndex] = nextFenceValue_;
			nextFenceValue_++;
		}
		//ループを抜けるとウィンドウを閉じる
		game::Game_Object_Manager::instance().clear();

		//ImGuiのシャットダウン(DX12 理🅂０ス解放前に必ず呼ぶ)
		ImGuiManager::instance().Shutdown();
	}

	//リソースにバリアを設定する
	D3D12_RESOURCE_BARRIER resourceBarrier(ID3D12Resource* resorce, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to)noexcept {
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = resorce;
		barrier.Transition.StateBefore = from;
		barrier.Transition.StateAfter = to;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		return barrier;
	}

private:
	Command_Queue commandQueue_{};           //コマンドキューインスタンス
	Swap_Chain swapChain_{};				 //スワップチェインインスタンス
	DepthBuffer depthBuffer_{};				 //デプスバッファインスタンス
	Command_Allocator commandAllocator_[2]{};//コマンドアロケータインスタンス
	Command_List commandList_{};			 //コマンドリストインスタンス

	Fence fence_{};              //フェンスインスタンス 
	UINT64 frameFrnceValue_[2]{};//現在のフレームのフェンス値
	UINT64 nextFenceValue_ = 1;  //次のフレームのフェンス値

	Render_Target renderTarget_{};	  //レンダーターゲットインスタンス
	Render_Target tempRenderTarget_{};//縮小バッファ(2枚)

	Root_Signature draw3DRS_{};           //3D描画ルートシグネチャーインスタンス
	Root_Signature drawScreenTextureRS_{};//テクスチャ描画ルートシグネチャインスタンス
	Root_Signature blurRS_{};			  //ブラー専用ルートシグネチャ(CBVスロット付き)

	Shader draw3DShader_{};			  //3D描画シェーダインスタンス
	Shader downSampleShader_{};		  //縮小バッファ作成シェーダインスタンス
	Shader blueShader_{};			  //ブラーシェーダインスタンス
	Shader drawScreenTextureShader_{};//スクリーンテクスチャ描画シェーダインスタンス

	Pipline_State_Object draw3DPSO_{};    //3D描画シェーダインスタンス
	Pipline_State_Object downSamplePSO_{};//縮小バッファ作成パイプラインステートオブジェクトインスタンス
	Pipline_State_Object blurPSO_{};	  //ブラーパイプラインステートオブジェクトインスタンス
	Pipline_State_Object compositePSO_{}; //スクリーンテクスチャ描画パイプラインステートオブジェクトインスタンス

	std::unique_ptr<game::Camera> camera_{}; //カメラ

	graphics::Blur blur_{}; //ブラーインスタンス

	//デバッグ用エディタインスタンスはリリースビルド出は存在しない
#if defined(_DEBUG)
	dev::editor::Game_Object_Editor gameObjectEditor_{};//ゲームオブジェクトエディタインスタンス
	dev::editor::Blur_Editor        blurEditor_{};		//ブラーエディタインスタンス
#endif // defined(_DEBUG)
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
