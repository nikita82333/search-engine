﻿# search-engine
## 🖋️ Описание
Учебный проект "Поисковый движок". Осуществляет поиск по некоторой базе текстовых документов.
### Алгоритм работы поискового движка:
0. Перед запуском в конфигурационном файле необходимо задать названия файлов, по которым движок будет осуществлять поиск.
1. Поисковый движок обходит все файлы и индексирует их так, чтобы потом по любому поисковому запросу находить наиболее релевантные документы. Переиндексация базы проиходит автоматически в отдельном потоке.
2. Пользователь задаёт запрос через JSON-файл requests.json либо может ввести "быстрый запрос" с клавиатуры. Запрос — это набор слов, по которым нужно найти документы.
3. Запрос трансформируется в список слов.
4. В индексе ищутся те документы, в которых встречаются эти слова (хотя бы одно).
5. Результаты поиска ранжируются по релевантности, сортируются и отдаются пользователю. Максимальное количество возможных документов в ответе задаётся в конфигурационном файле.
6. Программа формирует файл answers.json, в который записывает результаты поиска.
7. Далее пользователь может повторить поиск либо завершить работу с программой.
### Формат JSON-файлов
#### Файл конфигурации config.json
Cодержит название поискового движка, его версию, время обновления базы (с какой периодичностью необходимо делать переиндексирование базы, заново подгружать файлы и обсчитывать их поисковый рейтинг), максимальное количество вариантов в ответе, пути к текстовым файлам.
```json
{
  "config": {
  "name": "SearchEngine",
  "version": "0.2",
  "max_responses": 5,
  "base_update_interval_sec": 60
  },
  "files": [
    "/resources/file001.txt",
    "/resources/file002.txt",
    "/resources/file003.txt",
    "/resources/file004.txt"
  ]
}
```
- **name** — поле с названием поискового движка.
- **version** — поле с номером версии поискового движка.
- **max_responses** — поле, определяющее максимальное количество ответов на один запрос.
- **base_update_interval_sec** - поле определяет период обновления базы (в секундах).
- **files** содержит пути к файлам, по которым необходимо осуществлять поиск. Внутри списка files лежат пути к файлам.
Для указания расположения исходных текстовых файлов можно использовать относительные пути (как в примере), в этом случае папка resources должна располагаться в той же директории, что и исполняемый файл проекта search_engine.exe. Также могут быть заданы абсолютные пути.
#### Файл с запросами requests.json
Содержит запросы, которые необходимо обработать поисковому движку.
```json
{
  "requests": [
    "some words..",
    "some words..",
    "some words..",
    "some words.."
  ]
}
```
- **requests** состоит из списка запросов, которые необходимо обработать поисковым движком. Может содержать не более 1000 запросов.
- **("some words..")** — поисковый запрос, набор слов, разделённых одним или несколькими пробелами.
#### Файл с ответами на запросы answers.json
В него записываются результаты работы поискового движка.
```json
{
  "answers": {
    "request001": {
      "result": "true",
      "relevance": [
        { "docid": 0, "rank" : 1 },
        { "docid": 1, "rank" : 0.897 },
        { "docid": 2, "rank" : 0.750 },
        { "docid": 3, "rank" : 0.670 },
        { "docid": 4, "rank" : 0.561 }
      ]
    },
    "request002": {
      "result": "true",
      "docid": 0,
      "rank" : 1
    },
    "request003": {
      "result": "false"
    }
  }
}
```
- **request001 … 003** — идентификатор запроса, по которому сформирован ответ.
- **result** – результат поиска запроса. Если он принимает значение true, значит по данному запросу найден хотя бы один документ. Если результат имеет значение false, значит ни одного документа не найдено.
- **relevance** включается в файл answers.json, если на этот запрос удалось найти более одного документа.
## 🛠️ Используемые технологии
<div>
  <img src="https://github.com/devicons/devicon/blob/master/icons/cplusplus/cplusplus-original.svg" title="C++" alt="C++" width="40" height="40"/>&nbsp;
  <img src="https://github.com/devicons/devicon/blob/master/icons/cmake/cmake-original.svg" title="CMake" alt="CMake" width="40" height="40"/>&nbsp;
</div>
Для работы с json-файлами используется библиотека:
https://github.com/nlohmann/json

Для проведения автоматических модульных тестов к проекту подключена библиотека Google Test (https://github.com/google/googletest)

## 🔨 Сборка проекта
Проект собирается в системах Windows и Linux. Компилятор должен поддерживать стандарт C++17, Cmake 3.11+.
```
cmake --build \search_engine\cmake-build-debug --target search_engine -j 3
```

## 🚀 Запуск
```
\search_engine\cmake-build-debug\search_engine.exe
```
