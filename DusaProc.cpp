// ***************************************************************************
// Copyright (c) 2001 by Robert Conner
// ***************************************************************************

// Module Name     : Process Functions
// Descriptive Name: DeltaUSA processing module
// Purpose         : Contains high level functions.  Serves as an interface
//                   between user and data access functions
// Author Name     : Robert Conner
// Date Created    : February 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaProc.cpp
// Last Update     : R Conner 05/25/01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dusamain.h"
#include "dusadata.h"

// **************************************************************************
// Name        : addNewGroup
//
// Purpose     : Adds Group Input by User to ASO_LIST.txt
//
// Parameters  : None
//
// Return Value: OK    If valid group number entered
//               QUIT  If user exits function
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int addNewGroup (void)
{
    char    newGrpNum[GRP_NUM_LEN+1] = "";
    char mainframeNum[REG_GRP_LEN+1] = "";

    int exitStatus  = OK;
    
    clearString (newGrpNum);
    clearString (mainframeNum);
        
    displayTitle();

    do
    {
        printf ("\n   *** Add ASO Group to ASO List ***\n\n");
	    
        prompt ("\nPlease enter New ASO Group Number (or Q to Quit): ", 
            newGrpNum, GRP_NUM_LEN + 1);

        if (toupper (newGrpNum[0]) != 'Q')
        {
            if (validGrpNum (newGrpNum, GRP_NUM_LEN) == OK)
            {
                convert2MainFrame (newGrpNum, mainframeNum);

                if ((exitStatus = findGroupRecord (mainframeNum)) == GROUP_NOT_FOUND)
                {
                    if ((exitStatus = createGroupRecord (mainframeNum)) == OK)
                    {
                        if ((exitStatus = updateGroupRecord (mainframeNum, newGrpNum)) == OK)
	                        printf ("\a\nGroup %s added.", newGrpNum);

	                    getEnterKey2Continue();
                    }
                }    
            }
                            
            else
            {
    	        errorProcess (INVALID_GROUP_NUM);
				exitStatus = INVALID_GROUP_NUM;
            }
        }

        else
            exitStatus = QUIT;

    } while (exitStatus == INVALID_GROUP_NUM);

    return exitStatus;

} // end addNewGroup


// **************************************************************************
// Name        : deleteGroup
//
// Purpose     : Deletes Group Input by User from ASO_LIST.txt
//
// Parameters  : None
//
// Return Value: OK    If valid group number entered
//               QUIT  If user exits function
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int deleteGroup (void)
{
    char    newGrpNum[GRP_NUM_LEN+1] = "";
    char mainframeNum[REG_GRP_LEN+1] = "";

    int exitStatus  = OK;
    
    clearString (newGrpNum);
    clearString (mainframeNum);
        
    displayTitle();

    do
    {
        printf ("\n   *** Delete ASO Group from List ***\n\n");

        prompt ("\nPlease enter the Group Number to delete (or Q to Quit): ", 
            newGrpNum, GRP_NUM_LEN + 1);

        if (toupper (newGrpNum[0]) != 'Q')
        {
            if (validGrpNum (newGrpNum, GRP_NUM_LEN) == OK)
            {
                convert2MainFrame (newGrpNum, mainframeNum);

                if ((exitStatus = findGroupRecord (mainframeNum)) == GROUP_ALREADY_EXIST)
                {
                    if ((exitStatus = removeGroupRecord()) == OK)
                    {
    	                printf ("\a\nGroup %s deleted.", newGrpNum);

                        getEnterKey2Continue();
                    }
                }
            }
                            
            else
            {
    	        errorProcess (INVALID_GROUP_NUM);
				exitStatus = INVALID_GROUP_NUM;
            }
        }

        else
            exitStatus = QUIT;

    } while (exitStatus == INVALID_GROUP_NUM);

    return exitStatus;

} // end deleteGroup


// **************************************************************************
// Name        : processMainframeData
//
// Purpose     : Calls functions to get and compare processing dates to ensure
//               that desired files are processed.  Calls functions to read,
//               process and write files.
//
// Parameters  : None
//
// Return Value: OK    If all functions completed nominally
//
// Dependencies: Accurate aso_list.txt file exists in c:\deltausa
//
// Algorithm   : 
// **************************************************************************
int processMainframeData (void)
{
    char fromDate[LONG_DATE+1] = "";
    char thruDate[LONG_DATE+1] = "";
    int exitStatus = OK;
	
    // get control date range from user
    if ((exitStatus = inputDateRange (fromDate, thruDate)) == OK)
    {
        // compare to dates of all mainframe data files before processing
    	if ((exitStatus = compareDateRanges (fromDate, thruDate)) == OK)
		{
			printf ("\a\n\nAll dates were the same.");
			getEnterKey2Continue();
		}

    }
    
    if (exitStatus == OK)

        if ((exitStatus = readWriteFiles()) == OK)

            if ((exitStatus = summarizePayData()) == OK)

                if ((exitStatus = separatePayData()) == OK)
				{
                    printf ("\a\nData processing complete.");
                    getEnterKey2Continue();
				}

    return exitStatus;

} // end processMainframeData


// **************************************************************************
// Name        : inputDateRange
//
// Purpose     : Get processing date range from user in enforced format.
//
// Parameters  : char    *fromDate    Beginning date of processing range
//               char    *thruDate    Ending date of processing range
//
// Return Value: OK
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int inputDateRange (char *fromDate, char *thruDate)
{
    displayTitle();

    do
    {
        prompt ("\nPlease enter the beginning check date (mm/dd/yyyy): ", 
            fromDate, LONG_DATE);

        if (validDate (fromDate, LONG_DATE) != OK)
            printf ("\a\n*** You must enter date in format mm/dd/yyyy ***");

    } while (validDate (fromDate, LONG_DATE) != OK);    

    do
    {
        prompt ("\n   Please enter the ending check date (mm/dd/yyyy): ", 
            thruDate, LONG_DATE);

        if (validDate (thruDate, LONG_DATE) != OK)
            printf ("\a\n*** You must enter date in format mm/dd/yyyy ***");

    } while (validDate (thruDate, LONG_DATE) != OK);    
   
   return OK;

} // end function inputDateRange


