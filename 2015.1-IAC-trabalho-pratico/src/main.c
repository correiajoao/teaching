#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <stdio.h> 
#include <stdlib.h>

int main (int argc, char *argv[]) {

	int pid ; /* identificador de processo */
	
	pid = fork () ; /* replicação do processo */
	
	if ( pid < 0 ) { /* se fork não funcionou */
		perror ("Erro: ") ;
		exit (-1) ; /* encerra o processo com código de erro -1 */ 
	}else if( pid > 0 ){	
		//TODO guarde a cada segundo o consumo de memória (em Kilobytes) e CPU (em porcentagem) do processo filho
		//TODO após 10 segundos de execução, mate o proceso filho
	}else{
		
		if((strcmp(argv[1],"cpu") == 0)){
			printf("CPU\n");
			//TODO se argv[1] for igual a 'cpu', executar código com utilização intensa da UCP
		}else if((strcmp(argv[1],"cpu-mem") == 0)){
			printf("CPU-MEM\n");
			//TODO se argv[1] for igual a 'cpu-mem', executar código com utilização intensa da UCP e da memória:	
		}
		
	}
	return 0;
}
