/******************************************************************************
*                         User-Aware Flying AP Project
*                             FAP Controller + AP
*******************************************************************************
*                        Comunicacoes Moveis 2017/2018
*                             FEUP | MIEEC / MIEIC
*******************************************************************************/

#include <stdio.h>
// Module headers
#include "FapController.h"

int main()
{
	printf("=============================================\n"
		   "FAP CONTROLLER TEST\n"
		   "=============================================\n\n");

	printf("FAP Controller Test not yet implemented!\n"
		   "Will be implemented by Eduardo Almeida.\n");

	if (initializeFapController() == RETURN_VALUE_ERROR) {
		perror("initializeFapController");
		return -1;
	}
	char c;
	fprintf(stderr, "To stop it press '0'\n");
	while (scanf(" %c", &c)) {
		if (c == '0') {
			if (terminateFapController() == RETURN_VALUE_ERROR) {
				perror("terminateFapController");
				return -1;
			} else {
				return 0;
			}
		} else {
			fprintf(stderr, "To stop it press '1'\n");
		}
	}

	return 0;
}
