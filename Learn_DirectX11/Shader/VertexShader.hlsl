/**
 * @file VertexShader.hlsl
 * @author Lhxl
 * @date 2025-2-12
 * @version build14
 */

cbuffer CBuf {
	matrix transform;
};

float4 main(float3 pos : Position) : SV_Position {
	return mul(float4(pos, 1.0f), transform);
}