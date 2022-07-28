#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 4000

void export(FILE *fd, double value, int port) {
  fprintf(fd, "%d, %f\n", port, value);
}

void recv_and_display(int sockfd) {
  char buf[100];
  for (int i = 0; i < 100; i++) buf[i] = '\0';
  recv(sockfd, buf, 100, 0);
  printf("%s\n", buf);
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

int main(int argc, char *argv[]) {
  FILE *fd = fopen("response_time.csv", "a");
  int sockfd;
  struct sockaddr_in serv_addr;
  struct timespec start, end;
  struct sockaddr_in local_address;
  int addr_size = sizeof(local_address);

  int i;
  char buf[100];

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Unable to create socket\n");
    exit(0);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(PORT);

  if ((connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0) {
    printf("Unable to connect to server\n");
    exit(0);
  }

  getsockname(sockfd, (struct sockaddr *)&local_address, &addr_size);

  for (i = 0; i < 100; i++) buf[i] = '\0';

  for (int i = 1; i < argc; i++) {
    if (i > 1) strcat(buf, " ");
    strcat(buf, argv[i]);
  }

  strcpy(buf, buf);
  clock_gettime(CLOCK_MONOTONIC_RAW, &start);
  send(sockfd, buf, 100, 0);

  recv_and_display(sockfd);
  clock_gettime(CLOCK_MONOTONIC_RAW, &end);

  uint64_t delta_us = difftime_us(start, end);
  double time_taken_ms = (double)(delta_us) / 1000;
  printf("Time taken: %.2f ms\n", time_taken_ms);
  export(fd, time_taken_ms, (int)ntohs(local_address.sin_port));

  close(sockfd);

  return 0;
}
