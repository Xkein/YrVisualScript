#pragma once
#ifndef __HEADER_TOOL__
#include <ScriptBackend.hpp>
#include <DataTransfer.h>
#include <PuertsNamespaceDef.h>
#include <runtime/logger/logger.h>
#include <core/macro.h>
#include <AbstractClass.h>
#include <AbstractTypeClass.h>
#include <Helpers/String.h>
#include <Helpers/Template.h>

#define UsingArray(CLS)                                                                             \
namespace PUERTS_NAMESPACE                                                                          \
    {                                                                                               \
    template<size_t Size>                                                                           \
    struct ScriptTypeName<CLS[Size]>                                                                \
    {                                                                                               \
        static constexpr auto value()                                                               \
        {                                                                                           \
            return ScriptTypeNameWithNamespace<CLS>::value() + internal::Literal("[]");             \
        }                                                                                           \
    };                                                                                              \
    }                                                                                               \
    namespace PUERTS_NAMESPACE                                                                      \
    {                                                                                               \
    namespace v8_impl                                                                               \
    {                                                                                               \
        template<size_t Size>                                                                       \
        struct Converter<CLS[Size]>                                                                 \
        {                                                                                           \
            using data_type = CLS[Size];                                                            \
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, data_type& value)  \
            {                                                                                       \
                return DataTransfer::NewArrayBuffer(context, &(value[0]), sizeof(data_type));       \
            }                                                                                       \
                                                                                                    \
            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)   \
            {                                                                                       \
                if (value->IsArrayBufferView())                                                     \
                {                                                                                   \
                    v8::Local<v8::ArrayBufferView> buffView = value.As<v8::ArrayBufferView>();      \
                    return buffView->ByteLength() >= sizeof(data_type);                             \
                }                                                                                   \
                if (value->IsArrayBuffer())                                                         \
                {                                                                                   \
                    auto   ab = v8::Local<v8::ArrayBuffer>::Cast(value);                            \
                    size_t byteLength;                                                              \
                    (void)(DataTransfer::GetArrayBufferData(ab, byteLength));                       \
                    return byteLength >= sizeof(data_type);                                         \
                }                                                                                   \
                return false;                                                                       \
            }                                                                                       \
        };                                                                                          \
    }                                                                                               \
    }

#define UsingArray2D(CLS)                                                                           \
namespace PUERTS_NAMESPACE                                                                          \
    {                                                                                               \
    template<size_t X, size_t Y>                                                                    \
    struct ScriptTypeName<CLS[Y][X]>                                                                \
    {                                                                                               \
        static constexpr auto value()                                                               \
        {                                                                                           \
            return ScriptTypeNameWithNamespace<CLS>::value() + internal::Literal("[][]");           \
        }                                                                                           \
    };                                                                                              \
    }                                                                                               \
    namespace PUERTS_NAMESPACE                                                                      \
    {                                                                                               \
    namespace v8_impl                                                                               \
    {                                                                                               \
        template<size_t X, size_t Y>                                                                \
        struct Converter<CLS[Y][X]>                                                                 \
        {                                                                                           \
            using data_type = CLS[Y][X];                                                            \
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, data_type& value)  \
            {                                                                                       \
                return DataTransfer::NewArrayBuffer(context, &(value[0]), sizeof(data_type));       \
            }                                                                                       \
                                                                                                    \
            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)   \
            {                                                                                       \
                if (value->IsArrayBufferView())                                                     \
                {                                                                                   \
                    v8::Local<v8::ArrayBufferView> buffView = value.As<v8::ArrayBufferView>();      \
                    return buffView->ByteLength() >= sizeof(data_type);                             \
                }                                                                                   \
                if (value->IsArrayBuffer())                                                         \
                {                                                                                   \
                    auto   ab = v8::Local<v8::ArrayBuffer>::Cast(value);                            \
                    size_t byteLength;                                                              \
                    (void)(DataTransfer::GetArrayBufferData(ab, byteLength));                       \
                    return byteLength >= sizeof(data_type);                                         \
                }                                                                                   \
                return false;                                                                       \
            }                                                                                       \
        };                                                                                          \
    }                                                                                               \
    }

#define UsingPointer(PTR) __DefScriptTTypeName(PTR, PTR) __DefObjectType(PTR)                     \
    namespace PUERTS_NAMESPACE                                                                    \
    {                                                                                             \
    namespace v8_impl                                                                             \
    {                                                                                             \
    template <>                                                                                   \
    struct Converter<PTR>                                                                         \
    {                                                                                             \
        static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, PTR value)           \
        {                                                                                         \
            return API::GetUndefined(context);                                                    \
        }                                                                                         \
        static PTR toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)       \
        {                                                                                         \
            return nullptr;                                                                       \
        }                                                                                         \
        static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)     \
        {                                                                                         \
            return true;                                                                          \
        }                                                                                         \
    };                                                                                            \
    }                                                                                             \
    }

#define UsingYrClass(CLS)                                   \
    namespace PUERTS_NAMESPACE                              \
    {                                                       \
        template<>                                          \
        struct ScriptTypeName<CLS>                          \
        {                                                   \
            static constexpr auto value()                   \
            {                                               \
                return internal::Literal(#CLS);             \
            }                                               \
        };                                                  \
    }                                                       \
    namespace PUERTS_NAMESPACE                              \
    {                                                       \
        template<>                                          \
        struct is_uetype<CLS> : public std::true_type       \
        {};                                                 \
    }

#define UsingYrStruct(CLS) UsingCppType(CLS) UsingArray(CLS)

#define UsingContainer(CLS) __DefObjectType(CLS) __DefCDataPointerConverter(CLS)

#define __UsingIndexClass(TKEY, TVALUE, TYPEDEF) \
typedef IndexClass<TKEY, TVALUE> TYPEDEF; \
UsingContainer(TYPEDEF)

#define UsingIndexClass(TKEY, TVALUE) __UsingIndexClass(TKEY, TVALUE, CONCAT(__IndexClass__, CONCAT(__LINE__, __COUNTER__)));

#define UsingVectorClass(CLS) UsingContainer(VectorClass<CLS>)
#define UsingDynamicVectorClass(CLS) UsingVectorClass(CLS) UsingContainer(DynamicVectorClass<CLS>)
#define UsingTypeList(CLS) UsingDynamicVectorClass(CLS) UsingContainer(TypeList<CLS>)

#define RegisterVectorClass(CLS) \
    PUERTS_NAMESPACE::DefineClass<VectorClass<CLS>>() \
        /*.Method("SetCapacity", MakeFunction(&VectorClass<CLS>::SetCapacity))*/ \
        .Method("Clear", MakeFunction(&VectorClass<CLS>::Clear)) \
        /*.Method("FindItemIndex", MakeFunction(&VectorClass<CLS>::FindItemIndex))*/ \
        /*.Method("GetItemIndex", MakeFunction(&VectorClass<CLS>::GetItemIndex))*/ \
        .Method("GetItem", MakeFunction(&VectorClass<CLS>::GetItem)) \
        .Method("Reserve", MakeFunction(&VectorClass<CLS>::Reserve)) \
        .Property("Capacity", MakeProperty(&VectorClass<CLS>::Capacity)) \
        .Register()

#define RegisterDynamicVectorClass(CLS) \
    RegisterVectorClass(CLS); \
    PUERTS_NAMESPACE::DefineClass<DynamicVectorClass<CLS>>() \
        .Extends<VectorClass<CLS>>() \
        /*.Method("SetCapacity", MakeFunction(&DynamicVectorClass<CLS>::SetCapacity))*/ \
        .Method("ValidIndex", MakeFunction(&DynamicVectorClass<CLS>::ValidIndex)) \
        .Method("GetItemOrDefault", SelectFunction(CLS (DynamicVectorClass<CLS>::*)(int) const, &DynamicVectorClass<CLS>::GetItemOrDefault)) \
        .Method("GetItemOrDefault", SelectFunction(CLS (DynamicVectorClass<CLS>::*)(int, CLS) const, &DynamicVectorClass<CLS>::GetItemOrDefault)) \
        .Method("AddItem", MakeFunction(&DynamicVectorClass<CLS>::AddItem)) \
        .Method("AddUnique", MakeFunction(&DynamicVectorClass<CLS>::AddUnique)) \
        .Method("RemoveItem", MakeFunction(&DynamicVectorClass<CLS>::RemoveItem)) \
        .Method("Remove", MakeFunction(&DynamicVectorClass<CLS>::Remove)) \
        .Property("Count", MakeProperty(&DynamicVectorClass<CLS>::Count)) \
        .Register()

#define RegisterTypeList(CLS) \
    RegisterDynamicVectorClass(CLS); \
    PUERTS_NAMESPACE::DefineClass<TypeList<CLS>>() \
        .Extends<DynamicVectorClass<CLS>>() \
        .Register()

#define RegisterIndexClass(TKEY, TVALUE) {\
    typedef IndexClass<TKEY, TVALUE> __IndexClass; \
    PUERTS_NAMESPACE::DefineClass<__IndexClass>() \
        .Method("AddIndex", SelectFunction(bool (__IndexClass::*)(TKEY, TVALUE&&), &__IndexClass::AddIndex)) \
        .Method("RemoveIndex", MakeFunction(&__IndexClass::RemoveIndex)) \
        .Method("IsPresent", MakeFunction(&__IndexClass::IsPresent)) \
        .Method("Count", MakeFunction(&__IndexClass::Count)) \
        .Method("FetchIndex", SelectFunction(TVALUE& (__IndexClass::*)(TKEY), &__IndexClass::FetchIndex)) \
        .Method("Clear", MakeFunction(&__IndexClass::Clear)) \
        .Method("Reverse", MakeFunction(&__IndexClass::Reverse)) \
        .Method("Sort", MakeFunction(&__IndexClass::Sort)) \
        .Register(); }

namespace PUERTS_NAMESPACE
{
    template<typename API, typename Ret, unsigned int Address, const constant_ptr<Ret, Address>* Variable>
    struct PropertyWrapper<API, const constant_ptr<Ret, Address>*, Variable>
    {
        template<typename T>
        using DecayTypeConverter = typename API::template Converter<typename internal::ConverterDecay<T>::type>;

        static void getter(typename API::CallbackInfoType info)
        {
            auto context = API::GetContext(info);
            API::SetReturn(info, DecayTypeConverter<Ret*>::toScript(context, Variable->get()));
        }

        static void setter(typename API::CallbackInfoType info)
        {
            gLogger->error("{} could not be assign!", typeid(constant_ptr<Ret, Address>).name());
        }

        static const CTypeInfo* info()
        {
            return CTypeInfoImpl<Ret*, false>::get();
        }
    };

    template<typename API, typename Ret, unsigned int Address, size_t Count, const reference<Ret, Address, Count>* Variable>
    struct PropertyWrapper<API, const reference<Ret, Address, Count>*, Variable>
    {
        template<typename T>
        using DecayTypeConverter = typename API::template Converter<typename internal::ConverterDecay<T>::type>;

        static void getter(typename API::CallbackInfoType info)
        {
            auto context = API::GetContext(info);
            if constexpr (Count > 1)
            {
                API::SetReturn(info, typename API::template Converter<Ret[Count]>::toScript(context, Variable->get()));
            }
            else if constexpr (std::is_pointer_v<Ret>)
            {
                API::SetReturn(info, DecayTypeConverter<Ret>::toScript(context, Variable->get()));
            }
            else
            {
                API::SetReturn(info, DecayTypeConverter<Ret*>::toScript(context, &Variable->get()));
            }
        }

        static void setter(typename API::CallbackInfoType info)
        {
            if constexpr (std::is_move_assignable_v<Ret>)
            {
                auto context = API::GetContext(info);
                Variable->get() = DecayTypeConverter<Ret>::toCpp(context, API::GetArg(info, 0));
            }
            else
            {
                gLogger->error("{} could not be assign!", typeid(reference<Ret, Address, Count>).name());
            }
        }

        static const CTypeInfo* info()
        {
            return CTypeInfoImpl<Ret, false>::get();
        }
    };

    namespace v8_impl
    {
        template<typename T, unsigned int Address>
        struct Converter<constant_ptr<T, Address>>
        {
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, const constant_ptr<T, Address>& value)
            {
                return Converter<decltype(constant_ptr<T, Address>().get())>::toScript(context, value.get());
            }

            static decltype(auto) toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return constant_ptr<T, Address>().get();
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return Converter<decltype(constant_ptr<T, Address>().get())>::accept(context, value);
            }
        };
        template<typename T, unsigned int Address, size_t Count>
        struct Converter<reference<T, Address, Count>>
        {
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, const reference<T, Address, Count>& value)
            {
                return Converter<decltype(reference<T, Address, Count>().get())>::toScript(context, value.get());
            }

            static decltype(auto) toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return reference<T, Address, Count>().get();
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return Converter<decltype(reference<T, Address, Count>().get())>::accept(context, value);
            }
        };

        template<size_t Capacity, typename T>
        struct Converter<FixedString<Capacity, T>>
        {
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, const FixedString<Capacity, T>& value)
            {
                return v8::String::NewFromUtf8(context->GetIsolate(), static_cast<const T*>(value), v8::NewStringType::kNormal).ToLocalChecked();
            }

            static FixedString<Capacity, T> toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return FixedString<Capacity, T>(*v8::String::Utf8Value(context->GetIsolate(), value));
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return value->IsString();
            }
        };

        template<size_t Capacity>
        struct Converter<char[Capacity]>
        {
            using data_type = char[Capacity];
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, const data_type& value)
            {
                return Converter<const char*>::toScript(context, value);
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return value->IsString();
            }
        };

        template<>
        struct Converter<LARGE_INTEGER>
        {
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, LARGE_INTEGER value)
            {
                return Converter<decltype(LARGE_INTEGER::QuadPart)>::toScript(context, value.QuadPart);
            }

            static LARGE_INTEGER toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                LARGE_INTEGER ret;
                ret.QuadPart = Converter<decltype(LARGE_INTEGER::QuadPart)>::toCpp(context, value);
                return ret;
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return Converter<decltype(LARGE_INTEGER::QuadPart)>::accept(context, value);
            }
        };

        template<typename _Interface, const IID* _IID /*= &__uuidof(_Interface)*/>
        struct Converter<_COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<_Interface, _IID>>>
        {
            using com_ptr = _COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<_Interface, _IID>>;
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, const com_ptr& value)
            {
                return Converter<_Interface*>::toScript(context, value.GetInterfacePtr());
            }

            static com_ptr toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return Converter<_Interface*>::toCpp(context, value);
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                return Converter<_Interface*>::accept(context, value);
            }
        };

        inline v8::Local<v8::Value> FindOrAddYrObject(v8::Isolate* InIsolate, v8::Local<v8::Context>& Context, AbstractClass* YrObject, bool SkipTypeScriptInitial);
        template<typename T>
        struct Converter<T*, typename std::enable_if<std::is_convertible<T*, const AbstractClass*>::value>::type>
        {
            static v8::Local<v8::Value> toScript(v8::Local<v8::Context> context, T* value)
            {
                using TypeWithoutConst = typename std::remove_const<T>::type;
                return FindOrAddYrObject(context->GetIsolate(), context, (TypeWithoutConst*)(value), true);
            }

            static T* toCpp(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                T* Ret = DataTransfer::GetPointerFast<T>(value.As<v8::Object>());
                return Ret;
            }

            static bool accept(v8::Local<v8::Context> context, const v8::Local<v8::Value>& value)
            {
                if (value.As<v8::Object>()->IsNullOrUndefined())
                    return true;
                return DataTransfer::IsInstanceOf(context->GetIsolate(), T::StaticClass(), value.As<v8::Object>());
            }
        };
    } // namespace v8_impl
    

    template<typename T>
    struct ScriptTypeName<VectorClass<T>>
    {
        static constexpr auto value()
        {
            return internal::Literal("VectorClass<") + ScriptTypeNameWithNamespace<T>::value() + internal::Literal(">");
        }
    };

    template<typename T>
    struct ScriptTypeName<DynamicVectorClass<T>>
    {
        static constexpr auto value()
        {
            return internal::Literal("DynamicVectorClass<") + ScriptTypeNameWithNamespace<T>::value() + internal::Literal(">");
        }
    };

    template<typename T>
    struct ScriptTypeName<TypeList<T>>
    {
        static constexpr auto value()
        {
            return internal::Literal("TypeList<") + ScriptTypeNameWithNamespace<T>::value() + internal::Literal(">");
        }
    };

    template<typename T>
    struct ScriptTypeName<IndexBitfield<T>>
    {
        static constexpr auto value()
        {
            return internal::Literal("IndexBitfield<") + ScriptTypeNameWithNamespace<T>::value() + internal::Literal(">");
        }
    };

    template<typename TKey, typename TValue>
    struct ScriptTypeName<IndexClass<TKey, TValue>>
    {
        static constexpr auto value()
        {
            return internal::Literal("IndexClass<") + ScriptTypeNameWithNamespace<TKey>::value() + internal::Literal(", ") + ScriptTypeNameWithNamespace<TValue>::value() + internal::Literal(">");
        }
    };

    template<size_t Capacity, typename T>
    struct ScriptTypeName<FixedString<Capacity, T>>
    {
        static constexpr auto value()
        {
            return internal::Literal("FixedString<") + internal::Literal("N, ") + ScriptTypeNameWithNamespace<T>::value() + internal::Literal(">");
        }
    };

    template<>
    struct ScriptTypeName<LARGE_INTEGER>
    {
        static constexpr auto value()
        {
            return internal::Literal("LARGE_INTEGER");
        }
    };

    template<>
    struct ScriptTypeName<CounterClass>
    {
        static constexpr auto value()
        {
            return internal::Literal("CounterClass");
        }
    };

    template<typename _Interface, const IID* _IID /*= &__uuidof(_Interface)*/>
    struct ScriptTypeName<_COM_SMARTPTR<_COM_SMARTPTR_LEVEL2<_Interface, _IID>>>
    {
        static constexpr auto value()
        {
            return internal::Literal("com_ptr<") + ScriptTypeNameWithNamespace<_Interface>::value() + internal::Literal(">");
        }
    };
} // namespace PUERTS_NAMESPACE

template<auto Data, typename T, typename API, typename RegisterAPI>
void MakePropertyCheck(PUERTS_NAMESPACE::ClassDefineBuilder<T, API, RegisterAPI>& builder, const char* name)
{
    if constexpr (std::is_member_object_pointer_v<decltype(Data)>)
    {
        using data_type = std::invoke_result_t<decltype(Data), T&>;
        if constexpr (std::is_move_assignable_v<data_type>)
        {
            builder.Property(name, MakeProperty(Data));
        }
        else
        {
            builder.Property(name, MakeReadonlyProperty(Data));
        }
    }
    else
    {
        builder.Property(name, MakeProperty(Data));
    }
}

#include "core/tool/function.hpp"
template<auto Candidate, typename T, typename API, typename RegisterAPI>
void MakeMethodCheck(PUERTS_NAMESPACE::ClassDefineBuilder<T, API, RegisterAPI>& builder, const char* name)
{
    builder.Method(name, MakeFunction(static_cast<remove_noexcept_t<decltype(Candidate)>>(Candidate)));
}

template<typename T, typename API, typename RegisterAPI>
static void RegisterCheck(PUERTS_NAMESPACE::ClassDefineBuilder<T, API, RegisterAPI>& builder)
{
    using namespace PUERTS_NAMESPACE;
    if constexpr (!is_uetype<T>::value)
    {
        builder.Register();
        return;
    }
    auto& Cdb = builder;

    static std::vector<JSFunctionInfo> s_functions_ {};
    static std::vector<JSFunctionInfo> s_methods_ {};
    static std::vector<JSPropertyInfo> s_properties_ {};
    static std::vector<JSPropertyInfo> s_variables_ {};

    static std::vector<NamedFunctionInfo> s_constructorInfos_ {};
    static std::vector<NamedFunctionInfo> s_methodInfos_ {};
    static std::vector<NamedFunctionInfo> s_functionInfos_ {};
    static std::vector<NamedPropertyInfo> s_propertyInfos_ {};
    static std::vector<NamedPropertyInfo> s_variableInfos_ {};

    JSClassDefinition ClassDef = JSClassEmptyDefinition;

    ClassDef.ScriptName  = Cdb.className_;
    ClassDef.TypeId      = StaticTypeId<T>::get();
    ClassDef.SuperTypeId = Cdb.superTypeId_;

    ClassDef.Initialize = Cdb.constructor_;
    ClassDef.Finalize   = PUERTS_NAMESPACE::ClassDefineBuilder<T, API, RegisterAPI>::template FinalizeBuilder<T>::Build();

    s_functions_ = std::move(Cdb.functions_);
    s_functions_.push_back({nullptr, nullptr, nullptr});
    ClassDef.Functions = s_functions_.data();

    s_methods_ = std::move(Cdb.methods_);
    s_methods_.push_back({nullptr, nullptr, nullptr});
    ClassDef.Methods = s_methods_.data();

    s_properties_ = std::move(Cdb.properties_);
    s_properties_.push_back(JSPropertyInfo {nullptr, nullptr, nullptr, nullptr});
    ClassDef.Properties = s_properties_.data();

    s_variables_ = std::move(Cdb.variables_);
    s_variables_.push_back(JSPropertyInfo {nullptr, nullptr, nullptr, nullptr});
    ClassDef.Variables = s_variables_.data();

    s_constructorInfos_ = std::move(Cdb.constructorInfos_);
    s_constructorInfos_.push_back(NamedFunctionInfo {nullptr, nullptr});
    ClassDef.ConstructorInfos = s_constructorInfos_.data();

    s_methodInfos_ = std::move(Cdb.methodInfos_);
    s_methodInfos_.push_back(NamedFunctionInfo {nullptr, nullptr});
    ClassDef.MethodInfos = s_methodInfos_.data();

    s_functionInfos_ = std::move(Cdb.functionInfos_);
    s_functionInfos_.push_back(NamedFunctionInfo {nullptr, nullptr});
    ClassDef.FunctionInfos = s_functionInfos_.data();

    s_propertyInfos_ = std::move(Cdb.propertyInfos_);
    s_propertyInfos_.push_back(NamedPropertyInfo {nullptr, nullptr});
    ClassDef.PropertyInfos = s_propertyInfos_.data();

    s_variableInfos_ = std::move(Cdb.variableInfos_);
    s_variableInfos_.push_back(NamedPropertyInfo {nullptr, nullptr});
    ClassDef.VariableInfos = s_variableInfos_.data();

    RegisterJSClass(ClassDef);
}

#else
#define UsingArray
#define UsingArray2D
#define UsingYrClass
#define UsingYrStruct
#define UsingContainer(CLS)
#define UsingIndexClass(TKEY, TVALUE)
#define UsingVectorClass(CLS)
#define UsingDynamicVectorClass(CLS)
#define UsingTypeList(CLS)
#ifndef UsingCppType
    #define UsingCppType(CLS)
#endif // !UsingCPPType
#endif
