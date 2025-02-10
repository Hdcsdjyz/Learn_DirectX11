/**
 * @file VertexShader.hlsl
 * @author Lhxl
 * @date 2025-2-10
 * @version build11
 */


float4 main(float2 pos : Position) : SV_Position {
    return float4(pos.x, pos.y, 0.0f, 1.0f);
}