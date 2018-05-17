#include <iostream>

struct Base;
struct Derived;

struct Vtable{
	void (*OnlyBase)(void*);
	void (*Both)(void*);
	void (*OnlyDerived)(void*);
};


#define VIRTUAL_CLASS(BaseName) Vtable Vtable##BaseName; struct BaseName{ Vtable* VTable = &Vtable##BaseName;

#define END };

#define DECLARE_METHOD(Class, Name) void Name##Class(void* self); \
struct Help##Name##Class{Help##Name##Class(){ Vtable##Class.Name = Name##Class; }} help##Name##Class; \
void Name##Class(void* self)

#define VIRTUAL_CALL(Object, FunctionName) Object->VTable->FunctionName(Object);

// базовый класс
VIRTUAL_CLASS(Base)
int a;
END
// методы
DECLARE_METHOD(Base, Both) {
	int a = reinterpret_cast<Base*>(self)->a;
	std::cout << "Base::Both a = " << a << "\n";
}
DECLARE_METHOD(Base, OnlyBase) {
	int a = reinterpret_cast<Base*>(self)->a;
	std::cout << "Base::OnlyBase a = " << a << "\n";
}

// класс-наследник
VIRTUAL_CLASS(Derived)
int b;
END
// методы
DECLARE_METHOD(Derived, Both) {
	int b = reinterpret_cast<Derived*>(self)->b;
	std::cout << "Derived::Both b = " << b << "\n";
}
DECLARE_METHOD(Derived, OnlyDerived) {
	int b = reinterpret_cast<Derived*>(self)->b;
	std::cout << "Derived::OnlyDerived b = " << b << "\n";
}


int main()
{
	Base base = Base();
	base.a = 0;
	Base* pbase = &base;
	Derived derived = Derived();
	derived.b = 1;
	Base* reallyDerived = reinterpret_cast<Base*>(&derived);

	VIRTUAL_CALL(pbase, Both); // печатает УBase::BothФ
	VIRTUAL_CALL(reallyDerived, Both); // печатает УDerived::BothФ
	VIRTUAL_CALL(pbase, OnlyBase);  // печатает УBase::OnlyBaseФ
	VIRTUAL_CALL(reallyDerived, OnlyDerived);
	return 0;
}
