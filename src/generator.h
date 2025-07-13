#ifndef GENERATOR_H
#define GENERATOR_H

char* get_character_set(int flags);
char* get_string(uint64_t length, char* character_set, uint8_t set_length);
void generate(uint64_t length, uint64_t repeat, uint8_t flags, int save_fd);

#endif
