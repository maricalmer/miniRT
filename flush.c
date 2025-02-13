# include <stdlib.h>
#define CACHE_FLUSH_SIZE 100000000  // 100 MB

void flush_cache() {
    char *flush_array = malloc(CACHE_FLUSH_SIZE);
    if (!flush_array) return;

    for (size_t i = 0; i < CACHE_FLUSH_SIZE; i++) {
        flush_array[i] = i % 256;  // Fill with arbitrary values
    }

    free(flush_array);
}

int main(void)
{
	flush_cache();
}