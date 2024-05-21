# Traian's Extended C Standard Library  | TECSL -> TXL
>Author: [Dobrescu Andrei-Traian](mailto:andrei.dobrescu2402@stud.acs.upb.ro)

## Description

This is an extension/wrapper of the C Standard Library featuring Lists, Maps, Iterators, Strings, A prototype system with Traits, and so on.

#### TODO:
 - Graph
- ITER_VAL generic
- kv pair redo



- TreeSet can be used for storing friends of a user. Iter and IterRev can be implemented for a TreeSet
	- Start from Left most Leaf
	- Iterate through the next elements. Get the next element with the following rules:
		- If has Right SubTree, go to Successor.
		- If no Right SubTree & is Left Child, go to Parent.
		- If no Right SubTree & is Right Child, go to First Left Ancestor 
	- Iteration will be stopped when First Left Ancestor is NULL (at step 3 of iteration)

Operation|HashSet         |   TreeSet|
| ---- | ----- |----- |
| Sorted Insert| O(n)       | O(logn)|
| Unsorted Insert| O(1)       | N/A|
| Remove| O(1)       | O(logn)|
| Has   | O(1)       | O(1)|
| Sort  | O(n^2)   | N/A |
| Reunion  | O(n+m-k)   | O((n+m-k)log(n+m-k))|
| Intersection  | O(k)   | O(klogk)|


> n = card(A) 
> m = card(B) 
> k = card(A ∩ B) 

## Documentation Standard

For the documentation of certain macros, the type of the expected parameters has been placed in the docs. The standard for the notation is as follows:
| Notation | Meaning |
| ---- | ----- |
| `[type]` | Value of type `type` |
| `<type>` | Type parameter |
| `<type: Trait1 + Trait2>` | Type parameter that implements Trait1 and Trait2 |
| `(expr)` | Expression |
| `(name)` | The name for a new variable |


## The Prototype - `prototype_t`

Most data structures need to own their data, meaning that it should not reference it from somewhere else, as that can lead to big issues with lifetimes and mutability. Not only that, but some basic operations like comparisons, hashing, addition, etc that might be implemented differently across objects, needs to be used by such data structures.

For a data structure to own it's data, either the user has to promise that the variable outlives the data structure and that it wont do shady stuff with it (which cannot be guaranteed by the language), or you do a full copy of the data.

This requires that data structures need specific functions upon creation that specify how to handle the generic data type. This leads to a data structure having several function pointers in the definition, which in my opinion is horrible.

Prototypes are here to save the day. They are a read-only, static struct that store general behaviour (aka function pointers) for a specific data type.

A prototype, with the definition below:

```c
typedef struct {
	// size_t trait_code;
	size_t size;
	hash_t (*hash)(void *);
	cmp_t (*cmp)(void *, void *);
	void (*free)(void *);
	void (*clone)(void *, void *);
} prototype_t;
```
Stores pointers to all the possible operations a type needs to be used effectively with data structures.

The prototype of each type is stored in a static variable in a function. When the function first gets called, it initializes the prototype to the right values. From that point going forward, it will only return a pointer to that prototype, effectively creating a singleton.

A singleton is created for each type, with the help of the macro:

```c
DEFINE_PROTO(type_t, TRAIT1, TRAIT2, ...)
```

And can be exposed outside of the translation unit with: 
```c
DECLARE_PROTO(type_t)
```

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


## Implementation

### Integer Implementations

This library automatically implements `HASH`,`CMP`,`CLONE`,`FREE` for Intergers (aka `uint32_t`) 

### Strings - `str_t`

Strings represent just an abstraction over `char*` for the trait implementations of `str_t`.

### References - `ref_t`

References represent just an abstraction over `void*` for the trait implementations of `ref_t`. References are used when we want to store a pointer to a valid variable that we do not want to clone or free, making the data structure non-owning.

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

### Iterator - `iter_t`

The `iter_t` struct represents the state of an iteration over an iterable object. Iterable objects require to have some methods attached to them that define the creation of an `iter_t` and how to move to the next or previous element.

It is used in the `for_iter` and `for_iter_rev` macros.

#### The `for_iter`/`for_iter_rev` macro

The `for_iter`/`for_iter_rev` macros are a direct method of iterating over an Iterable object. They abstract the way of how they are iterated over, and greatly improves readability, as they act like `for-in` loops from high-level languages.

They take in the type of the iterable, a name for the iterable in the for loop (which is reality adds an underscore to the name to bypass an error in the style checker of the first homework, and with the power of macros, the user should not know that), and the iterable.
