#include "FileWrapper.h"

template<typename T>
FileWrapper<T>::FileWrapper( const std::string& filePath ) : file_( filePath ) {}

template<typename T>
T& FileWrapper<T>::getFile( void ) {
	return file_;
}

template<typename T>
FileWrapper<T>::~FileWrapper( void ) {
	file_.close();
}

template<typename T>
bool FileWrapper<T>::isFileOpen( void ) const {
	return file_.is_open() && file_.good();
}

// Явно инстанцируем необходимые типы
template class FileWrapper<std::ofstream>;
template class FileWrapper<std::ifstream>;
