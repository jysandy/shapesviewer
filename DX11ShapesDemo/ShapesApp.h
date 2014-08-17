#pragma once

#include <DirectXMath.h>
#include <DirectXColors.h>
#include <d3dapp.h>
#include <CompiledShader.h>
#include <MeshGenerator.h>
#include <Model.h>
#include <MeshData.h>
#include <GeometryBufferData.h>
#include <vector>
#include <memory>
#include <cstring>
#include "ConstantBufferData.h"
#include "LightConstantBuffer.h"

using D3dtut::ComPtr;
using D3dtut::D3DApp;
using D3dtut::CompiledShader;
using D3dtut::GeometryBufferData;
using D3dtut::Model;
using D3dtut::HR;

namespace ShapesDemo
{
	class ShapesApp : public D3dtut::D3DApp
	{
	public:
		ShapesApp(HINSTANCE hinstance);
		virtual ~ShapesApp();

		virtual bool Init();
		virtual void OnResize();

		virtual void UpdateScene(float dt);
		virtual void DrawScene();

	private:
		bool CreateShaders();
		bool CreateInputLayout();
		bool CreateMeshAndModels();
		bool CreateLights();
		bool CreateBuffers();

	private:
		ComPtr<ID3D11Buffer> vertexBuffer;
		ComPtr<ID3D11Buffer> indexBuffer;
		ComPtr<ID3D11Buffer> constantBuffer;
		ComPtr<ID3D11Buffer> lightConstantBuffer;
		ComPtr<ID3D11RasterizerState> cullFrontState;
		ComPtr<ID3D11VertexShader> vertexShader;
		ComPtr<ID3D11PixelShader> pixelShader;
		ComPtr<ID3D11VertexShader> outlineVertexShader;
		ComPtr<ID3D11PixelShader> outlinePixelShader;
		ComPtr<ID3D11InputLayout> inputLayout;
		
		CompiledShader compiledVS;
		CompiledShader compiledPS;
		CompiledShader compiledOPS;
		CompiledShader compiledOVS;

		GeometryBufferData geometryBufferData;
		std::vector<Model> models;

		LightConstantBuffer lightBufferData;
		Material sphereMaterial;
		Material gridMaterial;
		Material frustumMaterial;
	};
}