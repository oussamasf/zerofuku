#include <stdio.h>      // Standard input/output definitions
#include <stdlib.h>     // Standard library for memory allocation, process control, etc.
#include <string.h>     // String handling functions
#include <sys/socket.h> // Main sockets header
#include <netinet/in.h> // Internet address family
#include <unistd.h>     // POSIX operating system API

int main()
{
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in address;
  int addrlen = sizeof(address);

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY; // Listen on any IP address
  address.sin_port = htons(8080);       // Port number

  bind(sockfd, (struct sockaddr *)&address, sizeof(address));

  listen(sockfd, 3); // The second argument is the number of pending connections allowed

  int new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

  char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, world!";
  write(new_socket, hello, strlen(hello));

  close(sockfd);
}
