#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
class DeviceContext;

struct Vertex1 {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct Vertex2 {
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 Tex0;
	DirectX::XMFLOAT2 Tex1;
};


class VertexBuffer
{
public:
	VertexBuffer();
	bool load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, size_t size_byte_shader);
	UINT getSizeVertexList();
	bool release();
	~VertexBuffer();
private:
	UINT m_size_vertex;
	UINT m_size_list;
private:
	ID3D11Buffer* m_buffer;
	ID3D11InputLayout* m_layout;
private:
	friend class DeviceContext;
};