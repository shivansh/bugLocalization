#define SERVER
#include "standard.h"

#define MAX_CONN 5
#define BASEDIR "./server/serve_dir/"

uint16_t sockfd;
struct sockaddr_in client;
char buffer[BUFSIZE];

void
int_handler()
{
    /* Handle SIGINT (Ctrl+C). */
    close(sockfd);
    exit(EXIT_FAILURE);
}

char*
read_line(FILE *fp)
{
    int  bytes_read = 0;
    int  bufsize = BUFSIZE;
    char *buffer;
    char *tmp;
    char *line = malloc(bufsize);

    if (!line)
        return NULL;

    buffer = line;

    while (fgets(buffer, bufsize - bytes_read, fp)) {
        bytes_read = strlen(line);

        if (line[bytes_read - 1] == '\n') {
            line[bytes_read - 1] = '\0';
            return line;
        } else {
            bufsize = 2 * bufsize;
            tmp = realloc(line, bufsize);
            if (tmp) {
                line = tmp;
                buffer = line + bytes_read;
            } else {
                free(line);
                return NULL;
            }
        }
    }

    return NULL;
}

void
handle_client_connection(uint16_t sockfd)
{
    int  bytes_read;
    int  invalid = 0;
    int  valid_user = 0;
    char username[256];
    char password[256];
    char base_dir[256] = BASEDIR;
    FILE *fp;

    while(1) {
        bzero(buffer, BUFSIZE);

        if (!safe_read(sockfd, buffer))
            break;

        snprintf(username, strlen(buffer) + 1, "%s", buffer);
        FILE *user_fp;
        char *line;

        /* Authenticate the client's username. */
        user_fp = fopen("./server/users.txt", "a+");
        while(line = read_line(user_fp)) {
            if (strstr(line, username)) {
                valid_user = 1;
                bzero(buffer, BUFSIZE);
                snprintf(buffer, 11, "valid_user");
                send(sockfd, buffer, BUFSIZE, 0);

                bzero(buffer, 11);
                if (!safe_read(sockfd, buffer))
                    break;

                snprintf(password, strlen(buffer) + 1, "%s", buffer);
                if (strstr(line, password)) {
                    snprintf(buffer, strlen(username) + 7,  "Hello %s", username);
                    send(sockfd, buffer, BUFSIZE, 0);

                    /* Client will now send the filename. */
                    bzero(buffer, strlen(username) + 7);
                    if (!(bytes_read = safe_read(sockfd, buffer)))
                        break;

                    /* Check if file exists. */
                    strcat(base_dir, buffer);
                    if (!access(base_dir, F_OK)) {
                        /* Send a cue for file transfer initiation. */
                        snprintf(buffer, 11, "Initiating");
                        send(sockfd, buffer, BUFSIZE, 0);

                        bzero(buffer, 11);
                        fp = fopen(base_dir, "r");

                        /* Start sending the file in chunks. */
                        while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0) {
                            send(sockfd, buffer, bytes_read, 0);
                            /* Clear the buffer in case the size of next
                             * read is less than the buffer size BUFSIZE.
                             */
                            bzero(buffer, BUFSIZE);
                        }

                        /*
                         * Before closing the connection, send EOF
                         * to let the other side know that we no more
                         * want to send any data. This case is useful
                         * when we have data which is the exact multiple
                         * of the buffer size: BUFSIZE.
                         *
                         * A small timeout will ensure that the client
                         * performs two reads instead of a single read
                         * in which case the empty message will be
                         * indistinguishable from the rest.
                         */
                        usleep(100000);
                        send(sockfd, "", 1, 0);

                        fclose(fp);
                    } else {
                        bzero(buffer, bytes_read);
                        sprintf(buffer, "+----------------+\n"
                                        "| File not found |\n"
                                        "+----------------+");
                        send(sockfd, buffer, BUFSIZE, 0);
                    }
                } else {
                    invalid = 1;
                    break;
                }
            }

            free(line);
        }

        if (!valid_user) {
            bzero(buffer, BUFSIZE);
            sprintf(buffer, "%s", "no_user");
            send(sockfd, buffer, BUFSIZE, 0);
            bzero(buffer, 7);
            if (!safe_read(sockfd, buffer))
                exit(EXIT_SUCCESS);

            safe_write(buffer, strlen(buffer), user_fp);

            invalid = 1;
        }

        fclose(user_fp);

        if (invalid) {
            bzero(buffer, BUFSIZE);
            sprintf(buffer, "+---------------------------+\n"
                            "| Authentication failure!!! |\n"
                            "+---------------------------+");
            send(sockfd, buffer, BUFSIZE, 0);
        }
    }
}

int
main(int argc, char **argv)
{
    int      client_len;
    int      wstatus;
    pid_t    pid;
    pid_t    result;
    uint16_t port;
    uint16_t client_sockfd;
    struct   sockaddr_storage serverStorage;

    /* Enable line-buffering on standard output stream. */
    setlinebuf(stdout);

    /* Handle interrupts */
    signal(SIGINT, int_handler);

    if (argc != 3) {
        fprintf(stderr, "Usage: ./server <IP> <PORT>\nExiting!\n");
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[2]);
    sockfd = make_socket(argv[1], port, &client);

    if (listen(sockfd, MAX_CONN) == 0)
        printf("Listening on port %d\n", port);

    while(1) {
        client_len = sizeof(node);
        client_sockfd = accept(sockfd, (struct sockaddr*)&serverStorage,
                (socklen_t*)&client_len);

        if (client_sockfd < 0) {
            fprintf(stderr, "Socket accept unsuccessful: ");
            exit(EXIT_FAILURE);
        }

        printf("\n+-------------------------------------+\n"
                 "| Successfully established connection |\n"
                 "+-------------------------------------+\n");

        if ((pid = fork()) < 0) {
            fprintf(stderr, "Error on fork: ");
            close(client_sockfd);
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            /* This is the child process. */
            handle_client_connection(client_sockfd);
            close(client_sockfd);
            exit(EXIT_SUCCESS);
        } else {
            /*
             * This is the parent process.
             * We return immediately if no child has exited.
             */
            result = waitpid(pid, &wstatus, WNOHANG);
            close(client_sockfd);
        }

        /* Sleep for half a second. */
        usleep(500000);
    }

    close_sock(sockfd, "client");
    return 0;
}
