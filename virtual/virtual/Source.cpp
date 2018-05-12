#include <iostream>

struct Base;
struct Derived;

struct Vtable{
	void (*OnlyBase)();
	void (*Both)();
	void (*OnlyDerived)();
};


#define VIRTUAL_CLASS(BaseName) Vtable Vtable##BaseName; struct BaseName{ Vtable* VTable = &Vtable##BaseName;

#define END };

#define DECLARE_METHOD(Class, Name) void Name##Class() { std::cout << #Class << "::" << #Name << "\n";} \
struct Help##Name##Class{Help##Name##Class(){ Vtable##Class.Name = Name##Class; }} help##Name##Class;

#define VIRTUAL_CALL(Class, FunctionName) Class->VTable->FunctionName();

// базовый класс
VIRTUAL_CLASS(Base)
int a;
END
// методы
DECLARE_METHOD(Base, Both)
DECLARE_METHOD(Base, OnlyBase)

// класс-наследник
VIRTUAL_CLASS(Derived)
int b;
END
// методы
DECLARE_METHOD(Derived, Both)
DECLARE_METHOD(Derived, OnlyDerived)


int main()
{
	Base base = Base();
	base.a = 0;
	Base* pbase = &base;
	Derived derived = Derived();
	Base* reallyDerived = reinterpret_cast<Base*>(&derived);

	VIRTUAL_CALL(pbase, Both); // печатает УBase::BothФ
	VIRTUAL_CALL(reallyDerived, Both); // печатает УDerived::BothФ
	VIRTUAL_CALL(pbase, OnlyBase);  // печатает УBase::OnlyBaseФ
	VIRTUAL_CALL(reallyDerived, OnlyDerived);
	return 0;
}
