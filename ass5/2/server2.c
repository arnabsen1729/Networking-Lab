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

typedef struct word_struct {
  char syn[50];
  char ant[50];
} word;

const int count_words = 3;

int cmp(void const *a, void const *b) {
  char const *aa = ((word const *)a)->syn;
  char const *bb = ((word const *)b)->syn;

  return strcmp(aa, bb);
}

word words[3];

void init() {
  strcpy(words[0].syn, "left");
  strcpy(words[0].ant, "right");

  strcpy(words[1].syn, "right");
  strcpy(words[1].ant, "left");

  strcpy(words[2].syn, "top");
  strcpy(words[2].ant, "bottom");
}

void send_message(int newsockfd, const char *msg) {
  char buf[100];
  for (int i = 0; i < 100; i++) buf[i] = '\0';
  strcpy(buf, msg);
  send(newsockfd, buf, 100, 0);
}

int main() {
  init();
  FILE *fd = fopen("system_time.csv", "w");
  qsort(words, count_words, sizeof(word), cmp);

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
    int low = 0;
    int high = count_words - 1;
    int mid;
    int found = 0;
    while (low <= high) {
      mid = (low + high) / 2;
      int cmp = strcmp(buf, words[mid].syn);
      if (cmp == 0) {
        strcpy(res, words[mid].ant);
        found = 1;
        break;
      } else if (cmp < 0) {
        high = mid - 1;
      } else {
        low = mid + 1;
      }
    }

    if (found == 0) {
      strcpy(res, "Sorry, antonym not found.");
    } else {
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
