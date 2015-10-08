#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

int main (int argc, char *argv[]) {
	
	int i;
	int pid;
	float memory, ucp;
	char dataignored[50];
	char commandline[50];
	
	pid = fork(); 
	
	if ( pid < 0 ) { //Se o fork não funcionou
		perror ("Erro: ") ;
		exit (-1) ;  
	}else if( pid > 0 ){ //Se for o processo pai, inicia-se o processo de monitoramento 

		FILE *scanTerminal; // Ponteiro para a leitura do retorno dos comandos passados ao terminal
		if((strcmp(argv[1],"cpu") == 0)){ //Código de monitoramento do uso de UCP
			
			for(i=0;i<10;i++){
				sleep(1);
				printf("Time: %ds -- ", i);
				
				//Gerando linha de comando para monitoramento de UCP: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &ucp);
				fclose(scanTerminal);
				printf("UCP: %.2f%%", ucp);
			}
			
			//Após 10s, morte do filho
			//KILL(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			
		}else if ((strcmp(argv[1],"cpu-mem") == 0)){
			//Se o argv[1] for igual a 'cpu-mem', o pai irá monitorar o consumo de CPU e memória					
			for(i=0;i<10;i++){
				sleep(1);
				printf("Time: %ds -- ", i);
	
				//Gerando linha de comando para monitoramento de UCP: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &ucp);
				fclose(scanTerminal);
				printf("UCP: %.2f%% -- ", ucp);
				
				//Gerando linha de comando para o monitoramento de memória: cat /proc/[pid]/status | grep 'VmSize'
				sprintf(commandline,"%s%d%s","cat /proc/",pid,"/status | grep 'VmSize'");
				//Abrindo o processo terminal com um pipe
				scanTerminal = popen(commandline, "r");
				//Lendo dados de retorno
				fscanf(scanTerminal,"%s %f %s", dataignored, &memory, dataignored);
				fclose(scanTerminal);
				printf("Memory: %.2f KB\n", memory);
			}
			
			//Após 10s, morte do filho
			//kill(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
		}	
	}else{
		
		//Se for o processo filho ele analisa a entrada do argv[i] e executa o código correspontente  
		if((strcmp(argv[1],"cpu") == 0)){	
			for(;;){} //Código com uso intenso da UCP
		}else if((strcmp(argv[1],"cpu-mem") == 0)){
			for(;;){ //Código com uso intenso de UCP e Memória         
				malloc(1024);
				usleep(10000);	
			}
		}		
	}
	
	return 0;
}
