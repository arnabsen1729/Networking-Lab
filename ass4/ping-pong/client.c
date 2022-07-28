/**
 * @file client.c
 * @author Arnab Sen (arnabsen1729@gmail.com)
 * @brief Client program of ping-pong game
 *
 * The client will send 10 ping messages and determine the RTT for each message.
 *
 * @date 2022-02-19
 */

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

/**
 * @brief returns the time difference in microseconds
 *
 * @param start start time
 * @param end end time
 * @return uint64_t time difference in microseconds
 */
uint64_t difftime_us(struct timespec start, struct timespec end) {
  return (end.tv_sec - start.tv_sec) * 1000000 +
         (end.tv_nsec - start.tv_nsec) / 1000;
}

int main() {
  // data structures to store stats
  struct timespec start, end;
  int total_responses = 0;
  double total_time = 0, min_time = INT_MAX, max_time = 0;

  int soc_des;                   // socket file descriptor
  struct sockaddr_in dest_addr;  // socket destination address structure
  socklen_t dest_addr_len;       // length of the destination address structure
  char ping[] = "";              // ping message
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
  dest_addr.sin_family = AF_INET;  // Internet family of protocols
  dest_addr.sin_addr.s_addr = inet_addr(SERVERIP);  // Server IP
  dest_addr.sin_port = htons(PORT);  // Server port - changing byte order

  dest_addr_len = sizeof(dest_addr);

  /*
    set timeout for receiving data
  */
  setTimeOut(soc_des, 1);

  printf("PING server %s:%d with \"ping\" message\n\n", SERVERIP, PORT);

  /*
    send 10 ping messages to the server
  */
  for (int i = 0; i < 10; i++) {
    // noting down the time BEFORE sending the ping message
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    // send ping message to server
    int data_sent = sendto(soc_des, ping, dest_bytes, 0,
                           (struct sockaddr *)&dest_addr, dest_addr_len);
    if (data_sent < 0) {
      perror("cannot send");
      continue;
    }

    // receive response from server
    int data_received = recvfrom(soc_des, buff, BUFFSIZE - 1, 0,
                                 (struct sockaddr *)&dest_addr, &dest_addr_len);

    printf("packet %d: ", i + 1);

    if (data_received < 0) {
      printf("request timed out\n");
      continue;
    }

    buff[data_received] = '\0';

    // noting down the time AFTER receiving the ping message
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);

    uint64_t delta_us = difftime_us(start, end);
    double time_taken_ms = (double)(delta_us) / 1000;

    // log details
    printf("received= \"%s\" ; rtt= %.2f ms \n", buff, time_taken_ms);

    /*
      update the statistics
    */
    total_responses++;
    total_time += time_taken_ms;
    if (time_taken_ms < min_time) {
      min_time = time_taken_ms;
    }
    if (time_taken_ms > max_time) {
      max_time = time_taken_ms;
    }
  }

  close(soc_des);

  /*
    print the statistics
  */
  printf("\n\n--- statistics ---\n", total_responses);
  printf(
      "10 packets transmitted, %d received\n%.2f%% packet loss, time %.4f "
      "ms\n",
      total_responses, (10 - total_responses) * 10.0, total_time);
  printf("rtt min/avg/max = %.4f/%.4f/%.4f ms\n", min_time,
         total_time / total_responses, max_time);
  return 0;
}
