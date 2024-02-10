#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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

  while (1)
  { // Run an infinite loop to accept connections continuously
    int new_socket = accept(sockfd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    // Read the request from client
    char buffer[1024] = {0};
    read(new_socket, buffer, 1024);
    printf("Request: %s\n", buffer); // Print the client's request to standard output

    // Send the HTTP response
    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\n\nHello, world!";
    write(new_socket, hello, strlen(hello));

    // Close the socket for the current connection
    close(new_socket);
  }

  // Close the listening socket (this line will never be reached in this example)
  close(sockfd);

  return 0;
}
