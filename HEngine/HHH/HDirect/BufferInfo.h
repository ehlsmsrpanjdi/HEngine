#pragma once
#include <d3d11.h>

enum class BufferType
{
	VERTEX = 0,
	INDEX,
	CONSTANT,
	STRUCTURED,
	APPEND,
	COUNTER,
	INDIRECT,
	ARGUMENT
};

class BufferInfo
{
public:
	// constrcuter destructer
	BufferInfo() = delete;
	BufferInfo(BufferType _Type);
	~BufferInfo();

	// delete Function
	BufferInfo(const BufferInfo& _Other) = delete;
	BufferInfo(BufferInfo&& _Other) noexcept = delete;
	BufferInfo& operator=(const BufferInfo& _Other) = delete;
	BufferInfo& operator=(BufferInfo&& _Other) noexcept = delete;

	ID3DBlob* VSBlob;
	ID3DBlob* PSBlob;
	ID3DBlob* ErrorBlob;

	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;

	ID3D11Buffer* Buffer;

	ID3D11InputLayout* Layout;

	UINT List_Size;
	UINT Vertex_Size;

	BufferType Type;

protected:

private:
	friend class GraphicsEngine;
};

