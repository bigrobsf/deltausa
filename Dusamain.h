//****************************************************************************
// Copyright (c) Robert Conner 2001
//****************************************************************************
// DeltaUSA Main Header File
//****************************************************************************

// general purpose definitions

#define REGIONS        7   // seven processing regions
#define REGIONS_IL     8   // seven processing regions plus IL for IAM 5103
#define STATES         43  // all non-DDIC states
#define NUM_STATES     11  // number of DDIC states:
                           // 01, 02, 10, 11, 14, 19, 25, 27, 29, 44, 45
#define WORD_SIZE      12  // size of word string read from dataset files
#define PHRASE_SIZE    10  // size of phrase string read from dataset files
#define LINE_SIZE      132 // number of characters to read in per line of dataset file
#define PAY_LINE_SIZE  57  // number of characters to read in per line of pay_usa file
#define PATH_SIZE      30  // size of file path string

#define GROUP_START    24  // column group number begins in dataset
#define STATE_START    33  // column state abrev. begins in dataset
#define PHRASE_START   57  // column phrase GROUP FOR begins in dataset

#define GRP_NUM_SIZE   5   // mainframe group numbers must be smaller than this
#define GRP_NUM_LEN    8   // abstract system group numbers must be this length
#define REG_GRP_LEN    6   // region + mainframe group number max length with
#define ABREV_SIZE     2   // state / region abbreviation size 
#define LONG_DATE      10  // mm/dd/yyyy 
#define SHORT_DATE     8   // mm/dd/yy 
#define BUFFER_SIZE    256
#define MAX_LINES_LIST 18  // number of lines displayed per screen for ASO list
#define MAX_LINES      22  // number of lines displayed per screen
#define ST_CHAR_LOC1   12  // location in filePath of 1st char of state abbrev
#define ST_CHAR_LOC2   13  // location in filePath of 2nd char of state abbrev

// process control flags

#define OK                       0
#define QUIT                     1
#define FALSE                    0
#define TRUE                     1


// error codes

#define NO_MEM                    10
#define PRINTER_ERROR             15
#define NULL_STRING               20
#define BAD_DATA_RECORD           30
#define INVALID_RECORD_HEADER     40
#define INVALID_MENU_CHOICE       50
#define INVALID_INPUT_PARAMETER   60
#define PROC_SECTION_NOT_FOUND    65
#define CONVERSION_ERROR          70
#define DATE_MISMATCH_ERROR       80
#define INVALID_DATE_FORMAT       90

#define FILE_WRITE_ERROR          100
#define FILE_READ_ERROR           101
#define FILE_OPEN_ERROR           102
#define FILE_APPEND_ERROR         103
#define FILE_DELETE_ERROR         104
#define FILE_BACKUP_ERROR         105
#define FILE_CLOSE_ERROR          106
#define INVALID_FILE_PATH         107

#define INVALID_GROUP_NUM         200
#define INVALID_STATE_NUM         201
#define INVALID_STATE_ABREV       202
#define INVALID_NUM               203
#define INVALID_REGION_NUM        204

#define GROUP_ALREADY_EXIST       300
#define GROUP_NOT_FOUND           301
#define INVALID_GROUP_RECORD      302


// *******************************************************************
// structures
// *******************************************************************

// error code table
struct error_table
{
    int	errorCode;
    char *message;
};

typedef struct error_table ERROR_TABLE;


// *******************************************************************
// function prototypes
// *******************************************************************

// functions from DUSAmain.c
int  maintainAsoList (void);
int  selectReport (void);
int  selectHelpFunction (void);
void displayMainMenu (void);
void displayAsoListMenu (void);
void displayHelpMenu (void);
void displayReportMenu (void);



// functions from DUSAmisc.c

int  validNum (char temp[], int length);
void clearString (char temp[]);
int  miscGetChar (void);
void getEnterKey2Continue (void);
int  prompt (char *promptString, char *receivedString, unsigned int maxLength);
void clearScreen (void);
void allCaps (char temp[]);
int  check4GrpNum (char testNumArray[]);
int  check4State (char testArray[]);
int  validGrpNum (char testArray[], unsigned int length);
int  validState (char testArray[]);
int  validDate (char date[], unsigned int length);
int  errorProcess (int errorCode);
void removeCommas (char *inArrayIn, char *outArray);
void displayTitle (void);


// functions from DUSAproc.c

int  addNewGroup (void);
int  deleteGroup (void);
int  processMainframeData (void);
int  inputDateRange (char *fromDate, char *thruDate);

