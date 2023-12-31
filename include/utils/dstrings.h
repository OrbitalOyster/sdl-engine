/**
 * Some string utils
 */

#ifndef DSTRING_H
#define DSTRING_H

// Converts unsigned int to binary representation
char *uint_to_binary(unsigned int n);
// Returns number of matching chars (excluding null chars)
unsigned int get_str_match(char *str1, char *str2);
// Removes first n char of string
char *trim_str(char *s, unsigned int n);

#endif
