// ***************************************************************************
// Copyright (c) 2001 by Robert Conner
// ***************************************************************************

// Module Name     : Data List Management Modules
// Descriptive Name: DeltaUSA Data Manipulation Module
// Purpose         : Contains low level data access and file functions related
//                   to aso_list.txt and the aso group linked list.
// Author Name     : Robert Conner
// Date Created    : February 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaDataList.cpp
// Last Update     : R Conner 06/11/01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include "dusamain.h"
#include "dusadata.h"


// module variables

GROUP      *pGroupDir   = 0;   // points to group directory list
GROUP_INFO *pGroupArray = 0;   // points to group array for printing
GROUP      *pCurGroup   = 0;   // points to current group record
GROUP      *pPrevGroup  = 0;   // points to previous group record


// ****************************************************************************
// Name        : getFirstGroup
//
// Purpose     : Sets current group pointer to first group record in the list.
//               The current group pointer is set to zero if list is empty.
//
// Parameters  : None
//
// Return Value: OK if Success
//               GROUP_NOT_FOUND if the list is empty.
//
// Dependencies: None
//
// Algorithm   : Sets current record pointer to address of first group.   
//               If zero, returns "group not found" to calling function.
//               If current record pointer is valid, returns OK.  
//               Sets other group and group pointers to zero.
// 
// ****************************************************************************
int getFirstGroup (void)
{
    int exitStatus = OK;

    pCurGroup = pGroupDir;
	
    if (pCurGroup == 0)
        exitStatus = GROUP_NOT_FOUND;

    pPrevGroup  = 0;
	
    return (exitStatus);

} // end function getFirstGroup


// ****************************************************************************
// Name        : getNextGroup
//
// Purpose     : Advances the current group pointer to the next group record
//               if it exists.  Sets current group pointer to zero if there
//               are no more group records in the list.
//         
// Parameters  : None
//
// Return Value: OK if Success
//               GROUP_NOT_FOUND if current group is last in the list
//               INVALID_GROUP_RECORD if current group pointer is zero,
//                     (the list is empty)
//
// Dependencies: pCurGroup should point to the current group record
//               pPrevGroup should point to the previous group record if
//               it exists, otherwise zero
//
// Algorithm   : Check that current group pointer is at a valid record.
//               Set previous record to current and get next record.  Set 
//               current record to zero if at the end of the list.  
//               Return exitStatus to calling function.
// 
// ****************************************************************************
int getNextGroup (void)
{
    int exitStatus = OK;

    if (pCurGroup)
    {
        pPrevGroup = pCurGroup;
		
    	pCurGroup = pCurGroup->next;
		
        if (pCurGroup == 0)
	        exitStatus = GROUP_NOT_FOUND;
    }
	
    else
        exitStatus = INVALID_GROUP_RECORD;

    return (exitStatus);

} // end function getNextGroup



// ****************************************************************************
// Name        : getLastGroup
//
// Purpose     : Advances current group pointer to the last group record if
//               the list exists.  Set current group pointer to zero if
//               there's no group record in the list.
//
// Parameters  : None
//
// Return Value: OK if Success
//               GROUP_NOT_FOUND if list is empty
//
// Dependencies: None
//
// Algorithm   : Start search at the beginning of the list. While the current
//               record is not the last, get the next record.  Return exitStatus
//               to calling function when last record is reached.
// 
// ****************************************************************************
int getLastGroup (void)
{
    int exitStatus = OK;

    if ((exitStatus = getFirstGroup()) == OK) 
    {
        while (pCurGroup->next)
            exitStatus = getNextGroup();
    }

    else
		exitStatus = GROUP_NOT_FOUND;

     return (exitStatus);

} // end function getLastGroup


// ****************************************************************************
// Name        : findGroupRecord
//
// Purpose     : Searches for the group record that matches the group mainframeNum
//               requested by the calling function and sets the current group
//               and previous group pointers to their matching records if the
//               record exists.
//
// Parameters  : char *mainframeNum   char array (in)  points to group 
//                    mainframeNum string
//
// Return Value: GROUP_ALREADY_EXIST if matching group record found
//               GROUP_NOT_FOUND if list is empty or no match found
//
// Dependencies: mainframeNum is valid
//
// Algorithm   : 
// 
// ****************************************************************************
int findGroupRecord (char *mainframeNum)
{
    int exitStatus = GROUP_ALREADY_EXIST;

    if ((exitStatus = getFirstGroup()) == OK)
    {
        while ((exitStatus == OK) && 
            stricmp (mainframeNum, pCurGroup->mainframeNum) != 0)

        exitStatus = getNextGroup();
    }

    if (exitStatus == OK)
    	exitStatus = GROUP_ALREADY_EXIST;

	return (exitStatus);

} // end function findGroupRecord
	    

// ****************************************************************************
// Name        : appendGroupRecord
//
// Purpose     : Adds the new group record to the end of the group list
//
// Parameters  : GROUP *newGroup    structure pointer (in)
//               Address of the new group record to be added.
//
// Return Value: OK the new group record was added to list
//               INVALID_INPUT_PARAMETER if input is invalid
//
// Dependencies: pCurGroup should point to the current group record if one
//               exists, otherwise should equal zero.
//               pPrevGroup should point to the previous group record if one
//               exists.
//
// Algorithm   : If new group pointer is valid, go to the end of
//               the group list, if it exists.  Set the previous group record
//               to the current group record and have 'next' point to the new
//               group (next group in the list).  If there is no group list,
//               set previous group pointer to zero and the group directory
//               base / anchor to the new group.  Set the current group equal
//               to the next group and have 'next' point to zero (it is the
//               last record in the list).  If newGroup pointer is invalid, 
//               set exitStatus to invalid input parameter.
//               Return exitStatus to the calling function.
// 
// ****************************************************************************
int appendGroupRecord (GROUP *newGroup)
{
    int exitStatus = OK;

    if (newGroup)
    {
        if (getLastGroup() == OK)
		{
            pPrevGroup = pCurGroup;
            pPrevGroup->next = newGroup;
        }
    
    	else
		{
            pPrevGroup = 0;
            pGroupDir = newGroup;
		}

        pCurGroup = newGroup;
    }

    else
        exitStatus = INVALID_INPUT_PARAMETER;

    return (exitStatus);

} // end function appendGroupRecord


// ****************************************************************************
// Name        : createGroupRecord
//
// Purpose     : Creates a new group record, copies the mainframeNum into the 
//               mainframeNum field,
//               and appends the new record to the end of the group list.
//               pCurGroup will point to the current group structure and
//               pPrevGroup will point to the previous group structure.
//
// Parameters  : char *mainframeNum    char array (in)  contains the group 
//               mainframeNum to be searched for
//
// Return Value: OK if Success
//               INVALID_GROUP_RECORD if pCurGroup is invalid pointer
//               NO_MEM if problem with malloc
//
// Dependencies: pCurGroup should point to the current group structure
//               pPrevGroup should point to the previous group structure, or
//               zero if none.
//               mainframeNum should not exist
//
// Algorithm   : If the new group mainframeNum does not already exist, allocate memory
//               for the new group record.  If allocation successful, stamp
//               new record with supplied mainframeNum and append to group list.
//               Return exitStatus to calling function.
//
// ****************************************************************************
int createGroupRecord (char *mainframeNum)
{
    int exitStatus = OK;

    GROUP *pNewGroup = 0;

	pNewGroup = (GROUP *) malloc (sizeof (GROUP));

	if (pNewGroup)
	{
        strcpy (pNewGroup->mainframeNum, mainframeNum);
            
        if ((exitStatus = appendGroupRecord (pNewGroup)) == OK)
            pCurGroup->next = 0;
    }

    else
        exitStatus = NO_MEM;

    return exitStatus;

} // end function createGroupRecord


// ****************************************************************************
// Name        : updateGroupRecord
//
// Purpose     : Updates the current group record with new data.  Validates
//               curGroupId again the current group record and updates only
//               the data elements that contain valid data.  Skips others.
//
// Parameters  : char *mainframeNum    char array (in)      contains current
//                                                          group mainframeNum
//               char *newGrpNum       char array (in)      contains abstract
//                                                          system grp num
//
// Return Value: OK if Success
//               INVALID_GROUP_RECORD if curGroupId does not match
//                     the mainframeNum stored in the current group record or if
//                     pCurGroup is not a valid pointer
//
// Dependencies: pCurGroup should point to the current group structure
//               pPrevGroup should point to the previous group structure, or
//               zero if none.
//
// Algorithm:    If current group is valid and received mainframeNum matches
//               current group mainframeNum, copy newGrpNum into current group
//               absGroupNum.  Else set exitStatus to INVALID_GROUP_RECORD.
//
// ****************************************************************************
int updateGroupRecord (char *mainframeNum, char *newGrpNum)
{
    int exitStatus = OK;

    if (pCurGroup && (stricmp (pCurGroup->mainframeNum, mainframeNum) == 0))
        strcpy (pCurGroup->absGroupNum, newGrpNum);

    else
        exitStatus = INVALID_GROUP_RECORD;

    return exitStatus;

} // end function updateGroupRecord


// ****************************************************************************
// Name        : removeGroupRecord
//
// Purpose     : Deletes the current group record from the list
//
// Parameters  : char *mainframeNum       char array (in)   contains the group 
//                                                          mainframeNum
//
// Return Value: OK if Success
//               INVALID_GROUP_RECORD if pCurGroup is not valid
//
// Dependencies: pCurGroup should point to the current group structure
//               pPrevGroup should point to the previous group structure, or
//               zero if none.
//
// Algorithm   : If previous group is valid, set previous group next pointer to point
//               to current group next pointer.  Else, set group list anchor to point to
//               current group next pointer.  If current group is valid, free current
//               group pointer and set current and previous pointers to zero.  Else
//               set exitStatus to INVALID_GROUP_RECORD.
// 
// ****************************************************************************
int removeGroupRecord (void)
{
    int exitStatus = OK;

    if (pPrevGroup)
        pPrevGroup->next = pCurGroup->next;

    else
        pGroupDir = pCurGroup->next;

    if (pCurGroup)
    {
        free (pCurGroup);
    	pCurGroup = 0;
    	pPrevGroup = 0;
    }

    else
        exitStatus = INVALID_GROUP_RECORD;
		
    return exitStatus;

} // end function removeGroupRecord


// ****************************************************************************
// Name        : getGroupInfo
// 
// Purpose     : Copy current group record info into groupInfo buffer
//
// Parameters  : *groupInfo      groupInfo buffer  structure to hold groupInfo
//                                                 information
//
// Return Value: OK if Success
//               INVALID_GROUP_RECORD if current group record is invalid pointer
//               INVALID_INPUT_PARAMETER if groupInfo is an invalid pointer
//
// Dependencies: pCurGroup should point to the current group structure
//               pPrevGroup should point to the previous group structure, or
//               zero if none.
//
// Algorithm   : If groupInfo pointer is valid, check for valid current group
//               record.  If valid current group record, copy data into groupInfo
//               buffer.  Else, set exitStatus to INVALID_GROUP_RECORD.  If
//               groupInfo buffer is invalid, set exitStatus to INVALID_INPUT_PARAMETER.
// 
// ****************************************************************************
int getGroupInfo (GROUP_INFO *groupInfo)
{
    int exitStatus = OK;

    if (groupInfo)
    {
        if (pCurGroup)
		{
            strcpy (groupInfo->absGroupNum, pCurGroup->absGroupNum);
			strcpy (groupInfo->mainframeNum, pCurGroup->mainframeNum);
		}

		else
			exitStatus = INVALID_GROUP_RECORD;
    }

    else
        exitStatus = INVALID_INPUT_PARAMETER;

    return exitStatus;

} // end function getGroupInfo


// ****************************************************************************
// Name        : initASOListBuffer
// 
// Purpose     : Initialize listBuffer
//
// Parameters  : *listBuffer    structure groupInfo    structure to hold group 
//                                                     record info
//
// Return Value: OK                        Success  
//               INVALID_INPUT_PARAMETER   listBuffer is invalid pointer
//               
//
// Dependencies: None
//
// Algorithm   : If listBuffer is valid structure, initialize both members to
//               NULL strings, else set exitStatus to INVALID_INPUT_PARAMETER.
// 
// ****************************************************************************
int initASOListBuffer (GROUP_INFO *listBuffer)
{
	int exitStatus = OK;

	if (groupInfo)
	{
		strcpy (listBuffer->mainframeNum, "");
		strcpy (listBuffer->absGroupNum, "");
	}

	else
		exitStatus = INVALID_INPUT_PARAMETER;

   return exitStatus;

} // end function initASOListBuffer



// ****************************************************************************
// Name        : removeAllGroupRecords
//
// Purpose     : Deletes the current group record from the list
//
// Parameters  : char *mainframeNum       char array (in)   contains the group 
//                                                          mainframeNum
//
// Return Value: None
//
// Dependencies: None
//
// Algorithm   : While exitStatus is OK, if the first group in the list exists,
//               remove it.
// 
// ****************************************************************************
void removeAllGroupRecords (void)
{
	int exitStatus = OK;

    while (exitStatus == OK)
    {
		if ((exitStatus = getFirstGroup()) == OK)
			exitStatus = removeGroupRecord();
	}

	return;

} // end removeAllGroupRecords


// **************************************************************************
// Name        : copyAsoList2GrpDir
//
// Purpose     : Reads aso_list.txt into linked list
//
// Parameters  : None
//
// Return Value: OK                Success
//               FILE_OPEN_ERROR   Unable to open file for reading
//               FILE_CLOSE_ERROR  Unable to close file
//
// Dependencies: aso_list.txt exists
//
// Algorithm   : 
// **************************************************************************
int copyAsoList2GrpDir (void)
{
    int exitStatus = OK;
    int inChar = 0;

    FILE *pAsoListFile   = 0; // points to aso_list.txt
    
    char mfNum[REG_GRP_LEN + 1] = "";
    char gaNum[GRP_NUM_LEN + 1] = "";

    pAsoListFile = fopen ("c:\\deltausa\\aso_list.txt", "a+");

    if (pAsoListFile)
    {
        rewind (pAsoListFile);

        // read ASO_LIST.txt into array for processing
        while ((inChar = getc (pAsoListFile)) != EOF)
        {
            fscanf (pAsoListFile, "%s%s", mfNum, gaNum);

			inChar = getc (pAsoListFile);
                
            createGroupRecord (mfNum);
            updateGroupRecord (mfNum, gaNum);
        }

        if (fclose (pAsoListFile) != OK)
			exitStatus = FILE_CLOSE_ERROR;
       
    } // end if file opened successfully

    else
        exitStatus = FILE_OPEN_ERROR;
    
    return exitStatus;
    
} // end function copyAsoList2Array


// **************************************************************************
// Name        : writeAsoList2File
//
// Purpose     : Writes contents of linked list to aso_list.txt after backing
//               up existing aso_list.txt to aso_list.bak 
//
// Parameters  : None
//
// Return Value: OK                 Success
//               FILE_CLOSE_ERROR   Unable to close file
//               FILE_OPEN_ERROR    Unable to open file
//               FILE_BACKUP_ERROR  Unable to copy existing file to backup file
//               FILE_WRITE_ERROR   Unable to create file
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int writeAsoList2File (void)
{
    int exitStatus = OK;

    FILE *pAsoListFile   = 0; // points to aso_list.txt
    
	pAsoListFile = fopen ("c:\\deltausa\\aso_list.txt", "a+");

    if (pAsoListFile)
    {
		if ((exitStatus = backupAsoList (pAsoListFile)) == OK)
		{
            fclose (pAsoListFile);
			pAsoListFile = fopen ("c:\\deltausa\\aso_list.txt", "w+t");
		
			if (pAsoListFile)
			{
                if ((exitStatus = getFirstGroup()) == OK)
                
	    	    do
				{
	                fprintf (pAsoListFile, "%8s %8s\n", pCurGroup->mainframeNum,
                        pCurGroup->absGroupNum);

				} while (getNextGroup() == OK);
	            
                if (fclose (pAsoListFile) != OK)
					exitStatus = FILE_CLOSE_ERROR;
			}

    	    else
	    	    exitStatus = FILE_WRITE_ERROR;
		}

	    else
		    exitStatus = FILE_BACKUP_ERROR;
	}

    else
		exitStatus = FILE_OPEN_ERROR;

    return exitStatus;

} // end function writeAsoList2File


// **************************************************************************
// Name        : backupAsoList
//
// Purpose     : Creates backup copy of received file 
//
// Parameters  : *pAsoListFile    FILE pointer   Points to aso_list.txt
//
// Return Value: OK                       Success
//               FILE_CLOSE_ERROR         Unable to close file
//               FILE_OPEN_ERROR          Unable to open file
//               INVALID_INPUT_PARAMETER  Invalid FILE pointer
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int backupAsoList (FILE *pAsoListFile)
{
    int exitStatus = OK;
    int inChar= 0;

    FILE *pAsoListBackup = 0;

    if (pAsoListFile)
    {
        pAsoListBackup = fopen ("c:\\deltausa\\aso_list.bak", "w");

        if (pAsoListBackup)
        {
            printf ("\nBacking up aso_list.txt to aso_list.bak");

			rewind (pAsoListFile);

            while ((inChar = getc (pAsoListFile)) != EOF)
                putc (inChar, pAsoListBackup);

            printf ("\nBackup complete.\n");

            if (fclose (pAsoListBackup) != OK)
				exitStatus = FILE_CLOSE_ERROR;
        }
    
        else
            exitStatus = FILE_OPEN_ERROR;
    }
  
    else
        exitStatus = INVALID_INPUT_PARAMETER;

    return exitStatus;

} // end function backupAsoList


// **************************************************************************
// Name        : displayAsoList
//
// Purpose     : Gives user option to display list of ASO group on the screen
//               or to print as a report
//
// Parameters  : None
//
// Return Value: OK              Function completed successfully
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int displayAsoList (void)
{
    int exitStatus = OK;
    int userChoice = 0;

    do
    {
		displayTitle();

        printf ("\n                          *** Groups in ASO List ***\n");

        printf ("\n                     How would you like to view the list?\n");
        printf ("\n                        P. Print ASO List");
	    printf ("\n                        D. Display ASO List on Screen");
    	printf ("\n\n                     Select: ");

        userChoice = toupper (miscGetChar());

        if (userChoice == 'P')
		    exitStatus = printAsoList();

        if (userChoice == 'D')
            exitStatus = showAsoList();

		if (userChoice != 'P' && userChoice != 'D')
		{
			printf ("\a\nInvalid option, please try again.\n");
            getEnterKey2Continue();
		}

    } while (userChoice != 'P' && userChoice != 'D');   

    return exitStatus;

} // end displayAsoList


// **************************************************************************
// Name        : showAsoList
//
// Purpose     : Displays ASO List on screen
//
// Parameters  : None
//
// Return Value: OK         Function completed successfully
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int showAsoList (void)
{
    int exitStatus = OK;
    int lines = 0;
	int groupCount = 0;
	int indexNum = 0;

    system ("cls");
    printf ("\nGroups in ASO List\n\n");

    if ((exitStatus = buildASOPrintArray(&groupCount)) == OK)
    {
        for (indexNum = 0; indexNum < groupCount; indexNum++)
        {
            printf ("%8s %8s\n", pGroupArray[indexNum].mainframeNum, 
                pGroupArray[indexNum].absGroupNum);

            lines++;

            if (lines > MAX_LINES_LIST)
            {
                getEnterKey2Continue();
				lines = 0;
				
				system ("cls");
                printf ("\nGroups in ASO List\n\n");
            }

        }

		printf ("\nThe ASO List contains %d groups.\n", groupCount);
		getEnterKey2Continue();

	}

    return exitStatus;

} // end function showAsoList


// **************************************************************************
// Name        : printAsoList
//
// Purpose     : Sends ASO List report to printer
//
// Parameters  : None
//
// Return Value: OK               Success
//               PRINTER_ERROR    Problem with printer
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int printAsoList (void)
{
    int exitStatus = OK;
	int groupCount = 0;
	int indexNum = 0;

    FILE *prnt = 0;

    prnt = fopen ("LPT1", "w");

    if (prnt)
    {
        printTitle (prnt);

        fprintf (prnt, "Groups in ASO List\n\n");

        if ((exitStatus = buildASOPrintArray(&groupCount)) == OK)
		{
            for (indexNum = 0; indexNum < groupCount; indexNum++)
			
                fprintf (prnt, "%8s %8s\n", pGroupArray[indexNum].mainframeNum, 
                    pGroupArray[indexNum].absGroupNum);

        }

		fprintf (prnt, "\nThe ASO List contains %d groups.\n", groupCount);

        fclose (prnt);
    }

    else
        exitStatus = PRINTER_ERROR;

    return exitStatus;

} // end function printAsoList


// **************************************************************************
// Name        : buildASOPrintArray
//
// Purpose     : Copies aso linked list into array and sorts for display
//
// Parameters  : groupCount      int       number of groups (records) in list
//
// Return Value: OK                 Success
//               NO_MEM             Problem with realloc
//
// Dependencies: None
//
// Algorithm   : 
// **************************************************************************
int buildASOPrintArray (int *groupCount)
{
    int exitStatus = OK;
	int arrayCount = 0;

	if ((exitStatus = getFirstGroup()) == OK)
	{
		do
		{
			pGroupArray = (GROUP_INFO *) realloc (pGroupArray, sizeof (GROUP_INFO) * 
				(arrayCount+1));

			if (pGroupArray)
			{
				initASOListBuffer (pGroupArray+arrayCount);

				getGroupInfo (pGroupArray+arrayCount);

			    arrayCount++;
			}

			else
				exitStatus = NO_MEM;

		} while (getNextGroup() == OK && exitStatus == OK);

		qsort (pGroupArray, arrayCount, sizeof (GROUP_INFO), compareGroupNum);

	}

	*groupCount = arrayCount;

	return exitStatus;

} // end function buildASOPrintArray


// **************************************************************************
// Name        : compareGroupNum
//
// Purpose     : Sorts aso array by absGroupNum
//
// Parameters  : *arg1  GROUP_INFO pointer  points to first parameter
//               *arg2  GROUP_INFO pointer  points to second parameter
//
// Return Value: A negative integer if group1 alphabetizes before group2
//               A positive integer if group2 alphabetizes before group1
//
// Dependencies: None
//
// **************************************************************************
int compareGroupNum (const void *arg1, const void *arg2)
{
	GROUP_INFO *group1;
	GROUP_INFO *group2;

	int result;

	group1 = (GROUP_INFO *) arg1;
	group2 = (GROUP_INFO *) arg2;

	result = stricmp (group1->absGroupNum, group2->absGroupNum);

	return (result);

} // end function compareGroupNum


// **************************************************************************
// Name        : convert2MainFrame
//
// Purpose     : Converts newGrpNum entered by user into mainframe compatible form
//
// Parameters  : *newGrpNum       char array            contains group number
//                                                         entered by user
//               *mainFrameNum    char array            to contain group number
//                                                         converted into mainframe
//                                                         format by function
//
// Return Value: None
//
// Dependencies: None
//
// Algorithm   : 
//
// gaIndex = index for group abstract group number string
// mfIndex = index for mainframe group number string
// **************************************************************************
void convert2MainFrame (char *newGrpNum, char *mainFrameNum)
{
    unsigned int gaIndex = 0, mfIndex = 0;
    int nonZeroLead = FALSE;

    // begin conversion of state number to mainframe abbreviation
    if (newGrpNum[0] == '0' && newGrpNum[1] == '1')
        strcpy (mainFrameNum, "AL");

    if (newGrpNum[0] == '0' && newGrpNum[1] == '2')
        strcpy (mainFrameNum, "MS");

    if (newGrpNum[0] == '1' && newGrpNum[1] == '0')
        strcpy (mainFrameNum, "FL");
  
    if (newGrpNum[0] == '1' && newGrpNum[1] == '1')
        strcpy (mainFrameNum, "GA");
        
    if (newGrpNum[0] == '1' && newGrpNum[1] == '4')
        strcpy (mainFrameNum, "GA");
        
    if (newGrpNum[0] == '1' && newGrpNum[1] == '9')
        strcpy (mainFrameNum, "LA");
        
    if (newGrpNum[0] == '2' && newGrpNum[1] == '5')
        strcpy (mainFrameNum, "MS");
        
    if (newGrpNum[0] == '2' && newGrpNum[1] == '7')
        strcpy (mainFrameNum, "UT");
        
    if (newGrpNum[0] == '2' && newGrpNum[1] == '9')
        strcpy (mainFrameNum, "UT");
        
    if (newGrpNum[0] == '4' && newGrpNum[1] == '4')
        strcpy (mainFrameNum, "TX");
        
    if (newGrpNum[0] == '4' && newGrpNum[1] == '5')
        strcpy (mainFrameNum, "UT");
    // end conversion to state abbreviation        

    // gaIndex begins at four to scan remaining four digits.
    // mfIndex begins at two to accept valid digits.
    // The purpose of the block is to copy all digits except
    // leading zeroes.  Mainframe group numbers do not have leading zeroes

    for (gaIndex = 4, mfIndex = 2; gaIndex < GRP_NUM_LEN; gaIndex++)
    {
        if (newGrpNum[gaIndex] != '0')
        {
    	    mainFrameNum[mfIndex] = newGrpNum[gaIndex];
            mfIndex++;
            nonZeroLead = TRUE;
        }  

        else 
        {
    	    if (nonZeroLead == TRUE)
            {
	            mainFrameNum[mfIndex] = newGrpNum[gaIndex];
	            mfIndex++;
            }
        }
    } 
	
    return;
    
} // end function convert2MainFrame  	    


// **************************************************************************
// Name        : initDataBuffer
//
// Purpose     : Initialize data buffer
//
// Parameters  : *structIn        FILE_DATA pointer    structure for holding
//                                                     running totals of claims data
//
// Return Value: None
//
// Dependencies: None
//
// **************************************************************************
void initDataBuffer (FILE_DATA *structIn)
{
    structIn->numSvcFees   = 0;
    structIn->numWithholds = 0;
    structIn->numClaims    = 0;
    structIn->svcFeeAmt    = 0.0;
    structIn->withholdAmt  = 0.0;
    structIn->claimsAmt    = 0.0;
    
    return;

} // end function initDataBuffer