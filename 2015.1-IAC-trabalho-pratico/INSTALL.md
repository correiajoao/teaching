# Instruções para compilação e execução

###**Compilação**###

Para compilar o código certifique-se que está no mesmo diretório do arquivo main.c e execute:

```
	gcc main.c -o main	
```

Um arquivo binário main será gerado.


###**Execução**###

Existem dois tipos de execução, um para teste de UCP e outro para teste de UCP e memória.
Para o teste de UCP faça:

```
	./main cpu	
```

Para o teste de UCP e memória faça:

```
	./main cpu-mem	
```
Para ambas execuções será gerado um arquivo de saida com os dados (output.txt). 
