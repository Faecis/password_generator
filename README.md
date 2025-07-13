# Password Generator

A simple and secure password generator using entropy-supplying system calls.
This tool generates random passwords using entropy from system calls, allowing you to customize the character set and output options.

## Features

* Select which character groups to include: numbers, uppercase, lowercase, special characters
* Specify password length and number of passwords to generate
* Output to standard output or save to a file

## Usage

```sh
./password_generator <length> <repeat> <flags> [output_file]
```

* `<length>`: Length of each password (integer)
* `<repeat>`: Number of passwords to generate (integer)
* `<flags>`: 4-digit binary string to select character sets:
    * 1st digit: Numbers (1 = include, 0 = exclude)
    * 2nd digit: Uppercase letters
    * 3rd digit: Lowercase letters
    * 4th digit: Special characters
* `[output_file]`: (Optional) File to save the generated passwords

### Example

```sh
./password_generator 16 5 1101 passwords.txt
```

Generates 5 passwords of length 16, using numbers, uppercase, and special characters, and saves them to `passwords.txt`.
