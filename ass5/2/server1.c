#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define PORT 6001

void export(FILE *fd, double value, int port) {
  fprintf(fd, "%d, %f\n", port, value);
  fflush(fd);
}

char words[][10] = {"top", "bottom", "left", "right", "front", "back"};
char antonyms[][10] = {"bottom", "top", "right", "left", "back", "front"};

void send_message(int newsockfd, const char *msg) {
  char buf[100];
  for (int i = 0; i < 100; i++) buf[i] = '\0';
  strcpy(buf, msg);
  send(newsockfd, buf, 100, 0);
}

int main() {
  FILE *fd = fopen("system_time.csv", "w");
  int sockfd, newsockfd;
  int clilen;
  struct sockaddr_in cli_addr, serv_addr;

  int i;
  char buf[100];
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Cannot create socket\n");
    exit(0);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(PORT);

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("Unable to bind local address\n");
    exit(0);
  }

  listen(sockfd, 5);
  while (1) {
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0) {
      printf("Accept error\n");
      exit(0);
    }

    printf("Client %s:%d connected\n", inet_ntoa(cli_addr.sin_addr),
           (int)ntohs(cli_addr.sin_port));

    for (i = 0; i < 100; i++) buf[i] = '\0';
    recv(newsockfd, buf, 100, 0);
    printf("Word recv: %s\n", buf);

    clock_t start_clock_time = clock();

    char res[100];
    res[0] = '\0';

    int found = 0;
    for (i = 0; i < 6; i++) {
      if (strcmp(buf, words[i]) == 0) {
        strcpy(res, antonyms[i]);
        found = 1;
      }
    }

    if (found == 0) {
      strcpy(res, "Sorry, antonym not found.");
    }

    clock_t end_clock_time = clock();
    double time_taken =
        ((double)(end_clock_time - start_clock_time)) / CLOCKS_PER_SEC;

    printf("elapsed system time: %f ms\n", time_taken * 1000);
    export(fd, time_taken * 1000, (int)ntohs(cli_addr.sin_port));

    printf("Sending: %s\n", res);
    send_message(newsockfd, res);
    close(newsockfd);
  }

  return 0;
}
