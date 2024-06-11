#pragma once
#ifndef __HEADER_TOOL__
#include "core/reflection/reflection.h"
#include "runtime/ecs/entt.h"
#include "yr/api/yr_entity.h"
#include "yr/parse/ini_reader.h"
#include <entt/meta/factory.hpp>
using namespace entt::literals;

struct IniComponentLoader
{
    template<typename T>
    static bool Load(IniReader& parser, const char* pSection, const char* pKey, T& value, bool allocate = false)
    {
        if constexpr (IsParserImplemented<T>)
        {
            return parser.Read(pSection, pKey, value);
        }
        else
        {
            auto func = entt::resolve<T>().func("LoadIniComponent"_hs);
            return func.invoke(value, parser, pSection).cast<bool>();
        }
    }

    template<typename Type, bool (*Func)(Type&, IniReader&, const char*)>
    void Register(entt::meta_factory<Type>& factory)
    {
        factory.func<Func>("LoadIniComponent"_hs).prop("name"_hs, "LoadIniComponent");
    }

    template<typename TargetType, typename Type>
    void CreateComponent(entt::registry& reg, entt::entity entity)
    {
        reg.emplace<Type>();
    }

    template<typename TargetType, typename Type>
    void LoadComponent(TargetType* pObject, CCINIClass* pIni)
    {
        Type* pCom = GetYrComponent<Type>(pObject);
        const char* pID  = pObject->ID;
        IniReader reader {pIni};
        IniComponentLoader::Load(reader, pID, nullptr, *pCom);
    }
};

template<typename Type, typename... TargetType>
void RegisterIniComponentLoader()
{
    if constexpr (sizeof...(TargetType) == 1u)
    {
        (gEntt->on_construct<YrEntityComponent<TargetType>>().template connect<&IniComponentLoader::CreateComponent<TargetType, Type>>(), ...);
    }
    else
    {
        (RegisterIniComponentLoader<Type, TargetType>(), ...);
    }
}
#endif
