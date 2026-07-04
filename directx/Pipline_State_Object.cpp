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

	renderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	renderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0;i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i) {
		blendDesc.RenderTarget[i] = renderTargetBlendDesc;
	}

	//ラスタライザーステート
	//ポリゴンの塗りつぶし方法や裏面カリングの設定を行う
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;//裏面カリングなし
	rasterizerDesc.FrontCounterClockwise = false;;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS; 
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP; 
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//頂点レイアウトの取得
	const auto [inputElementDescs, numElements] = layout();

	//パイプラインステート
	//各種設定を構造体にまとめる
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};
	psoDesc.InputLayout = { inputElementDescs, static_cast<UINT>(numElements) };
	psoDesc.pRootSignature = rootSignature.get(); 
	psoDesc.VS = { shader.vertexShader()->GetBufferPointer(),shader.vertexShader()->GetBufferSize()};
	psoDesc.PS = { shader.pixelShader()->GetBufferPointer(),shader.pixelShader()->GetBufferSize() };
	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = blendDesc;
	psoDesc.DepthStencilState = depthStateDesc;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;
	auto res = Device::instance().get()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pipelineState_));
	if (FAILED(res)) {
		assert(false && "パイプラインステートの作成に失敗");
	}
	return true;
}

//パイプラインステートを取得
//パイプラインステートのポインタを返す
ID3D12PipelineState* Pipline_State_Object::get() const noexcept {
	if (!pipelineState_) {
		assert(false && "パイプラインステートが未作成です");
	}
	return pipelineState_.Get();
}