# Index

- [CPP Reminders](#cpp-reminders)
  - [OCF (Orthodox Canonical Form)](#ocf)
  - [References](#references)
  - [Initializer List](#initializer-list)
  - [Runtime Polymorphism & vtable](#runtime-polymorphism-vtable)
    - [Inheritance (Quick Reminder)](#inheritance-quick-reminder)
    - [The Problem that `virtual` Solves](#the-problem-that-virtual-solves)
    - [The vtable](#the-vtable-what-it-is)
    - [The vptr](#the-vptr-what-it-is)
    - [Pure Virtual & Abstract Classes](#pure-virtual-and-abstract-classes)
    - [Virtual Destructor](#virtual-destructor-why-it-matters)
  - [Inheritance Types](#inheritance-types)

- [Exercise 00](#ex00)
  - [Exceptions](#exceptions)
    - [Why Exceptions](#why-exceptions)
    - [Throw / Unwind / Catch](#what-really-happens-at-low-level)
    - [Inheritance Chain](#inheritance-chain)
    - [Why Inherit from `std::exception`](#why-inherit-from-stdexception)
    - [Catch Order](#catch-order-matters)
    - [`what()` Explained](#what-explained)
    - [Why Nested Classes](#why-nested-classes)
    - [How to Throw & Catch Exceptions](#how-to-throw-and-catch-exceptions)
  - [Common Oral Questions](#questions-typically-asked-for-ex00)
  - [Actual Exercise Implementation](#the-actual-exercise-implementation)

- [Exercise 01](#ex01)

# CPP Module 05

## CPP Reminders

### OCF

The orthodox canonical form, is a set of rules and structure for our class, in order to better manage memory, and the way we copy or assign it. and it essentially means having 3 different constructors which **their function is to initialize member variables** and prepare the class, and a destructor to destroy the object and free any allocated memory.

Constructors are essentially invoked (called), when initialization of an object takes place.

these four constructors/destructor must always exist in our class to respect OCF:
```c++
class Object {
	private:
	public:
	Object(); // Default constructor
	Object(const Object& other) // Copy constructor
	Object& operator=(const Object& other); // Copy assignment op
	~Object();
};
```

These declaration are based off, the syntax chosen for these constructors as documented in the [cpp reference](https://cppreference.com/).

- Default Constructor ([as documented here](https://en.cppreference.com/cpp/language/default_constructor)) :
can be called without any arguments, and initializes the class with default values,
default optimization can look something like [this](https://en.cppreference.com/cpp/language/default_initialization).

- Copy Constructor (as documented here) :
is called when the argument is the same type as the class, and is used to copy the \
values from that object without changing or mutating it.\
this is why it is called using a `const`.\
as well as the parameter being passed as a reference `&`, **to avoid calling the copy constructor recursively and causing a stack overflow**, because passing a variable as a parameter to be copied locally will result in that.

-  Copy assignment operator ([as documented here](https://en.cppreference.com/cpp/language/copy_assignment)) :
this method is essentially an overloaded operator `=`, which basically means, changing the behavior of an operator such as `=, +, -, ...`, to act differently when used on our class.

```c++
Object a;
Object b;

b = a;
```

this translates to :

```c++
b.operator=(a); //overloaded operator method.
```

which would be implemented like this :

```c++
Object& Object::operator=(const Object& other)
{
	if (&other != this)
	{
		this->value = other.value;
	}
	return *this;
}
```

okay the reason for returning the object itself, `*this` which is a pointer to the class calling the `operator=` method, is to allow chaining, as assigning it directly and returning `void`, will only copy one thing, not chain.

```c++
a = b = c;
// translates to for the compiler
a.operator=(b.operator=(c));
```

so the c values will get copied into b and return the b object, and b will values will get copied into a and return the a object, and the final object we will be left with is `a`.

and returning `Object&` reference to the b object itself, is just to avoid, calling the copy constructor on return, which is for optimization, and same goes for passing the parameter `const Object& other`.

as for the self-assignment check, `if (&other != this)` this is pretty much just for optimization in simple cases, but when classes own allocated memory, assigning the same object, u would have to delete the old object memory, and copy the same freed memory, causing undefined behavior, crashes, etc...

- Destructor ([as documented here](https://en.cppreference.com/cpp/language/destructor)) :
essentially the destructor, is a special method called at the end of an object's scope or lifetime, its job is to simply clean up or free any allocated resources. `~Object();`

### References

- Reference `&` : Always valid, can't be null, can't be reassigned. Use this 99% of the time in C++98.
- Pointer `*` : Can be null, can be reassigned. Use when you need dynamic memory or optional things.

### Initializer list

is basically a way to assign the member variables before the constructor body, because contrary to common belief, the variables are constructed either way and this is before the constructor body runs anyway.

essentially we follow this rule :

- **Can this member be default-constructed and then assigned?** (`int`, `std::string`, etc.) → An initializer list is still preferred because it avoids an extra assignment.
- **Must this member be initialized immediately?** (`const` members, references, or members with no default constructor) → You **must** use an initializer list.

so for complex objects like `std::string`, the get an empty string and for primitives like `int`..., they get garbage values. ([as documented here](https://en.cppreference.com/cpp/language/constructor))

### Runtime Polymorphism, vtable

#### Inheritance (Quick reminder)

Inheritance is one class **absorbing** everything from another class and being able to extend or modify it.

```cpp
class Animal {
public:
    std::string name;
    void eat() { std::cout << "nom\n"; }
};

class Dog : public Animal {
public:
    void bark() { std::cout << "woof\n"; }
};
```

`Dog` now has `name`, `eat()`, AND `bark()`. It _is_ an Animal — you can use a `Dog` anywhere an `Animal` is expected.

```cpp
Dog d;
d.eat();   // inherited from Animal
d.bark();  // Dog's own
```

The memory layout of a `Dog` object looks like this:

```
┌─────────────────────────┐
│   Animal part           │  ← comes first, always
│     std::string name    │
├─────────────────────────┤
│   Dog part              │
│     (nothing extra here)│
└─────────────────────────┘
```

#### The problem that virtual solves

Let's say you have this:

```cpp
class Animal {
public:
    void speak() { std::cout << "...\n"; }
};

class Dog : public Animal {
public:
    void speak() { std::cout << "woof\n"; }
};

class Cat : public Animal {
public:
    void speak() { std::cout << "meow\n"; }
};
```

Now you do this:

```cpp
Dog d;
Cat c;

Animal& a1 = d;
Animal& a2 = c;

a1.speak(); // what prints?
a2.speak(); // what prints?
```

Without `virtual`, both print `"..."`. The compiler sees `Animal&` and locks in `Animal::speak` at compile time. It doesn't care what the actual object is.

```
Compile time decision:
  a1.speak() → Animal::speak()   ← wrong, we wanted Dog::speak()
  a2.speak() → Animal::speak()   ← wrong, we wanted Cat::speak()
```

This is called **static dispatch**.  the function is resolved at compile time based on the declared type, not the actual object.

Add `virtual` and everything changes:

```cpp
class Animal {
public:
    virtual void speak() { std::cout << "...\n"; }
};
```

Now:

```
a1.speak() → Dog::speak()  → "woof"  ✓
a2.speak() → Cat::speak()  → "meow"  ✓
```

The function is now resolved at **runtime** based on what the object actually is. This is **dynamic dispatch**, or runtime polymorphism.

How does C++ know at runtime what the actual type is? That's the vtable.
####  The vtable, what it is

When a class has at least one `virtual` function, the compiler creates a hidden data structure called the **vtable** (virtual table). It's an array of function pointers, one slot per virtual function. It's created **once per class** at compile time and lives in static memory.

```
Animal vtable:
┌─────────────────────────────┐
│ slot[0] → Animal::speak     │
└─────────────────────────────┘

Dog vtable:
┌─────────────────────────────┐
│ slot[0] → Dog::speak        │  ← overwritten
└─────────────────────────────┘

Cat vtable:
┌─────────────────────────────┐
│ slot[0] → Cat::speak        │  ← overwritten
└─────────────────────────────┘
```

If a subclass doesn't override a virtual function, its vtable slot just copies the parent's pointer.

If you add more virtual functions:

```cpp
class Animal {
public:
    virtual void speak();
    virtual void eat();
    virtual void sleep();
};
```

```
Animal vtable:
┌──────────────────────────┐
│ slot[0] → Animal::speak  │
│ slot[1] → Animal::eat    │
│ slot[2] → Animal::sleep  │
└──────────────────────────┘
```

Each virtual function gets its own slot. The slots are in the same order for every class in the hierarchy, that's what makes the lookup work.

#### The vptr, what it is

Every **object** of a class with virtual functions gets a hidden pointer injected by the compiler called the **vptr** (virtual pointer).

The vptr points to the vtable of the object's **actual** class.

```
Dog d;
Cat c;
```

Memory layout:

```
d (Dog object):
┌─────────────────────────────────────────────┐
│ vptr ───────────────────────────────────────┼──→ Dog vtable
│                                             │      slot[0] → Dog::speak
│ (Animal part: name, etc.)                   │
│ (Dog part: any extra members)               │
└─────────────────────────────────────────────┘

c (Cat object):
┌─────────────────────────────────────────────┐
│ vptr ───────────────────────────────────────┼──→ Cat vtable
│                                             │      slot[0] → Cat::speak
│ (Animal part)                               │
│ (Cat part)                                  │
└─────────────────────────────────────────────┘
```

The vptr is always at the very beginning of the object's memory (on most compilers). It's set in the constructor

What happens at a virtual call

```cpp
Animal& a = d;  // base reference to a Dog
a.speak();
```

The compiler generates roughly this machine code:

```
1. a  is a reference — it points to the Dog object in memory
2. read the first bytes of that object → that's the vptr
3. vptr points to Dog's vtable
4. read slot[0] from Dog's vtable → address of Dog::speak
5. call that address
```

In pseudocode:

```cpp
(*a.vptr[0])(a);  // call whatever slot[0] points to, passing the object
```

This lookup happens **every single call**, at runtime. That's the cost.\
one pointer dereference through the vptr, then one pointer dereference through the vtable slot.

With a non-virtual call:

```
// compiler knows a is Animal, speak() is not virtual
// bakes Animal::speak address directly into the binary
// zero indirection, decided at compile time
```

#### Pure virtual and abstract classes

A **pure virtual** function is declared with `= 0`:

```cpp
class Shape {
public:
    virtual double area() = 0;  // pure virtual
};
```

This means:

- `Shape` has no implementation of `area()`
- `Shape` **cannot be instantiated**, it's now an **abstract class**
- Any class inheriting from `Shape` **must** override `area()` or it's also abstract

```cpp
Shape s;        // ERROR: cannot instantiate abstract class
```

```cpp
class Circle : public Shape {
public:
    double area() { return 3.14 * r * r; }  // must provide this
};

Circle c;  // fine
```

The vtable for an abstract class has a null pointer (or a special abort function) in the pure virtual slot, calling it directly would crash, which is why instantiation is forbidden.

```
Shape vtable:
┌─────────────────────────────┐
│ slot[0] → __cxa_pure_virtual │  ← crashes if called
└─────────────────────────────┘

Circle vtable:
┌─────────────────────────────┐
│ slot[0] → Circle::area      │  ← properly overridden
└─────────────────────────────┘
```

#### Virtual destructor, why it matters

This is the thing that we will look at in ex02 when you have `AForm*` pointers:

```cpp
class Animal {
public:
    ~Animal() { }  // NOT virtual
};

class Dog : public Animal {
public:
    std::string* data;
    Dog() { data = new std::string("woof"); }
    ~Dog() { delete data; }
};

Animal* a = new Dog();
delete a;  // ← calls Animal::~Animal() only, Dog::~Dog() never runs
           // data is never deleted → memory leak
```

If the destructor isn't virtual, `delete` on a base pointer only calls the base destructor. The derived destructor never runs, memory leaks.

Fix :

```cpp
class Animal {
public:
    virtual ~Animal() { }  // virtual destructor
};

delete a;  // now calls Dog::~Dog() then Animal::~Animal()  ✓
```

### Inheritance types

public, protected, private

The syntax for all tree :

```cpp
class Dog : public Animal    // most common
class Dog : protected Animal
class Dog : private Animal
```


| Member in Base     | `public` inheritance | `protected` inheritance | `private` inheritance |
| ------------------ | -------------------- | ----------------------- | --------------------- |
| `public` member    | stays `public`       | becomes `protected`     | becomes `private`     |
| `protected` member | stays `protected`    | stays `protected`       | becomes `private`     |
| `private` member   | inaccessible         | inaccessible            | inaccessible          |

## ex00 :

this exercise will further strengthen our cpp knowledge. 

### Exceptions

first new thing to notice here is exceptions, and they are basically a new way for us to handle errors, without exceptions we we would be checking return values in order to handle failure cases.

let's go over how they work and why is it better in this new manner.
#### Why Exceptions

exceptions are essentially a better way of handling errors.]
##### The old way

before the only way signal an error, is to return a specific integer, and check it on return, but this makes for multiple problems, for one callers might just ignore it, leading to bugs and constructors don't have any way to signal an error, if an error happens during construction.

```cpp
// C style — error codes. Ugly and unsafe.
int createBureaucrat(int grade) {
    if (grade < 1)   return -1;  // error: too high
    if (grade > 150) return -2;  // error: too low
    return 0;  // ok
}

// Caller MUST check every time. But they often don't.
int r = createBureaucrat(0);
if (r == -1) handleTooHigh();
if (r == -2) handleTooLow();
```

also as said  `Constructors have no return value. You cannot signal failure from a constructor using a return code. Exceptions are the only clean solution.`

##### The New (CPP) way

the main helpful thing about this new way, is we abstract how error handle even more, the try block is where stuff happen, and in case of any error "thrown", the execution of that code stops right there and then and we jump to the catch block to handle it however we want, and the constructed objects up to that point get destroyed cleanly as well.

```cpp
try {
    Bureaucrat b("Bob", 0);    // throws GradeTooHighException
    doWork(b);                  // ← never reached
    doMoreWork(b);              // ← never reached
}
catch (std::exception& e) {
    // ONE place handles both error types
    std::cout << e.what() << std::endl;
}
// program continues normally here
```

here instead, `Error handling lives in one place. Normal code stays clean. Constructors can signal failure. Local objects always clean themselves up.`

##### What really happens at low level

Three things really happen when an exception is thrown, let's go through them in detail `Throw / unwind / catch`
###### Throw

when we detect a invalid input or any kind of error, we create an exception object, `inherited from the base exception class`, and throw it.\
execution stops right there.

`ofc you can throw any object even primitives like an int and catch them, just to clarify, but it is irrelevant in our case.`

```cpp
Bureaucrat::Bureaucrat(const std::string& name, int g)
    : name(name) {
    if (g < 1)
        throw GradeTooHighException();  // ← fires here
    if (g > 150)
        throw GradeTooLowException();
    grade = g;  // never reached if throw fires
}
```

Note :

```sh
The object `b` in `main()` is never created. However the sub-objects that _were_ already constructed (like `std::string name` in the initializer list) do get their destructors called — C++ guarantees this.
```

###### Unwind

here stack unwinding occurs if any local variables here are declared or constructed objects, or functions, everything gets popped back from the stack till the caller function, which is `main()`

This is Stack unwinding.

Bureaucrat::Bureaucrat() ------------- **throw fires**
someHelper() ------------- **locals destroyed**
main() ------------- **catch found ✓**

Note : 

```sh
If C++ walks all the way back to the top of `main()` and still finds no matching `catch`, it calls `std::terminate()` and the program crashes with an "unhandled exception" message. Always catch.
```

###### Catch

The exception object is bound to the catch parameter. Normal execution continues after the closing brace of the catch block.

```cpp
try {
    Bureaucrat b("Bob", 0);
}
catch (std::exception& e) {
    // e is really a GradeTooHighException object
    // polymorphism routes what() to the right version
    std::cout << e.what() << std::endl;
    // output: "Grade Is Too High."
}
// ← execution continues here, exception is handled
```

```sh
Notice the `&` in `catch (std::exception& e)`. You **always catch by reference**. Catching by value would "slice" the object — you'd lose the subclass's `what()` override and always get the base class message instead.
```

##### Inheritance chain

```mermaid
classDiagram
class std_exception {
+what() const char* 🔵 virtual
}
class GradeTooHighException {
+what() const char* 🟢
}
class GradeTooLowException {
+what() const char* 🟢
}
std_exception <|-- GradeTooHighException : inherits
std_exception <|-- GradeTooLowException : inherits
```

in order to use exceptions in our implementation we need to create two classes that inherit from the exception base class.

- The subject explicitly says your exceptions must be catchable as `std::exception&`. Inheriting from it is what makes that possible

###### Why inherit from std::exception ?

by inheriting from the exception base class, we make sure any `catch (std::exception& e)` block can catch any exception that inherits from it,
the reference `e` holds a `GradeTooHighException` object for example, and using the `vptr` it can call the `what` function we overridden, this is runtime dispatch, or runtime polymorphism.

```cpp
// One catch block handles both exception types:
catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    // For GradeTooHighException → "Grade Is Too High."
    // For GradeTooLowException  → "Grade Is Too Low."
}
```

what if we just made our own object class and thrown it without inheriting.

```cpp
struct BadException {};  // no inheritance

try {
    throw BadException{};
}
catch (std::exception& e) {
    // NEVER runs — BadException is not a std::exception
}
// unhandled exception → std::terminate() → crash
```

it won't work because the reference e only holds classes inherited from it.

###### Catch order matters ?

If you have multiple catch blocks, the most specific must come first. C++ tries them top-to-bottom and stops at the first match.

```cpp
try { /* ... */ }
catch (Bureaucrat::GradeTooHighException& e) { // most specific
    std::cout << "too high: " << e.what() << std::endl;
}
catch (Bureaucrat::GradeTooLowException& e) {
    std::cout << "too low: " << e.what() << std::endl;
}
catch (std::exception& e) {  // catches anything else
    std::cout << "unknown: " << e.what() << std::endl;
}
```

```
If you put `std::exception&` first, it swallows everything and the specific blocks below it will never run. The compiler may warn you about unreachable catch blocks.
```

##### What() explained

the `what()` is a virtual method from the `exception` class that we will override and repurpose, it is declared like this

```cpp
  const char* what() const throw();
```

![](attachment/3019dbd8084b8992c6ddf368c5870891.png)

 - Why `const char*` and not `std::string`?

```cpp
what() /* is called _during_ exception handling. If it tried to allocate a */ `std::string` /* on the heap and that allocation failed — it would throw another exception. That's chaos. A string literal is safe because it lives in */ static memory /* for the entire lifetime of the program. No allocation, no risk.*/
```

 - Why the trailing `const`?

It makes `what()` callable on `const` exception objects. In many catch blocks you'll see:

```cpp
catch (const std::exception& e) {  // note: const ref
    e.what();  // only works if what() is marked const
}
```

-  Why `throw()`?

```cpp
It's a C++98 exception specification. The empty parentheses mean "this function is guaranteed not to throw." It also has to match the base class signature exactly to be a proper override:
```

- Note
in c++98 : `what() const throw()`, what we use
in c++11 : `what() const noexcept` same meaning just cleaner syntax, not available in 98.

##### Why nested classes

here it is all about scope, nested classes is a way to use the created class only within the scope of the object it is defined in, not the global scope

it is excepted, because the structure of the exercises 


| Nested ✓ (what we have)                                                                                                                                                                          | Global ✗ (bad idea)                                                                                                                                                     |
| ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Bureaucrat::GradeTooHighException                                                                                                                                                                | GradeTooHighException                                                                                                                                                   |
| Crystal clear ownership. <br>When Form gets its own<br>`GradeTooHighException`<br>in ex01, they peacefully coexist as `Form::GradeTooHighException`<br>and  `Bureaucrat::GradeTooHighException`. | Name collision in ex01. Both Bureaucrat and Form would try to declare <br>`GradeTooHighException` at global scope. One will shadow the other, or the linker will error. |

##### How to throw and catch exceptions

```cpp
// Inside Bureaucrat's own methods — no prefix needed
throw GradeTooHighException();

// From outside the Bureaucrat class
throw Bureaucrat::GradeTooHighException();

// Catching specifically
catch (Bureaucrat::GradeTooHighException& e) { /* ... */ }

// Catching the base — catches BOTH types (recommended)
catch (std::exception& e) { /* ... */ }
```

### Questions Typically asked for ex00

| Question                                                | Answer                                                                                                                                      |
| ------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------- |
| What does throw do?                                     | Creates an exception object and jumps to the nearest matching catch, destroying local variables along the way (stack unwinding).            |
| Why catch by reference?                                 | Catching by value slices the object — you lose the subclass's `what()` override. A reference keeps the real type alive.                     |
| Why inherit from std::exception?                        | So the exception is catchable with `catch (std::exception& e)`. Without inheritance, that catch block doesn't match.                        |
| What does the throw() at the end of what() mean?        | C++98 exception specification meaning "this function will not throw". Must match the base class signature exactly for the override to work. |
| Why return const char* and not std::string from what()? | what() is called during exception handling. Allocating a std::string could itself throw. A string literal lives in static memory — safe.    |
| Why are exceptions nested inside Bureaucrat?            | Scoping. Form will also have GradeTooHighException in ex01. Nesting prevents name collisions and shows ownership.                           |
| Why does incrementGrade decrease the number?            | Grade 1 is the highest rank. Incrementing means getting a higher rank, so the integer value decreases.                                      |
| What happens if you don't catch an exception?           | C++ calls std::terminate() which crashes the program.                                                                                       |

#### The Actual Exercise Implementation

let's at least explain what happens really in my code for two tests, one valid and one invalid.

Test 1, Valid construction

```cpp
std::cout << "--- Valid Construction & operator<< ---" << std::endl;
try {
    Bureaucrat b("Alice", 75);
    std::cout << b << "\n";
}
catch (std::exception &e) {
    std::cout << e.what() << std::endl;
}
```

**What actually happens:**

```
1. try block entered — C++ registers a "catch landing pad" for this scope

2. Bureaucrat b("Alice", 75) called
   → initializer list runs first:
       name("Alice")   — std::string constructed, value = "Alice"
   → constructor body runs:
       75 < 1  ? no
       75 > 150? no
       grade = 75      — assigned
   → b is fully constructed on the stack

3. operator<< called:
   → b.getName() returns const std::string& to "Alice" — no copy
   → b.getGrade() returns int 75
   → prints: "Alice, bureaucrat grade 75."

4. try block closes — b goes out of scope
   → b.~Bureaucrat() called — std::string name destroys itself
   → stack frame cleaned up

5. catch block: never entered — no exception was thrown
```

Note 
```
A "catch landing pad" is a specific block of compiler-generated assembly or machine code designed to receive control when an exception is thrown. When you enter a `try` block, the compiler registers this landing pad with the operating system's exception unwinding mechanism.
```

```
Once the runtime finds the stack frame that contains your `try` block, it redirects execution to the "landing pad" associated with that scope
```

 Test 2, Grade too high on construction
 
```cpp
std::cout << "--- Grade Too High on Construction ---" << std::endl;
try {
    Bureaucrat b("Bob", 0);
}
catch (std::exception &e) {
    std::cout << e.what() << std::endl;
}
```

**This is where exceptions get real. Step by step:**

```
1. try block entered — landing pad registered

2. Bureaucrat b("Bob", 0) called
   → initializer list runs:
       name("Bob")  — std::string constructed ✓
   → constructor body runs:
       0 < 1 ? YES
       → throw GradeTooHighException()
```

Right here, several things happen simultaneously:

```
a. GradeTooHighException object is created
   (tiny object, just a vptr pointing to GradeTooHighException vtable)

b. The constructor for Bureaucrat is ABANDONED
   grade = g never runs
   b is never fully constructed
   b never exists as a valid object

c. But name was already constructed in the initializer list
   → name.~std::string() is called — its memory is freed
   C++ guarantees sub-objects already built get destroyed

d. Stack unwinding begins:
   C++ walks up the call stack looking for a matching catch

   Call stack at this moment:
   #0  Bureaucrat::Bureaucrat("Bob", 0)    ← throw fired here
   #1  main()                              ← catch lives here

e. C++ reaches main(), checks: does catch(std::exception& e) match
   GradeTooHighException?

   → GradeTooHighException inherits from std::exception
   → YES, it matches

3. catch block entered:
   e is bound to the GradeTooHighException object
   e is typed as std::exception& but the actual object is GradeTooHighException

4. e.what() called:
   → e is std::exception& but what() is virtual
   → CPU reads e's vptr
   → vptr points to GradeTooHighException's vtable
   → vtable slot[0] → GradeTooHighException::what
   → returns "Grade Is Too High."
   → prints: "Grade Is Too High."

5. catch block exits
   GradeTooHighException object is destroyed

6. execution continues after the catch block — program doesn't crash
```

Key thing to lock in: **`b` never exists**. You can't use it after the try block because it was never constructed. The stack has no `b` object on it. The only thing that got constructed and then destroyed was the `std::string name` inside the initializer list.
## ex01 :