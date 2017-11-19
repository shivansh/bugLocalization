#include "standard.h"

uint16_t sockfd;

void
int_handler()
{
    /* Handle SIGINT (Ctrl+C). */
    close(sockfd);
    exit(EXIT_FAILURE);
}

void
handle_server_connection(int  server_port,
                         char *server_ip,
                         char *username,
                         char *password,
                         char *filename)
{
    int  bytes_read;
    char buffer[BUFSIZE];
    FILE *fp;
    struct sockaddr_in server;

    sockfd = make_socket(server_ip, server_port, &server);
    bzero(buffer, BUFSIZE);

    /* Get the username and password from client. */
    sprintf(buffer, "%s", username);
    send(sockfd, buffer, strlen(username), 0);

    bzero(buffer, strlen(username));
    if (!safe_read(sockfd, buffer))
        exit(EXIT_SUCCESS);

    if (!strcmp(buffer, "no_user")) {
        printf("Username \'%s\' does not exist. "
               "Do you want to register? [y/n] ", username);
        char ans;
        scanf("%c", &ans);
        switch(ans) {
            case 'y':
            case 'Y':
                printf("Retype password: ");
                scanf("%s", password);
                sprintf(buffer, "%s %s\n", username, password);
                send(sockfd, buffer, strlen(buffer) + 1, 0);
                bzero(buffer, strlen(username) + strlen(password) + 2);
                printf("Successfully registered!\n");
                close(sockfd);
                exit(EXIT_SUCCESS);

            default:
                close(sockfd);
                exit(EXIT_SUCCESS);
        }
    }

    sprintf(buffer, "%s", password);
    send(sockfd, buffer, strlen(password), 0);

    /* Server will send authentication status. */
    bzero(buffer, strlen(password));
    if (!safe_read(sockfd, buffer))
        exit(EXIT_SUCCESS);

    if (!strncmp(buffer, "Hello", 5)) {
        printf("%s\n", buffer);
        bzero(buffer, 5);

        printf("Enter the filename: ");
        scanf("%s", filename);
        sprintf(buffer, "%s", filename);
        send(sockfd, buffer, BUFSIZE, 0);

        bzero(buffer, strlen(filename));
        if (!safe_read(sockfd, buffer))
            exit(EXIT_SUCCESS);

        /* Receive file transfer initiation cue. */
        if (!strncmp(buffer, "Initiating", 10)) {
            fp = fopen(filename, "w");
            printf("+--------------------------+\n"
                   "| Initiating file transfer |\n"
                   "+--------------------------+\n");

            /* Receive file from the server in chunks. */
            while(1) {
                bzero(buffer, 10);
                bytes_read = safe_read(sockfd, buffer);

                if (bytes_read < BUFSIZE) {
                    if (strcmp(buffer, ""))
                        safe_write(buffer, bytes_read, fp);
                    break;
                } else
                    safe_write(buffer, bytes_read, fp);
            }

            fclose(fp);     /* Flush the stream. */
            printf("+------------------------+\n"
                   "| File transfer complete |\n"
                   "+------------------------+\n");
        } else
            printf("%s", buffer);
    } else
        printf("%s", buffer);
}

int
main(int argc, char **argv)
{
    int  pid;
    int  server_port;
    int  wstatus;
    char server_ip[16];
    char password[256];
    char username[256];
    char filename[256];

    /* Set line-buffering on standard output stream. */
    setlinebuf(stdout);

    /* Handle interrupts */
    signal(SIGINT, int_handler);

    if (argc != 3) {
        fprintf(stderr, "Usage: ./client username:password@<server_ip> <port>\n");
        exit(EXIT_FAILURE);
    }

    server_port = atoi(argv[2]);

    /* Extract username, password and server IP from argv[1]. */
    int i = 0;
    bzero(username, sizeof(username));
    while (argv[1][i] != ':') {
        username[i] = argv[1][i];
        i++;
    }
    i++;

    int j = 0;
    bzero(password, sizeof(password));
    while (argv[1][i] != '@') {
        password[j] = argv[1][i];
        i++;
        j++;
    }
    i++;

    j = 0;
    bzero(server_ip, sizeof(server_ip));
    while(argv[1][i] != '\0') {
        server_ip[j] = argv[1][i];
        i++;
        j++;
    }

    if ((pid = fork()) < 0) {
        fprintf(stderr, "Error on fork\n");
        close_sock(sockfd, "server");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        /*
         * This is the child process. It establishes a connection
         * with the server while parent process 'wait()'s on it.
         */
        handle_server_connection(server_port, server_ip,
                                 username, password, filename);
        close_sock(sockfd, "server");
    } else {
        /*
         * Parent process waits until the client
         * finishes communication with the server.
         */
        waitpid(pid, &wstatus, 0);
        if (wstatus != 0)
            fprintf(stderr, "Child process terminated with an error!\n");
    }

    return 0;
}
