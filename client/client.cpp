#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MY_PORT 31330

int main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, "Need server IPv4 address.\n");
    return 1;
  }

  int cfd;
  struct sockaddr_in a;

  memset(&a, 0, sizeof(struct sockaddr_in));
  a.sin_family = AF_INET;
  a.sin_port = htons(MY_PORT);
  if (0 == inet_pton(AF_INET, argv[1], &a.sin_addr)) {
    fprintf(stderr, "That's not an IPv4 address.\n");
    return 1;
  }

  cfd = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == connect(cfd, (struct sockaddr *)&a, sizeof(struct sockaddr_in))) {
    perror("connect");
    return 1;
  }

  // starts here

  int n;
  char buf[128];
  // n = read(cfd, buf, 100);
  // write(1, buf, n);
  strcpy(buf, "alan0");
  write(cfd, buf, strlen(buf));
  n = read(cfd, buf, 32);
  fprintf(stderr,"%s ::", buf);

  close(cfd);
  return 0;
}