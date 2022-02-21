#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>  // Internet family of protocols
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 50000
#define BUFFSIZE 1024

// SERVER

int main() {
  int soc;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len;
  char buff[BUFFSIZE];

  soc = socket(F_INET, SOCK_DGRAM, 0);  // SOCK_STREAM (TCP)

  if (soc == -1) {
    perror("socket");
    return 1;
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if ((bind(soc, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0) {
    perror("cannot bind");
    return 0;
  }

  printf("Server running on PORT 50000\n");

  while (true) {
    int n = recvfrom(soc, buff, BUFFSIZE - 1, 0,
                     (struct sockaddr *)&client_addr, &client_addr_len);
    if (n < 0) {
      perror("recv()");
    }
    buff[n] = '\0';
    printf("Recv: %s\n", buff);

    strcpy(buff, "pong");

    n = sendto(soc, buff, sizeof(buff), 0, (struct sockaddr *)&client_addr,
               client_addr_len);

    if (n < 0) {
      perror("send()");
    }
    printf("Sent: %s\n", buff);
  }

  close(soc);

  return 0;
}
