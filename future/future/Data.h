#pragma once
#include <memory>
#include <mutex>
#include <condition_variable>
#include "Exception.h"

template <class T>
class Data {
public:
	Data() : data(nullptr), exception(nullptr) {}
 
	void SetData(T* data);
	void SetException(Exception* exception);

	std::shared_ptr<T> GetData();
	Exception* GetException();

	std::mutex mutex;
	std::condition_variable cv;
private:
	std::shared_ptr<T> data;
	Exception* exception;
};