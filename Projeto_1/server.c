/* Fiz pequenas mudanças para ser sempre continuo -> do while*/
/* Por algum motivo ainda nao funciona pra parar a conexao */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

int main()
{
    struct sockaddr_in socket_address,client;
    char buf[MAX_LINE];
    unsigned int len;
    int s, new_s,ac,valid;
    
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
	if (bind(s,(struct sockaddr *)&socket_address,sizeof(socket_address)) < 0) {
        printf("Error in binding.\n");
        exit(1);
    }

	/* Criar escuta do socket para aceitar conexões */
	//(...)
	listen(s,MAX_PENDING);
    new_s = sizeof(client);
    /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
	//(...)
	
    ac = accept(s,(struct sockaddr *)&client,&new_s);
    if (ac == -1){
        printf("Error in accepting.\n");
        exit(1);
    }
    else {
        printf("####Connected to client####\n");
    }
    do{
        bzero(buf,MAX_LINE);
        valid = read(ac,buf, MAX_LINE);
        if (valid < 0){
            printf("ERROR\nCould not read from socket\n");
            exit(1);            
        }
        printf("Client: %s\n",buf);
        valid = write(ac,buf, MAX_LINE);
        if (valid < 0) {
            printf("ERROR \nCould not write to socket\n");
            exit(1);
        }
        
    } while(strcmp(buf,"quit") != 0);
    
    close(s);
    
    return 0;
}
