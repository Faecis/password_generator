#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include <unistd.h>


/**
 * Generates a character set string based on the provided flags.
 * 
 * @param flags Bitmask indicating which character groups to include:
 *              0b1000 - Numbers, 0b0100 - Uppercase, 0b0010 - Lowercase, 0b0001 - Special Characters.
 * @return Dynamically allocated string containing the selected characters. CALLER MUST FREE THE MEMORY.
 */
char* get_character_set(int flags) {
    const char* character_sets[] = {
        "0123456789",                       // Numbers
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",       // Uppercase
        "abcdefghijklmnopqrstuvwxyz",       // Lowercase
        "!@#$%^&*()-_=+[]{};:,.<>/?\\|`~"   // Special Characters
    };

    // Count the buffer size needed for the character set based on the flags.

    uint8_t size = 0;
    for (int i = 0; i < 4; i++) {
        if (flags & (1 << (3-i))) size += strlen(character_sets[i]);
    }

    // Allocate memory for the character set.
    
    char* characters_set = (char*)calloc(size+1, sizeof(char));
    if (characters_set == NULL) {
        fprintf(stderr, "ERROR - Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    // Fill the character set based on the flags.

    char* ptr = characters_set;
    for (int i = 0; i < 4; i++) {
        if (flags & (1 << (3-i))) ptr = stpcpy(ptr, character_sets[i]);
    }

    return characters_set;
}


/**
 * Generates a random string (password) of the specified length using the given character set.
 * 
 * @param password_length Length of the password to generate.
 * @param character_set   String containing all possible characters to use.
 * @param set_length      Length of the character_set string.
 * @return Dynamically allocated password string. CALLER MUST FREE THE MEMORY.
 */
char* get_string(uint64_t password_length, char* character_set, uint8_t set_length) {
    // Allocate memory for the password.

    char* password = calloc(password_length+1, sizeof(char));
    if (password == NULL) {
        fprintf(stderr, "ERROR - Memory allocation failed.\n");
        free(password);
        exit(EXIT_FAILURE);
    }

    // Generate the password using random numbers.

    for (uint64_t i = 0; i < password_length; i++) {
        uint8_t random = 0;
        if (getrandom(&random, sizeof(random), 0) < 0) {
            fprintf(stderr, "ERROR - Failed to get random number.\n");
            free(password);
            exit(EXIT_FAILURE);
        }

        password[i] = character_set[random % set_length];
    }

    return password;
}


/**
 * Generates and outputs one or more passwords based on the provided parameters.
 * 
 * @param password_length Length of each password to generate.
 * @param repeat          Number of passwords to generate.
 * @param flags           Bitmask for character set selection (see get_character_set).
 * @param save_fd         File descriptor to save output to (if > 0).
 */
void generate(uint64_t password_length, uint64_t repeat, uint8_t flags, int save_fd) {
    // Get the character set based on the flags.

    char* character_set = get_character_set(flags);
    if (character_set == NULL) {
        fprintf(stderr, "ERROR - Failed to get character set.\n");
        exit(EXIT_FAILURE);
    }
    uint8_t set_length = strlen(character_set);

    // Print the character set used for password generation.

    for (uint64_t i = 0; i < repeat; i++) {
        char* password = get_string(password_length, character_set, set_length);
        if (password == NULL) {
            fprintf(stderr, "ERROR - Failed to generate password.\n");
            exit(EXIT_FAILURE);
        }

        if (save_fd > 0) {
            write(save_fd, password, password_length);
            write(save_fd, "\n", 1);
        }
        fprintf(stdout, "%s\n", password);

        free(password);
    }

    free(character_set);

    return;
}
