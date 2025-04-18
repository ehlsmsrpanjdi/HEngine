#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <d3d11.h>
#include <sstream>


// DXGI_FORMAT을 크기에 따라 매핑하는 함수
DXGI_FORMAT GetDXGIFormatFromType(const std::string& type)
{
	if (type == "float") return DXGI_FORMAT_R32_FLOAT;
	if (type == "float2") return DXGI_FORMAT_R32G32_FLOAT;
	if (type == "float3") return DXGI_FORMAT_R32G32B32_FLOAT;
	if (type == "float4") return DXGI_FORMAT_R32G32B32A32_FLOAT;

	if (type == "uint")      return DXGI_FORMAT_R32_UINT;
	if (type == "uint2")     return DXGI_FORMAT_R32G32_UINT;
	if (type == "uint3")     return DXGI_FORMAT_R32G32B32_UINT;
	if (type == "uint4")     return DXGI_FORMAT_R32G32B32A32_UINT;

	return DXGI_FORMAT_UNKNOWN;
}

int SetOffset(DXGI_FORMAT _format) {
	switch (_format) {
	case DXGI_FORMAT_R32G32B32A32_FLOAT: return 16; // 4 * 4바이트
	case DXGI_FORMAT_R32G32B32_FLOAT:    return 12; // 3 * 4바이트
	case DXGI_FORMAT_R32G32_FLOAT:       return 8;  // 2 * 4바이트
	case DXGI_FORMAT_R32_FLOAT:          return 4;  // 1 * 4바이트

	case DXGI_FORMAT_R16G16B16A16_FLOAT: return 8;  // 4 * 2바이트
	case DXGI_FORMAT_R16G16_FLOAT:       return 4;  // 2 * 2바이트
	case DXGI_FORMAT_R16_FLOAT:          return 2;  // 1 * 2바이트

	case DXGI_FORMAT_R8G8B8A8_UNORM:     return 4;  // 4 * 1바이트
	case DXGI_FORMAT_R8G8_UNORM:         return 2;  // 2 * 1바이트
	case DXGI_FORMAT_R8_UNORM:           return 1;  // 1 * 1바이트

	case DXGI_FORMAT_R10G10B10A2_UNORM:  return 4;  // 10/10/10/2비트 합쳐서 32비트

	case DXGI_FORMAT_R32_UINT:           return 4;
	case DXGI_FORMAT_R32_SINT:           return 4;
	case DXGI_FORMAT_R32G32_UINT:        return 8;
	case DXGI_FORMAT_R32G32_SINT:        return 8;
	case DXGI_FORMAT_R32G32B32_UINT:     return 12;
	case DXGI_FORMAT_R32G32B32_SINT:     return 12;
	case DXGI_FORMAT_R32G32B32A32_UINT:  return 16;
	case DXGI_FORMAT_R32G32B32A32_SINT:  return 16;

	case DXGI_FORMAT_R16_UINT:           return 2;
	case DXGI_FORMAT_R16_SINT:           return 2;
	case DXGI_FORMAT_R16G16_UINT:        return 4;
	case DXGI_FORMAT_R16G16_SINT:        return 4;
	case DXGI_FORMAT_R16G16B16A16_UINT:  return 8;
	case DXGI_FORMAT_R16G16B16A16_SINT:  return 8;

	case DXGI_FORMAT_R8_UINT:            return 1;
	case DXGI_FORMAT_R8_SINT:            return 1;
	case DXGI_FORMAT_R8G8_UINT:          return 2;
	case DXGI_FORMAT_R8G8_SINT:          return 2;
	case DXGI_FORMAT_R8G8B8A8_UINT:      return 4;
	case DXGI_FORMAT_R8G8B8A8_SINT:      return 4;
	}
	return -1;
}

struct InputElement
{
	std::string semanticName;
	DXGI_FORMAT format;
	int offset = 0;

	InputElement(const std::string& type, const std::string& name) {
		std::string resolvedType = type;
		if (name.find("[4]") != std::string::npos) {
			if (type == "float") resolvedType = "float4";
			else if (type == "uint") resolvedType = "uint4";
		}


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



// HLSL 파일에서 Input Layout 정보 추출
std::vector<InputElement> ParseHLSLForInputLayout(const std::string& filename)
{
	std::ifstream file(filename);
	std::vector<InputElement> inputElements;
	std::string line;

	while (std::getline(file, line))
	{
		if (line.find("struct VS_INPUT") != std::string::npos) // VS_INPUT 구조체 찾기
		{
			while (std::getline(file, line) && line.find("};") == std::string::npos) // 구조체 끝날 때까지 읽기
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

// D3D11_INPUT_ELEMENT_DESC 자동 생성
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
