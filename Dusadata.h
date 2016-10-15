//****************************************************************************
// Copyright (c) Robert Conner 2001
//****************************************************************************
// DeltaUSA Data Header File
//****************************************************************************

// general purpose definitions

#define FILE_PATH       "c:\\deltausa\\"
#define AL_PATH         "c:\\deltausa\\alwthold.txt"
#define FL_PATH         "c:\\deltausa\\flwthold.txt"
#define GA_PATH         "c:\\deltausa\\gawthold.txt"
#define LA_PATH         "c:\\deltausa\\lawthold.txt"
#define MS_PATH         "c:\\deltausa\\mswthold.txt"
#define TX_PATH         "c:\\deltausa\\txwthold.txt"
#define UT_PATH         "c:\\deltausa\\utwthold.txt"


// *******************************************************************
// structures
// *******************************************************************

// group
struct group
{
    char mainframeNum[REG_GRP_LEN + 1];
    char absGroupNum[GRP_NUM_LEN + 1];
    group *next;
};

typedef struct group GROUP;


// group info
struct groupInfo
{
    char mainframeNum[REG_GRP_LEN + 1];
    char absGroupNum[GRP_NUM_LEN + 1];
};

typedef struct groupInfo GROUP_INFO;


// structure for data read from mainframe files for calculation
struct mainframeData
{
    long numSvcFees;
    long numWithholds;
    long numClaims;
    double svcFeeAmt;
    double withholdAmt;
    double claimsAmt;
};

typedef struct mainframeData FILE_DATA;


// *********************************************************************
// function prototypes
// *********************************************************************

// functions from dusadatalist.cpp

int  getFirstGroup (void);
int  getNextGroup (void);
int  getLastGroup (void);
int  findGroupRecord (char *mainframeNum);
int  appendGroupRecord (GROUP *newGroup);
int  createGroupRecord (char *mainframeNum);
int  updateGroupRecord (char *mainframeNum, char *newGrpNum);
int  removeGroupRecord (void);
int  getGroupInfo (GROUP_INFO *groupInfo);
int  initASOListBuffer (GROUP_INFO *listBuffer);
void removeAllGroupRecords (void);
int  copyAsoList2GrpDir (void);
int  writeAsoList2File (void);
int  backupAsoList (FILE *pAsoListFile);
int  displayAsoList (void);
int  showAsoList (void);
int  printAsoList (void);
int  buildASOPrintArray (int *groupCount);
int  compareGroupNum (const void *arg1, const void *arg2);
void convert2MainFrame (char *newGrpNum, char *mainFrameNum);
void initDataBuffer (FILE_DATA *structIn);


// functions from dusadatafiles.cpp

int  compareDateRanges (char *fromDate, char *thruDate);
int  readDateRange (char *fromDate, char *thruDate);
int  readWriteFiles (void);
int  skipToNeededSection (void);
int  readWriteData (char stateChar1, char stateChar2, char readLine[],
	 FILE_DATA *totals, FILE_DATA *ddicState, FILE_DATA *payState);
int  summarizePayData (void);
int  separatePayData (void);

 
// functions from dusaprint.cpp

int  printTitle (FILE *);
int  printHelp (char *helpPath, char *helpFile, FILE *pOutDevice);
int  displayText (char fileName[], int *userChoice, FILE *pOutDevice);
int  printReport (char fileName[]);
int  printRegionSummaries (char pathName[], int regionNum, FILE_DATA *totals,
     FILE_DATA *ddicState, FILE_DATA *payState);
int  printSvcFeesByType (long claimsASO[], float feesASO[], long claimsRISK[],
     float feesRISK[], long *claimTotASO, float *feeTotASO,
     long *claimTotRISK, float *feeTotRISK);
