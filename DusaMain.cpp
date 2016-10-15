// ***************************************************************************
// Copyright (c) 2001 by Robert Conner
// ***************************************************************************

// Module Name     : Menus Module
// Descriptive Name: DeltaUSA Menus
// Purpose         : This module contains menu functions for selecting system
//                   processing options.
// Author Name     : Robert Conner
// Date Created    : February 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaMain.cpp
// Last Update     : R Conner 05/25/01


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dusamain.h"
#include "dusadata.h"


// **************************************************************************
// Name        : Main
//
// Purpose     : Dispatches functions based on Main (Master) Menu Selection
//
// Parameters  : None
//
// Return Value: Zero (0) to system environment
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
int main (void)
{
    int menuChoice = 0;
    int exitStatus = OK;
    int statusStatus = OK;

    if ((exitStatus = copyAsoList2GrpDir()) == OK)
    {
        while (exitStatus == OK)
        {
            displayMainMenu();

            menuChoice = toupper (miscGetChar());
        
            switch (menuChoice)
            {
                case 'M': statusStatus = maintainAsoList();
                          break;
                case 'P': statusStatus = processMainframeData();
                          break;
                case 'H': statusStatus = selectHelpFunction();
                          break;
                case 'R': statusStatus = selectReport();
                          break;
                case 'X': exitStatus = QUIT;
                          break;
                default : statusStatus = INVALID_MENU_CHOICE;
                          break;
            } // end switch

            if (statusStatus != OK)
			{
                errorProcess (statusStatus);
                statusStatus = OK;
			}

        } // end while

        exitStatus = writeAsoList2File();

    } // end if copyAsoList2Array

    if (exitStatus != OK)
        errorProcess (exitStatus);
    else
       removeAllGroupRecords();

    return 0;
    
} // end function main


// **************************************************************************
// Name        : maintainAsoList
//
// Purpose     : Dispatches functions based on ASO List Menu Selection
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
int maintainAsoList (void)
{
    int menuChoice = 0;
    int quitStatus = OK;
    int statusStatus = OK;

    while (quitStatus == OK)
    {
        displayAsoListMenu();

        menuChoice = toupper (miscGetChar());
        
        switch (menuChoice)
        {
            case 'A': statusStatus = addNewGroup();
                      break;
            case 'D': statusStatus = deleteGroup();
                      break;
            case 'L': statusStatus = displayAsoList();
                      break;
            case 'H': statusStatus = selectHelpFunction();
                      break;
            case 'Q': quitStatus = QUIT;
                      break;
            default : printf ("\a\n\t\tInvalid option, please try again.\n");
                      getEnterKey2Continue();
				      break;
        } // end switch

    } // end while

    return statusStatus;

} // end function maintainAsoList


// **************************************************************************
// Name        : selectHelpFunction
//
// Purpose     : Dispatches functions based on Help Menu Selection
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
int selectHelpFunction(void)
{
    int menuChoice = 0;
    int quitStatus = OK;
    int statusStatus = OK;

	FILE *pOutDevice = 0;

    while (quitStatus == OK)
    {
        displayHelpMenu();

        menuChoice = toupper (miscGetChar());
        
        switch (menuChoice)
        {
            case 'M': statusStatus = printHelp ("c:\\deltausa\\dasohelp.txt",
						                        "ASO Group List Maintenance Help", pOutDevice);
					  break;
            case 'C': statusStatus = printHelp ("c:\\deltausa\\dcdshelp.txt",
						                        "Create Mainframe Data Set Help", pOutDevice);
                      break;
            case 'D': statusStatus = printHelp ("c:\\deltausa\\dddshelp.txt",
						                        "Download Data Set Help", pOutDevice);
                      break;
            case 'P': statusStatus = printHelp ("c:\\deltausa\\dpdfhelp.txt",
						                        "Process Data File Help", pOutDevice);
                      break;
            case 'I': statusStatus = printHelp ("c:\\deltausa\\disshelp.txt",
						                        "Import to Spreadsheet Help", pOutDevice);
                      break;
            case 'Q': quitStatus = QUIT;
                      break;
            default : printf ("\a\n\t\tInvalid option, please try again.\n");
                      getEnterKey2Continue();
				      break;
        } // end switch

    } // end while

    return statusStatus;

} // end function selectHelpFunction


// **************************************************************************
// Name        : selectReport
//
// Purpose     : Dispatches functions based on Report Menu Selection
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
int selectReport (void)
{
    int menuChoice = 0;
    int quitStatus = OK;
    int statusStatus = OK;

    while (quitStatus == OK)
    {
        displayReportMenu();

        menuChoice = toupper (miscGetChar());

        switch (menuChoice)
        {            
            case 'L': statusStatus = displayAsoList();
                      break;
            case 'R': statusStatus = printReport ("c:\\deltausa\\dregsum.prn");
                      break;
            case 'S': statusStatus = printReport ("c:\\deltausa\\dsvcfee.prn");
                      break;
            case 'H': statusStatus = selectHelpFunction();
                      break;
            case 'Q': quitStatus = QUIT;
                      break;
            default : printf ("\a\nInvalid option, please try again.\n");
                      getEnterKey2Continue();
				      break;
        } // end switch

    } // end while

    return statusStatus;

} // end function selectReport


// **************************************************************************
// Name        : displayMainMenu
//
// Purpose     : Displays selections for Main (Master) Menu
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
void displayMainMenu (void)
{
    displayTitle();

    printf ("               MAIN MENU");
    printf ("\n\n                    ");
    printf ("M. Maintain ASO Group List");
    printf ("\n                    ");
    printf ("P. Process Mainframe Files");
    printf ("\n                    ");
    printf ("H. Display Help Menu");
    printf ("\n                    ");
    printf ("R. Generate Report");
    printf ("\n\n                    ");
    printf ("X. Exit System");
    printf ("\n\n\n\n                    ");
    printf ("Select a menu item: ");
    
    return;

} // end function displayMainMenu


// **************************************************************************
// Name        : displayAsoListMenu
//
// Purpose     : Displays selections for ASO List Maintenance
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
void displayAsoListMenu (void)
{
    displayTitle();   

    printf ("          ASO LIST MAINTENANCE");
    printf ("\n\n                    ");
    printf ("A. Add New Group");
    printf ("\n                    ");
    printf ("D. Delete Group");
    printf ("\n                    ");
    printf ("L. Print ASO Group List");
    printf ("\n                    ");
    printf ("H. Display Help Menu");
    printf ("\n\n                    ");
    printf ("Q. Quit - Return to Main Menu");
    printf ("\n\n\n\n                    ");
    printf ("Select a menu item: ");

    return;

} // end function displayAsoListMenu


// **************************************************************************
// Name        : displayHelpMenu
//
// Purpose     : Displays selections for Help Menu
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
void displayHelpMenu (void)
{
    displayTitle();   

    printf ("              HELP MENU");
    printf ("\n\n                    ");
    printf ("Please help me to . . . ");
    printf ("\n\n                    ");
    printf ("M. Maintain the ASO Group List");
    printf ("\n                    ");
    printf ("C. Create DeltaUSA Report Data Sets");
    printf ("\n                    ");
    printf ("D. Download Data Sets from the Mainframe");
    printf ("\n                    ");
    printf ("P. Process the Mainframe Files");
    printf ("\n                    ");
    printf ("I. Import the Output Files into Excel");
    printf ("\n\n                    ");
    printf ("Q. Quit - Return to Main Menu");
    printf ("\n\n                    Select a menu item: ");

    return;

} // end function displayHelpMenu


// **************************************************************************
// Name        : displayReportMenu
//
// Purpose     : Displays selections for Report Menu
//
// Parameters  : None
//
// Return Value: None
//
// Error Codes : None
//
// Dependencies: None
//
// Algorithm   : Self - explanatory
// **************************************************************************
void displayReportMenu (void)
{
    displayTitle();

    printf ("             REPORT MENU");
    printf ("\n\n                    ");
    printf ("L. List ASO Groups");
    printf ("\n                    ");
    printf ("R. Print Region Summaries");
    printf ("\n                    ");
    printf ("S. Print Service Fees by Region and Type");
    printf ("\n                    ");
    printf ("H. Display Help Menu");
    printf ("\n\n                    ");
    printf ("Q. Quit - Return to Main Menu");
    printf ("\n\n\n\n                    ");
    printf ("Select a menu item: ");
    return;

} // end function displayReportMenu


