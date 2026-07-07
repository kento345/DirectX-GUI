#pragma once

#include<d3d12.h>
#include<dxgi1_4.h>
#include<wrl/client.h>

#include"../external/imgui/imgui.h"
#include"../external/imgui/backends/imgui_impl_win32.h"
#include"../external/imgui/backends/imgui_impl_dx12.h"


class ImGuiManager final
{
public:
	//インスタンスの取得
	static ImGuiManager& instance()noexcept {
		static ImGuiManager instance;
		return instance;
	}

public:
	//ImGuiの初期化
	bool initialize(
		HWND hwns,
		ID3D12Device* device,
		ID3D12CommandQueue* commandQueue,
		UINT numFrames,
		DXGI_FORMAT rtvFormat)noexcept;

	//新しいフレーム開始
	void newFrame()noexcept;

	//ImGuiの描画コマンドをコマンドリストに積む
	void render(ID3D12GraphicsCommandList* commandList)noexcept;

	//ImGuiをシャットダウン
	void Shutdown()noexcept;

private:
	//コンストラクタ
	ImGuiManager() = default;
	//デストラクタ
	~ImGuiManager() = default;

	//コピーとムーブの禁止
	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator = (const ImGuiManager&) = delete;
	ImGuiManager(ImGuiManager&&) = delete;
	ImGuiManager& operator = (ImGuiManager&&) = delete;

private:
	//ImGui専用CBV_SRV_UAVヒープ(フォントテクスチャ1枚分)
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeap_{};
};

