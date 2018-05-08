#include <vector>
#include <string>
#include <csetjmp>
#include <iostream>


class Exception;
class BaseClass;
void THROW(Exception*);

std::vector<BaseClass*> stack;

struct Catch {
	std::jmp_buf buf;
	int stackSize;
	bool inStackRewind = false;

	Catch(int stackSize): stackSize(stackSize) {}
};

class BaseClass {
public:
	BaseClass() { stack.push_back(this); }
	virtual ~BaseClass() {
		stack.pop_back();
	}
};

class Exception {
public:
	Exception(std::string str) : str(str) {}
	std::string GetStr() { return str; }
	virtual ~Exception() {}
protected:
	std::string str;
};

class CharacterStackObject: public BaseClass {
private:
	char c;
public:
	CharacterStackObject(char c) : c(c) {}
	~CharacterStackObject() { 
		std::cout << "CharacterStackObject " << c << " deleted\n"; }
};

std::vector<Catch> stackException;

class BadFileException : public Exception {
public:
	BadFileException(std::string str): Exception(str) {}
};

class OutOfMemoryException : public Exception {
	OutOfMemoryException(std::string str): Exception(str) {}
};

Exception* currentException = nullptr;

void THROW(Exception* e) {
	currentException = e;
	if (stackException.empty() || stackException.back().inStackRewind) {
		std::terminate();
	} else {
		std::longjmp(stackException.back().buf, 1);
	}
}

#define CATCH(type, exceptionName) \
	CatchLabel: \
		if (type exceptionName; (exceptionName = dynamic_cast<type>(currentException)) && !(currentException = nullptr))

#define NEXT_CATCH(type, exceptionName) \
	else if (type exceptionName; (exceptionName = dynamic_cast<type>(currentException)) && !(currentException = nullptr))

#define DEFAULT_CATCH else { THROW(currentException); }

#define TRY \
	stackException.push_back(Catch(stack.size())); \
	if (std::setjmp(stackException.back().buf) != 0) { \
		stackException.back().inStackRewind = true; \
		while (stack.size() > stackException.back().stackSize) { \
			stack.back()->~BaseClass(); \
		} \
        stackException.pop_back(); \
		goto CatchLabel; \
	} else

void fail2() {
	THROW(new Exception("Exception"));
}

void fail() {
	fail2();
}

void func() {
    TRY{
        std::cout << "Hello world\n";
        fail();
        std::cout << "Hello world2\n";
    }
    CATCH(BadFileException*, ex1) {
        std::cout << "Caught BadFileException exception!\n";
        std::cout << ex1->GetStr() << "\n";
    }
    NEXT_CATCH(OutOfMemoryException*, ex1) {
        std::cout << "Caught OutOfMemoryException exception!\n";
        std::cout << ex1->GetStr() << "\n";
    }
    DEFAULT_CATCH
}

int main() {

	CharacterStackObject cso_a('a');

    TRY {
		CharacterStackObject cso_b('b');
        func();
    }
    CATCH(Exception*, ex1) {
        std::cout << "Catches any exception!\n";
    }

	return 0;
}