#ifndef GRUPODESOCKETS_H
#define GRUPODESOCKETS_H
#include <stdio.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <poll.h>
#include "generaKey.h"
#include "userFile.h"
#include "connectFour.h"

typedef struct s_client {
    int vacio;
    int socket;
    FILE *stream;
    User user;
    bool ultimaQueryFueRegistrar;
    bool clientePreparadoParaJugar;
} client ;

#include "connectFourSettings.h"

typedef struct s_grupo_de_sockets {
    int serv_socket;
    client *client_info;
    struct pollfd *poll;
    int nclients;
    int numConexiones;
} grupo_de_sockets ;


void init_grupo_de_sockets(grupo_de_sockets *grupo, int servsocket);
void grupo_de_sockets_print_debug(grupo_de_sockets *grupo);
int grupo_de_sockets_guarda_socket(grupo_de_sockets *grupo, int socket, FILE *stream);
int grupo_de_sockets_borra_socket(grupo_de_sockets *grupo, int socket);
int grupo_de_sockets_acepta_nuevo_cliente(grupo_de_sockets *grupo);
void grupo_de_sockets_genera_fd_set(grupo_de_sockets *grupo, fd_set *fdset, int *maxfd);
bool clientesPreparadosParaJugar(grupo_de_sockets grupo);

void grupo_de_sockets_genera_pollinfo(grupo_de_sockets *grupo);

void grupo_de_sockets_poll(grupo_de_sockets *grupo,
    int (*hablaConElCliente)(client *)
);
#endif
