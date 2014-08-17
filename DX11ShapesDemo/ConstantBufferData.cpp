#include "ConstantBufferData.h"

namespace ShapesDemo
{
	void ConstantBufferData::SetWorldMatrix(DirectX::XMFLOAT4X4 in)
	{
		DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&in);
		temp = DirectX::XMMatrixTranspose(temp);
		DirectX::XMStoreFloat4x4(&world, temp);
	}
	
	void ConstantBufferData::SetViewMatrix(DirectX::XMFLOAT4X4 in)
	{
		DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&in);
		temp = DirectX::XMMatrixTranspose(temp);
		DirectX::XMStoreFloat4x4(&view, temp);
	}

	void ConstantBufferData::SetProjectionMatrix(DirectX::XMFLOAT4X4 in)
	{
		DirectX::XMMATRIX temp = DirectX::XMLoadFloat4x4(&in);
		temp = DirectX::XMMatrixTranspose(temp);
		DirectX::XMStoreFloat4x4(&projection, temp);
	}
}