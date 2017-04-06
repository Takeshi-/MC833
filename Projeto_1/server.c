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
    struct sockaddr_in socket_address;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s;
    
    if (argc != 3){
        printf("Numero de argumentos invalidos.\n");
        exit(1);
    }
    
    s = socket(AF_INET,SOCK_STREAM,0);
    
    if (s < 0) {
        printf("Erro ao criar socket\n");
        exit(1);
    }
    
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_address.sin_port = htons(12345); 
    
    /* criação da estrutura de dados de endereço */
    bzero((char *)&socket_address, sizeof(socket_address));
	//(...)

    /* criação de socket passivo */
	//(...)

	/* Associar socket ao descritor */
	//(...)
	if (bind(s,(struct sockaddr *)&socket_address,sizeof(socket_address)) == -1) {
        printf("Erro ao fazer bind.\n");
        exit(1);
    }

	/* Criar escuta do socket para aceitar conexões */
	//(...)
	//listen(s,);

    /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
	//(...)
    
    return 0;
}
