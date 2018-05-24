#include "Data.h"

template <class T>
void Data<T>::SetData(T* data) {
	this->data.reset(data);
	cv.notify_all();
}

template <class T>
void Data<T>::SetException(Exception* exception) {
	this->exception = exception;
	cv.notify_all();
}

template <class T>
std::shared_ptr<T> Data<T>::GetData() {
	return data;
}

template <class T>
Exception* Data<T>::GetException() {
	return exception;
}