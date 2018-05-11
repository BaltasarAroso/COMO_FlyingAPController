/******************************************************************************
*                         User-Aware Flying AP Project
*                             FAP Controller + AP
*******************************************************************************
*                        Comunicacoes Moveis 2017/2018
*                             FEUP | MIEEC / MIEIC
*******************************************************************************/

// Module headers
#include "FapController.h"

// C headers
#include <stdio.h>
#include <time.h>


// =========================================================
//           MACROS
// =========================================================

/**
 * Print a message.
 *
 * @param ...		Variable arguments to be passed to the printf().
 */
#define TEST_PRINT(...)      \
	do                       \
	{                        \
		printf(">> TEST: "); \
		printf(__VA_ARGS__); \
		printf("\n");        \
	} while (0);

/**
 * Assert if a condition is true, printing a message if it is not.
 *
 * @param condition		Condition to be tested.
 * @param errorMsg		Message to be displayed if the condition is not valid.
 */
#define ASSERT_CONDITION(condition, errorMsg, nErrors) \
	do                                                 \
	{                                                  \
		if (!(condition))                              \
		{                                              \
			TEST_PRINT("ERROR: %s\n", errorMsg);       \
			nErrors++;                                 \
		}                                              \
	} while (0);

/**
 * Print the test header.
 */
#define PRINT_TEST_HEADER()  \
	printf("==========\n"    \
		   "TEST: %s\n"      \
		   "==========\n\n", __func__);

/**
 * Print a test summary.
 *
 * @param nErrors	Number of errors detected.
 */
#define PRINT_TEST_SUMMARY(nErrors)                                      \
	do                                                                   \
	{                                                                    \
		if (nErrors > 0)                                                 \
			printf("\n# TEST SUMMARY: %d Errors detected\n\n", nErrors); \
		else                                                             \
			printf("\n# TEST SUMMARY: Tests passed!\n\n");               \
	} while (0);


// =========================================================
//           AUXILIARY FUNCTIONS
// =========================================================

/**
 * Sleep the program for a given period of time.
 * Note: the program may be interrupted by a SIGALARM,
 * but will resume its sleep after the interruption.
 *
 * @param seconds		Time to sleep (in seconds).
 */
void sleepProgram(unsigned int seconds)
{
	struct timespec req = {seconds, 0};
	struct timespec rem = {0, 0};

	// Sleep the entire time
	while (nanosleep(&req, &rem) < 0)
	{
		// Resume sleeping
		req.tv_sec = rem.tv_sec;
		req.tv_nsec = rem.tv_nsec;
	}
}


// =========================================================
//           TESTS
// =========================================================

/**
 * Test - FAP Management Protocol.
 *
 * @return		The number of errors detected.
 */
int runTest_fapManagementProtocol()
{
	PRINT_TEST_HEADER();

	int nErrors = 0;

	// Initialize FAP Management Protocol
	ASSERT_CONDITION(initializeFapManagementProtocol() == RETURN_VALUE_OK,
					 "Initializing FAP Management Protocol",
					 nErrors);


	// Move FAP and check if the FAP correctly updated its position
	sleepProgram(1); // Let some time pass

	GpsNedCoordinates fapGpsNedCoordinates;
	initializeGpsNedCoordinates(&fapGpsNedCoordinates,
								15, 20, 10, time(NULL));

	ASSERT_CONDITION(moveFapToGpsNedCoordinates(&fapGpsNedCoordinates) == RETURN_VALUE_OK,
					 "Moving FAP",
					 nErrors);

	GpsNedCoordinates newFapGpsNedCoordinates;
	ASSERT_CONDITION(getFapGpsNedCoordinates(&newFapGpsNedCoordinates) == RETURN_VALUE_OK,
					 "Getting FAP Coordinates",
					 nErrors);

	ASSERT_CONDITION(areGpsNedCoordinatesEqual(&fapGpsNedCoordinates, &newFapGpsNedCoordinates),
					 "FAP's coordinates were not updated",
					 nErrors);


	// Get all users' GPS coordinates
	sleepProgram(1); // Let some time pass

	GpsNedCoordinates usersGpsNedCoordinates[MAX_ASSOCIATED_USERS];
	int nUsers;

	ASSERT_CONDITION(getAllUsersGpsNedCoordinates(usersGpsNedCoordinates, &nUsers) == RETURN_VALUE_OK,
					 "Getting all users coordinates",
					 nErrors);


	// Terminate FAP Management Protocol
	sleepProgram(1); // Let some time pass

	ASSERT_CONDITION(terminateFapManagementProtocol() == RETURN_VALUE_OK,
					 "Terminating the FAP Management Protocol",
					 nErrors);


	// Finish test
	PRINT_TEST_SUMMARY(nErrors);

	return nErrors;
}

/**
 * Test - FAP Controller.
 *
 * @return		The number of errors detected.
 */
int runTest_fapController()
{
	PRINT_TEST_HEADER();

	int nErrors = 0;


	// Initialize FAP Controller
	initializeFapController();

	// Sleep for 3 update cycles (3 * 10s)
	sleepProgram(30);

	// Terminate FAP Controller
	terminateFapController();

	// Print test summary
	PRINT_TEST_SUMMARY(nErrors);

	return nErrors;
}


/**
 * Run all tests.
 */
void runTests()
{
	int nErrors = 0;

	// Run tests
	//nErrors += runTest_fapManagementProtocol();	// Just to prove the FAP Management Protocol is working
	nErrors += runTest_fapController();
}

// =========================================================
//           MAIN
// =========================================================

/**
 * Main.
 */
int main()
{
	printf("=============================================\n"
		   "FAP CONTROLLER TEST\n"
		   "=============================================\n\n");

	runTests();

	return 0;
}
