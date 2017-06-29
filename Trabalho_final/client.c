/* Fiz pequenas mudanças para ser sempre continuo -> do while */
/* Por algum motivo ainda nao funciona pra parar a conexao */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#define SERVER_PORT 12345
#define MAX_LINE 256

//TODO: loop 100 ms para entretenimento, mandar mensagem pedindo resposta

int main(int argc, char * argv[])
{
        struct hostent *host_address;
        struct sockaddr_in socket_address;
        char *host;
        char buf[MAX_LINE];
        char *timestamp;
        int s;
        int len;


	/* verificacao de argumentos */
	if (argc > 2){
		printf(" ERROR \nToo many arguments!\n Was expecting only one argument: HOSTNAME\n");
		exit(1);
	}

	if (argc < 2) {
		printf(" ERROR \nMissing argument!\n Was expecting one argument: HOSTNAME\n");
		exit(1);
	}

	/* le hostname */
	host = argv[1];


	/* traducao de nome para endereco IP */
	 if ( (host_address = gethostbyname(host) ) == NULL ) {
		printf(" ERROR \nCould not resolve HOSTNAME\n");
		exit(1);
  	}


    /* criacao da estrutura de dados de endereco */
    bzero((char *)&socket_address, sizeof(socket_address));
	socket_address.sin_family = AF_INET;
  	socket_address.sin_port = htons(SERVER_PORT);
   	bcopy((char *)host_address->h_addr, (char *)&socket_address.sin_addr.s_addr, host_address->h_length);


   /* criacao de socket ativo*/
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s < 0) {
    	printf("ERROR \nCould not open socket");
     	exit(1);
   	}

	/* estabelecimento da conexao */

	if ( connect(s, (struct sockaddr *)&socket_address, sizeof(socket_address) ) < 0) {
		printf(" ERROR \nConnection Failed\n");
		exit(1);
  	}

    printf("Connected to server %s.\n",argv[1]);

	/* obter informacoes sobre socket local */
	socklen_t s_len = sizeof(socket_address);

	if (getsockname(s, (struct sockaddr *)&socket_address, &s_len) == -1)
		printf("ERROR getsockname ERROR\n");
	else
		printf("Local IP Address: %s\n", inet_ntoa(socket_address.sin_addr));
		printf("Local Port Number: %d\n", ntohs(socket_address.sin_port));


    printf("To end connection press 'Ctrl+C' .\n\n");

    /* ler e enviar linhas de texto, receber eco */
    do {
        printf("\nPlease enter the following:\nSpeed Position Avenue(North=0,East=1,South=2,West=3) Direction(1 ou -1) Size Time\n");
        bzero(buf,	MAX_LINE);
        fgets(buf,MAX_LINE-1,stdin);
        /* Send message to the server */
        len = write(s, buf, MAX_LINE);
        
        if (len <= 0) {
            printf("ERROR\nCould not write to socket\n");
            exit(1);
        }

        /* Now read server response */
        bzero(buf,MAX_LINE);
        len = read(s, buf, MAX_LINE);

        if (len <= 0) {
            printf("ERROR\nCould not read from socket\n");
            exit(1);
        }

        printf("ECO Responce from server: %s\n",buf);

    } while (strcmp(buf,"quit\n") != 0);
	return 0;
}
