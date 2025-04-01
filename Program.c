#include "MemoryArena.h"
#include <Windows.h>

#define GB_64 1ULL << 36
#define MB_10 10 * (1ULL << 20)

typedef struct Person {

	const char* first_name;
	const char* last_name;
	short age;

} Person;

int main()
{
	Arena* arena = arena_alloc(MB_10);

	Person* daniele = arena_push_no_zero(arena, sizeof(Person));
	daniele->first_name = "Daniele";
	daniele->last_name = "Tulli";
	daniele->age = 31;

	Person* giusy = arena_push_no_zero(arena, sizeof(Person));
	giusy->first_name = "Giuseppina";
	giusy->last_name = "Paccione";
	giusy->age = 27;

	Person* francesca = arena_push_no_zero(arena, sizeof(Person));
	francesca->first_name = "Francesca";
	francesca->last_name = "Tulli";
	francesca->age = 27;

	Person* person_popped_out = arena_pop(arena, sizeof(Person));
	printf("Hello, %s %s! You are in the ARENA!!!", person_popped_out->first_name, person_popped_out->last_name);

	arena_release(arena);

	return 0;
}