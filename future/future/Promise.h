#pragma once
#include <memory>
#include "Exception.h"
#include "Data.h"
#include "Future.h"

template <class T>
class Promise {
public:
	friend class Future<T>;
	void SetData(T* data);
	void SetException(Exception* exception);

	Future<T> GetFuture();

	Promise() : data(new Data<T>()) {}

	Promise(const Promise&) = delete;
	Promise & operator = (const Promise&) = delete;

	Promise(Promise&&) noexcept = default;
	Promise & operator = (Promise&&) noexcept = default;
private:
	std::shared_ptr<Data<T>> data;
};