#include <stdbool.h>
#include "grupodesockets.h"

typedef struct connectFourSettings{
	bool gameStarted;
	client clientFirstPlayer;
	client clientSecondPlayer;
}ConnectFourSettings;

ConnectFourSettings initConnectFourSettings();

void assignClientsToConnectFourSettings(ConnectFourSettings	&cfs, client client1, client client2)

void updateGameStarted(ConnectFourSettings &cfs, bool gameStarted);

void gameStarted(ConnectFourSettings cfs);
