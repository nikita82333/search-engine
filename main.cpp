#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <exception>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
#include "CustomExceptions.hpp"

int main() {
    static constexpr std::string_view programVersion {"0.2"};
    ConverterJSON converterJson;

    try {
        converterJson.GetProgramName();
    } catch (const std::exception& exception) {
        std::clog << exception.what() << std::endl;
        return -1;
    }

    std::cout << "Starting program: " << converterJson.GetProgramName() << " "
              << programVersion << " ..." << std::endl << std::endl;

    if (programVersion != converterJson.GetConfigFileVersion()) {
        std::clog << "Warning: \"Config.json\" has incorrect file version!" << std::endl;
    }

    std::vector<std::string> textDocs;
    InvertedIndex invertedIndex;
    size_t baseUpdateInterval = converterJson.GetBaseUpdateInterval();

    std::condition_variable exitConditionVariable;
    std::mutex exitMutex, invertedIndexAccess;
    std::atomic<bool> exitFlag {false};

    std::thread updateIndex {[&](){
        while(!exitFlag.load()) {
            textDocs = converterJson.GetTextDocuments();
            invertedIndexAccess.lock();
            invertedIndex.UpdateDocumentBase(textDocs);
            invertedIndexAccess.unlock();

            std::unique_lock<std::mutex> lock(exitMutex);
            exitConditionVariable.wait_for(lock,std::chrono::seconds(baseUpdateInterval));
            std::chrono::milliseconds(10);
        }
    }};

    std::string userInput;
    std::cout << "List of commands." << std::endl;
    std::cout << "1. Search by requests from a file \"requests.json\"." << std::endl;
    std::cout << "2. Search by a request entered manually." << std::endl;
    std::cout << "3. Exit program." << std::endl;

    while (!exitFlag.load()) {
        std::cout << "Please enter the command number:" << std::endl;
        std::getline(std::cin, userInput);
        if (userInput == "1" || userInput == "2") {
            invertedIndexAccess.lock();
            SearchServer searchServer(invertedIndex);
            invertedIndexAccess.unlock();
            std::vector<std::vector<RelativeIndex>> relativeIndex;
            std::vector<std::string> requests;
            if (userInput == "1") {
                try {
                    requests = converterJson.GetRequests();
                } catch (const FileMissingException& exception) {
                    std::clog << exception.what() << std::endl;
                }
            } else {
                std::cout << "Please enter your request:" << std::endl;
                std::string request;
                std::getline(std::cin, request);
                requests.clear();
                requests.emplace_back(request);
            }

            relativeIndex = searchServer.Search(requests);
            std::vector<std::vector<std::pair<int, float>>> answers;
            for (const auto& answer : relativeIndex) {
                std::vector<std::pair<int, float>> docs;
                for (const auto& [docId, rank]: answer) {
                    docs.emplace_back(std::make_pair(docId,  rank));
                }
                answers.emplace_back(docs);
            }

            try {
                converterJson.PutAnswers(answers);
                std::cout << "Done! See search results in the file \"answers.json\"." << std::endl;
            } catch (const FileBusyException& exception) {
                std::clog << exception.what() << std::endl;
            }

        } else if (userInput == "3") {
            exitFlag.store(true);
            exitConditionVariable.notify_one();
        } else {
            std::clog << "No such command!" << std::endl;
        }
    }

    updateIndex.join();

    return 0;
}
