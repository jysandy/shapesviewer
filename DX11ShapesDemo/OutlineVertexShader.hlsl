cbuffer perObject : register(b0)
{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
}

struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
	float4 color : COLOR;
};

float4 main(VertexShaderInput input) : SV_POSITION
{
	float4x4 wvp;
	wvp = mul(mul(world, view), projection);
	float4 normal = mul(float4(input.normal, 0.0f), world);
		normal = normalize(normal);
	float lineThickness = 0.3;
	float4 newPos = float4(input.pos, 1.0f) + lineThickness * normal;

	float4 posH = mul(newPos, wvp);
	return posH;
}