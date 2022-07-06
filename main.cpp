#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <exception>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "CustomExceptions.h"

int main() {
    static constexpr std::string_view programVersion {"0.1"};
    ConverterJSON converterJson;

    try {
        converterJson.GetProgramName();
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return -1;
    }

    std::cout << "Starting program: " << converterJson.GetProgramName() << " "
              << programVersion << std::endl;

    if (programVersion != converterJson.GetConfigFileVersion()) {
        std::cerr << "Warning: \"Config.json\" has incorrect file version!" << std::endl;
    }

    std::vector<std::string> textDocs = converterJson.GetTextDocuments();

    InvertedIndex invertedIndex;
    invertedIndex.UpdateDocumentBase(textDocs);

    SearchServer searchServer(invertedIndex);

    std::vector<std::vector<RelativeIndex>> relativeIndex;
    try {
        relativeIndex = searchServer.Search(converterJson.GetRequests());
    } catch (const FileMissing& exception) {
        std::cerr << exception.what() << std::endl;
    }

    std::vector<std::vector<std::pair<int, float>>> answers;
    size_t answersCount = 0;
    for (const auto& answer : relativeIndex) {
        std::vector<std::pair<int, float>> docs;
        for (const auto& [docId, rank]: answer) {
            docs.emplace_back(std::make_pair(docId,  rank));
        }
        ++answersCount;
        answers.emplace_back(docs);
    }

    try {
        converterJson.PutAnswers(answers);
    } catch (const FileBusy& exception) {
        std::cerr << exception.what() << std::endl;
    }

    return 0;
}
