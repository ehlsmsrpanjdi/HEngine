#pragma once
#include <d3d11.h>

class DeviceContext;

class IndexBuffer
{
public:
	IndexBuffer();
	bool load(void* list_vertices, UINT size_index, UINT size_list);
	UINT getSizeVertexList();
	bool release();
	~IndexBuffer();
private:
	UINT m_size_index = 0;
	UINT m_size_list = 0;
private:
	ID3D11Buffer* m_buffer = nullptr;
private:
	friend class DeviceContext;
};