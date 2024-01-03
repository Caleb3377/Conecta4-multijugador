#include <stdbool.h>
#include "grupodesockets.h"
#include "connectFourSettings.h"
#include "generaKey.h"

void assignClientsToConnectFourSettings(ConnectFourSettings	&cfs, client client1, client client2) {
	int rnd = randInt(0, 1);
	cfs->gameStarted = true;
	if (rnd == 0) {
		cfs->clientFirstPlayer = client1;
		cfs->clientSecondPlayer = client2;
	} else {
		cfs->clientFirstPlayer = client2;
		cfs->clientSecondPlayer = client1;
	}
}

void setGameStarted(ConnectFourSettings &cfs, bool gameStarted) {
	cfs->gameStarted = gameStarted;
}

void gameStarted(ConnectFourSettings cfs) {
	return cfs.gameStarted;
}
