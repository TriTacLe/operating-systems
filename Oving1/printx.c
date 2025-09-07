#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int numb = atoi(argv[1]);

  char word[100] = "";
  printf("Enter a string: ");
  scanf("%s", word);

  for (int i = 0; i < numb; i++)
  {
    printf("%s\n", word);
  }
  return 0;
}
