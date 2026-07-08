#include "ImGuiManager.h"
#include "content/Blur_Editor.h"
#include<cassert>

//ImGuiを初期化
bool ImGuiManager::initialize(HWND hwnd, ID3D12Device* device, ID3D12CommandQueue* commandQueue, UINT numFrames, DXGI_FORMAT rtvFormat) noexcept
{
	//ImGuiコンテクストの作成
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	//スタイル設定(ダークテーマ)
	ImGui::StyleColorsDark();

	//ImGui専用のCBV_SRV_UAVヒープを作成
	//DescriptorHeapContainerの共有ヒープとは独立させることで互いに管理コードが干渉しない
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	const HRESULT hr = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&srvHeap_));
	if (FAILED(hr)) {
		assert(false && "ImGui用SRVヒープの作成に失敗しました");
		return false;
	}
	//Win32バックエンドの初期化
	if (!ImGui_ImplWin32_Init(hwnd)) {
		assert(false && "ImGui_ImplWin32_Initに失敗しました");
		return false;
	}

	//DX12バックエンドの初期化(ImGui1.91.5+ のInitInfo構造体APIを使用)
	//CommandQueueを渡すことでImGuiBackendFlags＿RenderHasTexturesが有効になり
	//フォントテクスチャのアップロードがバックエンド内部で自動処理される
	ImGui_ImplDX12_InitInfo initInfo{};
	initInfo.Device = device;
	initInfo.CommandQueue = commandQueue;
	initInfo.NumFramesInFlight = static_cast<int>(numFrames);
	initInfo.RTVFormat = rtvFormat;
	initInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;
	initInfo.SrvDescriptorHeap = srvHeap_.Get();
	initInfo.LegacySingleSrvCpuDescriptor = srvHeap_->GetCPUDescriptorHandleForHeapStart();
	initInfo.LegacySingleSrvGpuDescriptor = srvHeap_->GetGPUDescriptorHandleForHeapStart();

	if (!ImGui_ImplDX12_Init(&initInfo)) {
		assert(false && "ImGui_ImplDX12_Initに失敗しました");
		return false;
	}

	//日本語フォントの読み込み
	ImGuiIO& io = ImGui::GetIO();
	const auto* font = io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\msgothic.ttc",  // MS ゴシックのフォントファイルパス
		14.0f,                               // フォントサイズ
		nullptr,                             // デフォルト設定
		io.Fonts->GetGlyphRangesJapanese()   // 日本語グリフ範囲
	);
	IM_ASSERT(font != nullptr);
	
	return true;
}

//新しいフレームを開始
void ImGuiManager::newFrame() noexcept
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//ウィンドウ:フレーム情報
	ImGui::Begin("フレーム情報");
	{
		// ImGui が内部で計算した平均 FPS を表示する
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("フレーム時間: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
	}
	ImGui::End();
}

//ImGuiの描画コマンドをコマンドリストに積む
void ImGuiManager::render(ID3D12GraphicsCommandList* commandList) noexcept
{
	//ImGuiの描画データを確定
	ImGui::Render();

	//ImGui専用SRVヒープをコマンドリストにバインドする
	//ゲームの描画が完全に終わった後に呼ぶので,共有ヒープへの影響はない
	ID3D12DescriptorHeap* heaps[] = { srvHeap_.Get() };
	commandList->SetDescriptorHeaps(1, heaps);

	//実際の描画コマンドを積む
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}

//ImGuiをシャットダウン
void ImGuiManager::Shutdown() noexcept
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
