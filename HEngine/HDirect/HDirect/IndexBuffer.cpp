#include "IndexBuffer.h"
#include "GraphicsEngine.h"
IndexBuffer::IndexBuffer()
{
}
bool IndexBuffer::load(void* list_vertices, UINT size_index, UINT size_list)
{

	D3D11_BUFFER_DESC idb;
	idb.Usage = D3D11_USAGE_IMMUTABLE;
	idb.ByteWidth = size_index * size_list;
	idb.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idb.CPUAccessFlags = 0;
	idb.MiscFlags = 0;
	idb.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iinitData = {};
	iinitData.pSysMem = list_vertices;

	m_size_index = size_index;
	m_size_list = size_list;

	GraphicsEngine::get()->m_d3d_device->CreateBuffer(&idb, &iinitData, &m_buffer);

	return true;
}

UINT IndexBuffer::getSizeVertexList()
{
	return 0;
}

bool IndexBuffer::release()
{
	m_buffer->Release();
	delete this;
	return true;
}

IndexBuffer::~IndexBuffer()
{
}
