/* Fiz pequenas mudanças para ser sempre continuo -> do while*/
/* Por algum motivo ainda nao funciona pra parar a conexao */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#define LISTEN_PORT 12345
#define MAX_PENDING 5
#define MAX_LINE 256

enum Direction {
  north =0,
  east ,
  south ,
  west
} avenue;

struct Zona {
    float t_enter;
    float t_out;
}; typedef struct Zona Zona;

struct Carro{
    float veloc;
    float tempo;
    int av;
    int size;
    int pos;
    Zona z1,z2;
}; typedef struct Carro Carro;

Carro initiateCar(int val[]) {
    Carro car;
    car.veloc = val[0];
    car.pos = val[1];
    car.av = val[2];
    car.tempo = val[4];
    car.size = val[5];
    if ( (val[3] > 0 && (car.av == north || car.av == east)) || (val[3] < 0 && (car.av == west || car.av == south)) ) {
        car.z1.t_enter = -1;
        car.z2.t_enter = -1;
        car.z1.t_out = -1;
        car.z2.t_out = -1;
    }
    else {
        car.z1.t_enter = (car.pos/car.veloc + car.tempo);
        car.z2.t_enter = ((car.pos+1)/car.veloc + car.tempo);
        car.z1.t_out = ((car.pos+car.size+1)/car.veloc + car.tempo);
        car.z2.t_out = ((car.pos+car.size+1)/car.veloc + car.tempo);
    }
    return car;
}

int collision(Carro cars[],int col1,int *col2,float *colTime){
    flag =0;
    if (cars[col1].z1.t_enter >= cars[((col1+5)%4)].z2.t_enter && cars[col1].z1.t_enter <= cars[((col1+5)%4)].z2.t_out){
        (*col2) = (i+5)%4;
        (*colTime) = cars[col1].z1.t_enter;
        flag =1;
    }
        //Vizinho da esquerda
    if (cars[i].z2.t_enter >= cars[((i+7)%4)].z1.t_enter && cars[i].z2.t_enter <= cars[((i+7)%4)].z1.t_out){
        (*col2) = (i+7)%4;
        (*colTime) = cars[i].z2.t_enter;
        flag =1;
    }

    return flag;
}

float freia(int pos0, float t0,float tF) {
    return (pos0-2)/(tF-t0);
}

float acelera(int pos0, float t0,float tF) {
    return (pos0+2)/(tF-t0);
}

Carro *CopyStructNewV(Carro cars[],int col2, float newV){
    Carro *new = malloc(sizeof(Carro)*4);
    int i;
    for (i=0;i<4;i++) {
        new[i].pos = cars[i].pos;
        new[i].av = cars[i].av;
        new[i].tempo = cars[i].tempo;
        new[i].size = cars[i].size;
        if ( i != col2) {
            new[i].veloc = cars[i].veloc;
            new[i].z1.t_enter = cars[i].z1.t_enter;
            new[i].z2.t_enter = cars[i].z2.t_enter;
            new[i].z1.t_out = cars[i].z1.t_out;
            new[i].z2.t_out = cars[i].z2.t_out;
        }
        else {
            new[i].veloc = newV;
            new[i].z1.t_enter = (new[i].pos/new[i].veloc + new[i].tempo);
            new[i].z2.t_enter = ((new[i].pos+1)/new[i].veloc + new[i].tempo);
            new[i].z1.t_out = ((new[i].pos+new[i].size+1)/new[i].veloc + new[i].tempo);
            new[i].z2.t_out = ((new[i].pos+new[i].size+1)/new[i].veloc + new[i].tempo);
        }
    }
    return new;
}

int main()
{
  struct sockaddr_in socket_address, client;
  char buf[MAX_LINE];
  unsigned int len;
  int s, new_s, ac, valid,i,val[6],aux,col1,col2,flag;
  float colTime,newV;
  char *resp;
  pid_t p;
  socklen_t leng;
  Carro cars[4],*newCars;
  struct sockaddr_storage addr;
  char ipstr[INET6_ADDRSTRLEN];
  int port, sockfd, client_num, maxfd, nready, clients[FD_SETSIZE];
  fd_set all_fds, new_set;

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

  printf("Waiting for client connecttion...\n");

  maxfd = s;
  client_num = -1;
  for (i = 0; i < FD_SETSIZE; i++)
    clients[i] = -1;

  FD_ZERO(&all_fds);
  FD_SET(s, &all_fds);


  while (1) {
    new_set = all_fds;
    nready = select(maxfd+1, &new_set, NULL, NULL, NULL);
    if(nready < 0) {
      perror("select" );
      exit(1);
    }

    if(FD_ISSET(s, &new_set)) {
      len = sizeof(socket_address);
      /* aguardar/aceita conexão, receber e imprimir texto na tela, enviar eco */
  	  //(...)

      if ((new_s = accept(s, (struct sockaddr *)&socket_address, &len)) < 0) {
        printf("Error in accepting.\n");
        exit(1);
      }
      else {
        leng = sizeof(socket_address);

        if (getpeername(new_s, (struct sockaddr *)&socket_address, &leng) == -1)
    		  printf("ERROR\nCould not getsockname - 1\n");
    	  else {
    		  printf("\nConnected to:\nClient IP Address: %s\n", inet_ntoa(socket_address.sin_addr));
    		  printf("Client Port Number: %d\n\n", ntohs(socket_address.sin_port));
        }
      }
      for (i = 0; i < FD_SETSIZE; i++) {
        if (clients[i] < 0) {
          clients[i] = new_s; 	//guarda descritor
          break;
        }
      }
      if (i == FD_SETSIZE) {
        perror("Numero maximo de clientes atingido.");
      	exit(1);
      }
      FD_SET(new_s, &all_fds);		// adiciona novo descritor ao conjunto
      if (new_s > maxfd)
        maxfd = new_s;			// para o select
      if (i > client_num)
        client_num = i;		// índice máximo no vetor clientes[]
      if (--nready <= 0)
        continue;			// não existem mais descritores para serem lidos
    }
    for (i = 0; i <= client_num; i++) {	// verificar se há dados em todos os clientes
      if ( (sockfd = clients[i]) < 0)
        continue;
      if (FD_ISSET(sockfd, &new_set)) {
        if ( (len = recv(sockfd, buf, sizeof(buf), 0)) == 0) {
        //conexão encerrada pelo cliente
          printf("\nClient disconnected:\n");
          if (getpeername(new_s, (struct sockaddr *)&socket_address, &leng) == -1)
      		  printf("ERROR\nCould not getsockname\n");
        	else {
        		printf("Client IP Address: %s\n", inet_ntoa(socket_address.sin_addr));
            printf("Client Port Number: %d\n", ntohs(socket_address.sin_port));
          }
          close(sockfd);
          FD_CLR(sockfd, &all_fds);
          clients[i] = -1;
        }
        else {
          /* imprime ip e porta do cliente e envia texto de volta */
          leng = sizeof(socket_address);
          if (getpeername(new_s, (struct sockaddr *)&socket_address, &leng) == -1)
      		  printf("ERROR\nCould not getsockname\n");
        	else {
        		printf("Client IP Address: %s\n", inet_ntoa(socket_address.sin_addr));
            printf("Client Port Number: %d\n", ntohs(socket_address.sin_port));
          }
          printf("Message: %s\n",buf);

          aux=0;
          char * pch;
          pch = strtok (buf," ,");
          while (pch != NULL)
          {
              printf ("%s\n",pch);
              val[aux] = atoi(pch);
              pch = strtok (NULL, " ,");
              aux++;
          }

          if (val[4] > colTime){
              //TODO:imprimir chamar ambulancia
          }

          if (val[4] > cars[i].tempo) {
              cars[i] = initiateCar(val);
              flag = collision(cars,i,&col2,&colTime);
          }

          if (flag) {
              //TODO: alertar carros envolvidos de possivel colisao

              //resp = "colisao entre col1 e col2";

              if (cars[i].veloc <= 5) {
                      //resp = "Pare.\n"/;
                      //TODO: Mudar para qual mandar
                      valid = write(sockfd,resp, MAX_LINE);
                  }
                  else {
                      newV = freia(cars[i].pos,cars[i].tempo,colTime);
                      newCars = CopyStructNewV(cars,i,newV);
                      if (collision(newCars,i,&col2,&colTime)) {
                          newV = acelera(cars[i].pos,cars[i].tempo,colTime);
                          newCars = CopyStructNewV(cars,i,newV);
                          //TODO: arrumar saida
                          if (collision(newCars,i,&col2,&colTime)) {
                              //resp = "Pare.\n";
                              //TODO: Mudar para qual mandar
                              valid = write(sockfd,resp, MAX_LINE);
                          }
                          else {
                              //resp = "Acelere para %f.\n",newV;
                              //TODO: Mudar para qual mandar
                              valid = write(sockfd,resp, MAX_LINE);
                          }
                      }
                      else {
                          //resp = "Freie para %f.\n",newV;
                          //TODO: Mudar para qual mandar
                          valid = write(sockfd,resp, MAX_LINE);
                      }
                  }
              }
          }

          valid = write(sockfd,buf, MAX_LINE);
          if (valid <= 0) {
            printf("ERROR\nCould not write to socket\n");
            exit(1);
          }
        }
        if (--nready <= 0)
          break;
      }
    }
  }
  return 0;
}
