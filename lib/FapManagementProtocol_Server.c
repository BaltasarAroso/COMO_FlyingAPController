/******************************************************************************
*                         User-Aware Flying AP Project
*                             FAP Controller + AP
*******************************************************************************
*                        Comunicacoes Moveis 2017/2018
*                             FEUP | MIEEC / MIEIC
*******************************************************************************/

// =========================================================
// IMPORTANT NOTE:
//
// The FapManagementProtocol_Server.{c,h} library included in
// the FAP Controller project has a very simple implementation,
// which enables the test of the FAP Controller.
// These files should not be modified.
// =========================================================


// Module headers
#include "FapManagementProtocol_Server.h"
//#include "MavlinkEmulator.h"
#include "GpsCoordinates.h"

// MAVLink library
// [https://mavlink.io/en/getting_started/use_source.html]
//#include "mavlink/common/mavlink.h"

// JSON parser
// [https://github.com/udp/json-parser and https://github.com/udp/json-builder]
//#include "json/json.h"
//#include "json/json-builder.h"


// =========================================================
//           DEFINES
// =========================================================

// ----- MAVLINK PROTOCOL ----- //

// Use MAVLink helper functions
//#define MAVLINK_USE_CONVENIENCE_FUNCTIONS


// ----- FAP MANAGEMENT PROTOCOL - MESSAGES ----- //

// Protocol parameters
#define PROTOCOL_PARAMETERS_USER_ID						"userId"
#define PROTOCOL_PARAMETERS_MSG_TYPE					"msgType"
#define PROTOCOL_PARAMETERS_GPS_COORDINATES				"gpsCoordinates"
#define PROTOCOL_PARAMETERS_GPS_COORDINATES_LAT			"lat"
#define PROTOCOL_PARAMETERS_GPS_COORDINATES_LON			"lon"
#define PROTOCOL_PARAMETERS_GPS_COORDINATES_ALT			"alt"
#define PROTOCOL_PARAMETERS_GPS_COORDINATES_TIMESTAMP	"timestamp"
#define PROTOCOL_PARAMETERS_GPS_TIMESTAMP				"gpsTimestamp"

// Protocol "msgType" values
typedef enum _ProtocolMsgType
{
	USER_ASSOCIATION_REQUEST		= 1,
	USER_ASSOCIATION_ACCEPTED		= 2,
	USER_ASSOCIATION_REJECTED		= 3,
	USER_DESASSOCIATION_REQUEST		= 4,
	USER_DESASSOCIATION_ACK			= 5,
	GPS_COORDINATES_UPDATE			= 6,
	GPS_COORDINATES_ACK				= 7
} ProtocolMsgType;


// ----- FAP MANAGEMENT PROTOCOL - PARAMETERS ----- //

// GPS coordinates update period (in seconds)
#define GPS_COORDINATES_UPDATE_PERIOD_SECONDS			10
#define GPS_COORDINATES_UPDATE_TIMEOUT_SECONDS			(2 * GPS_COORDINATES_UPDATE_PERIOD_SECONDS)

// Max allowed distance from the users to the FAP (in meters)
#define MAX_ALLOWED_DISTANCE_FROM_FAP_METERS			300


// ----- FAP MANAGEMENT PROTOCOL - SERVER ADDRESS ----- //
#define SERVER_IP_ADDRESS		"127.0.0.1"
#define SERVER_PORT_NUMBER		40123


// =========================================================
//           GLOBAL VARIABLES
// =========================================================

// Flag indicating if the FAP Management Protocol was initialized
static int fmpInitialized = 0;

// FAP's GPS NED coordinates
static GpsNedCoordinates fapGpsNedCoordinates = {0};



// =========================================================
//           FUNCTIONS
// =========================================================

/**
 * Print a message from the FAP Management Protocol (FMP) module.
 *
 * @param ...		Variable arguments list to be passed to printf().
 */
#define FMP_PRINT(...)                                                   \
	do                                                                 \
	{                                                                  \
		printf("\n>> FapManagementProtocol_Server::%s(): ", __func__); \
		printf(__VA_ARGS__);                                           \
		printf("\n");                                                  \
	} while (0);


// =========================================================
//           PUBLIC API
// =========================================================
int initializeFapManagementProtocol()
{
	// Check if the FAP Management Protocol was already initialized
	if (fmpInitialized)
	{
		FMP_PRINT("ERROR: FAP Management Protocol (Server) was already initialized");
		return RETURN_VALUE_ERROR;
	}

	// Initialize pseudo-random generator
	srand(time(NULL));

	// Initialize the FAP's current GPS NED coordinates
	initializeGpsNedCoordinates(&fapGpsNedCoordinates,
								0,
								0,
								0,
								time(NULL));

	// Initialize FAP Management Protocol
	fmpInitialized = 1;
	FMP_PRINT("Initialized FAP Management Protocol (Server)");

	return RETURN_VALUE_OK;
}


int terminateFapManagementProtocol()
{
	// Check if the FAP Management Protocol is initialized
	if (!fmpInitialized)
	{
		FMP_PRINT("ERROR: Trying to terminate FAP Management Protocol (Server), but it is not yet initialized");
		return RETURN_VALUE_ERROR;
	}

	// Reset FAP's coordinates
	initializeGpsNedCoordinates(&fapGpsNedCoordinates,
								0,
								0,
								0,
								time(NULL));

	// Terminate FAP Management Protocol
	fmpInitialized = 0;
	FMP_PRINT("FAP Management Protocol (Server) terminated");

	return RETURN_VALUE_OK;
}


int moveFapToGpsNedCoordinates(const GpsNedCoordinates *gpsNedCoordinates)
{
	// Check if the FAP Management Protocol is initialized
	if (!fmpInitialized)
	{
		FMP_PRINT("ERROR: FAP Management Protocol (Server) was not yet initialized");
		return RETURN_VALUE_ERROR;
	}

	// Define the FAP's current GPS NED coordinates as the target coordinates
	if (copyGpsNedCoordinates(&fapGpsNedCoordinates, gpsNedCoordinates) != RETURN_VALUE_OK)
		return RETURN_VALUE_ERROR;

	// Define the timestamp as the current time
	strcpyTimestampIso8601(fapGpsNedCoordinates.timestamp, time(NULL));

	// Print
	FMP_PRINT("Moved FAP to the following coordinates:");
	PRINT_GPS_NED_COORDINATES(fapGpsNedCoordinates);

	return RETURN_VALUE_OK;
}


int getFapGpsNedCoordinates(GpsNedCoordinates *gpsNedCoordinates)
{
	// Check if the FAP Management Protocol is initialized
	if (!fmpInitialized)
	{
		FMP_PRINT("ERROR: FAP Management Protocol (Server) was not yet initialized");
		return RETURN_VALUE_ERROR;
	}

	// Initialize gpsNedCoordinates with the FAP's current GPS coordinates
	if (copyGpsNedCoordinates(gpsNedCoordinates, &fapGpsNedCoordinates) != RETURN_VALUE_OK)
		return RETURN_VALUE_ERROR;

	// Define the timestamp as the current time
	//strcpyTimestampIso8601(gpsNedCoordinates->timestamp, time(NULL));

	// Log
	FMP_PRINT("Current FAP coordinates:");
	PRINT_GPS_NED_COORDINATES(fapGpsNedCoordinates);

	return RETURN_VALUE_OK;
}


int getAllUsersGpsNedCoordinates(GpsNedCoordinates *gpsNedCoordinates, int *n)
{
	// Check if the FAP Management Protocol is initialized
	if (!fmpInitialized)
	{
		FMP_PRINT("ERROR: FAP Management Protocol (Server) was not yet initialized");
		return RETURN_VALUE_ERROR;
	}

	// Generate a pseudo-random number of users associated to the FAP (0 - MAX_ASSOCIATED_USERS)
	int nUsers = rand() % (MAX_ASSOCIATED_USERS + 1);
	*n = nUsers;

	// If there are no users, return immediately
	if (nUsers == 0)
	{
		FMP_PRINT("No users associated");
		return RETURN_VALUE_OK;
	}

	// Generate pseudo-random GPS NED coordinates for the users
	time_t currentTime = time(NULL);

	for (int i = 0; i < nUsers; i++)
	{
		gpsNedCoordinates[i].x = fapGpsNedCoordinates.x + ((rand() % (2 * MAX_ALLOWED_DISTANCE_FROM_FAP_METERS)) - MAX_ALLOWED_DISTANCE_FROM_FAP_METERS);
		gpsNedCoordinates[i].y = fapGpsNedCoordinates.y + ((rand() % (2 * MAX_ALLOWED_DISTANCE_FROM_FAP_METERS)) - MAX_ALLOWED_DISTANCE_FROM_FAP_METERS);
		gpsNedCoordinates[i].z = 0;
		strcpyTimestampIso8601(gpsNedCoordinates[i].timestamp, currentTime);
	}

	// Print
	FMP_PRINT("List of associated users (Generated %d users):", nUsers);

	for (int i = 0; i < nUsers; i++)
	{
		PRINT_GPS_NED_COORDINATES(gpsNedCoordinates[i]);
	}

	return RETURN_VALUE_OK;
}
