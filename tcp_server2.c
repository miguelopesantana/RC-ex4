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
#define BUFFER_SIZE	1024

void process_client(int client_fd, struct sockaddr_in *client_addr);
void erro(char *msg);

int main(){
    int socket_1, socket_2, client_1, client_2, client_no = 0;
    struct sockaddr_in addr, client1_addr, client2_addr;
    int client1_addr_size, client2_addr_size;
    
    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(SERVER_PORT);
    
    if((socket_1 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        erro("função socket 1");
    }
    if((socket_2 = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        erro("função socket 2");
    }

    if( bind(socket_1,( struct sockaddr*) & addr, sizeof(addr)) < 0){
        erro("função bind 1");
    }
    if( bind(socket_2,( struct sockaddr*) & addr, sizeof(addr)) < 0){
        erro("função bind 2");
    }

    if( listen(socket_1, 1) < 0){
	    erro("na funcao listen 1");
    }
    if( listen(socket_2, 1) < 0){
	    erro("na funcao listen 2");
    }

    client1_addr_size = sizeof(client1_addr);
    client2_addr_size = sizeof(client2_addr);
    
    while(1){
        while(waitpid(-1, NULL, WNOHANG)>0);

        client_1 = accept(socket_1, (struct sockaddr *)&client1_addr,(socklen_t *)&client1_addr_size);
        client_2 = accept(socket_2, (struct sockaddr *)&client2_addr,(socklen_t *)&client2_addr_size);
        
        if(client_1 > 0){
            if(fork() == 0){
                close(socket_1);
                process_client(client_1, &client1_addr);
                exit(0);
            }
            close(client_1);
        }
        if(client_2 > 0){
            if(fork() == 0){
                close(socket_2);
                process_client(client_2, &client2_addr);
                exit(0);
            }
            close(client_2);
        }   
    }
    return 0;
}

void process_client(int client_socket, struct sockaddr_in *client_addr){

    char dst[16];
    char message[BUFFER_SIZE];

    inet_ntop(client_addr -> sin_family, &client_addr -> sin_addr, dst, INET_ADDRSTRLEN);
    

}

void erro(char *msg){
    printf("Erro: %s\n", msg);
    exit(-1);
}