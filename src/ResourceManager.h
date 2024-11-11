#pragma once

#include <string>

class ResourceManager
{
public:
    static void init(std::string executablePath);
    static std::string getFilePath(const std::string& fileName);

private:
    static inline std::string m_assetPath;
};
