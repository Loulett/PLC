#include <iostream>

struct P {
	P* parent = nullptr;

};

P tBase, tDerived;

struct Base {
	virtual ~Base() {}
	P* type = &tBase;
	int a = 0;
};

struct Derived: public Base {
	P* type = &tDerived;
	int b = 1;
};


template <typename T>
Derived* dynamicCastDerived(T* object) {
	P* type = object.type; \
	while (type != Type || type != nullptr) {
		type = type.parent;
	}
	if (type == nullptr) {
		return nullptr;
	} else {
		return object;
	}
}


#define DYNAMIC_CAST(Type, Object) dynamicCast##Type(Object)

int main() {
	tDerived.parent = &tBase;

	Base* pBDerived = new Derived;
	Base* pBBase = new Base;
	Derived* pd;

	pd = DYNAMIC_CAST(Derived, pBDerived);

	//pd = dynamic_cast<Derived*>(pBDerived); //works

	std::cout << pd->b;
	pd = DYNAMIC_CAST(Derived, pBBase);
	//pd = dynamic_cast<Derived*>(pBBase); //does not work 

	return 0;
}