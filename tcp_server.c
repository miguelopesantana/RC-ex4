/*******************************************************************************
 * SERVIDOR no porto 9000, à escuta de novos clientes.  Quando surgem
 * novos clientes os dados por eles enviados são lidos e descarregados no ecran.
 *******************************************************************************/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SERVER_PORT     9000
#define BUF_SIZE	1024

void process_client(int client_fd, struct sockaddr_in *client_addr, int client_no);
void erro(char *msg);

int main() {
  int fd, client, client_no = 0;
  struct sockaddr_in addr, client_addr;
  int client_addr_size;

  bzero((void *) &addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(SERVER_PORT);
//file descriptor
  if ( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	erro("na funcao socket");
  if ( bind(fd,(struct sockaddr*)&addr,sizeof(addr)) < 0)
	erro("na funcao bind");
  if( listen(fd, 5) < 0)
	erro("na funcao listen");
  client_addr_size = sizeof(client_addr);
  while (1) {
    //clean finished child processes, avoiding zombies
    //must use WNOHANG or would block whenever a child process was working
    while(waitpid(-1,NULL,WNOHANG)>0);
    //wait for new connection
    client = accept(fd,(struct sockaddr *)&client_addr,(socklen_t *)&client_addr_size);
    if (client > 0) {
      client_no++;
      if (fork() == 0) {
        close(fd);
        process_client(client, &client_addr, client_no);
        exit(0);
      }
    close(client);
    }
  }
  return 0;
}

void process_client(int client_fd, struct sockaddr_in *client_addr, int client_no){
  
  char dst[16];
  char msg[BUF_SIZE];
  inet_ntop(client_addr -> sin_family, &client_addr -> sin_addr, dst, INET_ADDRSTRLEN);

  printf("** New message received **\nClient %d connecting from (IP:Port) %s:%d says ", client_no, dst, client_addr -> sin_port);
  sprintf(msg, "Received from server:\nServer received connection from (IP:Port) %s:%d; already receive %d connections!", dst, client_addr -> sin_port, client_no);
  write(client_fd, msg, strlen(msg)+1);

	int nread = 0;
	char buffer[BUF_SIZE];

  do {
    nread = read(client_fd, buffer, BUF_SIZE-1);
	   buffer[nread] = '\0';
	    printf("%s\n", buffer);
	     fflush(stdout);
  } while (nread>0);
	close(client_fd);
}

void erro(char *msg){
	printf("Erro: %s\n", msg);
	exit(-1);
}
