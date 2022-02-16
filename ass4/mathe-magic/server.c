#include <arpa/inet.h>
#include <netinet/in.h>  // Internet family of protocols
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 50000
#define BUFFSIZE 1024

bool parseInput(char *input, char *op, int *a, int *b) {
  char *token = strtok(input, ":");
  if (token == NULL) {
    return false;
  }
  strcpy(op, token);
  token = strtok(NULL, ":");
  if (token == NULL) {
    return false;
  }
  *a = atoi(token);
  token = strtok(NULL, ":");
  if (token == NULL) {
    return false;
  }
  *b = atoi(token);
  return true;
}

int getResult(char *op, int a, int b, int *flag) {
  if (strcmp(op, "add") == 0) {
    return a + b;
  } else if (strcmp(op, "sub") == 0) {
    return a - b;
  } else if (strcmp(op, "mul") == 0) {
    return a * b;
  } else if (strcmp(op, "div") == 0) {
    if (b == 0) {
      *flag = 1;
      return 0;
    }
    return a / b;
  } else {
    *flag = 2;
    return -1;
  }
}

int main() {
  int soc;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  char buff[BUFFSIZE];

  soc = socket(PF_INET, SOCK_DGRAM, 0);
  if (soc < 0) {
    perror("socket");
    exit(1);
  }

  server_addr.sin_family = PF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(soc, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    exit(1);
  }

  printf("Server successfully started at 127.0.0.1:%d\n\n", PORT);

  while (true) {
    socklen_t client_addr_len = sizeof(client_addr);
    int n = recvfrom(soc, buff, BUFFSIZE, 0, (struct sockaddr *)&client_addr,
                     &client_addr_len);
    if (n < 0) {
      perror("recvfrom");
      exit(1);
    }

    printf("Received %d bytes from %s:%d\n", n, inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));
    printf("Data: %s\n", buff);

    char operator[4];
    int operand_a, operand_b;
    char response[BUFFSIZE];

    if (parseInput(buff, operator, & operand_a, &operand_b)) {
      printf("Operator: %s\n", operator);
      printf("Operand A: %d\n", operand_a);
      printf("Operand B: %d\n", operand_b);

      /*
        0 -> success
        1 -> division by 0
        2 -> invalid operator
      */

      int flag = 0;

      int result = getResult(operator, operand_a, operand_b, &flag);
      if (flag == 0) {
        printf("Result: %d\n", result);
        sprintf(response, "%d", result);
      } else if (flag == 1) {
        printf("Division by 0\n");
        strcpy(response, "Division by 0");
      } else {
        printf("Invalid operator\n");
        strcpy(response, "Invalid operator");
      }
    } else {
      printf("Invalid input format\n");
      strcpy(response, "Invalid input format");
    }

    n = sendto(soc, response, BUFFSIZE - 1, 0, (struct sockaddr *)&client_addr,
               sizeof(client_addr));
    if (n < 0) {
      perror("sendto");
      exit(1);
    }

    printf("Sent %d bytes to %s:%d\n\n", n, inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));
  }

  return 0;
}
