#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include "generaKey.h"
#include "servidorTcpFunciones.h"
#include "split.h"
#include "userFile.h"
#include "grupodesockets.h"


int hablaConElCliente(client *cli, ConnectFour *connectF, ConnectFourSettings *connectFourS) {
	FILE *f = fopen(FILENAME, "a+");
	if(f == NULL){
        perror("ERROR fopen\n");
        exit(1);
	}
    char lineabuf[2000];

    char *leidos = fgets(lineabuf,2000,cli->stream);
    if (leidos == NULL) {
        fclose(f);
		return 0;
	}
	int len=strlen(lineabuf);
	if (lineabuf[len-1]=='\n') {
		lineabuf[len-1]='\0';
	}
	printf(">> [%s]\n",lineabuf);
	procesarComando(lineabuf, &f, cli, connectF, connectFourS);
    fclose(f);
    return 1;
}

bool procesarComando(char *comando, FILE **f, client *cli, ConnectFour *connectF, ConnectFourSettings *connectFourS) {
    int len;
    char **comandoSplit = split(comando, " ", &len);
    if (len == 0) {
        fprintf(cli->stream, "ERROR COMANDO\n");
        return false;
    }
    
    bool ultimaQueryFueRegistrar = cli->ultimaQueryFueRegistrar;
    if (strcmp(comandoSplit[0], "REGISTRAR") == 0) {
		cli->ultimaQueryFueRegistrar = true;
	}
	else {
		cli->ultimaQueryFueRegistrar = false;
	}
	
	if (strcmp(comandoSplit[0], "RESPUESTA") != 0 && ultimaQueryFueRegistrar) {
		fprintf(cli->stream, "REGISTRADO ERROR\n");
		return false;
	}
		
    if (strcmp(comandoSplit[0], "LOGIN") == 0)
        return login(cli->stream, len - 1, comandoSplit + 1, *f, &(cli->user));
    if (strcmp(comandoSplit[0], "REGISTRAR") == 0)
        return registrar(cli->stream, &(cli->user));
    if (strcmp(comandoSplit[0], "SETNAME") == 0)
        return setName(cli->stream, f, len - 1, comandoSplit + 1, &(cli->user));
	if (strcmp(comandoSplit[0], "RESPUESTA") == 0) 
		return respuesta(cli->stream, *f, &(cli->user), len - 1, comandoSplit + 1, ultimaQueryFueRegistrar);
<<<<<<< HEAD
	if (strcmp(comandoSplit[0], "RESPUESTA") == 0)
		return column(cli, connectF, connectFourS, len - 1, comandoSplit + 1);
=======
	if (strcmp(comandoSplit[0], "COLUMN") == 0)
		return column(*cli, connectF, connectFourS, len - 1, comandoSplit + 1);
>>>>>>> 35524875c8a58d5daa1f24467ee159821f9f44c3
		
	
    fprintf(cli->stream, "ERROR COMANDO DESCONOCIDO\n");
    return false;
}

bool login(FILE *stream, int argc, char* argv[], FILE *f, User *user) {
	//PROTOCOLO
    if (argc != 2) {
        fprintf(stream, "LOGIN ERROR\n");
        return false;
    }
    User u;
    strcpy(u.userId, argv[0]);
	if(getUserById(f, &u)){
		if(u.c == atoi(argv[1])){
            strcpy(user->userId, u.userId);
            strcpy(user->username, u.username);
            user->c = u.c;
			fprintf(stream, "LOGIN OK\n");
			return true;
		}
	}
	fprintf(stream, "LOGIN ERROR\n");
    return false;
}

bool registrar(FILE *stream, User *user) {
    int a = randInt(MINLIMITRAND, MAXLIMITRAND);
    int b = randInt(MINLIMITRAND, MAXLIMITRAND+1);
    int respuesta = a + b;
    char lineabuf[MAX];
	fprintf(stream,"RESUELVE %d %d\n",a, b);
	user->c = respuesta;
	
    return true;
}

bool respuesta(FILE *stream, FILE *f, User *user, int argc, char *argv[], bool ultimaQueryFueRegistrar) {
	if (ultimaQueryFueRegistrar == false || argc != 1 || user->c != atoi(argv[0])) {
		fprintf(stream, "REGISTRADO ERROR\n");
		return false;
	}
    User newUser;
    strcpy(newUser.username, "Invitado");
    newUser.c = user->c;
    strcpy(newUser.userId, generaKey());
    while (userExistsById(f, newUser)) {
        strcpy(newUser.userId, generaKey());
    }
    addUser(f, newUser);
    strcpy(user->userId, newUser.userId);
    strcpy(user->username, newUser.username);
    user->c = newUser.c;

    fprintf(stream, "REGISTRADO OK %s\n", newUser.userId);
    return true;
}

bool setName(FILE *stream, FILE **f, int argc, char* argv[], client *cli) {
    if (user == NULL || cli->user.userId[0] == '\0' || argc != 1 || strlen(argv[0]) > 16) {
        fprintf(stream, "SETNAME ERROR\n");
        return false;
    }
    strcpy(cli->user.username, argv[0]);
    if (updateUsernameById(f, FILENAME, cli->user)) {
		cli->clientePreparadoParaJugar = true;
        fprintf(stream, "SETNAME OK\n");
        return true;
    }
	fprintf(stream, "SETNAME ERROR\n");
    return false;
}

bool initProtocoloConnectFour(ConnectFour connectF, ConnectFourSettings connectFourS) {
	fprintf(connectFourS.clientFirstPlayer.stream, "START %s %d %d\n", connectFourS.clientSecondPlayer.user.username, connectF.n, connectF.m);
	fprintf(connectFourS.clientFirstPlayer.stream, "URTURN\n");
	fprintf(connectFourS.clientSecondPlayer.stream, "START %s %d %d\n", connectFourS.clientFirstPlayer.user.username, connectF.n, connectF.m);
	return true;
}

bool column(client cli, ConnectFour *connectF, ConnectFourSettings *connectFourS, int argc, char* argv[]) {
	if (argc != 1) {
		fprintf(cli.stream, "COLUMN ERROR\n");
		return false;
	}
	
	int col = atoi(argv[0]);
<<<<<<< HEAD
	if (!isValidMove(*connectF, col) {
=======
	if (!isValidMove(*connectF, col - 1)) {
>>>>>>> 35524875c8a58d5daa1f24467ee159821f9f44c3
		fprintf(cli.stream, "COLUMN ERROR\n");
		return false;
	}
	
<<<<<<< HEAD
=======
	makeMove(connectF, col);
	pasaTurno(connectF);
	
	if (isFirstPlayersTurn(*connectF)) {
		fprintf(connectFourS->clientFirstPlayer.stream, "URTURN %d\n", col);
	} else {
		fprintf(connectFourS->clientSecondPlayer.stream, "URTURN %d\n", col);
	}
	
	if (check_win(*connectF, col)) {
		fprintf(cli.stream, "VICTORY\n");
		if (isFirstPlayersTurn(*connectF)) {
			fprintf(connectFourS->clientFirstPlayer.stream, "DEFEAT\n");
		} else {
			fprintf(connectFourS->clientSecondPlayer.stream, "DEFEAT\n");
		}
		return false;
	}
	
	if (isFullBoard(*connectF)) {
		fprintf(connectFourS->clientFirstPlayer.stream, "TIE\n");
		fprintf(connectFourS->clientSecondPlayer.stream, "TIE\n");
		return false;
	}
>>>>>>> 35524875c8a58d5daa1f24467ee159821f9f44c3
	
	
	return true;
}

void printSockaddrIn(struct sockaddr_in *d) {
    printf("Direccion: %s ( %08x ) puerto: %d \n",
           inet_ntoa(d->sin_addr),
           ntohl(d->sin_addr.s_addr),
           ntohs(d->sin_port));
    return;
}



