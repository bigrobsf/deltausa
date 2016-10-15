// ***************************************************************************
// Copyright (c) Robert Conner 2001
// ***************************************************************************
//
// Module Name     : Miscellaneous Functions
// Descriptive Name: DeltaUSA Miscellaneous Functions
// Purpose         : Contains various functions to check validity of passed data,
//                   process passed strings, or perform utility functions.
// Author Name     : Robert Conner
// Date Created    : January 2001
// Source File     : c:\Program Files\Microsoft Visual Studio\My Projects\
//                   DeltaUSA\DusaMisc.cpp
// Last Update     : R Conner 05/25/01
//
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



// ****************************************************************************
// Name        : validNum
//
// Purpose     : Verify that only numbers are in string
//
// Parameters  : temp        character array    input by user
//               length      integer            passed from calling function
//
// Return Value: OK if a string contains proper chars and is proper
//                     length
//               INVALD_NUM if it contains invalid chars or is an 
//                     invalid length
//
// Dependencies: None
//
// Algorithm   : Counts the valid characters in passed string, if the number 
//               of valid characters equals the passed proper length of the
//               string, then the function returns a flag indicating 'success'.
// ***************************************************************************
int validNum (char temp[], int length)
{
    unsigned int index = 0;
    int num = 0, exitStatus = OK;

    for (index = 0; index < strlen(temp); index++)
        if (isdigit (temp[index]))
            num++;   // counts valid characters

    if (num != length)
        exitStatus = INVALID_NUM;

    return exitStatus;

} // end function validNum


// ****************************************************************************
// Name        : clearString
//
// Purpose     : Clear character array
//
// Parameters  : temp        character array       array to be cleared
//
// Return Value: None
//
// Dependencies: None
//
// Algorithm   : Replaces characters in array with NULL until the end of the
//               array (terminating NULL) is reached
// ****************************************************************************
void clearString (char temp[])
{
    unsigned int index = 0;

    for (index = 0; index < strlen (temp); index++)
        temp[index] = '\0';

    return;

} // end function clearString


// ****************************************************************************
// Name        : miscGetChar
//
// Purpose     : Get a key from the keyboard
//
// Parameters  : None
//
// Return Value: the ASCII value of the last character entered by the user
//
// Dependencies: None
//
// Algorithm   : clears the input buffer, while the last char entered into the
//               keyboard is not a newline (return), get the next char entered,
//               returning only the last key to the calling function.
// ****************************************************************************
int miscGetChar (void)
{
    int keyIn = 0;
    int lastKey = 0;

    fflush (stdin);
    while (keyIn != '\n')
    {
        lastKey = keyIn;
        keyIn = getchar();  // get all the keys from the keyboard buffer,
                            // but return only the last key to the calling 
    }                       // function.

    return (lastKey);

} // end function miscGetChar 


// ****************************************************************************
// Name        : getEnterKey2Continue
//
// Purpose     : Display prompt and ask user to press the Enter Key
//               to continue with the next process.
//
// Parameters  : None
//
// Return Value: None
//
// Dependencies: None
//
// Algorithm   : Display prompt, call input function and wait for the enter key
//               to be depressed, return control to calling function
// ****************************************************************************
void getEnterKey2Continue (void)
{
    int keyIn = 0;

    printf ("\n\nPress Enter Key to Continue");
    keyIn = miscGetChar();

    return;

} // end function getEnterKey2Continue


// ****************************************************************************
// Name        : prompt
//
// Purpose     : Prompt and get string from user
//
// Parameters  : *promptString      char array         Text to prompt user
//               *receivededString  char arrry         String to be used by 
//                                                     calling function as 
//                                                     user response
//               maxLength          unsigned integer   Maximum length of string
//
// Return Value: Length of char array input by user
//
// Dependencies: None
//
// Algorithm   : Display text prompting user for input, clear the input buffer,
//               get input string from user.  Repeat until a number or letter 
//               is entered as the first element in the input array.
//               Determine length of input string,
//               if length of input is not greater than maximum allowed length,
//               copy in the input string to array to be used as user response,
//               if length is greater, truncate input string at maximum length
//               and copy to array to be used as the user reponse.  Return
//               length of passed array to calling function.
// 
// ****************************************************************************
int prompt (char *promptString, char *receivedString, unsigned int maxLength)
{
    char inBuf[BUFFER_SIZE] = {00};
    unsigned int length = 0;

    do
    {
        printf (promptString);
        fflush (stdin);
        gets (inBuf);

	if (!isalnum (inBuf[0]))
            printf ("\a\nThis information is required.");

    } while (!isalnum (inBuf[0]));  // requires entry of at least one 
	                            // number or letter
    length = strlen (inBuf);

    if (length <= maxLength)
        strcpy (receivedString, inBuf);
    else
        strncpy (receivedString, inBuf, length);

    return (strlen (receivedString));

} // end function prompt


// ****************************************************************************
// Name        : allCaps
//
// Purpose     : Converts a Character Array to All Capital Letters
//
// Parameters  : temp            char array      received string
//
// Return Value: None
//
// Dependencies: None
//
// Algorithm   : Individually converts each char in an array to capitals by
//               calling toupper()
// ****************************************************************************
void allCaps (char temp[])
{
	unsigned int index = 0;

	for (index = 0; index < strlen (temp); index++)
		temp[index] = (char) toupper (temp[index]);

	return;

} // end function allCaps


// ****************************************************************************
// Name        : check4GrpNum
//
// Purpose     : Identifies a Candidate Array as a Group Number
//
// Parameters  : testNumArray          char array      received string
//
// Return Value: OK if the array is a group number
//               INVALID_GROUP_NUM is the array is not a mainframe
//                     group number
//
// Dependencies: None
//
// Algorithm   : 
//
// ****************************************************************************
int check4GrpNum (char testNumArray[])
{
    int count = 0, test = FALSE;
    int exitStatus = INVALID_GROUP_NUM;
   
    for (test = FALSE, count = 0; testNumArray[count] != 00; count++)
    {
        if (!isdigit (testNumArray[count]) && !isspace (testNumArray[count]))
            test = TRUE; // assigns TRUE to 'test' if a non-digit char is in
                         // array
    }
    if (test == FALSE && strlen (testNumArray) < GRP_NUM_SIZE
                      && strlen (testNumArray) > 0)
        exitStatus = OK; // found group number

    return exitStatus;

} // end function check4GrpNum


// ****************************************************************************
// Name        : check4State
//
// Purpose     : Identifies a Candidate Array as a State
//
// Parameters  : testArray          char array      received string
//
// Return Value: OK if the array is a two letter state abbreviation
//               INVALID_STATE_ABREV is the array is not a state abrev.
//
// Dependencies: None
//
// Algorithm   : 
//
// ****************************************************************************
int check4State (char testArray[])
{
    int count = 0, test = FALSE;
    int exitStatus = INVALID_STATE_ABREV;

    for (test = FALSE, count = 0; testArray[count] != 00; count++)
    {
        if(!isalpha (testArray[count]))
            test = TRUE; // assigns TRUE to 'test' if a non-alpha char is in
                         // array
    }

    if (test == FALSE && strlen (testArray) == ABREV_SIZE)
        exitStatus = OK; // found potential state
  
    if (!strcmp (testArray,"BY") || !strcmp (testArray,"OF") ||
        !strcmp (testArray,"ST"))
            exitStatus = INVALID_STATE_ABREV; // eliminate non-state two letter
                                              // words
    return exitStatus;

} // end function check4State


// ****************************************************************************
// Name        : validGrpNum
//
// Purpose     : Checks to see if group number is in proper format
//
// Parameters  : testArray          char array      received string
//
// Return Value: OK if the array is valid
//               INVALID_GROUP_NUM if array is not a group number
//
// Dependencies: 
//
// Algorithm   : 
//
// ****************************************************************************
int validGrpNum (char testArray[], unsigned int length)
{
    int exitStatus = OK; 
	unsigned int count = 0;
    unsigned int index = 0;

    if ((length) == strlen (testArray))
    {
        if (validState (testArray) == OK)
            count=+2;

        if (testArray[2] == '-')
            count++;

        if (testArray[3] == '0')
            count++;

        for (index = 4; index < length; index++)
            if (isdigit (testArray[index]))
                count++;   // counts valid characters
    }

    if (count != length)
        exitStatus = INVALID_GROUP_NUM;

    return exitStatus;

} // end function validGrpNum


// ****************************************************************************
// Name        : validState
//
// Purpose     : Checks to see if first two numbers in passed string are a
//               valid DDIC group abstract system state
// Parameters  : testArray          char array      received string
//
// Return Value: OK if the array is valid
//               INVALID_STATE_NUM is the array is invalid
//
// Dependencies: None
//
// Algorithm   : 
//
// ****************************************************************************
int validState (char testArray[])
{
    int exitStatus = INVALID_STATE_NUM;
    unsigned int state = 0;

    char stateNum[NUM_STATES][2] = {
	{'0','1'}, {'0','2'}, {'1','0'}, {'1','1'},
        {'1','4'}, {'1','9'}, {'2','5'}, {'2','7'},
        {'2','9'}, {'4','4'}, {'4','5'}};
    
    for (state = 0; state < NUM_STATES; state++)
    {
	if (stateNum[state][0] == testArray[0] &&
	    stateNum[state][1] == testArray[1])
	{
	    exitStatus = OK;
	    break;
	}
    }

    return exitStatus;
    
} // end function validState


// ****************************************************************************
// Name        : validDate
//
// Purpose     : Checks to see if passed string is a valid date format
//               
// Parameters  : testArray          char array      received string
//
// Return Value: OK if the array is valid
//               INVALID_DATE_FORMAT if not
//
// Dependencies: None
//
// Algorithm   : 
//
// ****************************************************************************
int validDate (char date[], unsigned int length)
{
    int exitStatus = OK;
	unsigned int count = 0; 
    unsigned int index = 0;

    if ((length) == strlen (date))
    {
        if (date[2] == '/')
            count++;

        if (date[5] == '/')
            count++;

        for (index = 0; index < length; index++)
            if (isdigit (date[index]))
                count++;   // counts valid characters
    }

    if (count != length)
        exitStatus = INVALID_DATE_FORMAT;
        
    return exitStatus;

} // end function validDate

            
// **************************************************************************
// Name        : removeCommas
//
// Purpose     : Removes commas from numbers with formatted output
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
void removeCommas (char *inArray, char *outArray)
{
    unsigned int count, index;

    for (count = 0, index = 0; count <= strlen (inArray); count++)
    {
      
        if(inArray[count] != ',') // remove commas from dollar amt 
        {
            outArray[index] = inArray[count];
            index++;
        }
    }

    return;

} // end function removeCommas


// **************************************************************************
// Name        : displayTitle
//
// Purpose     : Advances Screen Text to Proper Location, Displays System
//               Header
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
void displayTitle (void)
{
    system ("cls");
    printf  ("\n            ");
    printf ("D E L T A  D E N T A L  I N S U R A N C E  C O M P A N Y");
    printf ("\n\n                         ");
    printf ("DeltaUSA Fee Reporting System");
    printf ("\n\n                    ");
    printf ("        Copyright (c) DDIC 2001");  
    printf ("\n\n\n                   ");

    return;

} // end function displayTitle
            
            
// **************************************************************************
// Name        : errorProcess
// 
// Purpose     : Prints the error message associated with the error code
//               assigned to exitStatus variable
//
// Parameters  : errorCode      integer      code received by calling function
//
// Return Value: Zero, unless received errorCode = NO_MEM (20)
//
//
// Dependencies: None
//
// Algorithm   : 
//
// **************************************************************************
int errorProcess (int errorCode)
{
    int index = 0;

    ERROR_TABLE	errorTable[] = { 
		{NULL_STRING, "Empty string."},
		{INVALID_INPUT_PARAMETER, "Invalid input parameters."},
		{NO_MEM, "Out of memory."},
        {PRINTER_ERROR, "Error writing to print device."},
        {INVALID_NUM, "Invalid number."},
        {CONVERSION_ERROR, "Error converting abstract grp num to mainframe."},

		{INVALID_GROUP_NUM, "Invalid group number.  Please use format: xx-0xxxx"},
        {INVALID_STATE_NUM, "Invalid state number."},
        {INVALID_STATE_ABREV, "Invalid state abbreviation."},
        {INVALID_INPUT_PARAMETER, "Invalid input parameter."},
        {INVALID_GROUP_RECORD, "Invalid group record."},

        {GROUP_ALREADY_EXIST, "Group number already in ASO list."},
        {GROUP_NOT_FOUND, "Group was not found in ASO list."},
        
        {DATE_MISMATCH_ERROR, "Entered date does not match file date."},
        {INVALID_DATE_FORMAT, "Entered date must have format: mm/dd/yyyy"},

        {FILE_WRITE_ERROR, "Unable to write to file."},
        {FILE_READ_ERROR, "Unable to read from file."},
        {FILE_OPEN_ERROR, "Unable to open file."},
        {FILE_APPEND_ERROR, "Unable to append or create file."},
        {FILE_DELETE_ERROR, "Unable to delete file."},
        {FILE_BACKUP_ERROR, "Unable to backup file."},
		{FILE_CLOSE_ERROR, "Unable to close file.  Verify that disk is present and not full."},

		{BAD_DATA_RECORD, "Bad data record. Possible damaged data file."},
		{INVALID_RECORD_HEADER, "Bad record header. Possible damaged file."},
		{INVALID_MENU_CHOICE, "Invalid menu choice.  Please try again."},
		{INVALID_FILE_PATH, "Invalid path to mainframe files."},
		{PROC_SECTION_NOT_FOUND, "Cannot find section to process."},
		{0, ""}};

    if (errorCode == QUIT)
        errorCode = 0;   // do not display message for normal quit

    if (errorCode != 0)
    {
        while ((errorTable[index].errorCode != 0) && 
               (errorTable[index].errorCode != errorCode))
            index++;

        if (errorTable[index].errorCode == errorCode)
            printf("\a\nError %i:  %s\n", errorTable[index].errorCode,
                errorTable[index].message);
        else if (errorCode != 0)
            printf("\aError %i: NO ERROR MESSAGE FOUND FOR THIS ERROR CODE.\n",
             errorCode);
			
        if (errorCode != NO_MEM) 
            errorCode = 0;
            
	getEnterKey2Continue();
    }

    return (errorCode);

} // end function errorProcess
