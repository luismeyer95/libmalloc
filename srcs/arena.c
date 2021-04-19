#include <libft_malloc.h>

inline void try_init_state()
{
	static pthread_key_t key;

	pthread_mutex_lock(&malloc_mtx);
	if (!keyptr)
	{
		pthread_key_create(&key, NULL);
		init_arenas(arenas);
		keyptr = &key;
	}
	pthread_mutex_unlock(&malloc_mtx);
}

inline void init_arenas(t_arena *arenas)
{
	for (int i = 0; i < ARENA_COUNT; ++i)
	{
		pthread_mutex_init(&(arenas[i].arena_mtx), NULL);
		arenas[i].heap_lst = NULL;
		arenas[i].initialized = 0;
	}
}

inline void foreach_arena_mutex(int (*lockfunc)(pthread_mutex_t*))
{
	int count = 0;
	for (int i = 0; i < ARENA_COUNT; i = (i + 1) % ARENA_COUNT)
	{
		if (lockfunc(&(arenas[i].arena_mtx)) == 0)
		{
			count++;
			if (count == ARENA_COUNT)
				return;
		}
	}
}

inline t_arena *lock_arena()
{
	t_arena *arena = pthread_getspecific(*keyptr);
	if (arena == NULL)
		return trylock_and_assign_arena();
	else
	{
		pthread_mutex_lock(&arena->arena_mtx);
		return arena;
	}
}

inline t_arena *trylock_and_assign_arena()
{
	for (int i = 0; i < ARENA_COUNT; i = (i + 1) % ARENA_COUNT)
	{
		if (pthread_mutex_trylock(&(arenas[i].arena_mtx)) == 0)
		{
			arenas[i].initialized = 1;
			pthread_setspecific(*keyptr, &arenas[i]);
			return &arenas[i];
		}
	}
	return NULL;
}
