/**
 * @file PixelShader.hlsl
 * @author Lhxl
 * @date 2025-2-12
 * @version build14
 */

cbuffer CBuf {
	float4 face_colors[6];
};

float4 main(uint tid : SV_PrimitiveID) : SV_Target {
	return face_colors[tid / 2];
}