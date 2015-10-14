#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


//Função para o teste de consumo de ucp e memória
void memoryucptest(){
	for(;;){          
		malloc(1024);
	}
}

void ucptest(){
	for(;;);
} 

int main (int argc, char *argv[]) {
	
	int i,pid,core;
	float memory, ucp;
	char dataignored[50],commandline[50];
	
	pid = fork(); 
	
	if ( pid < 0 ) { //Se o fork não funcionou
		perror ("Erro: ") ;
		exit (-1) ;  
	}else if( pid > 0 ){ //Se for o processo pai, inicia-se o processo de monitoramento 
		
		printf("Monitoramento iniciado.\n");
		core = get_nprocs_conf();
		printf("Núcleos identificados: %d\n", core);

		FILE *scanTerminal; // Ponteiro para a leitura do retorno dos comandos passados ao terminal
		FILE *out; 			// Ponteiro para o arquivo de saida dos dados gerados
				
		if((strcmp(argv[1],"cpu") == 0)){ //Código de monitoramento do uso de UCP
			
			out = fopen("out.txt", "a");
			fprintf(out, "%s","\n");
				
			for(i=0;i<10;i++){
				sleep(1);
				
				printf("Time: %ds    ", i);
				fprintf(out, "%s%d    ","Time: ",i);
				
				//Gerando linha de comando para monitoramento de UCP: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &ucp);
				
				//Imprime os dados no terminal e no arquivo de saida
				printf("UCP: %.2f%%\n", ucp/core);
				fprintf(out, "UCP: %.2f%%\n", ucp/core);
				fclose(scanTerminal);
				
			}
			
			//Após 10s, morte do filho
			//KILL(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			fclose(out);
			
		}else if ((strcmp(argv[1],"cpu-mem") == 0)){
			
			out = fopen("out.txt", "a");
			fprintf(out, "%s","\n");
			
			//Se o argv[1] for igual a 'cpu-mem', o pai irá monitorar o consumo de CPU e memória					
			for(i=0;i<10;i++){
				sleep(1);
				
				printf("Time: %ds    ", i);
				fprintf(out, "%s%d    ","Time: ",i);
				
				//Gerando linha de comando para monitoramento de UCP: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &ucp);
				fclose(scanTerminal);
				printf("UCP: %.2f%%    ", ucp/core);
				fprintf(out,"UCP: %.2f%%    ", ucp/core);
				
				//Gerando linha de comando para o monitoramento de memória: cat /proc/[pid]/status | grep 'VmSize'
				sprintf(commandline,"%s%d%s","cat /proc/",pid,"/status | grep 'VmSize'");
				//Abrindo o processo terminal com um pipe
				scanTerminal = popen(commandline, "r");
				//Lendo dados de retorno
				fscanf(scanTerminal,"%s %f %s", dataignored, &memory, dataignored);
				
				printf("Memory: %.2f KB\n", memory);
				fprintf(out,"Memory: %.2f KB\n", memory);
				
				fclose(scanTerminal);
			}
			
			//Após 10s, morte do filho
			//kill(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			fclose(out);
		
		}	
	}else{
		
		//Se for o processo filho ele analisa a entrada do argv[i] e executa o código correspontente  
		if((strcmp(argv[1],"cpu") == 0)){	
			ucptest();
		}else if((strcmp(argv[1],"cpu-mem") == 0)){
			memoryucptest();
		}		
	}
	
	return 0;
}
