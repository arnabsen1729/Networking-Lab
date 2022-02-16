#include <arpa/inet.h>
#include <netinet/in.h>  // Internet family of protocols
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define SERVERIP "127.0.0.1"
#define PORT 50000
#define BUFFSIZE 1024

time_t getCurrentTime() {
  time_t rawtime;
  time(&rawtime);
  return rawtime;
}

int main() {
  pid_t child_pids[10];
  int total_time = 0, total_response = 0;

  int socDes;
  struct sockaddr_in destAddr;
  char ping[BUFFSIZE] = "ping", buff[BUFFSIZE];
  int dataBytes = 8, dataSent, dataReceived, destAddrLen;

  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;

  if ((socDes = socket(PF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("cannot create socket");
    return 0;
  }

  destAddr.sin_family = PF_INET;
  destAddr.sin_addr.s_addr = inet_addr(SERVERIP);  // Server IP
                                                   // Converts to 32-bit number
  destAddr.sin_port = htons(PORT);                 // Port number - byte order

  destAddrLen = sizeof(destAddr);

  if (setsockopt(socDes, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    perror("Error");
  }
  printf("Client sending requests to %s:%d\n\n", SERVERIP, PORT);

  for (int i = 0; i < 10; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      time_t sending_time = getCurrentTime();
      dataSent = sendto(socDes, ping, dataBytes, 0,
                        (struct sockaddr *)&destAddr, sizeof(destAddr));
      if (dataSent < 0) {
        perror("cannot send");
        return 1;
      }
      printf("Sent: %s at %s\n", ping, ctime(&sending_time));
      dataReceived = recvfrom(socDes, buff, BUFFSIZE - 1, 0,
                              (struct sockaddr *)&destAddr, &destAddrLen);

      if (dataReceived < 0) {
        perror("cannot receive");
        return 1;
      }
      time_t receiving_time = getCurrentTime();
      printf("Received: %s at %s\n", buff, ctime(&receiving_time));
      total_response++;
      total_time += receiving_time - sending_time;

      return 0;
    } else {
      child_pids[i] = pid;
    }
  }
  pid_t wpid;
  int status;
  while ((wpid = wait(&status)) > 0)
    ;

  close(socDes);

  printf("\nAverage response time: %.4f ms\n",
         (double)total_time / (double)total_response);
  printf("Total response: %d\n", total_response);
  printf("Packet loss: %.2f%%\n",
         (double)(10 - total_response) / (double)10 * 100);

  return 0;
}
