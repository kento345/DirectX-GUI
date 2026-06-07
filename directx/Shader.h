#pragma once

#include"Device.h"
#include<string>

class Shader final
{
public:
	//コンストラクタ
	Shader() = default;
	//デストラクタ
	~Shader() = default;

	//シェーダを作成
	bool create(std::string_view fillPath)noexcept;

	//頂点シェーダ取得
	ID3DBlob* vertexShader() const noexcept;

	//ピクセルシェーダ取得
	ID3DBlob* pixelShader() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3DBlob> vertexShader_{};//頂点シェーダ
	Microsoft::WRL::ComPtr<ID3DBlob> pixelShader_{}; //ピクセルシェーダ
};

