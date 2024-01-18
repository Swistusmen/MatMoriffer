#ifndef JSONREADER_H
#define JSONREADER_H

#include <fstream>
#include <nlohmann/json.hpp>

nlohmann::json readJsonFromFile(const std::string& pathname);

#endif // JSONREADER_H
