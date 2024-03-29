# **Search server**
> Данный обработчик поисковых запросов позволяет находить в сформированной базе документы,
> содержащие запрашиваемую комбинацию слов, с учетом:
> * статуса
> * релевантности (TF-IDF)
> * рейтинга
> * и иных параметров.

## **Используемые инструменты:**
> * C++ 17
> * CMake

## **Функционал:**
> * формирование базы документов, по которой производится поисковый запрос
> * парсер вводимого зпроса
> * поиск топ документов с соответствии с заданными критериями поиска
> * возможность исключения из результатов поиска документов, содержащих определенные слова

## **Типы задач:**
> * добавить документ в базу
> * найти топ совпадений по указанным параметрам

## **Что можно улучшить:**
> * добавить модуль обработки запросов и выдачи результата через json
> * добавить модуль сериализации / есериализации базы документов

## **Сборка и запуск**
> 1. Собрать с использованием CMake, используя приложенный файл CMakeLists.txt
> 2. Запустить исполняемый файл
