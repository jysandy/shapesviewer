#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Spotlight.h"
#include "Material.h"

namespace ShapesDemo
{
	struct LightConstantBuffer
	{
		PointLight pointLight;
		Material material;
		DirectionalLight directionalLight;
		Spotlight spotlight;
		DirectX::XMFLOAT3 cameraPosition;
		float pad;

		LightConstantBuffer() 
		{
			XMFLOAT4 init = XMFLOAT4(62, 62, 62, 62);
			XMFLOAT3 init3 = XMFLOAT3(62, 62, 62);
			Material m;
			m.ambient = init;
			m.diffuse = init;
			m.specular = init;
			material = m;
			cameraPosition = init3;
			pad = 62;
			PointLight p;
			p.ambient = init;
			p.diffuse = init;
			p.specular = init;
			p.pad = 62;
			p.range = 62;
			p.attenuation = init3;
			p.position = init3;
			
		}
		LightConstantBuffer(const LightConstantBuffer& in)
		{
			material = in.material;
			spotlight = in.spotlight;
			pointLight = in.pointLight;
			directionalLight = in.directionalLight;
			cameraPosition = in.cameraPosition;
		}
	};
}