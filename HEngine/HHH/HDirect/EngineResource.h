#pragma once
#include <unordered_map>
#include <iostream>
#include <memory>
#include "EngineHelper/HString.h"
#include <cassert>
// Ό³Έν :
template<typename ResType>
class EngineResource
{
public:
	// constrcuter destructer
	EngineResource() = default;
	virtual ~EngineResource() = default;

	// delete Function
	EngineResource(const EngineResource& _Other) = delete;
	EngineResource(EngineResource&& _Other) noexcept = delete;
	EngineResource& operator=(const EngineResource& _Other) = delete;
	EngineResource& operator=(EngineResource&& _Other) noexcept = delete;


	static std::shared_ptr<ResType> GetResource(std::string_view _str) {
		std::string str = HString::Upper(_str.data());

		if (Resources.contains(str) == false) {
			assert(false);
		}

		return Resources[str];
	}

	static void SetResource(std::shared_ptr<ResType> _res, std::string_view _str) {
		std::string str = HString::Upper(_str.data());
		Resources[str] = _res;
	}

protected:
	static std::unordered_map<std::string, std::shared_ptr<ResType>> Resources;

private:

};

template<typename ResType>
std::unordered_map<std::string, std::shared_ptr<ResType>> EngineResource<ResType>::Resources;
