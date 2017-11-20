int CountElse[17];
int CountIf[17];

void evaluate()
{
  int i;
  i = 0;
  while(i < 17){
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
#include "standard.h"
uint16_t sockfd;

void int_handler()
{
/* Handle SIGINT (Ctrl+C). */
  close(sockfd);
  exit(1);
}

void handle_server_connection(int server_port,char *server_ip,char *username,char *password,char *filename)
{
  int bytes_read;
  char buffer[1024];
  FILE *fp;
  struct sockaddr_in server;
  sockfd = make_socket(server_ip,server_port,&server);
  bzero(buffer,1024);
#ifdef BUG1
#endif
/* Get the username and password from client. */
  sprintf(buffer,"%s",username);
  send(sockfd,buffer,(strlen(username)),0);
  bzero(buffer,(strlen(username)));
  if (!safe_read(sockfd,buffer)) {
    increment_if(5);
    exit(0);
  }
   else 
    increment_else(5);
  if (!strcmp(buffer,"no_user")) {
    increment_if(6);
{
      printf("Username '%s' does not exist. Do you want to register? [y/n] ",username);
      char ans;
      scanf("%c",&ans);
      switch(ans){
        case 'y':
{
        }
        case 'Y':
{
          printf("Retype password: ");
          scanf("%s",password);
          sprintf(buffer,"%s %s\n",username,password);
          send(sockfd,buffer,strlen(buffer) + 1,0);
          bzero(buffer,strlen(username) + strlen(password) + 2);
          printf("Successfully registered!\n");
          close(sockfd);
          exit(0);
        }
        default:
{
          close(sockfd);
          exit(0);
        }
      }
    }
  }
   else 
    increment_else(6);
  sprintf(buffer,"%s",password);
  send(sockfd,buffer,(strlen(password)),0);
/* Server will send authentication status. */
  bzero(buffer,(strlen(password)));
  if (!safe_read(sockfd,buffer)) {
    increment_if(7);
    exit(0);
  }
   else 
    increment_else(7);
#ifdef BUG2
#endif
  if (!strncmp(buffer,"Hello",5)) {
    increment_if(8);
{
      printf("%s\n",buffer);
      bzero(buffer,5);
#ifdef BUG3
      sprintf(filename,"%s","no-op");
#endif
      sprintf(buffer,"%s",filename);
      send(sockfd,buffer,1024,0);
      bzero(buffer,(strlen(filename)));
      if (!safe_read(sockfd,buffer)) {
        increment_if(9);
        exit(0);
      }
       else 
        increment_else(9);
/* Receive file transfer initiation cue. */
      if (!strncmp(buffer,"Initiating",10)) {
        increment_if(10);
{
          fp = fopen(filename,"w");
#ifdef BUG4
#endif
          printf("+--------------------------+\n| Initiating file transfer |\n+--------------------------+\n");
/* Receive file from the server in chunks. */
          while(1){
            bzero(buffer,10);
            bytes_read = safe_read(sockfd,buffer);
            if (bytes_read < 1024) {
              increment_if(11);
{
                if (strcmp(buffer,"")) {
                  increment_if(12);
                  safe_write(buffer,bytes_read,fp);
                }
                 else 
                  increment_else(12);
                break; 
              }
            }
             else {
              increment_else(11);
              safe_write(buffer,bytes_read,fp);
            }
          }
/* Flush the stream. */
          fclose(fp);
          printf("+------------------------+\n| File transfer complete |\n+------------------------+\n");
        }
      }
       else {
        increment_else(10);
        printf("%s",buffer);
      }
    }
  }
   else {
    increment_else(8);
    printf("%s",buffer);
  }
}

int main(int argc,char **argv)
{
  int pid;
  int server_port;
  int wstatus;
  char server_ip[16];
  char password[256];
  char username[256];
  char filename[256];
/* Set line-buffering on standard output stream. */
  setlinebuf(stdout);
/* Handle interrupts */
  signal(2,int_handler);
  if (argc != 4) {
    increment_if(13);
{
      fprintf(stderr,"Usage: ./client username:password@<server_ip> <port> filename\n");
      exit(1);
    }
  }
   else 
    increment_else(13);
  server_port = atoi(argv[2]);
#ifdef BUG5
#endif
/* Extract username, password and server IP from argv[1]. */
  int i = 0;
  bzero(username,sizeof(username));
  while(argv[1][i] != ':'){
    username[i] = argv[1][i];
    i++;
  }
  i++;
  int j = 0;
  bzero(password,sizeof(password));
  while(argv[1][i] != '@'){
    password[j] = argv[1][i];
    i++;
    j++;
  }
  i++;
  j = 0;
  bzero(server_ip,sizeof(server_ip));
  while(argv[1][i] != '\0'){
    server_ip[j] = argv[1][i];
    i++;
    j++;
  }
  if ((pid = fork()) < 0) {
    increment_if(14);
{
      fprintf(stderr,"Error on fork\n");
      close_sock(sockfd,"server");
      exit(1);
    }
  }
   else {
    increment_else(14);
    if (pid == 0) {
      increment_if(15);
{
/*
         * This is the child process. It establishes a connection
         * with the server while parent process 'wait()'s on it.
         */
        handle_server_connection(server_port,server_ip,username,password,argv[3]);
        close_sock(sockfd,"server");
      }
    }
     else {
      increment_else(15);
{
/*
         * Parent process waits until the client
         * finishes communication with the server.
         */
        waitpid(pid,&wstatus,0);
        if (wstatus != 0) {
          increment_if(16);
          fprintf(stderr,"Child process terminated with an error!\n");
        }
         else 
          increment_else(16);
      }
    }
  }
  evaluate();
  return 0;
}
