#include <stdlib.h>
#include <stdio.h>

typedef struct Arena {

	void* buffer;
	size_t capacity;
	size_t position;
	size_t alignment;

} Arena;

Arena* arena_alloc(size_t cap);
void arena_release(Arena* arena);
void arena_set_auto_align(Arena* arena, size_t align);

size_t arena_pos(Arena* arena);

void* arena_push_no_zero(Arena* arena, size_t size);
void* arena_push_aligner(Arena* arena, size_t alignment);
void* arena_push(Arena* arena, size_t size);

void* arena_pop_to(Arena* arena, size_t pos);
void* arena_pop(Arena* arena, size_t size);
void* arena_clear(Arena* arena);
