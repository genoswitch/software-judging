/* Flashloader size (pico uses 4k blocks) */
__LOADER_SIZE = 1 * 4k;

/* Flashloader variable (extern in flashloader.c) */
__APPLICATION_START = 0 + __LOADER_SIZE;

__BUILD_INCLUDES_FLASHLOADER = 1;