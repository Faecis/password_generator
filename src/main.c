#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>


/**
 * 
 */
char* get_string(long long length, int flags) {
    const char** character_sets = {
        "0123456789",                       // Numbers
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",       // Uppercase
        "abcdefghijklmnopqrstuvwxyz",       // Lowercase
        "!@#$%^&*()-_=+[]{};:,.<>/?\\|`~"   // Special
    };

    char* password = calloc(length+1, sizeof(char));
    if (password == NULL) {
        fprintf(stderr, "Error - Memory allocation failed.\n");
        free(password);
        exit(EXIT_FAILURE);
    }

    for (long long i = 0; i < length; i++) {
        int random = 0;
        if (getrandom(&random, sizeof(random), 0) < 0) {
            fprintf(stderr, "Error - Failed to get random number.\n");
            free(password);
            exit(EXIT_FAILURE);
        }

        int mode = random % 8;
        if () {

        } else {
            i--;
            continue;
        }
    }
}

/**
 * 
 */
void output_string(long long length, int flags, int save_fd) {
    char* password = get_string(length, flags);
    if (password == NULL) {
        fprintf(stderr, "Error - Failed to generate password.\n");
        exit(EXIT_FAILURE);
    }

    if (save_fd > 0) {
        write(save_fd, password, length);
    }
    printf("%s\n", password);

    free(password);

    return;
}


int main(int argc, char** argv) {
    fprintf(stdout, "Pasword Generator\n\n");

    // Check if there are the correct number of arguments.

    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Error - Invalid number of arguments.\n");
        fprintf(stderr, "Error - Usage: %s <length> <flag>\n", argv[0]);
        fprintf(stderr, "Error - Usage: %s <length> <flag> <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Parse the length of the password.

    long long length = atoll(argv[1]);
    if (length <= 0) {
        fprintf(stderr, "Error - Invalid length: %s.\n", argv[1]);
        return EXIT_FAILURE;
    } else if (length > (1LL << sizeof(long long))-1) {
        fprintf(stderr, "Error - Length too large: %s.\n", argv[1]);
        fprintf(stderr, "Error - Maximum length: %lld\n", (1LL<<sizeof(long long))-1);
        return EXIT_FAILURE;
    }
    fprintf(stdout, "Password Length: %d\n", length);

    // Parse the flags for the password generation.

    int flags = 0;
    for (int i = 0; i < 4; i++) {
        if (argv[2][i] == '1') {
            flags |= (1 << 3-i);
        } else if (argv[2][i] != '0') {
            fprintf(stderr, "Error - Invalid flag: %s.\n", argv[1]);
            return EXIT_FAILURE;
        }

        switch (i) {
            case 0:
                fprintf(stdout, "Numbers: %s\n", (flags & 0b1000) ? "Enabled" : "Disabled");
                break;
            case 1:
                fprintf(stdout, "Uppercase: %s\n", (flags & 0b0100) ? "Enabled" : "Disabled");
                break;
            case 2:
                fprintf(stdout, "Lowercase: %s\n", (flags & 0b0010) ? "Enabled" : "Disabled");
                break;
            case 3:
                fprintf(stdout, "Special Characters: %s\n", (flags & 0b0001) ? "Enabled" : "Disabled");
                break;
            default:
                break;
        }
    }

    // Open the output file if provided.

    int save_fd = 0;
    if (argc == 4) {
        save_fd = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC);
        if (save_fd < 0) {
            fprintf(stderr, "Error - Opening output file: %s.\n", argv[3]);
            return EXIT_FAILURE;
        }
        fprintf(stdout, "Output File: %s\n", argv[3]);
    } else {
        fprintf(stdout, "Output File: N/A\n");
    }
    fprintf(stdout, "\n");

    // Generate and output the password.

    output_string();
    close(save_fd);

    fprintf(stdout, "Password generated.\n");

    return EXIT_SUCCESS;
}
