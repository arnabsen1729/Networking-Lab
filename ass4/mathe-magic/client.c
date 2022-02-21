/**
 * @file client.c
 * @author Arnab Sen (arnabsen1729@gmail.com)
 * @brief Client program of mathe-magic
 *
 * The client will prompt the user to enter a command.
 * It will be sent to the server and the response will be displayed.
 * @date 2022-02-19
 */

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

/**
 * @brief Sets a timeout to the socket
 *
 * This prevents waitind indefinitely for a response from the server.
 * If the server does not respond within the timeout, it will trigger
 * an error.
 *
 * @param soc The socket descriptor to set the timeout on
 * @param sec The timeout in seconds
 */
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
    exit(EXIT_FAILURE);
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
    // read input from user
    printf("> ");
    char input[BUFFSIZE];
    scanf("%s", input);

    // check if user wants to quit
    if (strcmp(input, "q") == 0) {
      printf("Quitting...\n");
      break;
    }

    // send input to server
    dest_bytes = sendto(soc_des, input, BUFFSIZE, 0,
                        (struct sockaddr *)&dest_addr, dest_addr_len);
    if (dest_bytes < 0) {
      perror("sendto");
      continue;
    }

    // receive response from server
    dest_bytes = recvfrom(soc_des, buff, BUFFSIZE, 0,
                          (struct sockaddr *)&dest_addr, &dest_addr_len);
    if (dest_bytes < 0) {
      perror("recvfrom");
      continue;
    }

    // print response to the user
    printf("%s\n", buff);
  }

  // close the socket
  close(soc_des);

  return 0;
}
