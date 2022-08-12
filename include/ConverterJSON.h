#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
private:
    std::vector<std::string> _fileNames;
    size_t _responsesLimit = 5;
    std::string _programName;
    std::string _configFileVersion;
    size_t _baseUpdateInterval = 30;
    bool _configIsLoaded = false;

    std::string _lastMissingFiles;

    void LoadConfig();
    static void NormalizeFileNames(std::vector<std::string>& fileNames);
    static std::string IndexToString(size_t index);

public:
    ConverterJSON() = default;

/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных
* в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного
* количества ответов на один запрос
* @return возвращает предельное количества ответов на один запрос
*/
    int GetResponsesLimit();

/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

/**
* Положить в файл answers.json результаты поисковых запросов
*/
    void PutAnswers(const std::vector<std::vector<std::pair<int, float>>>& answers);

/**
* Метод получает имя программы из файла config.json
* @return возвращает имя программы из файла config.json
*/
    std::string GetProgramName();

/**
* Метод получает версию файла config.json
* @return возвращает версию файла config.json
*/
    std::string GetConfigFileVersion();

/**
* Метод получает интервал оюновления базы из файла config.json
* @return возвращает интервал оюновления базы
*/
    size_t GetBaseUpdateInterval();

};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
