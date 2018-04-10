/******************************************************************************
*                         User-Aware Flying AP Project
*                             FAP Controller + AP
*******************************************************************************
*                        Comunicacoes Moveis 2017/2018
*                             FEUP | MIEEC / MIEIC
*******************************************************************************/

#include "FapManagementProtocol_Server.h"
#pragma once


// =========================================================
//           PUBLIC API
// =========================================================

/**
 * Initialize the FAP Controller operation.
 * This function should initialize the FAP Controller's internal components,
 * namely the FAP Management Protocol.
 *
 * @return		Return RETURN_VALUE_OK if there are no errors;
 * 				otherwise, return RETURN_VALUE_ERROR.
 */
int initializeFapController();

/**
 * Terminate the FAP Controller operation.
 * This function should terminate the FAP Controller's operation,
 * namely terminate the FAP Management Protocol.
 *
 * @return		Return RETURN_VALUE_OK if there are no errors;
 * 				otherwise, return RETURN_VALUE_ERROR.
 */
int terminateFapController();

/**
 * Update the FAP's coordinates.
 * This function should determine the new FAP coordinates, in order to
 * minimize the average distance to all users, and request the FAP
 * Management Protocol to move the FAP to the target coordinates.
 *
 * @return		Return RETURN_VALUE_OK if there are no errors;
 * 				otherwise, return RETURN_VALUE_ERROR.
 */
int updateFapCoordinates();

/**
 * Calculate the average value of the user coordinates, in order to
 * find the new fap position
 *
 * @return		Return RETURN_VALUE_OK if there are no errors;
 * 				otherwise, return RETURN_VALUE_ERROR.
 */
int averageUserCoords(GpsNedCoordinates *userCoords, GpsNedCoordinates fapCoords, int numUsers, GpsNedCoordinates *newCoord);
