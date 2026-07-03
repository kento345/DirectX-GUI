#include "Pipline_State_Object.h"
#include<cassert>

//パイプラインステートオブジェクトを作成
bool Pipline_State_Object::create(const Shader& shader, const Root_Signature& rootSignature, std::function<VertexLayout()> layout, int blendType, bool depth) noexcept {
	//デプスステート設定
	D3D12_DEPTH_STENCIL_DESC depthStateDesc{};
	depthStateDesc.DepthEnable = depth;
	depthStateDesc.StencilEnable = false;
	depthStateDesc.DepthWriteMask = depth ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
	depthStateDesc.DepthFunc = depth ? D3D12_COMPARISON_FUNC_LESS : D3D12_COMPARISON_FUNC_NONE;
	
	//ブレンドステート
	D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc{};
	renderTargetBlendDesc.BlendEnable = true;
	renderTargetBlendDesc.SrcBlend = blendType == 2 ? D3D12_BLEND_SRC_ALPHA : D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlend = blendType == 2 ? D3D12_BLEND_INV_SRC_ALPHA : blendType == 1 ? D3D12_BLEND_ONE : D3D12_BLEND_ZERO;
}