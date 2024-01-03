# En los ficheros makefile puede haber comentarios
# los comentarios se ponen con líneas que empiezan por #
all: cliente servidor
todos: cliente servidor

cliente: cliente.c
	gcc cliente.c -o cliente

servidor: servidor.c servidorTcpFunciones.c generaKey.c split.c userFile.c grupodesockets.c
	gcc -o servidor servidor.c servidorTcpFunciones.c generaKey.c split.c userFile.c grupodesockets.c
	
clean:
	rm -f cliente servidor


