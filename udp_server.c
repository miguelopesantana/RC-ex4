#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>

#define BUFLEN 512	// Tamanho do buffer
#define PORT 9876	// Porto para recepção das mensagens
#define MAX_LENGTH 100
#define NUM_STRINGS 5

void erro(char *s) {
	perror(s);
	exit(1);
	}

int main(void) {
  struct sockaddr_in si_minha, si_outra;

	int sckt, recv_len;
	socklen_t socketlen = sizeof(si_outra);
	char buffer[BUFLEN];

	// Cria um socket para recepção de pacotes UDP
	if((sckt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		erro("Erro na criação do socket");
	}

    // Preenchimento da socket address structure
	si_minha.sin_family = AF_INET;
	si_minha.sin_port = htons(PORT);
	si_minha.sin_addr.s_addr = htonl(INADDR_ANY);

	// Associa o socket à informação de endereço
	if(bind(sckt,(struct sockaddr*)&si_minha, sizeof(si_minha)) == -1) {
		erro("Erro no bind");
	}

	// Espera recepção de mensagem (a chamada é bloqueante)
	if((recv_len = recvfrom(sckt, buffer, BUFLEN, 0, (struct sockaddr *) &si_outra, (socklen_t *)&socketlen)) == -1) {
	  erro("Erro no recvfrom");
	}
	// Para ignorar o restante conteúdo (anterior do buffer)
	buffer[recv_len]='\0';

	printf("Recebi uma mensagem do sistema com o endereço %s e o porto %d\n", inet_ntoa(si_outra.sin_addr), ntohs(si_outra.sin_port));
	printf("Conteúdo da mensagem recebida: %s\n", buffer);

	char lista[NUM_STRINGS][MAX_LENGTH] = { {"bola"},
                                          {"sofa"},
                                          {"urso"},
                                          {"peruca"},
                                          {"baguete"} };

    bool flag = false;
    for(int i = 0; i < len(lista); i++){
		if(strcmp(buffer, lista[i]) == 0){
			flag = true;
			break;
		}else if(strcmp(buffer, lista[i]) != 0) {
			flag = false;
		}
	}
	if(flag == true){
		printf("A palavra inserida é reservada!");
	}else{
		printf("A palavra inserida não é reservada!");
	}
	// Fecha socket e termina programa
	close(sckt);
	return 0;
}
