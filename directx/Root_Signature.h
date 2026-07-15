#pragma once

#include"Device.h"
#include<wrl.h>

//3Dモデル描画用ルートパラメータ
struct Draw3DRootParameters
{
	Draw3DRootParameters() {
		//コンスタントバッファ(スロット b0)
		//今回の場合はカメラのビュー行列や射影行列が入る想定
		r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		r0.NumDescriptors = 1;
		r0.BaseShaderRegister = 0;
		r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		//コンスタントバッファ(スロット b0)
		//今回の場合はポリゴンのワールド行列や色が入る想定
		r1.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		r1.NumDescriptors = 1;
		r1.BaseShaderRegister = 1;
		r1.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//ルートパラメータの設定
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX; //頂点シェーダーのみで利用
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &r0;
		rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL; //全てのシェーダーで利用
		rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[1].DescriptorTable.pDescriptorRanges = &r1;

		//ルートシグネーチャの設定
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pStaticSamplers = nullptr;
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	D3D12_DESCRIPTOR_RANGE r0{}; //コンスタントバッファ(スロット b0)
	D3D12_DESCRIPTOR_RANGE r1{}; //コンスタントバッファ(スロット b1)
	D3D12_ROOT_PARAMETER rootParameters[2]{}; //ルートパラメータ
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{}; //ルートシグネチャ記述子
};

//フルスクリーン一枚絵描写用パラメーター
struct DrawScreenTextureRootParameters {
	DrawScreenTextureRootParameters() {
		//テクスチャーテーブル(t0~)
		r0.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		r0.NumDescriptors = 1;
		r0.BaseShaderRegister = 0;
		r0.RegisterSpace = 0;
		r0.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//スタティックサンプル(s0)	{
		samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		samplers[0].MipLODBias = 0;
		samplers[0].MaxAnisotropy = 0;
		samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplers[0].BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplers[0].MinLOD = 0.0f;
		samplers[0].MaxLOD = D3D12_FLOAT32_MAX;
		samplers[0].ShaderRegister = 0;
		samplers[0].RegisterSpace = 0;
		samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

		//ルートパラメータの設定
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &r0;

		//ルートシグネチャの設定
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = 1;
		rootSignatureDesc.pStaticSamplers = samplers;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	D3D12_DESCRIPTOR_RANGE r0{};
	D3D12_STATIC_SAMPLER_DESC samplers[1]{};
	D3D12_ROOT_PARAMETER rootParameters[1]{};
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
};

//ブラーシェーダー専用ルートパラメータ
struct BlurRootParameters
{
	BlurRootParameters() {
		//テクスチャテーブル(t0)s
		rSrv.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		rSrv.NumDescriptors = 1;
		rSrv.BaseShaderRegister = 0;
		rSrv.RegisterSpace = 0;
		rSrv.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//コンスタントバッファ(b0) <-ブラー設定用
		rCbv.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		rCbv.NumDescriptors = 1;
		rCbv.BaseShaderRegister = 0;
		rCbv.RegisterSpace = 0;
		rCbv.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		//スタティックサンプラー(s0)- DrawScreenTextureRootParametersと同じ設定
		sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		sampler.MipLODBias = 0;
		sampler.MaxAnisotropy = 0;
		sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		sampler.MinLOD = 0.0f;
		sampler.MaxLOD = D3D12_FLOAT32_MAX;
		sampler.ShaderRegister = 0;
		sampler.RegisterSpace = 0;
		sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
        
		//ルートパラメータ 0 :テクスチャ
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL; //全てのシェーダーで利用
		rootParameters[0].DescriptorTable.NumDescriptorRanges = 1;
		rootParameters[0].DescriptorTable.pDescriptorRanges = &rSrv;

		//ルートパラメータ 1 : ブラー設定 CBV
		 rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		 rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
		 rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
		 rootParameters[1].DescriptorTable.pDescriptorRanges = &rCbv;

		//ルートシグネーチャの設定
		rootSignatureDesc.pParameters = rootParameters;
		rootSignatureDesc.NumParameters = 2;
		rootSignatureDesc.pStaticSamplers = &sampler;
		rootSignatureDesc.NumStaticSamplers = 1;
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	}

	D3D12_DESCRIPTOR_RANGE rSrv{}; //テクスチャ　SRV　レンジ(ts0)
	D3D12_DESCRIPTOR_RANGE rCbv{}; //コンスタントバッファ　CBV　レンジ(b0)
	D3D12_STATIC_SAMPLER_DESC sampler{};///サンプラー(s0)
	D3D12_ROOT_PARAMETER rootParameters[2]{}; //ルートパラメータ
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{}; //ルートシグネチャ記述子
};


class Root_Signature final
{
public:
	//コンストラクタ
	Root_Signature() = default;
	//デストラクタ
	~Root_Signature() = default;

	template <class T>
	bool createFromProvider() noexcept {
		T provider;
		return create(provider.rootSignatureDesc);
	}
	//ルートシグネチャを作成
	bool create(const D3D12_ROOT_SIGNATURE_DESC& desc) noexcept; 

	//ルートシグネチャを取得
	ID3D12RootSignature* get() const noexcept;

	//生成済みか
	bool isValid() const noexcept { return static_cast<bool>(rootSignature_); }

private:
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_; //ルートシグネチャ
};

