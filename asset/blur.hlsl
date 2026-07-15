//5x5の範囲のピクセルを合成するブラーシェーダ

//頂点シェーダの入力構造体
struct VSInput
{
    float2 position : POSITIONT; //入力:頂点座標
    float2 texcoord : TEXCOORD; //入力:テクスチャ座標
};

//ピクセルシェーダの入力構造体
struct PSInput
{
    float4 position : POSITIONT; //出力:変換座標
    float2 texcoord : TEXCOORD; //出力:テクスチャ座標
};

Texture2D sourceTexture : register(t0); //シェーダに渡されたテクスチャ
SamplerState linearSampler : register(s0); //テクスチャの色を取得する為の設定(サンプラーステート)

//ブラー設定コンスタントバッファ(ImGui からリアルタイムに調整出来る)
cbuffer BlurSettings : register(b0)
{
    float blurStrength; //ブラーの強さ(0.0でぼかしなし,大きいほど強いぼかし)
    float thereshold; //ブラー対象の色の閾値(小さいほど強いブラー)
    float2 padding; //float4　アライメント用パディング
};

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
    
    //現在のUV位置を中心に5x5ピクセルの色を取得して合成
    //blurStrengthでサンプリングの広がりを制御する(1.0が標準,0.0でぼかしなし)
    
    //注意:テクスチャの色を取得すればするほど負荷が高い,本来は縦or横方向の二回に分けて処理する
    
    float4 color = float4(0, 0, 0, 0);
    for (int x = -2; x <= 2; x++)
    {
        for (int y = -2; y <= 2; y++)
        {
            color += sourceTexture.Sample(linearSampler, uv + (float2(x, y) * texelSize * blurStrength));
        }

    }
    
    //5x5ピクセルの平均色を計算(25 サンプルの平均)
    color = color * 0.04;
    
    // 「ITU-R BT.709」という規格の係数で輝度を計算する
	// 人間の目は緑に最も敏感なため、緑の係数が最も大きい
    float luminace = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
    
    // 輝度が閾値をどれだけ超えているかの割合を求め、色相を保ちながら暗い部分を黒に落とす
	// luminance がほぼ 0 の場合のゼロ除算を防ぐため、max で下限を設ける
    float contribution = max(luminace - thereshold, 0.0) / max(luminace, 0.0001);
    color.rgb *= contribution;
 
    
    return color;
}