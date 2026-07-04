//一枚のテクスチャを画面全体に描写するシェーダ
//頂点シェーダの入力構造体
struct VSInput
{
    float2 position : POSITIONT;//入力:頂点座標
    float2 texcoord : TEXCOORD; //入力:テクスチャ座標
};

//ピクセルシェーダの入力構造体
struct PSInput
{
    float4 position : SV_Position; //出力:変換後座標
    float2 texcoord : TEXCOORD;    //出力:テクスチャ座標
};

Texture2D sourceTexture : register(t0);   //シェーダに渡されたテクスチャ
SamplerState linearSampler : register(s0);//テクスチャの色を取得するための設定(サンプラーステート)

//頂点シェーダ
PSInput vs(VSInput input)
{
    PSInput output;
    
    //頂点をそのまま出力
    output.position = float4(input.position, 0.0, 1.0);
    
    //テクスチャ座標もそのまま出力
    output.texcoord = input.texcoord;
    
    return output;
}

//ピクセルシェーダ
float4 ps(PSInput input) : SV_Target
{
    //テクスチャの色をそのまま出力
    return sourceTexture.Sample(linearSampler, input.texcoord);
}