#include "FileService.hpp"

std::ifstream FileService::resolve (const std::string &fileName)
{
    if (fileName.find ("..") != std::string::npos)
    {
        return {};
    }

    return std::ifstream {"./files/" + fileName, std::ifstream::binary};
}
