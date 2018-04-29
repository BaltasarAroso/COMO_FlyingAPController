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
// The FapManagementProtocol_Server{.c,.h} are included in
// the project just to build the FAP Controller project.
// These files should not be modified.
// =========================================================


// Module headers
#include "FapManagementProtocol_Server.h"

// MAVLink library
// [https://mavlink.io/en/getting_started/use_source.html]
//#include "mavlink/common/mavlink.h"

// JSON parser
// [https://github.com/udp/json-parser and https://github.com/udp/json-builder]
//#include "json/json.h"
//#include "json/json-builder.h"

// C headers
// (...)

// =========================================================
//           FUNCTIONS
// =========================================================

// TODO: Develop the required functions to implement the FAP Management
// Protocol (Server)


// =========================================================
//           PUBLIC API
// =========================================================
int initializeFapManagementProtocol()
{
	// Just to compile the project
	return RETURN_VALUE_OK;
}


int terminateFapManagementProtocol()
{
	// Just to compile the project
	return RETURN_VALUE_OK;
}


int moveFapToGpsNedCoordinates(const GpsNedCoordinates *gpsNedCoordinates)
{
	// Just to compile the project
	return RETURN_VALUE_OK;
}


int getFapGpsNedCoordinates(GpsNedCoordinates *gpsNedCoordinates)
{
	// Just to compile the project
	return RETURN_VALUE_OK;
}


int getAllUsersGpsNedCoordinates(GpsNedCoordinates *gpsNedCoordinates, int *n)
{
	// Just to compile the project
	return RETURN_VALUE_OK;
}
