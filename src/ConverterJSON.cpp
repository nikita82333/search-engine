#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <exception>
#include "nlohmann/json.hpp"
#include "ConverterJSON.h"
#include "CustomExceptions.hpp"

void ConverterJSON::NormalizeFileNames(std::vector<std::string>& fileNames) {
    for (auto& fileName : fileNames) {
        if (fileName[0] == '/') fileName.erase(0, 1);
#ifdef _WIN32
        std::string win32FileName;
        for (char symbol : fileName) {
            if (symbol == '/') {
                win32FileName += "\\";
            } else {
                win32FileName += symbol;
            }
        }
        fileName = win32FileName;
#endif
    }
}

void ConverterJSON::LoadConfig() {
    std::ifstream configFile("config.json");
    if (configFile.is_open()) {
        nlohmann::json configJson;
        configFile >> configJson;
        configFile.close();
        if (configJson.contains("config")) {
            if (configJson["config"].contains("max_responses")) {
                _responsesLimit = configJson["config"]["max_responses"];
            }
            if (configJson["config"].contains("base_update_interval_sec")) {
                _baseUpdateInterval = configJson["config"]["base_update_interval_sec"];
            }
            try {
                configJson.at("files").get_to(_fileNames);
                _programName = configJson["config"]["name"];
                _configFileVersion = configJson["config"]["version"];
                _configIsLoaded = true;
            } catch (...) {
                throw ConfigFileIncorrectException();
            }
            NormalizeFileNames(_fileNames);
        } else {
            throw ConfigFileEmptyException();
        }
    } else {
        throw ConfigFileMissingException();
    }
}

std::string ConverterJSON::IndexToString(size_t index) {
    std::ostringstream sStream;
    if (index < 1000) {
        sStream << std::setw(3) << std::setfill('0') << std::to_string(index);
        return sStream.str();
    } else
        return "000";
}

std::vector<std::string> ConverterJSON::GetTextDocuments() {
    if (!_configIsLoaded) LoadConfig();
    std::vector<std::string> textDocs;
    std::string missingFiles;
    for (const auto& fileName : _fileNames) {
        std::ifstream textFile(fileName);
        if (textFile.is_open()) {
            std::string text;
            textFile >> text;
            while (!textFile.eof()) {
                std::string word;
                textFile >> word;
                text += " " + word;
            }
            textDocs.emplace_back(text);
            textFile.close();
        } else {
            missingFiles += "\'" + fileName + "\' ";
        }
    }
    if (!missingFiles.empty() && missingFiles != _lastMissingFiles) {
        std::cerr << "Warning: File(s) " << missingFiles << "is missing!" << std::endl;
    }
    _lastMissingFiles = missingFiles;
    return textDocs;
}

int ConverterJSON::GetResponsesLimit() {
    if (!_configIsLoaded) LoadConfig();
    return _responsesLimit;
}

std::vector<std::string> ConverterJSON::GetRequests() {
    std::ifstream requestsFile("requests.json");
    std::vector<std::string> requests;
    if (requestsFile.is_open()) {
        nlohmann::json requestsJson;
        requestsFile >> requestsJson;
        requestsJson.at("requests").get_to(requests);
        requestsFile.close();
    } else {
        throw FileMissingException("requests.json");
    }
    return requests;
}

void ConverterJSON::PutAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers) {
    std::ofstream answersFile("answers.json");
    if (answersFile.is_open()) {
        nlohmann::json answersJson;
        size_t iRequest = 1;
        for (const auto& answer : answers) {
            std::string requestString = "request" + IndexToString(iRequest);
            answersJson["answers"][requestString];
            if (!answer.empty()) {
                answersJson["answers"][requestString]["result"] = "true";
                if (answer.size() == 1) {
                    answersJson["answers"][requestString]["docid"] = answer[0].first;
                    answersJson["answers"][requestString]["rank"] = answer[0].second;
                } else {
                    size_t iDoc = 0;
                    for (const auto& [docId, rank]: answer) {
                        answersJson["answers"][requestString]["relevance"][iDoc]["docid"] = docId;
                        answersJson["answers"][requestString]["relevance"][iDoc]["rank"] = rank;
                        ++iDoc;
                    }
                }
            } else {
                answersJson["answers"][requestString]["result"] = "false";
            }
            ++iRequest;
        }
        answersFile << std::setw(2) << answersJson;
        answersFile.close();
    } else {
        throw FileBusyException("answers.json");
    }
}

std::string ConverterJSON::GetProgramName() {
    if (!_configIsLoaded) LoadConfig();
    return _programName;
}

std::string ConverterJSON::GetConfigFileVersion() {
    if (!_configIsLoaded) LoadConfig();
    return _configFileVersion;
}

size_t ConverterJSON::GetBaseUpdateInterval() {
    if (!_configIsLoaded) LoadConfig();
    return _baseUpdateInterval;
}
