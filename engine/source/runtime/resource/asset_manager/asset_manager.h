#pragma

#include "runtime/core/base/public_singleton.h"

namespace VE
{
    class AssetManager : public PublicSingleton<AssetManager>
    {
    public:
        void initialize();
        void clear() {}
    };
} // namespace VE
