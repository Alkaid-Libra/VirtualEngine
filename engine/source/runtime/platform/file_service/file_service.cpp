#include "runtime/platform/file_service/file_service.h"

#include <iostream>

namespace VE
{
    std::vector<std::filesystem::path> FileService::getFiles(const std::filesystem::path& directory)
    {
        std::vector<std::filesystem::path> files;
        for (const auto& directory_entry : std::filesystem::recursive_directory_iterator{directory})
        {
            if (directory_entry.is_regular_file())
            {
                files.push_back(directory_entry);
            }
        }
        return files;
    }
} // namespace VE
