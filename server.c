#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

void handle_request(int client_socket)
{
  char request[2048];
  int bytes_received = recv(client_socket, request, 2048, 0);

  if (bytes_received > 0)
  {
    // Handle GET request
    if (strncmp(request, "GET /", 5) == 0)
    {
      char response[] = "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "Some-header: ousf/ousf\r\n"
                        "X-Custom-Header: CustomValue\r\n"
                        "\r\n"
                        "Hello from the HTTP server!\n";
      send(client_socket, response, strlen(response), 0);
    }
    // Handle POST request
    else if (strncmp(request, "POST /", 6) == 0)
    {
      // Assuming the request body is immediately after the headers and a blank line
      char *body = strstr(request, "\r\n\r\n");
      if (body)
      {
        body += 4; // Move past the "\r\n\r\n"
                   // Here you can process the body content as needed
      }
      else
      {
        body = ""; // No body found, default to empty string
      }

      char header[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Type: text/plain\r\n"
                      "\r\n";
      // Calculate total response size
      size_t response_size = strlen(header) + strlen(body) + 1; // +1 for null-terminator
      char *response = (char *)malloc(response_size);

      if (response)
      {
        // Construct response with body content
        snprintf(response, response_size, "%s%s", header, body);
        send(client_socket, response, strlen(response), 0);
        free(response);
      }
      else
      {
        // Failed to allocate memory for response
        char error_response[] = "HTTP/1.1 500 Internal Server Error\r\n\r\n";
        send(client_socket, error_response, strlen(error_response), 0);
      }
    }
    else
    {
      char error_response[] = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
      send(client_socket, error_response, strlen(error_response), 0);
    }
  }

  close(client_socket);
}

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_address;

  // Create a socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("Failed to create socket");
    exit(1);
  }

  // Configure server address
  server_address.sin_family = AF_INET;
  server_address.sin_addr.s_addr = INADDR_ANY;
  server_address.sin_port = htons(8080); // Listen on port 8080

  // Bind the socket to the address
  if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
  {
    perror("Failed to bind socket");
    exit(1);
  }

  if (listen(server_socket, 5) == -1)
  {
    perror("Failed to listen on socket");
    exit(1);
  }

  printf("Server listening on port 8080...\n");

  while (1)
  {
    client_socket = accept(server_socket, NULL, NULL);
    if (client_socket == -1)
    {
      perror("Failed to accept connection");
      continue;
    }

    handle_request(client_socket);
  }

  close(server_socket); // Close the server socket before exiting
  return 0;
}
