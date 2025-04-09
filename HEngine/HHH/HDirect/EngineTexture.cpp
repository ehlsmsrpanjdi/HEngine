#include "EngineTexture.h"
#include <d3d11.h>
#include <wrl.h>
#include <wincodec.h>
#include <vector>
#include <cassert>
#include "EngineHelper/AllStruct.h"
#include "EngineHelper/EngineFile.h"

EngineTexture::EngineTexture()
{
}

EngineTexture::~EngineTexture()
{
	TextureMap.clear();
}

void EngineTexture::CreateAllTexture(ID3D11Device* device, ID3D11DeviceContext* context, std::shared_ptr<EngineFile> _filemanager)
{
	const std::map<std::string, std::string>& pngFiles = _filemanager->GetAllFile("png");
	for (const std::pair<const std::string, std::string>& pa : pngFiles)
	{
		std::wstring ws = HString::StoWC(pa.second);
		const WCHAR* wcc = ws.c_str();
		CreateTexture(device, context, wcc, HString::Upper(pa.first));
	}

	const std::map<std::string, std::string>& jpgFiles = _filemanager->GetAllFile("jpg");
	for (const std::pair<const std::string, std::string>& pa : jpgFiles)
	{
		std::wstring ws = HString::StoWC(pa.second);
		const WCHAR* wcc = ws.c_str();
		CreateTexture(device, context, wcc, HString::Upper(pa.first));
	}
}

void EngineTexture::CreateTexture(ID3D11Device* device, ID3D11DeviceContext* context, const wchar_t* filename, std::string_view _Origin)
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> wicFactory;
	HRESULT hr;
	hr = CoInitialize(nullptr);
	hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
	wicFactory->CreateDecoderFromFilename(filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
	decoder->GetFrame(0, &frame);

	Microsoft::WRL::ComPtr<IWICFormatConverter> converter;
	wicFactory->CreateFormatConverter(&converter);
	converter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, nullptr, 0, WICBitmapPaletteTypeCustom);

	UINT width, height;
	frame->GetSize(&width, &height);

	std::vector<UINT8> imageData(width * height * 4);
	converter->CopyPixels(nullptr, width * 4, imageData.size(), imageData.data());

	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = imageData.data();
	initData.SysMemPitch = width * 4;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	device->CreateTexture2D(&textureDesc, &initData, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	std::shared_ptr<Tex> tex = std::make_shared<Tex>();

	device->CreateShaderResourceView(texture.Get(), &srvDesc, &tex->textureSRV);

	TextureMap[_Origin.data()] = tex;

}


