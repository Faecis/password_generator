#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "generator.h"


/**
 * Program entry point.
 * Parses command-line arguments, configures password generation options,
 * and outputs generated passwords to stdout or a file.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of argument strings.
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error.
 */
int main(int argc, char** argv) {
    fprintf(stdout, "Pasword Generator\n\n");

    // Check if there are the correct number of arguments.

    if (argc < 4 || argc > 5) {
        fprintf(stderr, "ERROR - Invalid number of arguments.\n");
        fprintf(stderr, "ERROR - Usage: %s <length> <repeat> <flag>\n", argv[0]);
        fprintf(stderr, "ERROR - Usage: %s <length> <repeat> <flag> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse the length of the password and the number of repetitions.

    uint64_t length = (uint64_t)atoll(argv[1]);
    uint64_t repeat = (uint64_t)atoll(argv[2]);

    fprintf(stdout, "Password Length: %ld\n", length);
    fprintf(stdout, "Number of Repetitions: %ld\n", repeat);

    // Parse the flags for the password generation.

    uint8_t flags = 0;
    for (int i = 0; i < 4; i++) {
        if (argv[3][i] == '1') {
            flags |= 1 << (3-i);
        } else if (argv[3][i] != '0') {
            fprintf(stderr, "ERROR - Invalid flag: %s.\n", argv[3]);
            return EXIT_FAILURE;
        }
    }

    // Report the parsed flags.

    fprintf(stdout, "Numbers: %s\n", (flags & 0b1000) ? "Enabled" : "Disabled");
    fprintf(stdout, "Uppercase: %s\n", (flags & 0b0100) ? "Enabled" : "Disabled");
    fprintf(stdout, "Lowercase: %s\n", (flags & 0b0010) ? "Enabled" : "Disabled");
    fprintf(stdout, "Special Characters: %s\n", (flags & 0b0001) ? "Enabled" : "Disabled");

    // Open the output file if provided.

    int save_fd = 0;
    if (argc == 5) {
        save_fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC);
        if (save_fd < 0) {
            fprintf(stderr, "ERROR - Opening output file: %s.\n", argv[4]);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Output File: %s\n", argv[3]);
    } else {
        fprintf(stdout, "Output File: N/A\n");
    }
    fprintf(stdout, "\n");

    // Generate and output the password.

    generate(length, repeat, flags, save_fd);
    fprintf(stdout, "\n");

    if (save_fd > 0) close(save_fd);

    fprintf(stdout, "Password generated.\n");

    return EXIT_SUCCESS;
}
