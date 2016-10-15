// ***************************************************************************
// Copyright (c) Robert Conner 2001
// ***************************************************************************
//
// Module Name     : Print Module
// Descriptive Name: DeltaUSA Print Functions
// Purpose         : Contains function to sent text/reports to screen, file, or 
//                   printer.
// Author Name     : Robert Conner
// Date Created    : March 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaPrint.cpp
// Last Update     : R Conner 05/25/01
//
//
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "dusamain.h"
#include "dusadata.h"

// file dates
extern char readFromDate[LONG_DATE+1];
extern char readThruDate[LONG_DATE+1];

// **************************************************************************
// Name        : printTitle
//
// Purpose     : Prints System Header
//
// Parameters  : *pOutDevice          FILE       output device
//
// Return Value: OK               Output device was initialized properly
//               PRINTER_ERROR    Problem with output device
//
// Dependencies: None
// **************************************************************************
int printTitle (FILE *pOutDevice)
{
    int exitStatus = OK;
    time_t t;
    time (&t);

    if (pOutDevice)
    {
        fprintf (pOutDevice, "%s", ctime (&t));

        fprintf (pOutDevice, "\n\nD E L T A  D E N T A L  I N S U R A N C E  C O M P A N Y");
        fprintf (pOutDevice, "\n\nDeltaUSA Fee Reporting System");
        fprintf (pOutDevice, "\n\nCopyright (c) DDIC 2001\n\n");  
    }

    else
        exitStatus = PRINTER_ERROR;

    return exitStatus;

} // end function printTitle


// **************************************************************************
// Name        : printHelp
//
// Purpose     : Display Help Information
//
// Parameters  : *helpFile     char array   contains path of help file
//               *helpTitle    char array   contains title of help file
//               *pOutDevice   FILE         pointer to output device
//
// Return Value: OK               Output device was initialized properly
//               PRINTER_ERROR    Problem with output device
//
// Dependencies: None
// **************************************************************************
int printHelp (char *helpFile, char *helpTitle, FILE *pOutDevice)
{
    int exitStatus = 0;
    int userChoice = 0;

    do
	{
        displayTitle();

        printf ("     %s", helpTitle);

        printf ("\n\n                     How would you like to view the file?\n");
        printf ("\n                        P. Print Help File");
	    printf ("\n                        D. Display Help File on Screen");
	    printf ("\n\n                     Select: ");

        userChoice = toupper (miscGetChar());

        if (userChoice == 'P')
		{
            pOutDevice = fopen ("LPT1", "w");
            printTitle (pOutDevice);
		}

        if (userChoice == 'D')
		{
            pOutDevice = fopen ("CON", "w");
            displayTitle();
		}

		if (userChoice != 'P' && userChoice != 'D')
		{
			printf ("\a\nInvalid option, please try again.\n");
            getEnterKey2Continue();
		}

    } while (userChoice != 'P' && userChoice != 'D');

    if (pOutDevice)
	{
        exitStatus = displayText (helpFile, &userChoice, pOutDevice);

        fclose (pOutDevice);
    }

    else
        exitStatus = PRINTER_ERROR;

    return exitStatus;

} // end function printHelp

// **************************************************************************
// Name        : displayText
//
// Purpose     : Display Contents of Text File to Specified Device
//
// Parameters  : fileName     char array   filename to display
//               userChoice   int          flag to determine how output is formatted
//               pOutDevice   FILE         points to output device
//
// Return Value: OK               If file is initialized properly
//               FILE_READ_ERROR  Unable to open file for reading
//
// Dependencies: Called file must exist
// **************************************************************************
int displayText (char fileName[], int *userChoice, FILE *pOutDevice)
{
    int exitStatus = OK;
    int inChar = 0;
    int numLines = 0;

    FILE *inFile = 0;

    if ((inFile = fopen (fileName, "r")) != NULL)
    {
        while ((inChar = getc (inFile)) != EOF)
        {
            putc (inChar, pOutDevice);

            if (*userChoice == 'D')
            {
                if (inChar == '\n')
                {
					numLines++;
				    fclose (pOutDevice);
					pOutDevice = fopen ("CON", "w");
				}
				
                if (numLines >= MAX_LINES)
                {
                    getEnterKey2Continue();
                    numLines = 0;
                }
            }

        } // end while

		if (*userChoice == 'D')
			getEnterKey2Continue();

        fclose (inFile);
    }

    else
        exitStatus = FILE_READ_ERROR; 

    return exitStatus;

} // end function displayText


// **************************************************************************
// Name        : printReport
//
// Purpose     : 
//
// Parameters  : 
//
// Return Value: 
//
// Error Codes : 
//
// Dependencies: 
// **************************************************************************
int printReport (char fileName[])
{
    int exitStatus = 0;

	int userChoice = 'P'; // send to printer

    FILE *prnt = 0;

    prnt = fopen ("LPT1", "w");

    if (prnt)
    {
        exitStatus = displayText (fileName, &userChoice, prnt);

        fclose (prnt);
        
        printf ("\n\aReport printed.");
    }

    else
        exitStatus = PRINTER_ERROR;

    return exitStatus;

} // end function printReport


// **************************************************************************
// Name        : printRegionSummaries
//
// Purpose     : 
//
// Parameters  : 
//
// Return Value: 
//
// Dependencies: 
//
// Algorithm   : 
// **************************************************************************
int printRegionSummaries (char pathName[], int regionNum, FILE_DATA *totals,
    FILE_DATA *ddicState, FILE_DATA *payState)
{
    int exitStatus = OK;

    FILE *pRegSum = 0;

    printf ("\nWriting Region Summary Report to file.");

	if (regionNum == 1)
	{
		pRegSum = fopen ("c:\\deltausa\\dregsum.prn", "w");
		
		if (pRegSum)
		    fclose (pRegSum);
    }

    pRegSum = fopen ("c:\\deltausa\\dregsum.prn", "a+");

    if (pRegSum)
    {
        printTitle (pRegSum);

        fprintf (pRegSum, "\nDelta USA Service Fees and Withholds");
		fprintf (pRegSum, "\nProcessing Dates from %s through %s\n", 
			readFromDate, readThruDate);
   
        switch (regionNum)
        {
            case (1): fprintf (pRegSum, "\nALABAMA");
                      break;
            case (2): fprintf (pRegSum, "\nFLORIDA");
                      break;
            case (3): fprintf (pRegSum, "\nGEORGIA");
                      break;
            case (4): fprintf (pRegSum, "\nLOUISIANA");
                      break;
            case (5): fprintf (pRegSum, "\nMISSISSIPPI");
                      break;
            case (6): fprintf (pRegSum, "\nTEXAS");
                      break;
            case (7): fprintf (pRegSum, "\nUTAH");
                      break;
            default : fprintf (pRegSum, "\nINVALID REGION");
        }
     
        fprintf (pRegSum, "\nREGION SUMMARY             \t DDIC\t\t Other\t\t Total\n");

        fprintf (pRegSum, "\n# of Claims - Service Fees:\t%6ld\t\t%6ld\t\t%6ld", 
            ddicState->numSvcFees, payState->numSvcFees, 
                ddicState->numSvcFees + payState->numSvcFees);

        fprintf (pRegSum, "\n# of Claims -     Withhold:\t%6ld\t\t%6ld\t\t%6ld",
            ddicState->numWithholds, payState->numWithholds, 
                ddicState->numWithholds + payState->numWithholds);

        fprintf (pRegSum, "\n---------------------------\t------\t\t------\t\t------");

        fprintf (pRegSum, "\n# of Claims -        TOTAL:\t%6ld\t\t%6ld\t\t%6ld\n",
            ddicState->numClaims, payState->numClaims, 
                ddicState->numClaims + payState->numClaims);

        fprintf (pRegSum, "\n              Service Fees:\t$%9.2f\t$%9.2f\t$%9.2f",
            ddicState->svcFeeAmt, payState->svcFeeAmt, 
                ddicState->svcFeeAmt + payState->svcFeeAmt);

        fprintf (pRegSum, "\n                 Withholds:\t$%9.2f\t$%9.2f\t$%9.2f",
            ddicState->withholdAmt, payState->withholdAmt, 
                ddicState->withholdAmt + payState->withholdAmt);

        fprintf (pRegSum, "\n\n\nControl Total Claims Processed - Service Fees: %6ld",
            totals->numSvcFees);

        fprintf (pRegSum, "\n   Control Total Claims Processed - Withholds: %6ld", 
            totals->numWithholds);

        fprintf (pRegSum, "\n       Control Total Claims Processed - Total: %6ld",
            totals->numClaims);

        fprintf (pRegSum, "\n\n                 Control Total Fees Processed: $%9.2f",
            totals->svcFeeAmt);

        fprintf (pRegSum, "\n                  Control Withholds Processed: $%9.2f",
            totals->withholdAmt);

        fprintf (pRegSum, "\n\nFile Processed: %s\n", pathName);

        fprintf (pRegSum, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        fprintf (pRegSum, "\n\n\n\n\n\n\n\n\n\n");

        if (fclose (pRegSum) != OK)
			exitStatus = FILE_CLOSE_ERROR;
    }
   
    else
        exitStatus = FILE_WRITE_ERROR;

    return exitStatus;

} // end function printRegionSummaries


// **************************************************************************
// Name        : printSvcFeesByType
//
// Purpose     : 
//
// Parameters  : 
//
// Return Value: 
//
// Dependencies: 
//
// Algorithm   : 
// **************************************************************************
int printSvcFeesByType (long claimsASO[], float feesASO[], long claimsRISK[],
    float feesRISK[], long *claimTotASO, float *feeTotASO,
    long *claimTotRISK, float *feeTotRISK)
{
    int exitStatus = OK;
    int regionNum = 0;

    FILE *pSvcFee = 0;

    printf ("\n\nWriting Service Fee Report to file.");

    pSvcFee = fopen ("c:\\deltausa\\dsvcfee.prn", "w");

    if (pSvcFee)
    {
        printTitle (pSvcFee);

        fprintf (pSvcFee, "\nDelta USA Service Fees by Region and Group Type");
 		fprintf (pSvcFee, "\nProcessing Dates from %s through %s\n", 
			readFromDate, readThruDate);  
        fprintf (pSvcFee, "\n\t\t\t\t\t Claims\t\t Svc. Fees");
   
        for (regionNum = 0; regionNum < REGIONS_IL; regionNum++)
        {    
            switch (regionNum)
            {
                case (0): fprintf (pSvcFee, "\nREGION SUMMARY: ALABAMA");
                          break;
                case (1): fprintf (pSvcFee, "\nREGION SUMMARY: FLORIDA");
                          break;
                case (2): fprintf (pSvcFee, "\nREGION SUMMARY: GEORGIA");
                          break;
                case (3): fprintf (pSvcFee, "\n   IAM SUMMARY: ILLINOIS");
                          break;
                case (4): fprintf (pSvcFee, "\nREGION SUMMARY: LOUISIANA");
                          break;
                case (5): fprintf (pSvcFee, "\nREGION SUMMARY: MISSISSIPPI");
                          break;
                case (6): fprintf (pSvcFee, "\nREGION SUMMARY: TEXAS");
                          break;
                case (7): fprintf (pSvcFee, "\nREGION SUMMARY: UTAH");
                          break;
                default : fprintf (pSvcFee, "\nINVALID REGION");

            }

            fprintf (pSvcFee, "\n\t\tASO: \t\t\t%6ld\t\t$%10.2f",
                claimsASO[regionNum], feesASO[regionNum]); 

            fprintf (pSvcFee, "\n\t\tRISK:\t\t\t%6ld\t\t$%10.2f\n\n",
                claimsRISK[regionNum], feesRISK[regionNum]);

        } // end for loop

        // print summary        

        fprintf (pSvcFee, "\nTOTALS\t\t\t\t\t Claims\t\t Svc. Fees\t\t\n");

        fprintf (pSvcFee, "\nFile:  c:\\deltausa\\ASO_USA.txt:      %9ld\t\t$%10.2f",
            *claimTotASO, *feeTotASO);

        fprintf (pSvcFee, "\nFile: c:\\deltausa\\RISK_USA.txt:      %9ld\t\t$%10.2f\n",
            *claimTotRISK, *feeTotRISK);

        fprintf (pSvcFee, "\nGRAND TOTALS:                        %9ld\t\t$%10.2f\n",
            *claimTotRISK + *claimTotASO, *feeTotRISK + *feeTotASO);

        if (fclose (pSvcFee) != OK)
			exitStatus = FILE_CLOSE_ERROR;
    }

    else
        exitStatus = FILE_WRITE_ERROR;

    return exitStatus;
      
} // end function printSvcFeesByType


