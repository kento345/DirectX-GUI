#pragma once

#include <d3d12.h>
#include <unordered_map>
#include <optional>
#include <memory>

class Descriptor_Heap;  /// 前方宣言

//簡易シングルトンパターンで作成する

class DescriptorHeapContainer final {
public:
    //インスタンスの取得
    static DescriptorHeapContainer& instance() noexcept {
        static DescriptorHeapContainer instance;
        return instance;
    }

    //ディスクリプタヒープを生成する
    bool create(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible = false) noexcept;
    //解放予約されているディスクリプタを解放する

    void applyPendingFree() noexcept;

    //ディスクリプタヒープを取得する
    ID3D12DescriptorHeap* get(D3D12_DESCRIPTOR_HEAP_TYPE type) const noexcept;

    //ディスクリプタを確保する
    std::optional<UINT> allocateDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type) noexcept;

    //開放予定のディスクリプタを登録する
    void releaseDescriptor(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT descriptorIndex) noexcept;

private:
    //コンストラクタ
    DescriptorHeapContainer();

    //デストラクタ
    ~DescriptorHeapContainer();

    //コピーとムーブの禁止
    DescriptorHeapContainer(const DescriptorHeapContainer& r) = delete;
    DescriptorHeapContainer& operator=(const DescriptorHeapContainer& r) = delete;
    DescriptorHeapContainer(DescriptorHeapContainer&& r) = delete;
    DescriptorHeapContainer& operator=(DescriptorHeapContainer&& r) = delete;

private:
    std::unordered_map<UINT, std::unique_ptr<Descriptor_Heap>> map_{};  /// 種類毎のデスクリプタマップ
};
