#pragma once
#include "runtime/core/base/public_singleton.h"

#include <vector>
#include <filesystem>

namespace VE
{
    class FileService : public PublicSingleton<FileService>
    {
        friend class PublicSingleton<FileService>;
    
    public:
        std::vector<std::filesystem::path> getFiles(const std::filesystem::path& directory);

    protected:
        FileService() = default;
    };
} // namespace VE
