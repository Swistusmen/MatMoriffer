#include "JsonReader.h"

nlohmann::json readJsonFromFile(const std::string& pathname)
{
    std::ifstream f(pathname);
    if (!f.is_open()){
        return nlohmann::json();
    }
    nlohmann::json data = nlohmann::json::parse(f);
    f.close();
    return data;
}
