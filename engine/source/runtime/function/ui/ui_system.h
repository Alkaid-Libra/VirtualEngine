#pragma once

#include "runtime/core/base/public_singleton.h"

namespace VE
{
    class VUIManager final : public PublicSingleton<VUIManager>
    {
        friend class PublicSingleton<VUIManager>;

    public:
        VUIManager(const VUIManager&) = delete;
        VUIManager& operator=(const VUIManager&) = delete;

        int initialize();
        // int initialize() {return 0;}
        int update();
        int clear();

    protected:
        VUIManager() = default;
    };
} // namespace VE
