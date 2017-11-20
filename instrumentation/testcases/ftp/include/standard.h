#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFSIZE 1024

struct sockaddr_in node;

void
close_sock(uint16_t sockfd, char *node)
{
    fprintf(stderr, "\nDisconnecting with %s...\n", node);
    close(sockfd);
}

int
safe_read(uint16_t sockfd, char *buffer)
{
    /* Handle errors while reading from a socket. */
    int bytes_read;

    bytes_read = read(sockfd, buffer, BUFSIZE);
    if (bytes_read < 0) {
        fprintf(stderr, "Error while reading from socket: ");
        exit(EXIT_FAILURE);
    } else if (bytes_read == 0) {
#ifdef SERVER
        /* Connection terminated. */
        fprintf(stderr, "Seems like client has closed the connection.");
        close_sock(sockfd, "client");
#else
        fprintf(stderr, "Seems like server has closed the connection.");
        close_sock(sockfd, "server");
#endif
        return 0;
    }

    return bytes_read;
}

void
safe_write(char *buffer, int bytes_read, FILE *fp)
{
    /* Handle errors while writing to a file. */
    int bytes_written;

    /*
     * It is possible that fwrite may fail while writing.
     * Hence, we keep on attempting at writing until successful.
     */
    while(1) {
        bytes_written = fwrite(buffer, 1, bytes_read, fp);
        if (bytes_written == bytes_read)
            break;
    }
}

uint16_t
make_socket(char *server_ip,
            uint16_t port,
            struct sockaddr_in *node)
{
    /*
     * Subroutine to bidn a socket with the machine
     * address and return its file descriptor.
     */
    uint16_t sockfd;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Socket instantiation unsuccessful: ");
        exit(EXIT_FAILURE);
    }

    printf("Socket instantiated\n");
    /*
     * The htons() function converts from
     * host byte order to network byte order.
     */
    node->sin_family      = AF_INET;
    node->sin_port        = htons(port);
    node->sin_addr.s_addr = inet_addr(server_ip);

#ifdef SERVER
    if (bind(sockfd, (struct sockaddr*)node, sizeof(*node)) < 0) {
        perror("Error while binding socket: ");
        exit(EXIT_FAILURE);
    }
    printf("Socket bound to port %d\n", port);
#else
    if (connect(sockfd, (struct sockaddr*)node, sizeof(*node)) < 0) {
        fprintf(stderr, "Error while connecting socket: ");
        exit(EXIT_FAILURE);
    }
    printf("Socket connected to server at port %d\n", port);
#endif

    return sockfd;
}
