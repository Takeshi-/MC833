#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

int main(int argc, char *argv[])
{
    struct sockaddr_in socket_address,client;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s,ac,valid;
    
    if (argc != 3){
        printf("Number of arguments incorrect.\n");
        exit(1);
    }
    
    /* criação da estrutura de dados de endereço */
    bzero((char *)&socket_address, sizeof(socket_address));
	//(...)
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(12345); 
    bzero(&(socket_address.sin_zero),8);

    /* criação de socket passivo */
	//(...)
	s = socket(AF_INET,SOCK_STREAM,0);
    
    if (s < 0) {
        printf("Error in making socket.\n");
        exit(1);
    }

	/* Associar socket ao descritor */
	//(...)
	if (bind(s,(struct sockaddr *)&socket_address,sizeof(socket_address)) == -1) {
        printf("Error in binding.\n");
        exit(1);
    }

	/* Criar escuta do socket para aceitar conexões */
	//(...)
	listen(s,MAX_PENDING);
    new_s = sizeof(client);
    /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
	//(...)
	while(1){
        ac = accept(s,(struct sockaddr *)&client,&new_s);
        if (ac == -1){
            printf("Error in accepting.\n");
            exit(1);
        }
        else {
            printf("Connected to client.\n");
        }
        valid = read(ac,(char *)buf, MAX_LINE);
        if (strcmp(buf,"Close")){
            printf("Client closed connection.\n");
            break;
        }
        printf("Client: %s\n",buf);
        valid = write(ac,(char *)buf, MAX_LINE);
    }
    
    close(s);
    return 0;
}
