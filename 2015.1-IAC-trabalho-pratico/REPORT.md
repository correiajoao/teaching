# Identificação

* Página do repositório do trabalho ([https://github.com/correiajoao/teaching/tree/master/2015.1-IAC-trabalho-pratico](TODO)) 
* Discente 1
	* Nome: João Lucas Marques Correia
	* Matrícula: 15111944 
* Discente 2
	* Nome: Marcos Gleysson Silva do Nascimento
	* Matrícula: 15111984
_________________________________________________________________________

###**Fundamentação teórica**###

Os primeiros sistemas operacionais surgiram unicamente associados a ideia de execução de um programa por vez, esses sistemas foram denominados monoprogramáveis. Os sistemas monoprogramáveis caracterizam-se por permitir que um único programa tenha o controle total dos recursos computacionais da máquina. Desse modo, durante a maior parte do tempo, alguns recursos permanecem ociosos, por exemplo, durante o momento em que o programa realiza uma leitura em disco, o processador permanece ocioso, desperdiçando recursos do sistema.

Com a evolução dos sistemas monoprogramáveis, surgiram os sistemas multiprogramáveis. Neste tipo de sistema vários programas são executados de forma concorrente, compartilhando os recursos computacionais e diminuindo a subutilização dos mesmos, sendo assim, quando um processo não está utilizando determinado recurso, o mesmo pode ser alocado a outro processo.

Nesse sentido, é necessário que os sistemas multiprogramáveis
disponibilizem meios para gerenciar o uso dos recursos de forma organizada por todos os processos, tais funções são desempenhadas através da implementação de diversas técnicas e dispositivos, como as políticas de escalonamento, gerência de memória, interrupções, dentre outros.

Para efetuar um estudo referente a esses mecanismos provenientes dos sistemas multiprogramáveis, foram desenvolvidos alguns testes que analisam o consumo de recursos através de códigos escritos na linguagem C. Estes fazem o uso de processos pais (monitores) e processos filhos (consumidores de recurso) buscando examinar o consumo de processamento ou  memória.

###**Utilização intensa da UCP**###

Para o caso de teste de utilização intensa da UCP, o seguinte código foi executado no processo filho:
```c
	for(;;){
	}
```
Este trecho de código gera um consumo intenso de UCP, pois há a entrada em um *loop* infinito, não havendo ações que possam o colocar no estado de espera. Os dados obtidos a partir desse experimento estão representados a seguir. 

![Figura 1](https://raw.githubusercontent.com/correiajoao/teaching/master/2015.1-IAC-trabalho-pratico/chats/cpu.png)


No instante zero, o processo já foi criado, porém apresenta uma taxa de 0% de consumo de UCP, isso se deve ao fato de que processos recém criados não vão diretamente para a a execução, ficando, inicialmente, no estado de pronto e esperando a sua primeira execução. No instante um, o processo já se encontra no estado de execução e assume uma taxa de consumo de 25% de UCP, essa que se mantém constante durante todo o ciclo de vida do processo. Esse valor de 25% de consumo de processamento se deve ao fato do processo ter sido executado em um computador com arquitetura de quatro núcleos, como consequência, o programa é executado em um núcleo por vez, assumindo o controle de um quarto do total disponível no sistema computacional. No instante dez não existem dados, pois, o processo filho já foi finalizado pelo processo pai.

###**Utilização intensa de UCP e memória**###


Para o caso de teste de consumo simultâneo de UCP e memória foi executado o seguinte código no processo filho:
```c
	for(;;){
		malloc(sizeof(10000));
	}
```
O código mostrado acima, como no teste anterior, executa um *loop* infinito para a análise do consumo de UCP, além disso realiza a cada execução do *loop* a alocação de quatro bits, uma vez que é a quantidade de bits necessária para alocar o inteiro passado como parâmetro ao *sizeof*. O código foi executado em dois computadores distintos, e consequentemente, em dois ambientes computacionais distintos, e seus resultados para cada caso estão descritos nas seções abaixo.


- 	Ambiente sem memória SWAP

Para o ambiente sem memória SWAP, os dados de execução obtidos foram os da figura a seguir.

![Figura 2](https://raw.githubusercontent.com/correiajoao/teaching/master/2015.1-IAC-trabalho-pratico/chats/cpu-memory.png)

Para esse ambiente, os dados obtidos no gráfico de consumo de UCP se mostraram equivalentes aos obtidos no experimento anterior, entretanto os dados do gráfico de consumo de memória são bastante notáveis. No instante zero, o processo possui um total de 5KB (valor que está desprezivel para a escala do gráfico) de memória alocada, no instante um, esse valor sobe para 500MB, esse padrão de crescimento segue até o instante oito, onde o valor assume uma taxa estática, uma vez que ao atingir o limite de alocação física de memória disponível no sistema, o sistema operacional não permite mais alocação.


- 	Ambiente com memória SWAP

No ambiente com memória SWAP, os dados de execução obtidos foram os da figura abaixo.

![Figura 3](https://raw.githubusercontent.com/correiajoao/teaching/master/2015.1-IAC-trabalho-pratico/chats/cpu-memory-swap.png)

Para esse ambiente é possível notar que os dados de consumo de UCP variam significativamente durante o intervalo de tempo considerado, isso se deve ao fato de que após o processo alocar toda a memória física disponível, o sistema inicia o uso da memória destinada a SWAP, como consequência, o processo demora mais para realizar a alocação de memória, pois a cada solicitação de alocação é necessário que o sistema retire processos da memória e os transfira para o disco rígido, essa operação de transferência comparada às velocidades com que o processador trabalha é infinitamente maior e, assim, o processo acaba ficando por um longo tempo no estado de espera. Por esse motivo no instante três o processamento diminuí, porém a alocação de memória continua a crescer.

 Nesse tipo de teste, travamentos constantes na máquina que o executou ocorreram pois inúmeras trocas SWAP irão acontecer para liberação de espaço de alocação ao processo.

###**Funcionalidades adicionais**###
-	Comentários em inglês
-	Documentação do código
-	Gravação dos dados em arquivo 

