#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include "userFile.h"
#include "generaKey.h"
#include "split.h"
#define MAX 2000
#define FILE_CLIENT "cliente.txt"


void genera_pollinfo(struct pollfd pollArr[2], int cli_sock) {
    pollArr[0].fd = STDIN_FILENO;
    pollArr[0].events = POLLIN;
    pollArr[0].revents = 0;
    pollArr[1].fd = cli_sock;
    pollArr[1].events = POLLIN;
    pollArr[1].revents = 0;
}

bool finProtocolo(char *lineabuf) {
    return strstr(lineabuf, "FULL") != NULL && strstr(lineabuf, "VICTORY") != NULL && strstr(lineabuf, "DEFEAT") != NULL && strstr(lineabuf, "TIE") != NULL && strstr(lineabuf, "ERROR") != NULL && strstr(lineabuf, "REGISTRADO ERROR") != NULL && strstr(lineabuf, "SETNAME ERROR") == NULL;
}

void loginOrRegisterUser(FILE *cli_stream, char *lineabuf, char *nameClient){
	
	FILE *f = fopen(FILE_CLIENT, "a+");
	User user;
	int a = 0, b = 0;
	if(!readFirstUser(f, &user)){ //registrar
		fprintf(cli_stream,"%s\n", "REGISTRAR");
		fgets(lineabuf,MAX,cli_stream);
		len = strlen(lineabuf);
		if (lineabuf[len-1]=='\n') lineabuf[len-1]='\0';
		char **menssage = split(lineabuf, '\0', len);
		a = atoi(menssage[1]);
		b = atoi(menssage[2]);
		fprintf(cli_stream,"%s [%d]\n", "RESPUESTA", a+b);
		user = {id, nameClient, a+b};
		rewind(f);
		addUser(f, u);
		fclose(f);
		return;
	}
	//loggear
}

bool ejecutaProtocolo(FILE *cli_stream, char *lineabuf, char *nameClient) {
    fgets(lineabuf, MAX, stdin);
    int len = strlen(lineabuf);
    if (lineabuf[len-1]=='\n') lineabuf[len-1]='\0';

    if (strcmp(lineabuf, "EXIT") == 0 || strcmp(lineabuf, "exit") == 0) return false;
    fprintf(cli_stream,"%s\n",lineabuf);
    if(strcmp(lineabuf, "LISTA") == 0){
        while(fgets(lineabuf,MAX,cli_stream)){
            len = strlen(lineabuf);
            if (lineabuf[len-1]=='\n') lineabuf[len-1]='\0';
            if(!strcmp(lineabuf, "E")) return true;
            printf(">> [%s]\n",lineabuf);
        }
    }
    else{
        if (!fgets(lineabuf,MAX,cli_stream)) return false;
        len = strlen(lineabuf);
        if (lineabuf[len-1]=='\n') lineabuf[len-1]='\0';
        printf(">> [%s]\n",lineabuf);
    }
    return true;
}

void habla_con_el_servidor(int cli_sock, char *nameClient) {
    struct pollfd pollArr[2];
    FILE *cli_stream = fdopen(cli_sock,"r+");
    setbuf(cli_stream,NULL);
    char lineabuf[MAX]; lineabuf[0] = '\0';

    while (!finProtocolo(lineabuf)) {
        genera_pollinfo(pollArr, cli_sock);
        poll(pollArr, 2, -1);
        if (pollArr[0].revents & POLLIN) { // cliente ha escrito algo en stdin
            if (!ejecutaProtocolo(cli_stream, lineabuf, nameClient)) break;
        }
        if (pollArr[1].revents & POLLIN) { // servidor envia protocolo UPTIME
            if (!fgets(lineabuf,MAX,cli_stream)) break;
            int len = strlen(lineabuf);
            if (lineabuf[len-1]=='\n') lineabuf[len-1]='\0';
            printf(">> UPTIME [%s]\n",lineabuf);
        }
    }
    fclose(cli_stream);
}



int main(int argc, char *argv[]) {
    if (argc <= 3) {
        printf("%s <direccion_ip> <puerto> <nombre>\n", argv[0]);
        exit(-1);
    }
    if(strlen(argv[3]) > MAX_LENGTH_NAME){
		printf("Se sobrepaso la longitud maxima del nombre, elige un nombre de %d caracteres como máximo\n", MAX_LENGTH_NAME);
        exit(-1);
	}
    
    char *ip_str= argv[1];
    int puerto = atoi( argv[2]);

    int dirip;
    if (inet_pton(AF_INET,ip_str ,&dirip) != 1 ) {
        printf("'%s' no es una IP válida\n", argv[1]);
        exit(-1);
    }

    int s = socket(AF_INET, SOCK_STREAM ,0);
        
    struct sockaddr_in remoto;
    remoto.sin_family = AF_INET;
    remoto.sin_addr.s_addr = dirip;
    remoto.sin_port = htons(puerto);


    int r = connect(s,(struct sockaddr *)&remoto,sizeof(remoto));
    if ( r != 0 ) {
        printf("La conexión no se ha establecido\n");
        exit(-1);
    }

    printf("CONEXIÓN TCP ESTABLECIDA\n");
    printf("Dirección IP: %s ( 0x%08x ) \n", ip_str, ntohl(dirip));
    printf("Puerto: %u \n",puerto);
	habla_con_el_servidor(s, argv[3]);
}

