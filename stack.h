#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

/**
 * Declaration of a stacknode_t type
 */
typedef struct stacknode_t {
  char* data;
  struct stacknode_t *next;
} stacknode_t;

/**
 * Declaration of a stack_t type
 */
typedef struct stack_t {
  stacknode_t *top;
  int size;
} stack_t;

stack_t *initstack();
void* pop(stack_t* s);
void push(stack_t* s, void* item);
void freestack(stack_t* s);
void printstack(stack_t* s);

#endif
