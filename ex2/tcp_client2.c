/**********************************************************************
 * CLIENTE liga ao servidor (definido em argv[1]) no porto especificado
 * (em argv[2]), escrevendo a palavra predefinida (em argv[3]).
 * USO: >cliente <enderecoServidor>  <porto>  <Palavra>
 **********************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

void erro(char *msg);

int main(int argc, char *argv[]){
    char endServer[100];
    int client_1, client_2;
    struct sockaddr_in addr;
    struct hostent *hostPtr;
    
    if(argc != 4){
        printf("Cliente <host> <port> <string>\n");
        exit(-1);
    }

    strcpy(endServer, argv[1]);
    if ((hostPtr = gethostbyname(endServer)) == 0){
        erro("Não consegui obter endereço");
    }

    bzero((void *) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr -> h_addr)) -> s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));
    
    //criar socket jogador 1
    if((client_1 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        erro("socket 1");
    }
    //criar socket jogador 2
    if((client_2 = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        erro("socket 2");
    }
    //iniciar ligação do socket do jogador 1
    if(connect(client_1,(struct sockaddr *)&addr, sizeof(addr)) < 0){
        erro("Connect");
    }
    //iniciar ligação do socket do jogador 2
    if(connect(client_2,(struct sockaddr *)&addr, sizeof(addr)) < 0){
        erro("Connect");
    }

    //-------------- dúvida -------------------------
    int nread = 0;
    char buffer[BUFFER_SIZE];
    nread = readf(client_1, argv[3], 1 + strlen(argv[3]));
    buffer[nread] = '\0';
    printf("%s\n", buffer);
    fflush(stdout);
    //---------------------------------------------------

    close(client_1);
    close(client_2);
    exit(0);

}

void erro(char *msg){
    printf("Erro: %s\n", msg);
    exit(-1);
}