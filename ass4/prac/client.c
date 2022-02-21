#include <arpa/inet.h>
#include <limits.h>
#include <netinet/in.h>  // Internet family of protocols
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define SERVERIP "127.0.0.1"
#define PORT 50000
#define BUFFSIZE 1024

int main() {
  int soc_des;                   // socket file descriptor
  struct sockaddr_in dest_addr;  // socket destination address structure
  socklen_t dest_addr_len;       // length of the destination address structure
  char ping[] = "ping";          // ping message
  char buff[BUFFSIZE];           // buffer for receiving data
  int dest_bytes = sizeof(ping);

  soc_des = socket(PF_INET, SOCK_DGRAM, 0);
  if (soc_des == -1) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  dest_addr.sin_family = AF_INET;
  dest_addr.sin_addr.s_addr = inet_addr(SERVERIP);  //"a.b.c.d" => 0100100010
  dest_addr.sin_port = htons(PORT);

  dest_addr_len = sizeof(dest_addr);

  int data = sendto(soc_des, ping, dest_bytes, 0, (struct sockaddr *)&dest_addr,
                    dest_addr_len);
  if (data < 0) {
    perror("sendto()");
  }

  data = recvfrom(soc_des, buff, BUFFSIZE - 1, 0, (struct sockaddr *)&dest_addr,
                  &dest_addr_len);

  if (data < 0) {
    perror("recvfrom()");
  }

  buff[data] = '\0';
  printf("Recv: %s\n", buff);

  close(soc_des);

  return 0;
}
