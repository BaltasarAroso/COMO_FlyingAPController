/******************************************************************************
*                         User-Aware Flying AP Project
*                             FAP Controller + AP
*******************************************************************************
*                        Comunicacoes Moveis 2017/2018
*                             FEUP | MIEEC / MIEIC
*******************************************************************************/

// Module headers
#include "FapController.h"

// Extra headers for alarm function
#include <signal.h>
#include <unistd.h>

// =========================================================
//           DEFINES
// =========================================================

// FAP's position update period (in seconds)
#define FAP_POSITION_UPDATE_PERIOD_SECONDS	10


// =========================================================
//           FUNCTIONS
// =========================================================

int averageUserCoords(GpsNedCoordinates *userCoords, GpsNedCoordinates fapCoords, int numUsers, GpsNedCoordinates *newCoord) {
	if (userCoords == NULL) {
		return RETURN_VALUE_ERROR;
	}
	if (!numUsers) {
		return RETURN_VALUE_OK;
	}

	int sumX = 0, sumY = 0;

	for (int i = 0; i < numUsers; i++) {
		sumX += userCoords[i].x;
		sumY += userCoords[i].y;
	}
	newCoord->x = sumX/(float)numUsers;
	newCoord->y = sumY/(float)numUsers;
	newCoord->z = fapCoords.z;

	return RETURN_VALUE_OK;
}

void ALARMhandler(int sig) {
  fprintf(stderr, "\n\n\t\t!!! TIME TO SLEEP FINISHED !!!\n\n");
  alarm(FAP_POSITION_UPDATE_PERIOD_SECONDS); // new alarm
	updateFapCoordinates();
}


// =========================================================
//           PUBLIC API
// =========================================================
int initializeFapController() {
	if (initializeFapManagementProtocol() == RETURN_VALUE_ERROR) {
		return RETURN_VALUE_ERROR;
	}

	signal(SIGALRM, ALARMhandler); // start the alarm
	alarm(FAP_POSITION_UPDATE_PERIOD_SECONDS);

	return RETURN_VALUE_OK;
}


int terminateFapController() {
	if (terminateFapManagementProtocol() == RETURN_VALUE_ERROR) {
		return RETURN_VALUE_ERROR;
	}
	fprintf(stderr, "Terminate Alarm and End Program!\n");
	alarm(0);

	return RETURN_VALUE_OK;
}


int updateFapCoordinates() {

	GpsNedCoordinates *userCoords, fapCoords, newCoord;
	userCoords = (GpsNedCoordinates*) calloc (MAX_ASSOCIATED_USERS, sizeof(GpsNedCoordinates));
	int numUsers;

	if (getAllUsersGpsNedCoordinates(userCoords, &numUsers) == RETURN_VALUE_ERROR) {
		return RETURN_VALUE_ERROR;
	}
	if (getFapGpsNedCoordinates(&fapCoords) == RETURN_VALUE_ERROR) {
		return RETURN_VALUE_ERROR;
	}
	if (averageUserCoords(userCoords, fapCoords, numUsers, &newCoord) == RETURN_VALUE_ERROR) {
		return RETURN_VALUE_ERROR;
	}

	free(userCoords);

	return moveFapToGpsNedCoordinates(&newCoord);
}
