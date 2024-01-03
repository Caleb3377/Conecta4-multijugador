#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "grupodesockets.h"

void init_grupo_de_sockets(grupo_de_sockets *grupo, int servsocket) {
    grupo->serv_socket=servsocket;
    grupo->nclients=0;
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        grupo->client_info[i].vacio=1;
    }
}

void grupo_de_sockets_print_debug(grupo_de_sockets *grupo) {
    printf("--grupo_de_sockets-----------------------\n");
    printf(" serv_sock: %d   N_ %d \n",grupo->serv_socket,grupo->numConexiones);
    printf(" nclientes: %d \n",grupo->nclients);
    printf("clientes: [ ");
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        client *cli = &(grupo->client_info[i]);
        printf("(%d) %d ",cli->vacio,cli->socket);
    }
    printf("]\n");
    printf("pollinfos [ ");
    for (i=0; i<1+grupo->numConexiones; i++) {
        struct pollfd *pollinfo = &(grupo->poll[i]);
        printf("%d %d-%d ",pollinfo->fd,pollinfo->events,pollinfo->revents);
    }
    printf("]\n");
    printf("_________________________________________\n");
}


int grupo_de_sockets_guarda_socket(grupo_de_sockets *grupo, int socket, FILE *stream) {
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        if (grupo->client_info[i].vacio) {
            grupo->client_info[i].vacio=0;
            grupo->client_info[i].socket=socket;
            grupo->client_info[i].stream=stream;
            strcpy(grupo->client_info[i].user.username, "Invitado");
            grupo->client_info[i].user.userId[0] = '\0';
            grupo->client_info[i].ultimaQueryFueRegistrar = false;
            grupo->client_info[i].clientePreparadoParaJugar = false;
            grupo->nclients+=1;
            return 1;
        }
    }
    return 0;
}


int grupo_de_sockets_borra_socket(grupo_de_sockets *grupo, int socket) {
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        if ( (!grupo->client_info[i].vacio) && grupo->client_info[i].socket==socket) {
            grupo->client_info[i].vacio=1;
            grupo->nclients-=1;
            return 1;
        }
    }
    return 0;
}


int grupo_de_sockets_acepta_nuevo_cliente(grupo_de_sockets *grupo) {
    struct sockaddr_in cli_dir;
    socklen_t cli_dir_len = sizeof(cli_dir);
    int cli_sock = accept(grupo->serv_socket,(struct sockaddr *)&cli_dir,&cli_dir_len);
    FILE *cli_stream=fdopen(cli_sock,"r+");
    setbuf(cli_stream,NULL);
    if ( ! grupo_de_sockets_guarda_socket(grupo,cli_sock,cli_stream) ) {
        fprintf(cli_stream, "FULL\n");
        fclose(cli_stream);
        return 0;
    } else {
		fprintf(cli_stream, "WELCOME\n");
	}
    
    return cli_sock;
}


void grupo_de_sockets_genera_fd_set(grupo_de_sockets *grupo, fd_set *fdset, int *maxfd) {
    FD_ZERO(fdset);
    FD_SET(grupo->serv_socket,fdset);
    *maxfd=grupo->serv_socket+1;
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        client *cli = &(grupo->client_info[i]);
        if (!cli->vacio) {
            FD_SET(cli->socket,fdset);
            if (cli->socket>=*maxfd) *maxfd=cli->socket+1;
        }
    }
}


bool clientesPreparadosParaJugar(grupo_de_sockets grupo) {
	for (int i = 0; i < grupo.numConexiones; i++) {
		if (grupo.client_info[i].clientePreparadoParaJugar == false) return false;
	}
	return true;
}


void grupo_de_sockets_genera_pollinfo(grupo_de_sockets *grupo) {
    grupo->poll[0].fd=grupo->serv_socket;
    grupo->poll[0].events=POLLIN;
    grupo->poll[0].revents=0;
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        client *cli = &(grupo->client_info[i]);
        if (!cli->vacio) {
            grupo->poll[i+1].fd=cli->socket;
            grupo->poll[i+1].events=POLLIN;
            grupo->poll[i+1].revents=0;
        } else {
            grupo->poll[i+1].fd=-1;
        }
    }
}




void grupo_de_sockets_poll(grupo_de_sockets *grupo,
    int (*hablaConElCliente)(client *)
) {

    grupo_de_sockets_genera_pollinfo(grupo);

    int r = poll(grupo->poll,1+grupo->numConexiones, -1);
    //printf("poll...\n");

    if ( grupo->poll[0].revents & POLLIN ) {
        // hay nueva conexion para aceptar
        grupo_de_sockets_acepta_nuevo_cliente(grupo);
        //if (call_new_client) call_new_client(NULL);
    }
    int i;
    for (i=0; i<grupo->numConexiones; i++) {
        if (grupo->poll[i+1].revents) {
            // han llegado datos puede ser el cierre
            if (hablaConElCliente) {
                int r = hablaConElCliente(&(grupo->client_info[i]));
                if (r == 0) {
                    grupo_de_sockets_borra_socket(grupo,grupo->poll[i+1].fd);
                } 
            }
        }
    }
}


