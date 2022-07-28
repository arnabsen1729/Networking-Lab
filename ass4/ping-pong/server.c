/**
 * @file server.c
 * @author Arnab Sen (arnabsen1729@gmail.com)
 * @brief Server program of ping-pong game
 *
 * The server will listen for incoming connections and send back pong
 * messages for every ping message received.
 *
 * @date 2022-02-19
 */

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

/**
 * @brief Sleep for a amount of time in milliseconds
 *
 * @param msec
 * @return int
 */
int msleep(long msec) {
  struct timespec ts;
  int res;

  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}

int main() {
  srand(time(NULL));

  int soc;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  int data_bytes;
  socklen_t client_addr_len;
  char buff[BUFFSIZE];

  /*
    create a socket based on UDP protocol
  */
  if ((soc = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("cannot create socket");
    return 0;
  }

  /*
    initialize the address structure
  */
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;  // inet_addr("127.0.0.1") ;

  server_addr.sin_port = htons(PORT);  // Port number - byte order

  /*
    bind the socket to the address
  */
  if ((bind(soc, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0) {
    perror("cannot bind");
    return 0;
  }

  printf("Server successfully started at 127.0.0.1:%d\n\n", PORT);

  client_addr_len = sizeof(client_addr);

  while (true) {
    // receive ping from client
    data_bytes = recvfrom(soc, buff, BUFFSIZE - 1, 0,
                          (struct sockaddr *)&client_addr, &client_addr_len);
    if (data_bytes < 0) {
      perror("cannot receive");
      return 1;
    }

    buff[data_bytes] = '\0';
    printf("Received: %s\n", buff);
    buff[0] = '\0';
    // strcpy(buff, "pong");

    /*
      before responding with pong we are simulating a latency
      to simulate a network delay
    */
    // msleep(rand() % 2000);  // sleep()

    // send pong to client
    data_bytes = sendto(soc, buff, data_bytes, 0,
                        (struct sockaddr *)&client_addr, client_addr_len);

    if (data_bytes < 0) {
      perror("cannot send");
      return 1;
    }
    printf("DATA SENT: %d\n", data_bytes);
    printf("Sent: %s\n", buff);
  }

  return 0;
}
