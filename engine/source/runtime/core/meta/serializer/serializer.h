#pragma once
#include "runtime/core/meta/json.h"
#include "runtime/core/meta/reflection/reflection.h"

#include <cassert>

namespace VE
{
    template<typename...>
    inline constexpr bool always_false = false;

    class VSerializer
    {
    public:
        template<typename T>
        static VJson writePointer(T* instance)
        {
            return VJson::object { {"$typeName", VJson{"*"}}, {"$context", VSerializer::write(*instance)} };
        }

        template<typename T>
        static T*& readPointer(const VJson& json_context, T*& instance)
        {
            assert(instance == nullptr);
            std::string type_name = json_context["$typeName"].string_value();
            assert(!type_name.empty());
            if ('*' == type_name[0])
            {
                instance = new T;
                read(json_context["$context"], *instance);
            }

            return VJson::object { {"$typeName", VJson{"*"}}, {"$context", VSerializer::write(*instance)} };
        }

        template<typename T>
        static VJson write(const Reflection::ReflectionPtr<T>& instance)
        {
            T* instance_ptr = static_cast<T*>(instance.operator->());
            std::string type_name = instance.getTypeName();
            return VJson::object { {"$typeName", VJson(type_name)},
                                   {"$context", Reflection::TypeMeta::writeByName(type_name, instance_ptr)} };
        }

        template<typename T>
        static VJson read(const VJson& json_context, Reflection::ReflectionPtr<T>& instance)
        {
            std::string type_name = json_context["$typeName"].string_value();
            instance.setTypeName(type_name);
            return readPointer(json_context, instance.getPtrReference());
        }

        template<typename T>
        static VJson write(const T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return writePointer((T)instance);
            }
            else
            {
                static_assert(always_false<T>, "VSerializer::write<T> has not been implemented yet!");
                return VJson();
            }
        }

        template<typename T>
        static T& read(const VJson& json_context, T& instance)
        {
            if constexpr (std::is_pointer<T>::value)
            {
                return readPointer(json_context, instance);
            }
            else
            {
                static_assert(always_false<T>, "VSerializer::read<T> has not been implemented yet!");
                return instance;
            }
        }
    };

    // implementation of base types - template instantionlize
    template<>
    VJson VSerializer::write(const char& instance);
    template<>
    char& VSerializer::read(const VJson& json_context, char& instance);

    template<>
    VJson VSerializer::write(const int& instance);
    template<>
    int& VSerializer::read(const VJson& json_context, int& instance);

    template<>
    VJson VSerializer::write(const unsigned int& instance);
    template<>
    unsigned int& VSerializer::read(const VJson& json_context, unsigned int& instance);

    template<>
    VJson VSerializer::write(const float& instance);
    template<>
    float& VSerializer::read(const VJson& json_context, float& instance);

    template<>
    VJson VSerializer::write(const double& instance);
    template<>
    double& VSerializer::read(const VJson& json_context, double& instance);

    template<>
    VJson VSerializer::write(const bool& instance);
    template<>
    bool& VSerializer::read(const VJson& json_context, bool& instance);

    template<>
    VJson VSerializer::write(const std::string& instance);
    template<>
    std::string& VSerializer::read(const VJson& json_context, std::string& instance);
} // namespace VE
