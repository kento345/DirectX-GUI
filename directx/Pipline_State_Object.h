#pragma once

#include"Device.h"
#include"Shader.h"
#include"Root_Signature.h"
#include<functional>

using VertexLayout = std::pair<D3D12_INPUT_ELEMENT_DESC* ,size_t>;

class Pipline_State_Object final
{
public:
	//コンストラクタ
	Pipline_State_Object() = default;
	//デストラクター
	~Pipline_State_Object() = default;

	//パイプラインステートオブジェクトを作成
	bool create(const Shader& shader, const Root_Signature& rootSignature, std::function<VertexLayout()> layout, int blendType, bool depth)noexcept; 

	//パイプラインステートオブジェクトを取得
	ID3D12PipelineState* get() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelineState_{}; //パイプラインステートオブジェクト
};

