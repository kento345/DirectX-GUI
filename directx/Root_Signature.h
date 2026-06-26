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


class Root_Signature final
{

};

