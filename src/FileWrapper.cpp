#include "FileWrapper.h"

template<typename T>
FileWrapper<T>::FileWrapper(const std::string& filePath) : file(filePath) {}

template<typename T>
T& FileWrapper<T>::getFile() {
    return file;
}

template<typename T>
FileWrapper<T>::~FileWrapper() {
    file.close();
}

// Явно инстанцируем необходимые типы
template class FileWrapper<std::ofstream>;
template class FileWrapper<std::ifstream>;
