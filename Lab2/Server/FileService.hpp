#pragma once
#include <fstream>

class FileService final
{
public:
    static std::ifstream resolve (const std::string &fileName);
};
