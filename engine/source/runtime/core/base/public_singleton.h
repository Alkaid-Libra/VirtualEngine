#pragma once
#include<type_traits>

namespace VE
{
    template<typename T>
    class PublicSingleton
    {
    public:
        static T& getInstance() noexcept(std::is_nothrow_constructible<T>::value)
        {
            static T instance;
            return instance;
        }
        virtual ~PublicSingleton() noexcept = default;
        PublicSingleton(const PublicSingleton&) = delete;
        PublicSingleton& operator=(const PublicSingleton&) = delete;

    protected:
        PublicSingleton() = default;
    };
}