#include <assert.h>
#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[] )
{
  assert(argc == 2);

  char * name = NULL;
  int str_len = strlen(argv[1]);
  name = (char*) malloc(sizeof(char) * (str_len + 1));
  if (name == NULL) {
      perror("Memory allocation failed!\n");
      return 1;
  }
  // Check if each character is a digit
  for (int i = 0; i < strlen(argv[1]); i++) {
      if (argv[1][i] < '0' || argv[1][i] > '9') {
          printf("Invalid input: Input should contain only digits!!\n");
          return 1;
      }
  }
  strcpy(name, argv[1]);
//   printf("%s", name);
  // At this point, the input is a valid 5-digit decimal number
  int chartoint = atoi(name);

  sleep(chartoint);

  return 0;
}
