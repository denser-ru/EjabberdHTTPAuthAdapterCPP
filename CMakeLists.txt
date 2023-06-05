cmake_minimum_required(VERSION 3.25)
project(EjabberdHTTPAuthAdapterCPP)

set(CMAKE_CXX_STANDARD 11)

# Путь к исходным файлам
set(SOURCE_FILES
        src/main.cpp
        src/ConfigReader.cpp
        src/HttpCommandExecutor.cpp
        src/ResponseParser.cpp
        )

# Путь к заголовочным файлам
set(HEADER_FILES
        inc/ConfigReader.h
        inc/HttpCommandExecutor.h
        inc/ResponseParser.h
        )

# Добавление исполняемого файла
add_executable(EjabberdHTTPAuthAdapter ${SOURCE_FILES} ${HEADER_FILES})

# Директория для поиска заголовочных файлов
include_directories(inc)

# Добавление библиотеки libcurl
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(EjabberdHTTPAuthAdapter ${CURL_LIBRARIES})

