#include <stdbool.h>
#include <stdio.h>

long flen(FILE *file) {
  fseek(file, 0L, SEEK_END);
  long filelen = ftell(file);
  rewind(file);

  return filelen;
}

long readstring(FILE *bin, char *buffer) {
  int curri = 0;

  while (true) {
    fread(buffer + curri, sizeof(char), 1, bin);
    if (buffer[curri] == 0x0)
      break;
    curri++;
  }

  return curri + 1;
}
