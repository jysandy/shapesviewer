#pragma once

#include <DirectXMath.h>

namespace ShapesDemo
{
	struct Material
	{
		DirectX::XMFLOAT4 ambient;
		DirectX::XMFLOAT4 diffuse;
		DirectX::XMFLOAT4 specular;	//w == specular power

		Material(){}
		Material(const Material& in)
		{
			ambient = in.ambient;
			diffuse = in.diffuse;
			specular = in.specular;
		}
	};
}