#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define SIZE 1024

int main(int argc, char *argv[]){
    int fd, req_size, num_req;
    float etime;
    if (argc < 4) {
		  printf("Uso: %s filename req_size num_req [-sr, -rr, -ir, -fr frag_size, -sw, -rw]\n",argv[0]); 
		  exit(1);
	  }
	  req_size = atoi(argv[2]);
    printf("REQSIZE = %d\n", req_size);
	  if(req_size <= 0 || req_size > 1024*1024) {
		  printf("0 < req_size < 2^20\n");
		  exit(1);
	  }else
		  req_size = SIZE;
 
    num_req = atoi(argv[3]);
    printf("NUMREQ = %d\n", num_req);
    if(num_req <=0 || num_req * req_size > 1024*1024*1024) {
      printf("Tam arquivo (req_size * num_req) < 2^30\n");
      exit(1);
    }

    if((fd = open(argv[1], O_RDWR)) < 0) {
		  perror("Erro abrindo arquivo");
		  exit (0);
	  }
    int nbytes = num_req * req_size;

    /*OPERAÇÕES AQUI INICIO*/


    /*ESCRITA SEQUENCIAL INICIO*/
    char buffer[req_size];     // como cada char utiliza 1 byte foi utilizado esse vetor auxiliar para a escrita de cada requisicao
    int i = 0;            
    struct timeval inic, fim;   //estruturas utilizadas para calcular o tempo demandado pelo codigo
    while(i < req_size){
        buffer[i] = 'a';
        i++;
    }
  	gettimeofday(&inic,0);

    /*escrevendo os bloco do buffer num_req vezes*/
    for(i = 0; i < num_req; i++){
      write(fd, buffer, req_size * sizeof(char));
    }
	  gettimeofday(&fim,0);
    close(fd);

    etime = (fim.tv_sec + fim.tv_usec/1000000.) - (inic.tv_sec + inic.tv_usec/1000000.) ;
    printf("\nTempo decorrido escrita: %f s\n", etime);
    /*ESCRITA SEQUENCIAL FIM*/

    /*ESCRITA ALEATORIA INICIO*/
	  if((fd = open(argv[1], O_RDWR)) < 0) {
		  perror("Erro abrindo arquivo");
		  exit (0);
	  }
	  i = 0;
	  while(i < req_size){
      buffer[i] = 'b';
      i++;
    }
    
	  gettimeofday(&inic,0);
    /*escrevendo os bloco do buffer num_req vezes nas posições aleatórias sorteadas pela função rand()*/
    for (i = 0; i < num_req; i++){
		  lseek(fd, rand() % nbytes, SEEK_SET);
      write(fd, buffer, req_size * sizeof(char));
    }
	  gettimeofday(&fim,0);
	  close(fd);

	  etime = (fim.tv_sec + fim.tv_usec/1000000.) - (inic.tv_sec + inic.tv_usec/1000000.) ;
    printf("\nTempo decorrido escrita aleatoria: %f s\n", etime);
	  /*ESCRITA ALEATORIA FIM*/


    /*INICIO LEITURA SEQUENCIAL*/

    open(argv[1], O_RDONLY);

    if (fd == -1) {
        perror("Erro ao abrir o arquivo");
        exit(1);
    }
    int contador = 0;

        gettimeofday(&inic,0);



    while(contador < num_req) {
        contador++;
        nbytes = read(fd, buffer, (req_size-1));
        if (nbytes < 0) {
            printf("Erro na leitura do arquivo   %d ",contador);
            close(fd);
            exit(1);
        }
    }

    close(fd);

    gettimeofday(&fim,0);
    etime = (fim.tv_sec + fim.tv_usec/1000000.) - (inic.tv_sec + inic.tv_usec/1000000.) ;
    printf("\nTempo decorrido leitura sequencial: %f s\n", etime);

    /*OPERAÇÕES AQUI FIM*/



    //close(fd);
    return 0;
}
