#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "generaKey.h"

char* generaKey() {
    char* key = malloc((KEY_LENGTH+1)*sizeof(char)); // Reservar memoria para la cadena
    int i;

    // Inicializar generador de números aleatorios con una semilla única
    srand(time(NULL));

    // Generar cadena aleatoria
    for (i = 0; i < KEY_LENGTH; i++) {
        int r = rand() % 36; // 26 letras + 10 dígitos = 36 caracteres posibles
        if (r < 26) { // Letra mayúscula
            key[i] = 'A' + r;
        } else { // Dígito
            key[i] = '0' + r - 26;
        }
    }

    key[KEY_LENGTH] = '\0'; // Agregar el caracter nulo al final de la cadena

    return key;
}

int randInt(int low, int high) {
	assert(high >= low);
	srand(time(NULL));
	return rand() % (high - low + 1) + low;
}
