#include <arpa/inet.h>
#include <netinet/in.h>  // Internet family of protocols
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define SERVERIP "127.0.0.1"
#define PORT 50000
#define BUFFSIZE 1024

void setTimeOut(int soc, int sec) {
  struct timeval tv;
  tv.tv_sec = sec;
  tv.tv_usec = 0;
  if (setsockopt(soc, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror("setsockopt failed");
    exit(EXIT_FAILURE);
  }
}

int main() {
  int soc_des;                   // socket file descriptor
  struct sockaddr_in dest_addr;  // socket destination address structure
  socklen_t dest_addr_len;       // length of the destination address structure
  char ping[] = "ping";          // ping message
  char buff[BUFFSIZE];           // buffer for receiving data
  int dest_bytes = sizeof(ping);

  /*
    create a socket based on UDP protocol
  */
  if ((soc_des = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("cannot create socket");
    return 0;
  }

  /*
    initialize the destination address structure
  */
  dest_addr.sin_family = PF_INET;  // Internet family of protocols
  dest_addr.sin_addr.s_addr = inet_addr(SERVERIP);  // Server IP
  dest_addr.sin_port = htons(PORT);  // Server port - changing byte order
  dest_addr_len = sizeof(dest_addr);

  /*
    set timeout for receiving data
  */
  setTimeOut(soc_des, 1);

  printf("Enter arithmetic operations (q to quit)\n");
  printf("format <operator>:<first-operand>:<second-operand>\n\n");
  while (true) {
    printf("> ");
    char input[BUFFSIZE];
    scanf("%s", input);
    if (strcmp(input, "q") == 0) {
      printf("Quitting...\n");
      break;
    }
    dest_bytes = sendto(soc_des, input, BUFFSIZE, 0,
                        (struct sockaddr *)&dest_addr, dest_addr_len);
    if (dest_bytes < 0) {
      perror("sendto");
      continue;
    }
    dest_bytes = recvfrom(soc_des, buff, BUFFSIZE, 0,
                          (struct sockaddr *)&dest_addr, &dest_addr_len);
    if (dest_bytes < 0) {
      perror("recvfrom");
      continue;
    }
    printf("%s\n", buff);
  }

  close(soc_des);

  return 0;
}
