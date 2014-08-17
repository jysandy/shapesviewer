#include "ShapesApp.h"

namespace ShapesDemo
{
	ShapesApp::ShapesApp(HINSTANCE hinstance) : D3DApp(hinstance), compiledVS("VertexShader.cso"), compiledPS("PixelShader.cso"), compiledOPS("OutlinePixelShader.cso"),
		compiledOVS("OutlineVertexShader.cso")
	{
		this->windowCaption = L"Shapes";
	}

	ShapesApp::~ShapesApp()
	{

	}

	bool ShapesApp::Init()
	{
		return D3DApp::Init()
			&& CreateShaders()
			&& CreateInputLayout()
			&& CreateMeshAndModels()
			&& CreateBuffers()
			&& CreateLights();
	}

	void ShapesApp::OnResize()
	{
		D3DApp::OnResize();


		D3D11_RASTERIZER_DESC rDesc;
		SecureZeroMemory(&rDesc, sizeof(rDesc));
		rDesc.FillMode = D3D11_FILL_SOLID;
		rDesc.CullMode = D3D11_CULL_FRONT;
		rDesc.DepthClipEnable = true;
		rDesc.MultisampleEnable = true;
		device->CreateRasterizerState(&rDesc, cullFrontState.address());
	}

	bool ShapesApp::CreateShaders()
	{
		using D3dtut::HR;
		HR(
			device->CreateVertexShader(compiledVS.get(), compiledVS.getSize(), nullptr, vertexShader.address()));

		HR(
			device->CreatePixelShader(compiledPS.get(), compiledPS.getSize(), nullptr, pixelShader.address()));

		HR(
			device->CreateVertexShader(compiledOVS.get(), compiledOVS.getSize(), nullptr, outlineVertexShader.address()));

		HR(
			device->CreatePixelShader(compiledOPS.get(), compiledOPS.getSize(), nullptr, outlinePixelShader.address()));

		return true;
	}

	bool ShapesApp::CreateInputLayout()
	{
		D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			},
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};

		HR(
			device->CreateInputLayout(vertexDesc, 4, compiledVS.get(), compiledVS.getSize(), inputLayout.address()));

		return true;
	}

	bool ShapesApp::CreateMeshAndModels()
	{
		using namespace DirectX;
		using D3dtut::MeshData;
		using D3dtut::MeshGenerator;

		const int gridWidth = 200;
		const int frustumHeight = 50;
		const int sphereRadius = 10;

		//Create a level grid, for the 'floor'.
		MeshData gridData = MeshGenerator::CreateGrid(gridWidth, gridWidth, 5, 5, DirectX::Colors::LawnGreen,
			[](float, float)
		{
			return 0.0f;
		}
		);

		MeshData frustumData = MeshGenerator::CreateFrustum(15, 20, 40, 32, frustumHeight, DirectX::Colors::Crimson);
		MeshData sphereData = MeshGenerator::CreateSphere(sphereRadius, 40, 32, DirectX::Colors::Navy);

		this->geometryBufferData.Append(gridData);
		this->geometryBufferData.Append(frustumData);
		this->geometryBufferData.Append(sphereData);

		float topLeftX = -gridWidth / 4.0f;
		float topLeftZ = gridWidth / 4.0f;

		//Create and position the grid model.
		Model grid = Model(gridData.getId(), "Grid");
		XMMATRIX foo = XMMatrixIdentity();
		XMStoreFloat4x4(&grid.world, foo);
		models.push_back(grid);

		//Create and position the frustum models.
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				Model frustum = Model(frustumData.getId(), "Frustum");
				XMMATRIX translation = XMMatrixTranslation(topLeftX + i * gridWidth / 2.0f, 0, topLeftZ - j * gridWidth / 2.0f);
				XMStoreFloat4x4(&frustum.world, translation);
				models.push_back(frustum);
			}
		}

		//Create and position the sphere models.
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				Model sphere = Model(sphereData.getId(), "Sphere");
				XMMATRIX translation = XMMatrixTranslation(topLeftX + i * gridWidth / 2.0f, frustumHeight + sphereRadius, topLeftZ - j * gridWidth / 2.0f);
				XMStoreFloat4x4(&sphere.world, translation);
				models.push_back(sphere);
			}
		}

		return true;
	}

	bool ShapesApp::CreateLights()
	{
		using namespace DirectX;

		sphereMaterial.ambient = XMFLOAT4(Colors::CadetBlue);
		sphereMaterial.diffuse = XMFLOAT4(Colors::CadetBlue);
		sphereMaterial.specular = XMFLOAT4(0.8, 0.8, 0.8, 96.0);

		frustumMaterial.ambient = XMFLOAT4(Colors::Crimson);
		frustumMaterial.diffuse = XMFLOAT4(Colors::Crimson);
		frustumMaterial.specular = XMFLOAT4(0.6, 0.6, 0.6, 64.0);

		gridMaterial.ambient = XMFLOAT4(Colors::LightSlateGray);
		gridMaterial.diffuse = XMFLOAT4(Colors::LightSlateGray);
		gridMaterial.specular = XMFLOAT4(0.2, 0.2, 0.2, 16.0f);

		DirectionalLight dLight;
		dLight.ambient = XMFLOAT4(0.2, 0.2, 0.2, 1.0);
		dLight.diffuse = XMFLOAT4(0.9, 0.9, 0.9, 1.0);
		dLight.specular = XMFLOAT4(0.3, 0.3, 0.3, 1.0);
		XMFLOAT3 dir = XMFLOAT3(1, -1, 1);
		XMVECTOR foo = XMLoadFloat3(&dir);
		foo = XMVector3Normalize(foo);
		XMStoreFloat3(&dLight.lightDirection, foo);
		lightBufferData.directionalLight = dLight;

		//Position and direction of spotlight and point light to be set dynamically.

		Spotlight sLight;
		sLight.ambient = XMFLOAT4(0, 0, 0, 1);
		sLight.diffuse = XMFLOAT4(Colors::Yellow);
		sLight.specular = XMFLOAT4(1, 1, 1, 1);
		sLight.spotlightPower = 384.0f;
		sLight.range = 10000.0f;
		sLight.attenuation = XMFLOAT3(0.1, 0, 0);
		lightBufferData.spotlight = sLight;

		//The point light revolves with a radius of 100 at a height of 40.
		PointLight pLight;
		pLight.ambient = XMFLOAT4(0.0, 0.0, 0.0, 1.0);
		pLight.diffuse = XMFLOAT4(0.9, 0.9, 0.9, 1.0);
		pLight.specular = pLight.diffuse;
		pLight.position = XMFLOAT3(100.0, 40.0, 0.0);
		pLight.range = 130.0f;
		pLight.attenuation = XMFLOAT3(0.9, 0, 0);
		lightBufferData.pointLight = pLight;

		return true;
	}

	bool ShapesApp::CreateBuffers()
	{
		//Create vertex buffer
		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = geometryBufferData.getVertices().size() * sizeof(D3dtut::Vertex);
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = geometryBufferData.getVertices().data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HR(
			device->CreateBuffer(&desc, &data, vertexBuffer.address()));

		//Create index buffer
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = geometryBufferData.getIndices().size() * sizeof(unsigned int);

		data.pSysMem = geometryBufferData.getIndices().data();

		HR(
			device->CreateBuffer(&desc, &data, indexBuffer.address()));

		//Create constant buffers
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(ConstantBufferData);
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HR(
			device->CreateBuffer(&desc, nullptr, constantBuffer.address()));

		desc.ByteWidth = sizeof(LightConstantBuffer);
		data.pSysMem = &lightBufferData;
		HR(
			device->CreateBuffer(&desc, &data, lightConstantBuffer.address()));

		return true;
	}

	void ShapesApp::UpdateScene(float dt)
	{
		using namespace DirectX;
		D3DApp::UpdateScene(dt);
		static float theta = 0.0f;
		//Time period of the revolving point light is 1 seconds.
		float omega = DirectX::XM_2PI / 3.0f;
		const float radius = 130.0;

		theta += omega * dt;

		lightBufferData.pointLight.position.x = radius * std::cosf(theta);
		lightBufferData.pointLight.position.z = radius * std::sinf(theta);

		lightBufferData.spotlight.position = camera.getCameraPosition();
		lightBufferData.spotlight.direction = camera.getCameraDirection();

		lightBufferData.cameraPosition = camera.getCameraPosition();
	}

	void ShapesApp::DrawScene()
	{
		assert(sizeof(LightConstantBuffer) % 16 == 0);
		assert(sizeof(DirectionalLight) % 16 == 0);
		assert(sizeof(PointLight) % 16 == 0);
		assert(sizeof(Spotlight) % 16 == 0);
		assert(sizeof(Material) % 16 == 0);

		deviceContext->ClearRenderTargetView(renderTargetView.get(), DirectX::Colors::LightCyan);
		deviceContext->ClearDepthStencilView(depthStencilView.get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		deviceContext->IASetInputLayout(inputLayout.get());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		unsigned int stride = sizeof(D3dtut::Vertex);
		unsigned int offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.address(), &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.get(), DXGI_FORMAT_R32_UINT, 0);


		for (auto& model : models)
		{
			auto indexData = geometryBufferData.getIndexOffsetData();
			auto modelIndexData = indexData[model.getId()];

			D3D11_MAPPED_SUBRESOURCE mappedResource;
			deviceContext->Map(constantBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			ConstantBufferData *bufferData = reinterpret_cast<ConstantBufferData*>(mappedResource.pData);
			bufferData->SetWorldMatrix(model.world);
			bufferData->SetViewMatrix(camera.getViewMatrix());
			bufferData->SetProjectionMatrix(projectionMatrix);
			deviceContext->Unmap(constantBuffer.get(), 0);

			//Drawing outlines for cel shading.
			/*
			deviceContext->VSSetShader(outlineVertexShader.get(), nullptr, 0);
			deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.address());
			deviceContext->PSSetShader(outlinePixelShader.get(), nullptr, 0);
			deviceContext->RSSetState(cullFrontState.get());
			deviceContext->DrawIndexed(modelIndexData.indexCount, modelIndexData.baseIndexOffset, modelIndexData.baseVertexOffset);
			*/
			if (model.name == "Grid")
			{
				lightBufferData.material = gridMaterial;
			}
			else if (model.name == "Sphere")
			{
				lightBufferData.material = sphereMaterial;
			}
			else if (model.name == "Frustum")
			{
				lightBufferData.material = frustumMaterial;
			}


			deviceContext->Map(lightConstantBuffer.get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			LightConstantBuffer *lightData = reinterpret_cast<LightConstantBuffer*>(mappedResource.pData);
			*lightData = lightBufferData;
			deviceContext->Unmap(lightConstantBuffer.get(), 0);

			deviceContext->PSSetShader(pixelShader.get(), nullptr, 0);
			deviceContext->PSSetConstantBuffers(1, 1, lightConstantBuffer.address());
			deviceContext->VSSetShader(vertexShader.get(), nullptr, 0);
			deviceContext->VSSetConstantBuffers(0, 1, constantBuffer.address());
			deviceContext->RSSetState(nullptr);
			deviceContext->DrawIndexed(modelIndexData.indexCount, modelIndexData.baseIndexOffset, modelIndexData.baseVertexOffset);
		}

		HR(swapChain->Present(1, 0));
	}
}