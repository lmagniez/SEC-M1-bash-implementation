#ifndef STACK
#define STACK

#define STACK_SIZE 1024
#define newstack(type, name) type s##name[STACK_SIZE], **s##name##top = s##name

#define pop(name) (*(--s##name##top))
#define push(name, element) (*(s##name##top++) = (element))
#define empty(name) (s##name##top == s##name)
#define size(name) (s##name##top - s##name)

#endif