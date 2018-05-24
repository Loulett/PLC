#include <thread>
#include <iostream>
#include "Future.cpp"
#include "Promise.cpp"
#include "Data.cpp"

int main() {
	int data = 5;

	Promise<int> promise;
	promise.SetData(&data);

	Future<int> future = promise.GetFuture();

	promise.SetException(new Exception("exception"));

	try {
		future.Get();
	}
	catch (Exception* e) {
		std::cout << e->what();
	}

	return 0;
}