/**
 * Some string utils
 */

#ifndef DSTRING_H
#define DSTRING_H

#include <stddef.h>

// Converts unsigned int to binary string representation
char *uint_to_binary(unsigned int n);
// Returns number of matching chars (excluding null chars)
size_t get_str_match(char *str1, char *str2);
// Removes first n char of string
char *trim_str(char *s, size_t n);

#endif /* DSTRINGS_H */
