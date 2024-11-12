# Simple HTTP Server in C

This project implements a basic HTTP server in C that listens for client connections, handles incoming HTTP requests, and sends a simple HTTP response. The server is designed to demonstrate basic networking concepts using sockets and the `TCP/IP` protocol in C.

## Features

- **Socket Programming:** The server uses `socket()`, `bind()`, `listen()`, and `accept()` to handle client connections.
- **HTTP Protocol Handling:** The server responds with a basic "404 Not Found" message for any request. If the request is for the root path (`/`), it responds with "200 OK".
- **Error Handling:** The server handles errors such as failed socket creation, binding, listening, and receiving/sending data.

## Requirements

- **C Compiler:** A C compiler such as GCC.
- **Operating System:** Linux or macOS (due to usage of POSIX system calls like `socket()`, `bind()`, and `recv()`).
- **Libraries:** Standard C libraries such as `<stdio.h>`, `<stdlib.h>`, `<string.h>`, and `<unistd.h>`.

## Installation

1. Clone or download the project repository to your local machine.

2. Compile the server:

    ```bash
    gcc -o simple_http_server server.c
    ```

3. Run the server:

    ```bash
    ./simple_http_server
    ```

The server will start listening on port `4221`.

## Usage

1. Once the server is running, it will wait for an incoming connection from a client.
2. The client can send an HTTP request (e.g., using `curl` or a browser) to the server.
3. The server will send a response:
   - For requests to the root path (`/`), it will respond with "200 OK".
   - For all other requests, it will respond with "404 Not Found".

Example request using `curl`:

```bash
curl http://localhost:4221/

