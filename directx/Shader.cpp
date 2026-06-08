#include "Shader.h"
#include<cassert>

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

//シェーダ作成
bool Shader::create(std::string_view filePath) noexcept {
	//シェーダを読み込み,コンパイルして生成

	//シェーダファイルパス
	const std::wstring temp = std::wstring(filePath.begin(), filePath.end());
	//シェーダのコンパイルエラーなどが分かるようにする
	ID3DBlob* error{};

	auto res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "vs", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader_, &error);
	if (FAILED(res)) {
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "頂点シェーダのコンパイルに失敗しました");
	}
	res = D3DCompileFromFile(temp.data(), nullptr, nullptr, "ps", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader_, &error);
	if (FAILED(res)) {
		char* p = static_cast<char*>(error->GetBufferPointer());
		assert(false && "ピクセルシェーダのコンパイルに失敗しました");
	}
	if (error) {
		error->Release();
	}
	return true;
}

//頂点シェーダ取得
ID3DBlob* Shader::vertexShader()const noexcept{
	if (!vertexShader_) {
		assert(false && "頂点シェーダが未作成です");
	}

	return vertexShader_.Get();
}

//ピクセルシェーダ取得
ID3DBlob* Shader::pixelShader()const noexcept {
	if (!pixelShader_) {
		assert(false && "ピクセルシェーダが未作成です");
	}
	return pixelShader_.Get();
}