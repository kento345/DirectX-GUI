//ダウンサンプリング(縮小バッファ作成)シェーダ

//頂点シェーダの入力構造体
struct VSInput
{
    float2 position : POSITIONT;//入力:頂点座標
    float2 texcoord : TEXCOORD; //入力:テクスチャ座標
};

//ピクセルシェーダの入力構造体
struct PSInput
{
    float4 position : POSITIONT;//出力:変換座標
    float2 texcoord : TEXCOORD; //出力:テクスチャ座標
};

Texture2D sourceTexture : register(t0);   //シェーダに渡されたテクスチャ
SamplerState linearSampler : register(s0);//テクスチャの色を取得する為の設定(サンプラーステート)


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
    //テクスチャのサイズから1ピクセル分の大きさを計算
    uint width, height;
    sourceTexture.GetDimensions(width, height);
    float2 texelSize = float2(1.0 / (float) width, 1.0 / (float) height);
    
    //テクスチャ座標を取得
    float2 uv = input.texcoord;
    
    //現在のUV位置を中心に2x2ピクセルの色を取得して合成
    //元のテクスチャが大きい解像度で,書き込箕作が小さい解像度になるため,
    //4ピクセル分の色の平均を書き込むことで,若干きれいなダウンサンプリングを行う
    
    //現在のUV位置からどの程度離れたピクセルの色を取得するかは,解像度の下がり方によって調整が必要
    //float(x,y)を指定している部分がそれに当たる
    float4 color = float4(0, 0, 0, 0);
    color += sourceTexture.Sample(linearSampler, uv + (float2(-1, -1) * texelSize));
    color += sourceTexture.Sample(linearSampler, uv + (float2( 1, -1) * texelSize));
    color += sourceTexture.Sample(linearSampler, uv + (float2(-1,  1) * texelSize));
    color += sourceTexture.Sample(linearSampler, uv + (float2( 1,  1) * texelSize));
    
    return (color * 0.25);
}