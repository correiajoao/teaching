#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


/* Descrição: Essa função executa um teste de uso intenso de cpu e memória
   Paramentros: 
   Retorno: void
*/
void cpumemorytest(){
	for(;;){          
		malloc(sizeof(10000));
	}
}
/* Descrição: Essa função executa um teste de uso intenso de cpu
   Paramentros:
   Retorno: void
*/
void cputest(){
	for(;;){
	}
} 

int main (int argc, char *argv[]) {	
	
	int i,pid,core;
	float memory, cpu;
	char dataignored[50],commandline[50];
	
	pid = fork(); 
	//Se o fork não funcionou esse trecho de código é executado
	if ( pid < 0 ) { 
		perror ("Erro: ") ;
			exit (-1) ;  
	
	//Se o processo for o pai esse trecho de código é executado
	}else if( pid > 0 ){  
		
		printf("Monitoramento iniciado.\n");
		core = get_nprocs_conf();
		printf("Núcleos identificados: %d\n", core);

		FILE *scanTerminal; // Ponteiro para a leitura do retorno dos comandos passados ao terminal
		FILE *out; 			// Ponteiro para o arquivo de saida dos dados gerados
				
		if((strcmp(argv[1],"cpu") == 0)){ //Código de monitoramento do uso de CPU	
			
			out = fopen("out.txt", "a");
			fprintf(out, "%s","\n");
			
			for(i=0;i<10;i++){
				printf("Time: %ds    ", i);
				fprintf(out, "%s%d    ","Time: ",i);
		
				//Gerando linha de comando para monitoramento de 	: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &cpu);
				
				//Imprime os dados no terminal e no arquivo de saida
				printf("CPU: %.2f%%\n", cpu/core);
				fprintf(out, "CPU: %.2f%%\n", cpu/core);
				fclose(scanTerminal);
				
				//Espera 1s para o controle dos 10s
				sleep(1);
			}
			
			//Após 10s, morte do filho
			//KILL(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			//O arquivo de gravação é fechado
			fclose(out);

		}else if ((strcmp(argv[1],"cpu-mem") == 0)){
			
			out = fopen("out.txt", "a");
			fprintf(out, "%s","\n");
			
			//Se o argv[1] for igual a 'cpu-mem', o pai irá monitorar o consumo de CPU e memória					
			for(i=0;i<10;i++){	
				printf("Time: %ds    ", i);
				fprintf(out, "%s%d    ","Time: ",i);
				
				//Gerando linha de comando para monitoramento de CPU: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Lendo dados do retorno
				fscanf(scanTerminal,"%s %f", dataignored, &cpu);
				fclose(scanTerminal);
				printf("CPU: %.2f%%    ", cpu/core);
				fprintf(out,"CPU: %.2f%%    ", cpu/core);
				
				//Gerando linha de comando para o monitoramento de memória: cat /proc/[pid]/status | grep 'VmSize'
				sprintf(commandline,"%s%d%s","cat /proc/",pid,"/status | grep 'VmSize'");
				
				//Abrindo o processo terminal com um pipe
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline, "r");
				
				//Lendo dados de retorno
				fscanf(scanTerminal,"%s %f %s", dataignored, &memory, dataignored);
				
				printf("Memory: %.2f KB\n", memory);
				fprintf(out,"Memory: %.2f KB\n", memory);
				fclose(scanTerminal);
				
			    //Espera 1s para o controle dos 10s
				sleep(1);
			}
			
			//Após 10s, morte do filho
			//kill(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			fclose(out);
		
		}	
		
	//Se o processo for o filho esse trecho de código é executado	
	}else if(pid == 0){

		if((strcmp(argv[1],"cpu") == 0)){ //Se o parâmetro de entrada for 'cpu' inicia-se o teste de cpu 	
			cputest();
		}else if((strcmp(argv[1],"cpu-mem") == 0)){ //Se o parâmetro de entrada for 'cpu-mem' inicia-se o teste de cpu e memória
			cpumemorytest();
		}		
	
	}
	
	return 0;
}
