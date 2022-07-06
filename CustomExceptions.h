#ifndef SEARCH_ENGINE_CUSTOMEXCEPTIONS_H
#define SEARCH_ENGINE_CUSTOMEXCEPTIONS_H


class ConfigFileIncorrect : public std::exception {
public:
    const char* what() const noexcept override {
        return "Config file has incorrect format!";
    }
};

class ConfigFileEmpty : public std::exception {
public:
    const char* what() const noexcept override {
        return "Config file is empty!";
    }
};

class ConfigFileMissing : public std::exception {
public:
    const char* what() const noexcept override {
        return "Config file is missing!";
    }
};

class FileMissing : public std::runtime_error {
public:
    explicit FileMissing(const std::string& fileName)
    : std::runtime_error(("Warning: File \"" + fileName + "\" is missing").c_str()) {};
};

class FileBusy : public std::runtime_error {
public:
    explicit FileBusy(const std::string& fileName)
    : std::runtime_error(("Warning: File \"" + fileName + "\" is occupied by another process!").c_str()) {};
};


#endif //SEARCH_ENGINE_CUSTOMEXCEPTIONS_H
