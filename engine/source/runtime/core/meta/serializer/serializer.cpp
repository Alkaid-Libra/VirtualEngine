#include "serializer.h"
#include <assert.h>

namespace VE
{
    template<>
    VJson VSerializer::write(const char& instance)
    {
        return VJson(instance);
    }
    template<>
    char& VSerializer::read(const VJson& json_context, char& instance)
    {
        assert(json_context.is_number());
        return instance = json_context.number_value();
    }

    template<>
    VJson VSerializer::write(const int& instance)
    {
        return VJson(instance);
    }
    template<>
    int& VSerializer::read(const VJson& json_context, int& instance)
    {
        auto hh = json_context.type();
        assert(json_context.is_number());
        return instance = static_cast<int>(json_context.number_value());
    }

    template<>
    VJson VSerializer::write(const unsigned int& instance)
    {
        return VJson(static_cast<int>(instance));
    }
    template<>
    unsigned int& VSerializer::read(const VJson& json_context, unsigned int& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<unsigned int>(json_context.number_value());
    }

    template<>
    VJson VSerializer::write(const float& instance)
    {
        return VJson(instance);
    }
    template<>
    float& VSerializer::read(const VJson& json_context, float& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    VJson VSerializer::write(const double& instance)
    {
        return VJson(instance);
    }
    template<>
    double& VSerializer::read(const VJson& json_context, double& instance)
    {
        assert(json_context.is_number());
        return instance = static_cast<float>(json_context.number_value());
    }

    template<>
    VJson VSerializer::write(const bool& instance)
    {
        return VJson(instance);
    }
    template<>
    bool& VSerializer::read(const VJson& json_context, bool& instance)
    {
        assert(json_context.is_bool());
        return instance = json_context.bool_value();
    }

    template<>
    VJson VSerializer::write(const std::string& instance)
    {
        return VJson(instance);
    }
    template<>
    std::string& VSerializer::read(const VJson& json_context, std::string& instance)
    {
        assert(json_context.is_string());
        return instance = json_context.string_value();
    }
} // namespace VE
