RULES.md
========


Rules
======

1. If two functions have the same name, 
   the second occurence of this function
   gets suffix _X. 

```
int f(int a) { return a; }
int f(floor a) { return a; }
```
into
```
int f(int a) { return a; }
int f_2(floor a) { return a; }
```

2. Namespace is prefixed into X__

```
namespace N {
void f() { }
}
```
into
```
void N__f() { }
```

3. Template arguments get translated into macro functions

```
template<class A>
A f(A *a) { return *a; } 
```
into
```
#define dec_f(N,A)   A N(f) (A*);
#define def_f(N,A)   A N(f) (A *a) { return a; }
```

4. Class functions get translated into macro functions

```
class A {
	int private;
	int f() { return private; }
}
```
into
```
struct A {
	int private;
};
int f(struct A * restrict this) { return A->private; }
```

5. Constructor and destructors gets suffix _constructor and _destructor

```
class A {
	int private;
	A() : private(0) {}
	A2(int a) { private = a; }
	~A() {}
};
```
into
```
struct A { 
	int private;
};
struct A * A__constructor() 
{ 
	struct A * restrict this = malloc(sizeof(struct A)); 
	if ( !this ) { return NULL; }
	this->private = 0;
	return this;
}
struct A * A__constructor_2(int a) 
{ 
	struct A * restrict this = malloc(sizeof(struct A)); 
	if ( !this ) { return NULL; }
	this->private = a;
	return this;
} 
void A__destructor(struct A ** restrict this) 
{
	free(*this);
	this = NULL;
}
```



```
template<class A, class B> class C {
	A public;
	B private;
	C() : public(0) , private(0) { }
	~C() {}
	A getPublic() { return public; }
	B getPrivate() { return private; }
	void setPublic(A public) { this->public = public; }
	void setPrivate(B private) { this->private = private; }
}
```
into
```
#define dec_C(N,A,B) \
struct N(C) { \
	int public; \
	int private; \
}; \
struct N(C) * N(C__constructor)() 
{ 
	struct N(C) * restrict this = malloc(sizeof(struct C)); 
	if ( !this ) { return NULL; }
	this->public = 0;
	this->private = 0;
	return this;
}
void N(C__decstructor)(struct N(C) ** restrict this)
{
	free(*this);
	this = NULL;
}
A N(C__getPublic)(struct N(C) * restrict this) { return this->public; }
B N(C__getPrivate)(struct N(C) * restrict this) { return this->private; }
void N(C__setPublic)(struct N(C) * restrict this, A public) { this->public = public; }
void N(C__setPrivate)(struct N(C) * restrict this, B private) { this->private = private; }
```

```
std::vector<int> a(5,1);
```
into
```
#define N_std__vector(name) int_ ## N
dec_std__vector(N_std__vector,int);
def_std__vector(N_std__vector,int);
#undef  N_std__vector

int_std__vector *a = int_std__vector__constructor_3(5,1);
```

5. new delete

```
int *a = new int[5];
delete(a);
```
into
```
int *a = malloc(sizeof(int)*5);
free(a);
```

6. Return type deduction for functions

```
auto square(int n) 
{
    return n * n;
}
```
into
```
#define square(N,auto0)
auto0 square(int n)
{
	return n*n;
}
```

7. Generic lambdas

```
auto lambda = [](auto a, auto b) { return a * b; };
```
into
```
#define def_lambda_lambda(N,auto0,auto1,auto2) \
	auto0 N(lambda_lambda)(auto1 a, auto2 b) { return a * b; };
#define dec_lambda(N,auto0,auto1,auto2)
	auto0(*N(lambda))(auto1,auto2) = N(lambda_lambda);
```

8. Extended capturing in lambdas
```
auto timer = [val = system_clock::now()] { return system_clock::now() - val; };
```
into
```
#define def_lambda_timer(N,auto0) \
auto0 N(lambda_times)() { return system_clock__now() - val; }
```

Author
=======
 Created on: 23 paź 2017
     Author: kamil
   Licensed: Jointly under BSD License and Beerware License.

     