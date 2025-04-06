#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <d3d11.h>
#include <sstream>


// DXGI_FORMAT�� ũ�⿡ ���� �����ϴ� �Լ�
DXGI_FORMAT GetDXGIFormatFromType(const std::string& type)
{
	if (type == "float" || type == "half") return DXGI_FORMAT_R32_FLOAT;
	if (type == "float2") return DXGI_FORMAT_R32G32_FLOAT;
	if (type == "float3") return DXGI_FORMAT_R32G32B32_FLOAT;
	if (type == "float4") return DXGI_FORMAT_R32G32B32A32_FLOAT;
	return DXGI_FORMAT_UNKNOWN;
}

int SetOffset(DXGI_FORMAT _format) {
	switch (_format) {
	case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16; // 4 * 4����Ʈ
	case DXGI_FORMAT_R32G32B32_FLOAT:    return 12; // 3 * 4����Ʈ
	case DXGI_FORMAT_R32G32_FLOAT:       return 8;  // 2 * 4����Ʈ
	case DXGI_FORMAT_R32_FLOAT:          return 4;  // 1 * 4����Ʈ
	case DXGI_FORMAT_R16G16B16A16_FLOAT: return 8;  // 4 * 2����Ʈ
	case DXGI_FORMAT_R8G8B8A8_UNORM:     return 4;  // 4 * 1����Ʈ
	case DXGI_FORMAT_R10G10B10A2_UNORM:  return 4;  // Ư���� 10/10/10/2 ��Ʈ
	}
}

struct InputElement
{
	std::string semanticName;
	DXGI_FORMAT format;
	int offset = 0;

	InputElement(const std::string& type, const std::string& name) {
		semanticName = name;
		if (semanticName == "POSITION") {
			format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else {
			format = GetDXGIFormatFromType(type);
		}
		offset = SetOffset(format);
	}
};



// HLSL ���Ͽ��� Input Layout ���� ����
std::vector<InputElement> ParseHLSLForInputLayout(const std::string& filename)
{
	std::ifstream file(filename);
	std::vector<InputElement> inputElements;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("struct VS_INPUT") != std::string::npos) // VS_INPUT ����ü ã��
		{
			while (std::getline(file, line) && line.find("};") == std::string::npos) // ����ü ���� ������ �б�
			{
				std::vector<std::string> elements;
				if (line == "{") continue;
				std::istringstream iss(line);
				std::string word;
				while (iss >> word)
				{
					if (std::string::npos != word.find(';')) {
						word.erase(word.find(';'));
					}
					elements.push_back(word);
				}
				inputElements.emplace_back(elements[0], *(elements.end() - 1)); // type, name
			}
			return inputElements;
		}
	}

	return inputElements;
}

// D3D11_INPUT_ELEMENT_DESC �ڵ� ����
std::vector<D3D11_INPUT_ELEMENT_DESC> CreateInputLayoutFromHLSL(const std::vector<InputElement>& inputElements)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	int offset = 0;
	for (size_t i = 0; i < inputElements.size(); i++)
	{
		D3D11_INPUT_ELEMENT_DESC desc = {};
		desc.SemanticName = inputElements[i].semanticName.c_str();
		desc.SemanticIndex = 0;
		desc.Format = inputElements[i].format;
		desc.InputSlot = 0;
		desc.AlignedByteOffset = offset;
		desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc.InstanceDataStepRate = 0;
		layout.push_back(desc);

		offset += inputElements[i].offset;
	}

	return layout;
}
//
//int main()
//{
//    std::string hlslFile = "shader.hlsl";
//    std::vector<InputElement> inputElements = ParseHLSLForInputLayout(hlslFile);
//    std::vector<D3D11_INPUT_ELEMENT_DESC> layout = CreateInputLayoutFromHLSL(inputElements);
//
//    // Ȯ�� ���
//    for (const auto& desc : layout)
//    {
//        std::cout << "Semantic: " << desc.SemanticName << " | Format: " << desc.Format << std::endl;
//    }
//
//    return 0;
//}
