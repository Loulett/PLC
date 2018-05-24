#pragma once
#include <memory>
#include "Data.h"

template <class T>
class Future {
public:
	Future(std::shared_ptr<Data<T>> data): data(data) {}
	std::shared_ptr<T> Get();
	std::shared_ptr<T> TryGet();

private:
	std::shared_ptr<Data<T>> data;
};