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

// ������� �����
VIRTUAL_CLASS(Base)
int a;
END
// ������
DECLARE_METHOD(Base, Both)
DECLARE_METHOD(Base, OnlyBase)

// �����-���������
VIRTUAL_CLASS(Derived)
int b;
END
// ������
DECLARE_METHOD(Derived, Both)
DECLARE_METHOD(Derived, OnlyDerived)


int main()
{
	Base base = Base();
	base.a = 0;
	Base* pbase = &base;
	Derived derived = Derived();
	Base* reallyDerived = reinterpret_cast<Base*>(&derived);

	VIRTUAL_CALL(pbase, Both); // �������� �Base::Both�
	VIRTUAL_CALL(reallyDerived, Both); // �������� �Derived::Both�
	VIRTUAL_CALL(pbase, OnlyBase);  // �������� �Base::OnlyBase�
	VIRTUAL_CALL(reallyDerived, OnlyDerived);
	return 0;
}
