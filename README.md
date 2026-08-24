# Traian's Extended C Standard Library  | TECSL -> TXL
>Author: [Dobrescu Andrei-Traian](mailto:andrei.dobrescu2402@stud.acs.upb.ro)

## Description

This is an extension/wrapper of the C Standard Library featuring Lists, Maps, Iterators, Strings, A prototype system with Traits, and so on.

## Documentation Standard

For the documentation of certain macros, the type of the expected parameters has been placed in the docs. The standard for the notation is as follows:
| Notation | Meaning |
| ---- | ----- |
| `[type]` | Value of type `type` |
| `<type>` | Type parameter |
| `<type: Trait1 + Trait2>` | Type parameter that implements Trait1 and Trait2 |
| `(expr)` | Expression |
| `(name)` | The name for a new variable |

# Clojures

## TXL Function notation

When we want to define a function, we need to specify its return type, as well as the types of the parameters. And since I found no way to infer the required types (yet), I needed an easier way to write the "type" of a function as a type.

Using the fact that, using some clever macro tricks, we do not need to place `,` between the parameters of a macro if A. the parameter is held between `()` or B. we know what the parameter is going to be (and can be written in the name of a macro) we can define a nice notation for function definitions:

```c
(atype_t, btype_t) $ ret_type_t
```
`atype_t, btype_t, ...` are the types of the parameters and the `ret_type_t` is the return type.

### The `using()` and `$fn()` macro

Using these 2 macros we can use the TXL Function notation. Example:

```c
// Saying we use this function type
using((int, int) $ void);

// The function in question
void uhhh(int a, int b) {
	printf("i want uhh number %d and.. number %d.", a, b);
}

// Using it as a type
$fn((int, int) $ void) test = uhhh;

```

And this works for functions returning functions, taking functions as parameters, etc.

### The `$c()` macro

Using several GCC magic tricks (nested functions and compount statements) we can define functions as expressions, and the later use them. The syntax is:
```c
$fn(fn-type) func = $c((fn-type) {
	/* Function code here */
})
``` 
Example:
```c
$fn((int) $ int) func = $c(((int a) $ int) {
	return a + 1;
});

func(5); // 6
```

Important note: GCC Nested functions do not *move* the values used from outside the function, meaning that outside variables can only be used safely if they outlive any clojure call.

# Pseudo classes
C is a procedural language, meaning we need to work with data and functions separately. Packaging data and its behaviour can be a bit problematic since functions are completely separated from the data meaning it can't be easily found. There can be some standards, like starting all functions related to a singly linked list with `ssl_`. While this is a solution, TXL uses a different, more esoteric approach, and that is to literally "attach" a function to a type, and later any variable with that type can use that function through a specific macro. Example:

```c
/* uint32_t  <-- The type we want to attach a certain function */

/* Our target function. Adds to an int another int */
uint32_t* add(uint32_t* self, uint32_t b) {
	*self += b;
	return self;
}

/* Defining a "virtual table" */
DEFINE_VT(uint32_t, {
	uint32_t* (*add)(uint32_t* self, uint32_t b);
});

/* Creating our "virtual table" */
SET_VT(uint32_t, {
	FUNC(add)
});

/* This is a macro required for methods to work */
#define METHOD$add(...) (add, __VA_ARGS__)

/* We finished attaching a method to a type! Time to use it */

/* We specify what types are going to look for when infering the type */
#define __TYPES GET_TYPES(uint32_t)

uint32_t x = 1;
uint32_t y = 2;
uint32_t z = 3;

/* WTF? */
$(&x,add(y),add(z));

printf("%d", x); // 6
```

I will now briefly explain how the `$()` macro works (you can see how it's done in the `src/impl/proto.h` file as well)

## Virtual tables

While here they are named poorly, these virtual tables are neat packages that store all the functions related to a type. Each type has a custom type of virtual table (you can get the type of the vtable with the `VT_TYPE` macro), thus they can't be used as generic types (For that, there is the `prototype_t` structure, which is detailed at the Generics section).

Therefore, when we want to attach a function to a type, we simply add it as an entry to the virtual table.

Virtual tables need to be both defined, and be set. The macros have been designed for the definition to be set in a header file and the creation in a source `.c` file.

The functions that are defined and attached to a type are recommened to be static, since that allows for very generic names like `add` and `drop` to be reused across types.

Functions that have the same name as in the definition (like in the example above) can be added in the virtual table with the `FUNC` macro, or if there is a different name, they can be added with `FUNC_AS` macro.

To access the virtual table of a type we can use the `VT` macro. With this knowledge we can now detail the `$u()` macro

## The `$u()` macro

This macro is used for calling "static methods" from our virtual table. These are usually called with `$u()` because they do not take `self` as a first parameter. These are usually methods that create the attached type (kind of like constructors), but they can really be anything. Example:

```c
/* uint32_t  <-- The type we want to attach a static function */

/* Our target function. Adds two ints, no self */
uint32_t add(uint32_t a, uint32_t b) {
	return a + b;
}

/* Defining a "virtual table" */
DEFINE_VT(uint32_t, {
	uint32_t (*add)(uint32_t a, uint32_t b);
});

/* Creating our "virtual table" */
SET_VT(uint32_t, {
	FUNC(add)
});

uint32_t x = 1;
uint32_t y = 2;

$u(uint32_t,add(x,y)); // 3
```

`$u()` takes as a first parameter the type, than the function we want to call. This gets literally transformed to:
```c
VT(uint32_t).add(x,y);
```

The `$u()` supports chaining calls (but the static method must return a pointer to the object in question).

## Type inferal

One of first the challanges with adding methods to C is how to obtain the type of a variable. Or better yet, I want to get the virtual table of the type of a variable. Since we saw we can get the virtual table from the type. We need to perform some GCC magic.

### The `_Generic` function

The main workhorse behind type inferal is the `_Generic` function, which runs at compile time, and depending on the literal type of a provided expression, "returns" another expression. Example:

```c
#define INFER(x) _Generic((x), uint32_t : VT(uint32_t), str_t: VT(str_t))

uint32_t x = 5;
INFER(x) // returns VT(uint32_t)
str_t y = "damn son";
INFER(y) // returns VT(str_t)
```
You can read more [here](https://en.cppreference.com/w/c/language/generic).

### Localizing inferal with `__TYPES`

To make the system more flexible and expandable, the types added in the `INFER()` macro are specified in the file you are doing the inferals. It is not recommended to add inferals in `.h` files, and only in source `.c` files.

The `__TYPES` definitions stores the literal content that will be added in the `_Generic()` function whenever calling `INFER()`. Calling the `INFER()` macro without a defined `__TYPES` causes a compilation error. The content of `__TYPES` can be easily created with the `GET_TYPES` macro, which by default takes a maximum of 11 types (actual number can be increased without penalties). This can be manually expanded to any number or `__TYPES` can be broken into sections of the code (by means of `#define` and `#undef`)

## Self

> Note: Self insertion uses a set of macros taken from [this post](https://stackoverflow.com/questions/21474061/detect-presence-or-absence-of-arguments-in-a-c-macro).	

The final step to reaching full methods is a way to insert `self` as the first parameter when calling a method. The main trick that solves this problem is the `METHOD$` class of macros.

These macros are used to separate the name of a method from its arguments. Example:

```c
#define METHOD$add(...) (add, __VA_ARGS__)
/* METHOD$ can be appened to the start and that calls the above. */
METHOD$add(5, 10) // (add, 5, 10)
```

by attaching the result of the above macro with the `__METHOD_NAME` or the `__METHOD_ARGS` macros we basically separated the name and the arguments of the method.

With this we can build a macro `SELF_INSERT()` that inserts a parameter in the function arguments.

```c
/* The last 3 parameters can be obtained from add(5,10) using METHOD$ class macros */
SELF_INSERT(3, add, 5, 10) // add(3, 5, 10)
```

Using this, we can now detail the `$()` macro.

## The `$()` macro

This macro infers the type of the first parameter, gets the virtual table, then calls the `SELF_INSERT` function to insert self in the parameters, then call the function.

The `$()` macro can be chained, meaning that if multiple functions are passed, they work as intended:
```c
atype_t a;
/* First parameter must be atype_t* */
$(&a,b(),c())
// BECOMES
$($(&a,b()),c())
// BECOMES
VT(btype_t).c(VT(atype_t).b(a))
```

Since all of this is performed at compile time, there *should* be no runtime penalties.

# Generics

The above mentioned pseudo classes have one big drawback, and that is their reliance on knowing the exact type of something at compile time. While this works for most stuff, once you want to build code that supports multiple types, all of the above becomes useless. Therefore there must be a system that somehow ties functions to specific functions that multiple objects have in common. All of the common functions are placed in the `prototype_t`.

## The Prototype - `prototype_t`

Most data structures need to own their data, meaning that it should not reference it from somewhere else, as that can lead to big issues with lifetimes and mutability. Not only that, but some basic operations like comparisons, hashing, addition, etc that might be implemented differently across objects, needs to be used by such data structures.

For a data structure to own it's data, either the user has to promise that the variable outlives the data structure and that it wont do shady stuff with it (which cannot be guaranteed by the language), or you do a full copy of the data.

This requires that data structures need specific functions upon creation that specify how to handle the generic data type. This leads to a data structure having several function pointers in the definition, which in my opinion is horrible.

Prototypes are here to save the day. They are a read-only, static struct that store general behaviour (aka function pointers) for a specific data type.

A prototype, with a similar definition:

```c
typedef struct {
	// size_t trait_code;
	size_t size;
	hash_t (*hash)(void *);
	cmp_t (*cmp)(void *, void *);
	void (*free)(void *);
	void (*clone)(void *, void *);
	/* ... and many more functions */
} prototype_t;
```

Stores pointers to all the possible operations a type needs to be used effectively with data structures.

The prototype of each type is stored in a static variable in a function. When the function first gets called, it initializes the prototype to the right values. From that point going forward, it will only return a pointer to that prototype, effectively creating a singleton.

A singleton is created for each type (to circumvent the no global variables restriction from SDA), with the help of the macro:

```c
DEFINE_PROTO(type_t, TRAIT1, TRAIT2, ...)
```

And can be exposed outside of the translation unit with: 
```c
DECLARE_PROTO(type_t)
```
the `DECLARE_PROTO` macro is called automatically with the `DEFINE_VT` macro

A prototype must be created only once per type.

The prototype of a type can be accessed using:

```c
PROTOTYPE(type_t)
```
which gives access to all implementations of the traits.

The prototype also includes the size of the data, so passing the data size around for creating, or cloning generic data, is now replaced by sending the prototype.
This means that now data structures need to store only the prototype of the data inside.

The prototype guaratees that the size filed is valid in each prototype.

The data structures in this homework use prototypes, and therefore all types who interact with these structures need to have a prototype.

### Prototype problems
One of the big problems is that with many many traits the `prototype_t` struct becomes very large, and will consume a lot of memory. Not only that, but it is not a very flexible system, in the sense that defining new traits is not very straightforward.

## Traits

Traits guarantee that a specific type can suffer a certain operation, perform certain tasks, or requires special attention. They act like Rust's Traits (or Java Interfaces).

### How it works

When a type implements a trait, it just means that the prototype of the type has functions defined related to that trait.

```c
/* We want to know how to hash test_t */
typedef struct {
    int some_data;
} test_t;

/* The name of this function tells us how to hash.
Because it uses the naming convention, test_t implements the trait Hash. */
hash_t test_t_hash(void* data) {
    /* How to hash test_t */
}
/* We construct the Prototype for test_t, marking that we want to implement HASH */
DEFINE_PROTO(test_t, HASH)

test_t important_data = (test_t) {.some_data = 69 };

/* We just hashed `important_data`! */
hash_t hash = PROTOTYPE(test_t)->hash(&important_Data);
```

In the above example, the struct `test_t` implements the Hash trait, because there is a function named `hash` with `test_t` in front of it, and the prototype uses that name to add the specific function to it automatically.

> Note: Some trait creation directives might not require functions to be implemented, and automatically implement them. (Like FREE_DEFAULT)

In the source code there will be examples where some functions have 2 names, so their naming follows both the trait naming convention and the API naming convention.

#### The DEFINE_PROTO macro

This is a variadic macro that creates a singleton with the prototype we need for a type, automatically implementing the traits we passed to it. This is done by using a clever method described [here](https://stackoverflow.com/a/11994395), by basically overloading the macro depending on the number of arguments; With this we can do a for loop on the passed parameters. For each Trait argument we call the `IMPL(trait)` macro that tells what functions are required and how to create them for that trait.

### The HASH Trait

Required function: `hash_t #hash(void*)`

This trait signifies that the object can be hashed into a `hash_t`

### The CLONE Trait

Required function: `void #clone(void*, void*)`

The `#clone` function deep copies the data from the 2nd `void*` parameter to the first.

Some scenarios require that objects just perform shallow copies (like when hashmaps just need to store a reference to the data), or the deep copy is identical to the shallow copy (in case of `int`). There is a default implementation macro `impl_default_clone(type)` that implements shallow copy for the specified type.

### The FREE Trait

Required function: `void #free(void*)`

This trait specifies how an object should be freed. In the case of data stored on the stack, the function pointer to this function can be set to `NULL`, to signal that this value should not be freed. (In the case of `int`)

#### The FREE_DEFAULT Implementation

When initializing the prototype for a type, we might not need a free function, so to mark that we know it should not be freed, the `FREE_DEFAULT` keyword automatically implements the FREE Trait and marks the data as to not be freed by the data structures

### The CMP Trait

Required function: `cmp_t #cmp(void*, void*)`

The `#cmp` function compares the values at the 2 passed parameters and returns a `cmp_t` value, signaling if the first paramater is `EQUAL`, `BIGGER` or `SMALLER` than the second parameter. 

### The ITER Trait

Required functions: `iter_t #iter_new(type_t)`, `void #iter_next(iter_t*)`

Types that implement `Iter` can be iterated over with `for_iter`

### The ITER_REV Trait

Required functions: `iter_t #iter_rev(type_t)`, `void #iter_prev(iter_t*)`

Types that implement `IterRev` can be iterated over with `for_iter_rev`

#### The ITER_FULL Implementation

`ITER_FULL` implements both `ITER` and `ITER_REV`, and is preffered this is used over implementing `ITER` and `ITER_REV` because of the double `iter_get` implementation.

### The SET Trait

Required functions and traits: `ITER`, `void #set_add(void *, void *)`, `void #set_remove(void *, void *)`, `size_t #set_has(void *, void *)`

The `SET` Trait describes that an object is a set of unique values. It guarantees that we can add remove and check that an element exists in that set, as well as iterate over it. These guarantees allow these objects to use set operations.

## Implementation

### Integer Implementations

This library automatically implements `HASH`,`CMP`,`CLONE`,`FREE` for Intergers (aka `uint32_t`) 

### C Strings - `str_t`

C Strings represent just an abstraction over `char*` for the trait implementations of `str_t`. They most of the time represent a C String (string ending in `\0`) but in some cases it might be used as a buffer of unknown size, or a string view (this issue has not been handled yet in this implementation)

### Strings - `string_t`

Strings represent a heap allocated mutable string, which has a buffer and a length attached (same params as a `vec_t` for ease of implementation).

### References - `ref_t`

References represent just an abstraction over `void*` for the trait implementations of `ref_t`. References are used when we want to store a pointer to a valid variable that we do not want to clone or free, making the data structure non-owning.

### Vectors - `vec_t`

The vector implementation is a generic resizable array with a rich API.

The vector implementation grows / shrinks with a rate of 1.5.

Notable API functions are the `vec_insert()` and `vec_remove()` functions that insert/remove elements anywhere in the vector like a list.


#### The `VEC_INDEX_OF()` macro

This is a macro on purpose and not a function because it requires as a parameter an expression to check against, and uses a restricted keyword `el` to access an element from the vector. 

If it was a function, then passing an expression like that would be impossible, and a function would need to be used. With functions come another issue, as we would need to implement Dependency Injection in C (~~which from what I read online, global variables are required~~ clojures can solve this issue, but it hasnt been implemented yet).

### Lists - `dlist_t` and `list_t`

The list folder has 2 implementations for a generic doubly and singly linked list with a rich API.

When working with lists I tried a lot to hide nodes from usage, but it is very useful when it comes to efficiency.

`dlist_t` implements `Iter` and `IterRev`, while `list_t` implements just `Iter`.

### Queues - `queue_t`

The queue is a Singly Linked List but you can only add at the end and remove at the front. It also owns its data.

### Hashmap - `hmap_t`

The Hashmap is a Dynamic Array of `list_t`. It can resize when it fills up. The Load Factor is set to `0.72`.

`hmap_t` does not implement `Iter` or `IterRev` because adding or removing elements in the hashmap can completely change the order of elements. (And because I would also need to change a lot in the `iter_t` struct for this to work).

### LinkedHashMap - `lmap_t`

The Ordered Map has a normal hashmap and a Doubly Linked List inside that keeps the order of the elements and keeps the `O(1)` access time. The order of insertion is kept in the hashmap as insertion is done only at the end. 
Because `lmap_t` has a `dlist_t` to keep the order, `lmap_t` implements `Iter` and `IterRev`.

The `lmap_t` API exposes a way to get and remove the first element in the list of the `lmap_t`.

### Hashset - `hset_t`

The Hashset acts like a hashmap but it has no values stored, meaning it has only keys. This makes the hashset by design have unique values. This property makes it perfect for storing a collection of items, because most of the basic operations with collections (add, remove, has) are all O(1). While it does not keep an order.

Since the Hashset implements `SET` it can use the set operations with Treesets.

### Iterator - `iter_t`

The `iter_t` struct represents the state of an iteration over an iterable object. Iterable objects require to have some methods attached to them that define the creation of an `iter_t` and how to move to the next or previous element.

It is used in the `for_iter` and `for_iter_rev` macros.

#### The `for_iter`/`for_iter_rev` macro

The `for_iter`/`for_iter_rev` macros are a direct method of iterating over any object that implements `ITER` or `ITER_REV`. They abstract the way of how they are iterated over, and greatly improves readability, as they act like `for-in` loops from high-level languages.

They take in the type of the iterable, a name for the iterable in the for loop (which is reality adds an underscore to the name to bypass an error in the style checker of the first homework, and with the power of macros, the user should not know that), and the iterable.

#### The `ITER_VAL` macro

This macro gets the current element that the iterator is at. Since iterators can iterate over different things (like list nodes, or tree nodes, or vector elements) getting the current elements changes with the iterable. For creating a generic "iterator get" operation requires the need to store the prototype of the iterable in the for (so we can access a `iter_get` function that is implemented with the data type), which leads to either storing it in the iterator, or creating a new variable in the for loop. We went with the latter, since it was easier to implement and change, creating the `iter_wrapper_t` object.