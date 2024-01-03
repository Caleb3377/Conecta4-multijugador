#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "servidorTcpFunciones.h"
#include "grupodesockets.h"
#include "connectFour.h"
#include "connectFourSettings.h"

#define NUM_CONEX 2

int main(int argc, char *argv[]) {
	if (argc < 3) {
        printf("Especifica el puerto, filas del tablero y número de columnas\n");
        printf("./%s <puerto> <filas> <columnas>\n", argv[0]);
        exit(-1);
    }
  
    int puerto = atoi( argv[1]);
    int n = atoi(argv[2]);
    int m = atoi(argv[3]);

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = INADDR_ANY;
    local.sin_port = htons( puerto );

    int s = socket(AF_INET,SOCK_STREAM,0);
    
    int r = bind(s,(struct sockaddr *)&local,sizeof(local));
    if ( r < 0 ) {
        printf("No puedo hacer bind al puerto %d\n",puerto);
        exit(-1);
    }

    listen(s,NUM_CONEX);

    int send_tick=0;

    grupo_de_sockets sockets;
    sockets.numConexiones = NUM_CONEX;
    sockets.client_info = (client*)malloc(NUM_CONEX * sizeof(client));
    sockets.poll = (struct pollfd *)malloc((NUM_CONEX + 1) * sizeof(struct pollfd));
    init_grupo_de_sockets(&sockets,s);
    
    ConnectFour connectF;
    ConnectFourSettings connectFourS;
    setGameStarted(&connectFourS, false);

    while (1) {
        grupo_de_sockets_poll(&sockets, &hablaConElCliente);
        if (!gameStarted(connectFourS) && clientesPreparadosParaJugar(sockets)) {
			connectF = init(n, m);
			assignClientsToConnectFourSettings(&connectFourS, sockets.client_info[0], sockets.client_info[1]);
			initProtocoloConnectFour(connectF, connectFourS);
		}
        
    }
	return 0;
}
