#include "MemoryArena.h"
#include <Windows.h>

Arena* arena_alloc(size_t cap)
{
	if (cap == 0) {
		fprintf(stderr, "Memory not allocated because it was requested with Zero capacity\n");
		ExitProcess(-1);
	}

	Arena* arena = (Arena*)malloc(sizeof(Arena));

	if (arena == NULL) {
		fprintf(stderr, "Failed to allocate memory for Arena\n");
		ExitProcess(-1);
	}

	arena->buffer = VirtualAlloc(NULL, cap, MEM_RESERVE, PAGE_NOACCESS);

	if (arena->buffer == NULL) {
		fprintf(stderr, "Failed to allocate memory for Arena buffer\n");
		free(arena);
		ExitProcess(-1);
	}

	arena->capacity = cap;
	arena->position = 0;

	return arena;
}

void arena_release(Arena* arena)
{
	if (arena == NULL) {
		printf("Arena is already null\n");
		return;
	}

	if (arena->buffer == NULL) {
		printf("Arena buffer is already null\n");
	}

	VirtualFree(arena->buffer, 0, MEM_RELEASE);
	free(arena);
}

size_t arena_pos(Arena* arena)
{
	return arena->position;
}

void* arena_push_no_zero(Arena* arena, size_t size)
{
	if (size == 0) {
		fprintf(stderr, "Memory not allocated because it was requested with Zero size\n");
		ExitProcess(-1);
	}

	if (arena == NULL) {
		fprintf(stderr, "Arena is null\n");
		ExitProcess(-1);
	}

	if (arena->buffer == NULL) {
		fprintf(stderr, "Arena buffer is null\n");
		ExitProcess(-1);
	}

	if (arena->position + size > arena->capacity) {
		fprintf(stderr, "Arena overflow\n");
		ExitProcess(-1);
	}

	void* mem_to_commit = (char*)arena->buffer + arena->position;

	VirtualAlloc(mem_to_commit, size, MEM_COMMIT, PAGE_READWRITE);

	arena->position += size;

	return mem_to_commit;
}

void* arena_push(Arena* arena, size_t size)
{
	void* ptr = arena_push_no_zero(arena, size);

	if (ptr == NULL) {
		fprintf(stderr, "Failed memory allocation");
		ExitProcess(-1);
	}

	memset(ptr, 0, size);

	return ptr;
}

void* arena_pop_to(Arena* arena, size_t pos)
{
	if (arena == NULL) {
		fprintf(stderr, "Arena is null\n");
		ExitProcess(-1);
	}

	if (arena->buffer == NULL) {
		fprintf(stderr, "Arena buffer is null\n");
		ExitProcess(-1);
	}

	if (pos > arena->position) {
		fprintf(stderr, "Requested position is out of bounds\n");
		ExitProcess(-1);
	}

	arena->position = pos;
	return (char*)arena->buffer + arena->position;
}

void* arena_pop(Arena* arena, size_t size)
{
	if (size == 0) {
		fprintf(stderr, "Failed to pop element because it was requested with Zero size\n");
		ExitProcess(-1);
	}

	if (arena == NULL) {
		fprintf(stderr, "Arena is null\n");
		ExitProcess(-1);
	}

	if (arena->buffer == NULL) {
		fprintf(stderr, "Arena buffer is null\n");
		ExitProcess(-1);
	}

	if (size > arena->position) {
		fprintf(stderr, "Cannot pop more than the current position\n");
		ExitProcess(-1);
	}

	arena->position -= size;
	return (char*)arena->buffer + arena->position;
}

void* arena_clear(Arena* arena)
{
	if (arena == NULL) {
		fprintf(stderr, "Arena is already null\n");
		return NULL;
	}

	if (arena->buffer == NULL) {
		fprintf(stderr, "Arena buffer is already null\n");
		return NULL;
	}

	arena->position = 0;

	return arena;
}

/*
void arena_set_auto_align(Arena* arena, size_t align)
{
	if (align == 0)
		return;

	arena->alignment = align;
	arena->position = (arena->position + arena->alignment - 1) & ~(arena->alignment - 1);
}

void* arena_push_aligner(Arena* arena, size_t alignment)
{
	if (alignment == 0) {
		fprintf(stderr, "Memory not allocated because it was requested with Zero size\n");
		ExitProcess(-1);
	}

	if (arena == NULL) {
		fprintf(stderr, "Arena is null\n");
		ExitProcess(-1);
	}

	if (arena->buffer == NULL) {
		fprintf(stderr, "Arena buffer is null\n");
		ExitProcess(-1);
	}

	size_t position_aligned = (arena->position + alignment - 1) & ~(alignment - 1);

	arena->position = position_aligned;

	return (char*)arena->buffer + position_aligned;
}
*/
