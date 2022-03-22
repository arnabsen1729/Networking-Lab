#include <stdio.h>
#include <string.h>

char **split(char *string, char delim) {
  int occurrences = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    occurrences += (string[i] == delim);
  }
  char **array = malloc(sizeof(char *) * occurrences);

  char *token = strtok(string, delim);
  int index = 0;
  // loop through the string to extract all other tokens
  while (token != NULL) {
    array[index] = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(array[index], token);
    token = strtok(NULL, delim);
  }

  return array;
}

int main() {
  char s[] = "1 2 3 4 5";
  char **array = split(s, ' ');
  for (int i = 0; i <) return 0;
}
