#ifndef SERVIDOR_TCP_FUNCIONES_H
#define SERVIDOR_TCP_FUNCIONES_H
#include <stdbool.h>
#include <stdio.h>
#include "grupodesockets.h"
#include "userFile.h"
#include "connectFour.h"
#include "connectFourSettings.h"

#define FILENAME "servidor.txt"
#define MAX 20
#define MINLIMITRAND 1
#define MAXLIMITRAND 9

bool procesarComando(char *comando, FILE **f, client *cli, ConnectFour *connectF, ConnectFourSettings *connectFourS);

bool login(FILE *stream, int argc, char* argv[], FILE *f, User *user);

bool registrar(FILE *stream, User *user);

bool respuesta(FILE *stream, FILE *f, User *user, int argc, char *argv[], bool ultimaQueryFueRegistrar);

bool setName(FILE *stream, FILE **f, int argc, char* argv[], client *cli);

int hablaConElCliente(client *cli, ConnectFour *connectF, ConnectFourSettings *connectFourS);

void printSockaddrIn(struct sockaddr_in *d);

bool initProtocoloConnectFour(ConnectFour connectF, ConnectFourSettings connectFourS);

bool column(client cli, ConnectFour *connectF, ConnectFourSettings *connectFourS, int argc, char* argv[]);

#endif
