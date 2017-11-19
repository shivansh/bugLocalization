int CountElse[22];
int CountIf[22];

void evaluate()
{
  int i;
  i = 0;
  while(i < 22){
    printf("%d %d\n",CountIf[i],CountElse[i]);
    ++i;
  }
}

void increment_else(int index)
{
  ++CountElse[index];
}

void increment_if(int index)
{
  ++CountIf[index];
}
#define SERVER
#include "standard.h"
#define MAX_CONN 5
#define BASEDIR "./server/"
uint16_t sockfd;
struct sockaddr_in client;
char buffer[1024];

void int_handler()
{
/* Handle SIGINT (Ctrl+C). */
  close(sockfd);
  exit(1);
}

char *read_line(FILE *fp)
{
  int bytes_read = 0;
  int bufsize = 1024;
  char *buffer;
  char *tmp;
  char *line = (malloc(bufsize));
  if (!line) {
    increment_if(5);
    return ((void *)0);
  }
   else 
    increment_else(5);
  buffer = line;
  while(fgets(buffer,bufsize - bytes_read,fp)){
    bytes_read = (strlen(line));
    if (line[bytes_read - 1] == '\n') {
      increment_if(6);
{
        line[bytes_read - 1] = '\0';
        return line;
      }
    }
     else {
      increment_else(6);
{
        bufsize = 2 * bufsize;
        tmp = (realloc(line,bufsize));
        if (tmp) {
          increment_if(7);
{
            line = tmp;
            buffer = line + bytes_read;
          }
        }
         else {
          increment_else(7);
{
            free(line);
            return ((void *)0);
          }
        }
      }
    }
  }
  return ((void *)0);
}

void handle_client_connection(uint16_t sockfd)
{
  int bytes_read;
  int invalid = 0;
  int valid_user = 0;
  char username[256];
  char password[256];
  char base_dir[256] = "./server/";
  FILE *fp;
  while(1){
    bzero(buffer,1024);
    if (!safe_read(sockfd,buffer)) {
      increment_if(8);
      break; 
    }
     else 
      increment_else(8);
    snprintf(username,strlen(buffer) + 1,"%s",buffer);
    FILE *user_fp;
    char *line;
/* Authenticate the client's username. */
    user_fp = fopen("./server/users.txt","a+");
    while(line = read_line(user_fp)){
      if (strstr(line,username)) {
        increment_if(9);
{
          valid_user = 1;
          bzero(buffer,1024);
          snprintf(buffer,11,"valid_user");
          send(sockfd,buffer,1024,0);
          bzero(buffer,11);
          if (!safe_read(sockfd,buffer)) {
            increment_if(10);
            break; 
          }
           else 
            increment_else(10);
          snprintf(password,strlen(buffer) + 1,"%s",buffer);
          if (strstr(line,password)) {
            increment_if(11);
{
              snprintf(buffer,strlen(username) + 7,"Hello %s",username);
              send(sockfd,buffer,1024,0);
/* Client will now send the filename. */
              bzero(buffer,strlen(username) + 7);
              if (!(bytes_read = safe_read(sockfd,buffer))) {
                increment_if(12);
                break; 
              }
               else 
                increment_else(12);
/* Check if file exists. */
              strcat(base_dir,buffer);
              if (!access(base_dir,0)) {
                increment_if(13);
{
/* Send a cue for file transfer initiation. */
                  snprintf(buffer,11,"Initiating");
                  send(sockfd,buffer,1024,0);
                  bzero(buffer,11);
                  fp = fopen(base_dir,"r");
/* Start sending the file in chunks. */
                  while((bytes_read = (fread(buffer,1,1024,fp))) > 0){
                    send(sockfd,buffer,bytes_read,0);
/* Clear the buffer in case the size of next
                             * read is less than the buffer size BUFSIZE.
                             */
                    bzero(buffer,1024);
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
                  send(sockfd,"",1,0);
                  fclose(fp);
                }
              }
               else {
                increment_else(13);
{
                  bzero(buffer,bytes_read);
                  sprintf(buffer,"+----------------+\n| File not found |\n+----------------+");
                  send(sockfd,buffer,1024,0);
                }
              }
            }
          }
           else {
            increment_else(11);
{
              invalid = 1;
              break; 
            }
          }
        }
      }
       else 
        increment_else(9);
      free(line);
    }
    if (!valid_user) {
      increment_if(14);
{
        bzero(buffer,1024);
        sprintf(buffer,"%s","no_user");
        send(sockfd,buffer,1024,0);
        bzero(buffer,7);
        if (!safe_read(sockfd,buffer)) {
          increment_if(15);
          exit(0);
        }
         else 
          increment_else(15);
        safe_write(buffer,(strlen(buffer)),user_fp);
        invalid = 1;
      }
    }
     else 
      increment_else(14);
    fclose(user_fp);
    if (invalid) {
      increment_if(16);
{
        bzero(buffer,1024);
        sprintf(buffer,"+---------------------------+\n| Authentication failure!!! |\n+---------------------------+");
        send(sockfd,buffer,1024,0);
      }
    }
     else 
      increment_else(16);
  }
}

int main(int argc,char **argv)
{
  int client_len;
  int wstatus;
  pid_t pid;
  pid_t result;
  uint16_t port;
  uint16_t client_sockfd;
  struct sockaddr_storage serverStorage;
/* Enable line-buffering on standard output stream. */
  setlinebuf(stdout);
/* Handle interrupts */
  signal(2,int_handler);
  if (argc != 3) {
    increment_if(17);
{
      fprintf(stderr,"Usage: ./server <IP> <PORT>\nExiting!\n");
      exit(1);
    }
  }
   else 
    increment_else(17);
  port = (atoi(argv[2]));
  sockfd = make_socket(argv[1],port,&client);
  if (listen(sockfd,5) == 0) {
    increment_if(18);
    printf("Listening on port %d\n",port);
  }
   else 
    increment_else(18);
  while(1){
    client_len = (sizeof(node));
    client_sockfd = (accept(sockfd,((struct sockaddr *)(&serverStorage)),((socklen_t *)(&client_len))));
    if (client_sockfd < 0) {
      increment_if(19);
{
        fprintf(stderr,"Socket accept unsuccessful: ");
        exit(1);
      }
    }
     else 
      increment_else(19);
    printf("\n+-------------------------------------+\n| Successfully established connection |\n+-------------------------------------+\n");
    if ((pid = fork()) < 0) {
      increment_if(20);
{
        fprintf(stderr,"Error on fork: ");
        close(client_sockfd);
        exit(1);
      }
    }
     else {
      increment_else(20);
      if (pid == 0) {
        increment_if(21);
{
/* This is the child process. */
          handle_client_connection(client_sockfd);
          close(client_sockfd);
          exit(0);
        }
      }
       else {
        increment_else(21);
{
/*
             * This is the parent process.
             * We return immediately if no child has exited.
             */
          result = waitpid(pid,&wstatus,1);
          close(client_sockfd);
        }
      }
    }
/* Sleep for half a second. */
    usleep(500000);
  }
  close_sock(sockfd,"client");
  evaluate();
  return 0;
}
