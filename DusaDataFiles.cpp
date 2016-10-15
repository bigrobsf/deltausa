// ***************************************************************************
// Copyright (c) 2001 by Robert Conner
// ***************************************************************************

// Module Name     : File Data Processing Functions
// Descriptive Name: DeltaUSA File Processing Module
// Purpose         : Contains function to read and process data set and 
//                   intermediate processing files
// Author Name     : Robert Conner
// Date Created    : February 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaDataFiles.cpp
// Last Update     : R Conner 06/11/01

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include "dusamain.h"
#include "dusadata.h"

FILE *pDataSet    = 0; // points to file downloaded from mainframe
FILE *pDdicUSA    = 0; // points to ddic_usa.txt
FILE *pPayUSA     = 0; // points to pay_usa.txt
FILE *pPaySummary = 0; // points to pay_summ.txt
FILE *pRiskPay    = 0; // points to risk_pay.txt
FILE *pAsoPay     = 0; // points to aso_pay.txt


// file dates (global to be accessed by printing module)
char readFromDate[LONG_DATE+1] = "";
char readThruDate[LONG_DATE+1] = "";

// processing regions
char regionAbrev[REGIONS][ABREV_SIZE] = {{'A','L'}, {'F','L'}, {'G','A'}, {'L','A'},
                                         {'M','S'}, {'T','X'}, {'U','T'}};

// processing regions with Illinois
char regionAbrevIL[REGIONS_IL][ABREV_SIZE] = {{'A','L'}, {'F','L'}, {'G','A'}, {'I','L'}, 
										{'L','A'}, {'M','S'}, {'T','X'}, {'U','T'}};

// non-DDIC states
char stateAbrev[STATES][ABREV_SIZE] = {{'A','R'}, {'A','Z'}, {'C','A'}, {'C','O'},
                                       {'H','I'}, {'I','A'}, {'I','D'}, {'I','L'},
                                       {'K','S'}, {'K','Y'}, {'M','A'}, {'M','I'},
                                       {'I','N'}, {'O','H'}, {'M','N'}, {'N','D'},
                                       {'N','E'}, {'M','O'}, {'S','C'}, {'N','C'},
                                       {'N','H'}, {'M','E'}, {'V','T'}, {'N','J'},
                                       {'C','T'}, {'N','M'}, {'O','K'}, {'O','R'},
                                       {'P','A'}, {'D','C'}, {'D','E'}, {'M','D'},
                                       {'N','Y'}, {'W','V'}, {'P','R'}, {'R','I'},
                                       {'S','D'}, {'T','N'}, {'V','A'}, {'W','A'},
                                       {'W','I'}, {'W','Y'}, {'V','I'}};


// **************************************************************************
// Name        : compareDateRanges
//
// Purpose     : Compares expected date ranges (keyed in by user) with date
//               ranges in dataset files
//
// Parameters  : fromDate   char array   beginning of date range
//               thruDate   char array   ending of date range
//
// Return Value: OK                  if all dates match
//               DATE_MISMATCH_ERROR if dates do not match
//               INVALID_REGION_NUM  if regionNumber is invalid
//               FILE_OPEN_ERROR     if error opening file
//
// Dependencies: None
//
// Algorithm   : Looping through one dataset file at a time, calls function to 
//               compare received date strings to dates in each file.
// **************************************************************************
int compareDateRanges (char *fromDate, char *thruDate)
{
    int exitStatus = OK; 

    unsigned int regionNumber = 1;
   
    char pathName[PATH_SIZE+1] = "";

    displayTitle();
    
    printf ("\nReading files to verify entered date range . . .");

    while (regionNumber <= REGIONS && exitStatus == OK)
    {
    	clearString (pathName);

        switch (regionNumber)
        {
    	    case (1): strcpy (pathName, AL_PATH);
	                  break;
	        case (2): strcpy (pathName, FL_PATH);
	                  break;
    	    case (3): strcpy (pathName, GA_PATH);
	                  break;
	        case (4): strcpy (pathName, LA_PATH);
	                  break;
	        case (5): strcpy (pathName, MS_PATH);
	                  break;
    	    case (6): strcpy (pathName, TX_PATH);
	                  break;
	        case (7): strcpy (pathName, UT_PATH);
	                  break;
            default : exitStatus = INVALID_REGION_NUM;
        } // end switch

        pDataSet = fopen (pathName, "r");

        if (pDataSet)
        {
             printf ("\nProcessing file %d", regionNumber);
             exitStatus = readDateRange (readFromDate, readThruDate);

             if ((stricmp (readFromDate, fromDate) == 0) &&
                 (stricmp (readThruDate, thruDate) == 0))
             {
                 printf (" the dates match.");
                 exitStatus = OK;
             }

             else
             {
                  printf (" the dates DO NOT match.");
    	          exitStatus = DATE_MISMATCH_ERROR;
             }
             
             if (fclose (pDataSet) != OK)
				 exitStatus = FILE_CLOSE_ERROR;
        }

        else
            exitStatus = FILE_OPEN_ERROR;

        regionNumber++;    
            
    } // end while
	
    return exitStatus;
    
} // end function compareDateRanges


// **************************************************************************
// Name        : readDateRange
//
// Purpose     : Gets dates from dataset files
//
// Parameters  : fromDate   char array   first valid date from dataset
//               thruDate   char array   second valid date from dataset
//
// Return Value: OK if valid date found
//
// Dependencies: Assumes that dates appear in first few lines of dataset
//
// **************************************************************************
int readDateRange (char *fromDate, char *thruDate)
{
    int exitStatus = OK;
    char findDate[LONG_DATE+1];
    printf ("\nNow scanning for dates . . .");

    // get "from date"
    while (((exitStatus = validDate (findDate, LONG_DATE)) != OK) &&
        (getc (pDataSet) != EOF))
    {
        fscanf (pDataSet, " %s", findDate);
    }

    if (validDate (findDate, LONG_DATE) == OK)
        strcpy (fromDate, findDate);

    clearString (findDate);
    
    // get "through date"    
    while (((exitStatus = validDate (findDate, LONG_DATE)) != OK) &&
        (getc (pDataSet) != EOF))
    {
        fscanf (pDataSet, " %s", findDate);
    }

    if (validDate (findDate, LONG_DATE) == OK)
        strcpy (thruDate, findDate);
                                                
    return exitStatus;

} // end function readDateRange


// **************************************************************************
// Name        : readWriteFiles
//
// Purpose     : Reads dataset files and creates pay_usa.txt and ddic_usa.txt
//
// Parameters  : None
//
// Return Value: OK                 if success
//               INVALID_FILE_PATH  if regionNum contains an invalid number
//               FILE_OPEN_ERROR    if error opening dataset file
//
// Dependencies: Assumes dataset report formats have not changed: line length,
//               is the same, group number and state abbrieviation are in same
//               location (see readme.txt)
//
// Algorithm   : 
// **************************************************************************
int readWriteFiles (void)
{
    int exitStatus = OK;

    unsigned int regionNum = 1;
    char stateChar1 = ' ';
    char stateChar2 = ' ';

    char pathName [PATH_SIZE+1] = "";
	char groupCandidate[GRP_NUM_SIZE] = "";
	char stateCandidate[ABREV_SIZE+1] = "";
	char readLine[LINE_SIZE+1] = "";

    FILE_DATA totals;    // structure to accumulate totals
    FILE_DATA ddicState; // structure to hold data for DDIC states
    FILE_DATA payState;  // structure to hold data for non-DDIC states (this
                         // must be reimbursed to control plans)

    displayTitle();
    
    printf ("\nProcessing mainframe files . . .");

	// create new files, erasing prior ones
    pDdicUSA = fopen ("c:\\deltausa\\ddic_usa.txt", "w");
	pPayUSA  = fopen ("c:\\deltausa\\pay_usa.txt", "w"); 

    while (regionNum <= REGIONS && exitStatus == OK)
    {
    	clearString (pathName);

        switch (regionNum)
        {
    	    case (1): strcpy (pathName, AL_PATH);
	                  break;
	        case (2): strcpy (pathName, FL_PATH);
	                  break;
    	    case (3): strcpy (pathName, GA_PATH);
	                  break;
	        case (4): strcpy (pathName, LA_PATH);
	                  break;
    	    case (5): strcpy (pathName, MS_PATH);
	                  break;
	        case (6): strcpy (pathName, TX_PATH);
	                  break;
    	    case (7): strcpy (pathName, UT_PATH);
	                  break;
            default : exitStatus = INVALID_FILE_PATH;

        } // end switch

        initDataBuffer (&totals);
        initDataBuffer (&ddicState);
        initDataBuffer (&payState);

        stateChar1 = (char) toupper (pathName[ST_CHAR_LOC1]);
        stateChar2 = (char) toupper (pathName[ST_CHAR_LOC2]);
        
        pDataSet = fopen (pathName, "r");

        if (pDataSet && pDdicUSA && pPayUSA)
        {
            printf ("\nProcessing file %s", pathName);

            if ((exitStatus = skipToNeededSection()) == OK)
            {
    	        while (getc (pDataSet) != EOF)
				{ 
				    fgets (readLine, LINE_SIZE, pDataSet);

					groupCandidate[0] = readLine[GROUP_START];
					groupCandidate[1] = readLine[GROUP_START+1];
					groupCandidate[2] = readLine[GROUP_START+2];
					groupCandidate[3] = readLine[GROUP_START+3];
					groupCandidate[4] = '\0';

					stateCandidate[0] = readLine[STATE_START];
					stateCandidate[1] = readLine[STATE_START+1];
					stateCandidate[2] = '\0';

    				if ((check4GrpNum (groupCandidate) == OK) && 
	       	            (check4State  (stateCandidate) == OK))
					{
   					    exitStatus = readWriteData (stateChar1, stateChar2, readLine, 
						    &totals, &ddicState, &payState);

					}
			
				} // end while

				if (exitStatus == OK)
			        exitStatus = printRegionSummaries (pathName, regionNum, 
    	                &totals, &ddicState, &payState);
			
			} // end skip to needed section

            if (fclose (pDataSet) != OK)
				exitStatus = FILE_CLOSE_ERROR;
        }
        
        else
            exitStatus = FILE_OPEN_ERROR;

        regionNum++;    
            
    } // end while

    if (pDdicUSA && pPayUSA)
    {
		// add trailer data to pay_usa.txt
        fprintf (pPayUSA, "XX XX 9999         0       0.00       0.00");
        fprintf (pPayUSA, "         0.00\n");

        if (fclose (pPayUSA) != OK)
			exitStatus = FILE_CLOSE_ERROR;

		if (fclose (pDdicUSA) != OK)
			exitStatus = FILE_CLOSE_ERROR;
	}

    return exitStatus;

} // end function readWriteFiles


// **************************************************************************
// Name        : skipToNeededSection
//
// Purpose     : Moves file pointer to location in data set where processing 
//               should start
//
// Parameters  : None
//
// Return Value: OK                        Success
//               PROC_SECTION_NOT_FOUND    Did not find "GROUP FOR" in file
//
// Dependencies: Phrase "GROUP FOR" must start in expected column of dataset
//               report
//
// **************************************************************************
int skipToNeededSection (void)
{
    int exitStatus = PROC_SECTION_NOT_FOUND;

	char readLine[LINE_SIZE+1] = "";
	char candidatePhrase[PHRASE_SIZE] = "";

    while ((getc (pDataSet) != EOF) && (exitStatus != OK))
	{ 
	    fgets (readLine, LINE_SIZE, pDataSet);

        clearString (candidatePhrase);

		candidatePhrase[0] = readLine[PHRASE_START];
		candidatePhrase[1] = readLine[PHRASE_START+1];
		candidatePhrase[2] = readLine[PHRASE_START+2];
		candidatePhrase[3] = readLine[PHRASE_START+3];
		candidatePhrase[4] = readLine[PHRASE_START+4];
		candidatePhrase[5] = readLine[PHRASE_START+5];
		candidatePhrase[6] = readLine[PHRASE_START+6];
		candidatePhrase[7] = readLine[PHRASE_START+7];
		candidatePhrase[8] = readLine[PHRASE_START+8];
		candidatePhrase[9] = '\0';

		if (stricmp (candidatePhrase, "GROUP FOR") == 0)

			exitStatus = OK;

	} // end while

    return exitStatus;

} // end function skipToNeededSection


// **************************************************************************
// Name        : readWriteData
//
// Purpose     : Reads in data one line at a time and writes it to output files
//               based on whether the data is determined to be for a DDIC state
//               or non-DDIC state.  Calculates running totals in fileData 
//               structures.
//
// Parameters  : stateChar1  char               First char of region abbreviation
//               stateChar2  char               Second char of region abbreviation
//               readLine    char array         Valid line of data from report
//               *totals     fileData structure Accumulates totals of all data read  
//               *ddicState  fileData structure Accumulates totals of all DDIC state
//                                              data read
//               *payState   fileData structure Accumulates totals of all non-DDIC
//                                              data read (amounts to be paid)
//
// Return Value: OK                  Function was completed         
//
// Dependencies: All files referenced are open and readLine contains valid data
//
// Algorithm   : 
// **************************************************************************
int readWriteData (char stateChar1, char stateChar2, char readLine[],
				   FILE_DATA *totals, FILE_DATA *ddicState, FILE_DATA *payState)
{
    int exitStatus = OK;
    
	long     numClaims = 0;
    long    numSvcFees = 0;
    long  numWithholds = 0;
    double      amount = 0.0;
    double   svcFeeAmt = 0.0;
    double withholdAmt = 0.0;
    double   claimsAmt = 0.0;

	char groupNum[GRP_NUM_SIZE] = "";
	char state   [ABREV_SIZE+1] = "";

    char clmString[WORD_SIZE+1] = "";
	char amtString[WORD_SIZE+1] = "";
	char expString[WORD_SIZE+1] = "";
    char newClmStr[WORD_SIZE+1] = "";
    char newAmtStr[WORD_SIZE+1] = "";
    char newExpStr[WORD_SIZE+1] = "";

    sscanf (readLine, "%s %s %s %s %s", groupNum, state, clmString, amtString, expString);    

    removeCommas (clmString, newClmStr);
    removeCommas (amtString, newAmtStr);
    removeCommas (expString, newExpStr);
  
    numClaims = atol (newClmStr);
    amount    = atof (newAmtStr);
    claimsAmt = atof (newExpStr);
   
    // cannot divide by zero
    if (numClaims != 0)
	{
        if (amount / numClaims == .25)
		{
            svcFeeAmt = amount;
            numSvcFees = numClaims;
		}

        else
		{
            withholdAmt = amount;
            numWithholds = numClaims;
		}
	}

    totals->numSvcFees   += numSvcFees;
    totals->numWithholds += numWithholds;
    totals->numClaims    += numClaims;
    totals->svcFeeAmt    += svcFeeAmt;
    totals->withholdAmt  += withholdAmt;
    totals->claimsAmt    += claimsAmt;

    if (!strcmp(state,"AL") || !strcmp(state,"AK") ||
        !strcmp(state,"FL") || !strcmp(state,"GA") ||
        !strcmp(state,"LA") || !strcmp(state,"MS") ||
        !strcmp(state,"MT") || !strcmp(state,"NV") ||
        !strcmp(state,"TX") || !strcmp(state,"UT"))
	{   

        // write to DDIC file
	    fprintf (pDdicUSA, "%c%c %s %4s %9ld %10.2f %10.2f %12.2f\n",
            stateChar1, stateChar2,state, groupNum,
	    	numClaims, svcFeeAmt, withholdAmt, claimsAmt);

        ddicState->numSvcFees   += numSvcFees;
        ddicState->numWithholds += numWithholds;
        ddicState->numClaims    += numClaims;
        ddicState->svcFeeAmt    += svcFeeAmt;
        ddicState->withholdAmt  += withholdAmt;
        ddicState->claimsAmt    += claimsAmt;
	} 

    else	   
	{ 
        // write to Pay (non-DDIC) file
	    fprintf (pPayUSA, "%c%c %s %4s %9ld %10.2f %10.2f %12.2f\n",
            stateChar1, stateChar2, state, groupNum,
	     	numClaims, svcFeeAmt, withholdAmt, claimsAmt);

        payState->numSvcFees   += numSvcFees;
        payState->numWithholds += numWithholds;
        payState->numClaims    += numClaims;
        payState->svcFeeAmt    += svcFeeAmt;
        payState->withholdAmt  += withholdAmt;
        payState->claimsAmt    += claimsAmt;
	}
            
	return exitStatus;

} // end function readWriteData


// **************************************************************************
// Name        : summarizePayData
//
// Purpose     : Summarizes data into matrix by region and state and writes
//               totals to text file
//
// Parameters  : None
//
// Return Value: OK                 Success
//               FILE_WRITE_ERROR   Unable to open or create file for writing
//                                  (disk may be full)
//               FILE_READ_ERROR    Unable to open file for reading (may not exist)
//               FILE_CLOSE_ERROR   Unable to close file (disk may be full)
//
// Dependencies: pay_usa.txt must exist
//
// Algorithm   : 
// **************************************************************************
int summarizePayData (void)
{
    int exitStatus = OK;

    char region[ABREV_SIZE + 1];
    char grpNum[GRP_NUM_SIZE];
    char  state[ABREV_SIZE + 1];

    long    accumNumClaims[REGIONS][STATES] = {0};
    float   accumSvcFeeAmt[REGIONS][STATES] = {0};
    float accumWithholdAmt[REGIONS][STATES] = {0};
    float   accumClaimsAmt[REGIONS][STATES] = {0};  
   
    int     regionNum = 0;
    int      stateNum = 0;
    long    numClaims = 0;
    float   svcFeeAmt = 0.0;
    float withholdAmt = 0.0;
    float   claimsAmt = 0.0;

	pPayUSA = 0;
	pDdicUSA = 0;
	pDataSet = 0;

    printf ("\n\nCreating summary file of amounts for DDPA remittance advice.");

    if ((pPayUSA = fopen ("c:\\deltausa\\pay_usa.txt", "r")) != NULL)
    {
        do
        {
            fscanf (pPayUSA, " %s %s %s %ld %f %f %f", region, state, grpNum,
                &numClaims, &svcFeeAmt, &withholdAmt, &claimsAmt);
       
            // fill accumlation matrices as states and regions in text file match
            // global region and state matrices
        
            for (regionNum = 0; regionNum < REGIONS; regionNum++)
            {
                for (stateNum = 0; stateNum < STATES; stateNum++)
                {
                    if (region[0] == regionAbrev[regionNum][0] &&
                        region[1] == regionAbrev[regionNum][1] &&
                         state[0] ==   stateAbrev[stateNum][0] &&
                         state[1] ==   stateAbrev[stateNum][1])
                    {
                        accumNumClaims  [regionNum][stateNum] += numClaims;
                        accumSvcFeeAmt  [regionNum][stateNum] += svcFeeAmt;
                        accumWithholdAmt[regionNum][stateNum] += withholdAmt;
                        accumClaimsAmt  [regionNum][stateNum] += claimsAmt; 

                        break; // exit loop once a match is made
                    }
                }
            }

        } while (strcmp(region,"XX") != 0);
    
        // write data to text file
        if ((pPaySummary = fopen ("c:\\deltausa\\pay_summ.txt", "w")) != NULL)
        {
            for (regionNum = 0; regionNum < REGIONS; regionNum++)
            {
                for (stateNum = 0; stateNum < STATES; stateNum++)
                
                    fprintf (pPaySummary, "%c%c%c%c %9ld %10.2f %10.2f %12.2f\n",
                        regionAbrev[regionNum][0], regionAbrev[regionNum][1],
                        stateAbrev  [stateNum][0], stateAbrev  [stateNum][1],
                        accumNumClaims  [regionNum][stateNum],
                        accumSvcFeeAmt  [regionNum][stateNum],
                        accumWithholdAmt[regionNum][stateNum], 
                        accumClaimsAmt  [regionNum][stateNum]);
            }
        }

        else
            exitStatus = FILE_WRITE_ERROR;
    }
  
    else
        exitStatus = FILE_READ_ERROR;

    if (fclose (pPayUSA) != OK)
		exitStatus = FILE_CLOSE_ERROR;

    if (fclose (pPaySummary) != OK)
		exitStatus = FILE_CLOSE_ERROR;

    return exitStatus;

} // end function summarizePayData


// **************************************************************************
// Name        : separatePayData
//
// Purpose     : Reads amounts to be paid from pay_usa.txt and separates amounts
//               for writing to risk_pay.txt and aso_pay.txt based on group type.
//               Group type is determined by user maintained file aso_list.txt 
//
// Parameters  : None
//
// Return Value: OK                 Success
//               FILE_WRITE_ERROR   Unable to open or create file for writing
//                                  (disk may be full)
//               FILE_READ_ERROR    Unable to open file for reading (may not exist)
//               FILE_CLOSE_ERROR   Unable to close file (disk may be full)
//
// Dependencies: aso_list.txt and pay_usa.txt must exist
//
// Algorithm   : 
// **************************************************************************
int separatePayData (void)
{
    int exitStatus = OK;

    char    region[ABREV_SIZE+1] = "";
    char    grpNum[GRP_NUM_SIZE] = "";
    char       state[ABREV_SIZE] = "";
    char      regionGrp[REGIONS] = "";
	char readLine[PAY_LINE_SIZE] = "";
    long   claimsASO[REGIONS_IL] = {0};
    long  claimsRISK[REGIONS_IL] = {0};
    float    feesASO[REGIONS_IL] = {0};
    float   feesRISK[REGIONS_IL] = {0};
   
    int          regionNum = 0;
    long         numClaims = 0;
    long   totNumClaimsAso = 0;
    long  totNumClaimsRisk = 0;
    float        svcFeeAmt = 0.0;
    float totSvcFeeAmtAso  = 0.0;
    float totSvcFeeAmtRisk = 0.0;
    float      withholdAmt = 0.0;
    float        claimsAmt = 0.0;

    printf ("\n\nSeparating amounts based on group type.");
   
    if ((pPayUSA = fopen ("c:\\deltausa\\PAY_USA.txt", "r")) != NULL)
    {
        if (((pAsoPay = fopen ("c:\\deltausa\\aso_pay.txt", "w")) != NULL) &&
            ((pRiskPay = fopen ("c:\\deltausa\\risk_pay.txt", "w")) != NULL))
        {
            do
            {
                fgets (readLine, PAY_LINE_SIZE, pPayUSA);
                
				sscanf (readLine, " %s %s %s %ld %f %f %f", region, state, grpNum,
                    &numClaims, &svcFeeAmt, &withholdAmt, &claimsAmt);

                strcpy (regionGrp, region);
                strcat (regionGrp, grpNum);

                if ((exitStatus = findGroupRecord (regionGrp)) == GROUP_ALREADY_EXIST)
                {
                    fprintf (pAsoPay, "ASO %s %s %9ld %10.2f %12.2f\n", regionGrp, 
                        state, numClaims, svcFeeAmt, claimsAmt);

                    // summarize data by region, put National IAM in its own region
                    for (regionNum = 0; regionNum < REGIONS_IL; regionNum++)
                    {
    	                if(regionGrp[0] == regionAbrevIL[regionNum][0] &&
	                        regionGrp[1] == regionAbrevIL[regionNum][1])
						{
                            if(regionGrp[2] == '5' && regionGrp[3] == '1' &&
                               regionGrp[4] == '0' && regionGrp[5] == '3') // NATL IAM?
                            {
                                claimsASO[3] += numClaims;
                                  feesASO[3] += svcFeeAmt;
                            }

                            else
                            {
                                claimsASO[regionNum] += numClaims;
                                  feesASO[regionNum] += svcFeeAmt;
                            }

                            break; // exit loop once a match is made
                        }
                    }

                    totNumClaimsAso += numClaims;
                    totSvcFeeAmtAso += svcFeeAmt;
  
                   // break;  // if match found, move on to next record in pPayUSA

                } // end if group in aso list
                  
                // if NOT found in ASO_LIST.txt then it is RISK group      
                else
                {
                    fprintf (pRiskPay, "RISK %s %s %9ld %10.2f %12.2f\n", regionGrp,
                        state, numClaims, svcFeeAmt, claimsAmt);

                    for(regionNum = 0; regionNum < REGIONS_IL; regionNum++)
                    {
                        if(regionGrp[0] == regionAbrevIL[regionNum][0] &&
                           regionGrp[1] == regionAbrevIL[regionNum][1])
                        {
                            claimsRISK[regionNum] += numClaims;
	                          feesRISK[regionNum] += svcFeeAmt;

    	                    break; // exit loop once a match is made
                        }
                    }

                    totNumClaimsRisk += numClaims;
                    totSvcFeeAmtRisk += svcFeeAmt;
                }

            } while (stricmp (region, "XX") != 0);

        } // end if write to pAsoPay and pRiskPay

        else
            exitStatus = FILE_WRITE_ERROR;

    } // end if read pPayUSA
 
    else
        exitStatus = FILE_READ_ERROR;

    exitStatus = printSvcFeesByType (claimsASO, feesASO, claimsRISK, feesRISK, &totNumClaimsAso,
        &totSvcFeeAmtAso, &totNumClaimsRisk, &totSvcFeeAmtRisk);

    if (fclose (pPayUSA) != OK)
		exitStatus = FILE_CLOSE_ERROR;

    if (fclose (pRiskPay) != OK)
		exitStatus = FILE_CLOSE_ERROR;

    if (fclose (pAsoPay) != OK)
		exitStatus = FILE_CLOSE_ERROR;

    return exitStatus;

} // end function separatePayData


