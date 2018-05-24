#include <mutex>
#include <condition_variable>
#include "Future.h"

template <class T>
std::shared_ptr<T> Future<T>::Get() {
	std::unique_lock<std::mutex> lock(data->mutex);

	data->cv.wait(lock, [this]() {return data->GetData() || data->GetException();});

	if (data->GetException()) {
		throw data->GetException();
	}

	return data->GetData();
}

template <class T>
std::shared_ptr<T> Future<T>::TryGet() {
	return data->GetData();
}