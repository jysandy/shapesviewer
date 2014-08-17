#pragma once

#include <DirectXMath.h>

namespace ShapesDemo
{
	struct ConstantBufferData
	{
	private:
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;

	public:
		void SetWorldMatrix(DirectX::XMFLOAT4X4 in);
		void SetViewMatrix(DirectX::XMFLOAT4X4 in);
		void SetProjectionMatrix(DirectX::XMFLOAT4X4 in);
	};
}