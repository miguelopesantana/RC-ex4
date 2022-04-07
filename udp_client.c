#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define PORT 9876
#define BUF_SIZE 512 //Tamanho do Buffer

void erro(char *msg);

int main(int argc, char *argv[]){
    struct sockaddr_in addr;

    if(argc != 4){
        printf("cliente <host> <port> <string>\n");
        exit(-1);
    }

    int sckt;
    int recv_len;
    int scktlen = sizeof(addr);
    char buffer[BUF_SIZE];
    char endServer[100];
    struct hostent *hostPtr;

    strcpy(endServer, argv[1]);
    if((hostPtr = gethostbyname(endServer)) == 0){
        erro("Não consegui obter endereço");
    }

    bzero((void*) &addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = ((struct in_addr *)(hostPtr -> h_addr)) -> s_addr;
    addr.sin_port = htons((short) atoi(argv[2]));

    if((sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        erro("Erro na criação do socket");
    }

    if((sendto(sckt, argv[3], strlen(argv[3]), 0, (struct sockaddr *)&addr, sizeof(addr))) == -1){
        erro("Erro no sendto");
    }

    if(( recv_len = recvfrom(sckt, buffer, BUF_SIZE, 0, (struct sockaddr *)&addr, (socklen_t *)&scktlen)) == -1){
        erro("Erro o recvfrom");
    }

    printf("%s", buffer);
    close(sckt);
    exit(0);
}

void erro(char *msg){
    printf("Erro: %s\n", msg);
    exit(-1);
}