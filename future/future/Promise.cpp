#include "Promise.h"

template <class T>
void Promise<T>::SetData(T* data) {
	this->data->SetData(data);
}

template <class T>
void Promise<T>::SetException(Exception* exception) {
	this->data->SetException(exception);
}

template <class T>
Future<T> Promise<T>::GetFuture() {
	return Future<T>(data);
}