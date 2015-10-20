#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 


/* Description: This function executes an intense cpu use
   Parameters:
   Return: void
*/
void cputest(){
	for(;;){
	}
} 

/* Description: This function executes an intense use of cpu and memory 
   Parameters:
   Return: void
*/
void cpumemorytest(){
	for(;;){          
		malloc(sizeof(10000));
	}
}

/* Description: This is the main function 
   Parameters: int argc, char *argv[]
   Return: int
*/
int main (int argc, char *argv[]) {	
	
	int i;	//Controller
	int pid;	//Process identification
	int core;	//Number of cpu's cores
	float cpu;	//Cpu usage
	float memory;	//Memory usage
	char dataignored[50];	//Aux to reading data
	char commandline[50];	//Monitoring commands 	
	
	//fork(2) - Linux man page - http://linux.die.net/man/2/fork
	pid = fork(); 
	
	//If an error occurred in the fork
	if ( pid < 0 ) { 
		perror ("Erro: ") ;
			exit (-1) ;  
	
	//If is the father process
	}else if( pid > 0 ){  
		
		printf("Starting monitoring\n");
		core = get_nprocs_conf();
		printf("Identified cores: %d\n", core);

		FILE *scanTerminal; 
		FILE *output; 		
		
		//Code for monitoring cpu
		if((strcmp(argv[1],"cpu") == 0)){  	
			
			output = fopen("output.txt", "a");
			fprintf(output, "%s","\n");
			
			for(i=0;i<10;i++){
				printf("Time: %ds    ", i);
				fprintf(output, "%s%d    ","Time: ",i);
		
				//Generating command line for monitoring cpu: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Running the command and capturing the output
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Reading data
				fscanf(scanTerminal,"%s %f", dataignored, &cpu);
				
				//Printing data in the terminal and output file
				printf("CPU: %.2f%%\n", cpu/core);
				fprintf(output, "CPU: %.2f%%\n", cpu/core);
				fclose(scanTerminal);
				
				//Waits one second
				sleep(1);
			}
			
			//After 10 seconds the child is dead
			//KILL(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			fclose(output);
		
			printf("Output file generated: output.txt\n");
		}else if ((strcmp(argv[1],"cpu-mem") == 0)){
			
			output = fopen("output.txt", "a");
			fprintf(output, "%s","\n");
			
			//Code for monitoring cpu and memory					
			for(i=0;i<10;i++){	
				printf("Time: %ds    ", i);
				fprintf(output, "%s%d    ","Time: ",i);
				
				//Generating command line for monitoring cpu: ps -p [pid] -o %cpu
				sprintf(commandline, "%s%d%s", "ps -p ", pid," -o %cpu"); 	
				
				//Running the command and capturing the output
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline,"r");
				
				//Reading data  
				fscanf(scanTerminal,"%s %f", dataignored, &cpu);
				fclose(scanTerminal);
				
				//Printing data in the terminal and output file
				printf("CPU: %.2f%%    ", cpu/core);
				fprintf(output,"CPU: %.2f%%    ", cpu/core);
				
				//Generating command line for monitoring memory: cat /proc/[pid]/status | grep 'VmSize'
				sprintf(commandline,"%s%d%s","cat /proc/",pid,"/status | grep 'VmSize'");
				
				//Running the command and capturing the output
				//popen(3) - Linux man page - http://linux.die.net/man/3/popen
				scanTerminal = popen(commandline, "r");
				
				//Reading data 
				fscanf(scanTerminal,"%s %f %s", dataignored, &memory, dataignored);
			
				//Printing data in the terminal and output file
				printf("Memory: %.2f KB\n", memory);
				fprintf(output,"Memory: %.2f KB\n", memory);
				fclose(scanTerminal);
				
			  	//Waits one second
				sleep(1);
			}
			
			//After ten seconds the child is dead
			//kill(1) - Linux man page - http://linux.die.net/man/1/kill
			kill(pid, SIGKILL);
			fclose(output);
			
			printf("Output file generated: output.txt\n");
		}	
		
	//If is the child process	
	}else if(pid == 0){

		if((strcmp(argv[1],"cpu") == 0)){
			cputest();
		}else if((strcmp(argv[1],"cpu-mem") == 0)){
			cpumemorytest();
		}		
	
	}
	
	return 0;
}
