/*  CT609 NUIG Week 8 Code
*
*  DO NOT SHARE THIS CODE
*  Julian Fitzgibbon 30 NOV 2018
*
*  Purpose:
*  1. Search directory specified by TRANSACTIONS_LOANOFFERS_PATH for all valid loan offer files
*  2. Stores the filenames in dynamic memory.
*  3. Extracts the loanrequest and loanoffer timestamps of each file and stores in dynamic memory.
*  4. Prints contents of dynamic memory loanoffer structures.
*
*  Prequisites/Requirements:
*  Installation of the Dirent api
*
*
*  Sample Usage in main()
*  Copy the Loan Offer Files provided by your facilitator to TRANSACTIONS_LOANOFFERS_PATH
*  Execute Code
**************************************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dirent.h"

//Location of Loan Offer Files
#define TRANSACTIONS_LOANOFFERS_PATH "c:\\transactions\\loanoffers\\"
//Length of Loan offer file
#define VALID_LOANOFFERFILE_LENGTH 31
#define SUFFIX_LENGTH 10
#define VALID_LOANREQUEST_TIMESTAMP_LENGTH 10
#define VALID_LOANOFFER_TIMESTAMP_LENGTH 10

//Let our dynamic memory  grow by 10 records  at a time
#define GROWBY 10

//File Validtion
//Let's define some types of errors in the FileType (includes timestamp errors)
typedef enum { INVALID_UNKNOWN, INVALID_DOT, INVALID_LENGTH, INVALID_SUFFIX, INVALID_NONNUMERIC, INVALID_FUTURETIME, VALID } FileType;


//ToDo Loan Request structure 


//Loanoffers structure 
//Contains Timestamps from loanofferfile
//
//ToDo: You will need to add the other 
//loan offer attributes such as char senderAddress[ADDRESS_LENGTH], offerrate and status here...
//
typedef struct
{
	time_t requesttimestamp;
	time_t offertimestamp;
} Loanoffers;

//Function Declarations
//Returns a count and a list of Valid files
char ** find_valid_transaction_files(int * confirmed_transactions_count);

//Checks a filename is a valid loan offer file
FileType check_valid_filename(const char *filename, char *pszValidloanofferfile);

//Loads the timestamps from (filecount) loan offer files (in validfilelist) to memory structure for loan offer records  (loanoffers)
int loadloanofferfilelist(char **validfilelist, int filecount, Loanoffers **loanoffers, int *lo_recordnumber);


//Converts a text timestamp to a time_t and a readable time.
time_t convert_timestamp(char *filetimestamp, char *bufferdisplaytime);

//Extracts the RequestLoan timestamp from the filename
void extract_requestloan_timestamp(const char *filename, char *requestloantimestamp);

//Extracts the Loanoffer timestamp from the filename
void extract_loanoffer_timestamp(const char *filename, char *loanoffertimestamp);

//Print (loanoffercount) Loan Offer records from Array of Structures (loanoffers) out!  
void printloanoffers(Loanoffers **loanoffers, int loanoffercount);

//get time in a readable format
void getDisplayTime(time_t filetimestamp_t, char * buffer);

//check for timestamp validity
int isValidTimestamp(char *timestamptocheck);

//Valid File Suffix
char suffix[] = "_Offer.txt";


//
// An outline of the loading of the data is provided for you here in advance
// The below code works for loading the loanoffer file names but doesn't open them. 
// You will need to alter main to load each record in the loan offer data. (See ToDo) 
// And write the function to load the Loan requests yourself!
//
int main()
{
	int validfile_count = 0; //Number of valid files
	int numOfferrecords = 0;
	char **validfilelist; //Pointer to memory buffer to hold valid loan offer files
	//where the list of valid loan offer files will be kept. 

	int loadstatus = 0;

	//TODO
	//Load the Loan Request Data from the loanrequests.txt file
	//into an array of loan requests structure 
	//

	/*
	* Let's create an array, Loanoffersrecords,
	* of pointers to 20 Loan Offer Records
	*
	*/
	Loanoffers *Loanoffersrecords[20];
	//Lets start off with 20 and we can reallocate if need more.

	//
	//Calling the find_valid_transaction_files function
	//This will tell us the number of loanoffers files (count)
	//and store their names in validfilelist address
	//
	validfilelist = find_valid_transaction_files(&validfile_count);
	printf("\n\n***Summary Report***\nThere were %d Valid loan offer Files found in %s\n", validfile_count, TRANSACTIONS_LOANOFFERS_PATH);

	//Now that we have a list of the loan offer filenames. 
	//Lets store the timestamps from the filenames in the loanoffersrecords structure
	//
	//You could alter the code
	//to make sure that loan offers that do not have a
	//(corresponding) Loan Request in the loan requests file are not loaded
	//
	loadstatus = loadloanofferfilelist(validfilelist, validfile_count, Loanoffersrecords, &numOfferrecords);


	if (loadstatus == 0 || numOfferrecords < 0)
	{
		puts("Fatal error encountered! Couldn't load Loan Offers");
		return;
	}
	else
	{
		//Found Some loan offers - let's print them!
		printf("%d Loan Request records successfully loaded\n", numOfferrecords);
		printloanoffers(Loanoffersrecords, numOfferrecords);
	}

	getchar();

}

/**************************************************************************************************
* Function Name:	 printloanoffers
*
*  Paramaters:
*
*
*  Purpose:
*					 prints (filecount) loan offer records
*					stored in memory structure for loan offer records (loanoffers)
**************************************************************************************************/
void printloanoffers(Loanoffers **loanoffers, int loanoffercount)
{
	time_t currentrequestloan_t;
	time_t currentoffer_t;
	time_t previousrequestloan_t;
	char request_timedisplaybuffer[26];
	char offer_timedisplaybuffer[26];

	previousrequestloan_t = time(NULL);

	printf("Loan Offer Structure\n");
	for (int j = 0; j < loanoffercount; j++)
	{
		currentrequestloan_t = (*loanoffers)[j].requesttimestamp;

		if (currentrequestloan_t != previousrequestloan_t)
		{
			memset(request_timedisplaybuffer, '\0', (26 * sizeof(char)));
			getDisplayTime(currentrequestloan_t, request_timedisplaybuffer);
			//printf("----Loan Request Time: %lld ---\n", currentrequestloan_t);
			printf("----Sent Loan Request at: %s ---\n", request_timedisplaybuffer);
		}

		previousrequestloan_t = currentrequestloan_t;

		currentoffer_t = (*loanoffers)[j].offertimestamp;
		getDisplayTime(currentoffer_t, offer_timedisplaybuffer);

		//printf("Offer Time: %lld \n", currentoffer_t);
		printf("--Recieved Loan Offer at: %s--\n", offer_timedisplaybuffer);

	}

}


/**************************************************************************************************
* Function Name:	 loadloanofferfilelist
*
*  Paramaters:
**
*  Returns:			 int - indicates success or failure
*
*  Purpose:
*					 Loads the timestamps from (filecount) loan offer files
*					(in validfilelist) to memory structure for loan offer records  (loanoffers)
**************************************************************************************************/
int loadloanofferfilelist(char **validfilelist, int filecount, Loanoffers **loanoffers, int *lo_recordnumber)
{

	char loanrequest_timestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH + 1];
	char loanoffer_timestamp[VALID_LOANOFFER_TIMESTAMP_LENGTH + 1];
	time_t loanrequest_t;
	time_t loanoffer_t;
	char request_bufferdisplaytime[26];
	char offer_bufferdisplaytime[26];

	// Allocate some memory for the Loan Offers struct (GROWBY is #defined at top of program.)
	*loanoffers = malloc(sizeof(Loanoffers)* GROWBY);


	for (int j = 0; j < filecount; j++)
	{
		//printf("LoanOffer FileName %d of %d was: %s\n", j + 1, filecount, validfilelist[j]);

		//Extract and Convert Loan Request Timestamp
		extract_requestloan_timestamp(validfilelist[j], loanrequest_timestamp);
		loanrequest_t = convert_timestamp(loanrequest_timestamp, request_bufferdisplaytime);

		//
		//Skiping Validation
		//You could make sure the loan request exists in the Request structure
		//before you load the loan offer record.
		//

		//Extract and Convert Loan Offer Timestamp
		extract_loanoffer_timestamp(validfilelist[j], loanoffer_timestamp);
		loanoffer_t = convert_timestamp(loanoffer_timestamp, offer_bufferdisplaytime);

		//Store the timestamp data
		(*loanoffers)[*lo_recordnumber].requesttimestamp = loanrequest_t;
		(*loanoffers)[*lo_recordnumber].offertimestamp = loanoffer_t;

		//ToDo
		//Now you neeed to open the filename validfilelist[j] 
		//to obtain the data for the loan offer
		//
		//printf("Opening Loanoffer FileName %d of %d: %s\n", j + 1, filecount, validfilelist[j]);


		//Next Record
		(*lo_recordnumber)++;

	}
	return 1;
}

/**************************************************************************************************
* Function Name:	 find_valid_transaction_files
*  Paramaters:		 int * confirmed_transactions_count - number of confirmed transactions
**
*  Returns:			 char** - pointer to block of memory that contains list of valid loanoffer files
*
*  Purpose:			 Obtain valid loan offer files - for loading into memory
**************************************************************************************************/
char ** find_valid_transaction_files(int * confirmed_transactions_count)
{
	//char *transactions_conf[1000];
	char currentfile[200];
	char validloanofferfile[VALID_LOANOFFERFILE_LENGTH + 1];
	int lineno = 0;

	char **file_loanofferfiles; //pointer to an array of file loanofferfiles
	/*
	* Let's make some space  in memory for each of the Pointers that will point
	* to the valid file_loanofferfiles we have obtained.
	*
	* We request space to hold a  block of <addresses> (pointers to characters - not "char" but "char *")
	*
	* GROWBY Pointers for all the Lines should be enough!
	*
	*/
	file_loanofferfiles = (char**)malloc(sizeof(char*)* GROWBY);


	//This code uses the dirent api to traverse the directory
	//You will need to install it so that it works.
	//
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(TRANSACTIONS_LOANOFFERS_PATH)) != NULL)
	{
		/* Get all the files and directories within TRANSACTIONS_LOANOFFERS_PATH */
		while ((ent = readdir(dir)) != NULL)
		{
			strcpy(currentfile, ent->d_name);
			printf("\n---Searching Directory: Current File is %s---\n\n", currentfile);
			if (check_valid_filename(currentfile, validloanofferfile) == VALID)
			{
				//printf("Valid loanofferfile is %s\n", validloanofferfile);
				file_loanofferfiles[lineno] = (char *)malloc(sizeof(char)* VALID_LOANOFFERFILE_LENGTH + 1);

				if (file_loanofferfiles[lineno] == NULL)
				{
					printf("Error allocating memory");
					getchar();
					exit(EXIT_FAILURE);
				}
				else
				{
					//Copy the data
					//printf("Allocated Memory!\n");
					//strcpy(file_loanofferfiles[lineno], validloanofferfile);

					strcpy(file_loanofferfiles[lineno], currentfile);
					printf("---Saving: Current File is %s---\n\n", currentfile);


					lineno++;
				}

				/*
				* Every time we reach a multiple of GROWBY we increase the
				*
				* size of the array by GROWBY, so we can handle
				*
				* arbitrary sized files, only bounded by our memory size
				*
				*/

				if ((lineno % GROWBY) == 0)

				{
					/*
					*If we have done GROWBY lines then we need more memory!
					*
					*/
					file_loanofferfiles = (char**)realloc(file_loanofferfiles, sizeof(char*)* (GROWBY + lineno));
					//printf("Reallocated more memory when at line %d\n", lineno);
					getchar();
				}

			}


		}
		closedir(dir);
		*confirmed_transactions_count = lineno;
		return file_loanofferfiles;
	}
	else {
		/* could not open directory */
		perror("could not open directory");
		exit(EXIT_FAILURE);
	}
}

/*
* Function Name: extract_requestloan_timestamp
* Purpose: Extract the 10 chars that represent the  loanrequest timestamp
* Paramaters: char * Filename
*			  char *requestloantimestamp - address of string to store loan offertimestamp
*
* Example: 1544021094_1544024694_Offer.txt
* Total length VALID_LOANREQUEST_TIMESTAMP_LENGTH & "_" VALID_LOANOFFER_TIMESTAMP_LENGTH + strlen(_Offer.txt)+ \0
* Places 1544021094 to paramater requestloantimestamp
*/
void extract_requestloan_timestamp(const char *filename, char *requestloantimestamp)
{
	strncpy(requestloantimestamp, filename, VALID_LOANREQUEST_TIMESTAMP_LENGTH);
	requestloantimestamp[VALID_LOANREQUEST_TIMESTAMP_LENGTH] = '\0';
	//printf("RequestLoan timestamp Extracted is: %s\n", requestloantimestamp);
}

/*
* Function Name : extract_loanoffer_timestampPurpose : Extract the 10 chars that represent the loanoffer timestamp
* Paramater: Valid loanofferFilename, address of string to store loan offertimestamp
*
* Example: 1544021094_1544024694_Offer.txt
* Total length VALID_LOANREQUEST_TIMESTAMP_LENGTH & "_" VALID_LOANOFFER_TIMESTAMP_LENGTH + strlen(_Offer.txt)+ \0
* Returns 1544024694 in loanoffertimestamp
*/
void extract_loanoffer_timestamp(const char *filename, char *loanoffertimestamp)
{
	char temploanofferfilename[VALID_LOANOFFERFILE_LENGTH + 1];
	strcpy(temploanofferfilename, filename);
	char * extractdash;
	extractdash = strtok(&temploanofferfilename[VALID_LOANOFFER_TIMESTAMP_LENGTH + 1], "_");

	if (extractdash != NULL)
	{
		//strncpy(loanoffertimestamp, extractdash, VALID_LOANOFFER_TIMESTAMP_LENGTH);
		strcpy(loanoffertimestamp, extractdash);
		loanoffertimestamp[VALID_LOANOFFER_TIMESTAMP_LENGTH] = '\0';
		//printf("Loanoffer timestamp extracted: %s\n\n", loanoffertimestamp);
	}

}

/*
* Function Name: isValidTimestamp
* Purpose: Checks the timestamp is numeric
* Paramaters:
*			  char *timestamptocheck - timestamp
*
*
*/
int isValidTimestamp(char *timestamptocheck)
{
	int valid_timestamp;
	//time_t filetimestamp_t;
	char buffer[26];
	//time_t now_t;

	//printf("\nisValidTimestamp: timestamptocheck for validity: %s\n", timestamptocheck);

	//Timestamps must be numeric
	for (int i = 0; i < (int)strlen(timestamptocheck) - 1; i++)
	{
		if (isdigit(timestamptocheck[i]))
			continue;
		else
		{
			valid_timestamp = INVALID_NONNUMERIC;
			return valid_timestamp; //bad loanofferfile
		}

	}

	//Could be useful to exclude files with future timestamps.
	//Let's exclude for now
	//
	//now_t = time(NULL) + 10;
	/*if (filetimestamp_t > now_t)
	{
	printf("\nERROR: Invalid TIMESTAMP %lld is in the future: %s\n", filetimestamp_t, buffer);
	valid_timestamp = INVALID_FUTURETIME;
	}
	else
	{
	*/
	//printf("%lld is a valid time:  %s\n", filetimestamp_t, buffer);
	valid_timestamp = VALID;
	//}
	return valid_timestamp;
}

// Format timestamp for display
void getDisplayTime(time_t filetimestamp_t, char * buffer)
{

	char mybuffer[26];
	struct tm *tm_info;

	tm_info = localtime(&filetimestamp_t);

	strftime(mybuffer, sizeof(mybuffer), "%Y-%m-%d %H:%M:%S", tm_info);

	//printf("Time converted to time_t: %lld and String Time: %s\n", filetimestamp_t, mybuffer);
	strcpy(buffer, mybuffer);

}

time_t convert_timestamp(char *timestamptocheck, char *bufferdisplaytime)
{

	long long filetimestamp_l;
	time_t filetimestamp_t;
	char buffer[26];
	struct tm *tm_info;

	//Convert Time
	filetimestamp_l = strtoll(timestamptocheck, NULL, 10);
	filetimestamp_t = (time_t)filetimestamp_l;

	// Format timestamp for display
	tm_info = localtime(&filetimestamp_t);
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
	strcpy(bufferdisplaytime, buffer);

	//printf("Time converted to time_t: %lld and String Time: %s\n", filetimestamp_t, bufferdisplaytime);
	return filetimestamp_t;
}

/* Function Name: check_valid_filename
*  Paramaters:	  const char * filename - name of file to be validated
*				  char * pszValidloanofferfile - address of valid loanofferfile data to be updated if found
*
*  Returns:		Enum FileType - value indicates validity of filename
*
*  Purpose:
*  Given a filename checks it's a valid loanoffer transaction.
*  If yes updates pszValidloanofferfile with the loanofferfile component of the file.
*
*
**************************************************************************************************/
FileType check_valid_filename(const char *filename, char *pszValidloanofferfile)
{
	FileType valid_filename = INVALID_UNKNOWN;
	int filename_length;
	char *end_location;
	//char loanofferfile[VALID_LOANOFFERFILE_LENGTH + 1];
	const char *dot = strrchr(filename, '.');

	char *requestloantimestamp;
	char *loanoffertimestamp;

	//Simple invalid case 
	if (!dot || dot == filename) {
		valid_filename = INVALID_DOT;
		return valid_filename;
	}

	//
	//Check Filename length is that of a loanofferfile file?
	// Sample: 1544021094_1544024694_Offer.txt
	// 31 characters or VALID_LOANOFFERFILE_LENGTH
	else
	{
		filename_length = strlen(filename);
		//printf("Filename validation: Length is %d\n", filename_length);
		if (filename_length != VALID_LOANOFFERFILE_LENGTH)
		{
			valid_filename = INVALID_LENGTH;
			return valid_filename;
		}
	}

	//
	//Check for the proper Suffix "_Offer.txt"
	//
	//printf("Looking for %s in %s \n", suffix, filename);
	end_location = strstr(filename, suffix);
	if (end_location == NULL)
	{
		//printf("File Suffix not found in file %s\n", filename);
		valid_filename = INVALID_SUFFIX;
		return valid_filename;
	}
	//
	// Valid File Suffix found!
	// Now Check Timestamps in File Name are valid
	// 

	else
	{

		//printf("Found %s in %s \n", suffix, filename);

		//Extract Timestamps from File Name
		// 1544021094_1544024694_Offer.txt
		// 1st part is Request loan timestamp
		// 2nd part is loan offer  timestamp 
		//
		// 1st part: Extract Request loan timestamp
		// Extract the 10 chars that represent the loanrequest timestamp
		//
		requestloantimestamp = (char *)malloc(sizeof(char)*(VALID_LOANREQUEST_TIMESTAMP_LENGTH + 1));
		if (requestloantimestamp == NULL) printf("Error gettting memory\n");
		memset(requestloantimestamp, '\0', (VALID_LOANREQUEST_TIMESTAMP_LENGTH + 1 * sizeof(char)));
		extract_requestloan_timestamp(filename, requestloantimestamp);
		//printf("Request Loan Timeststamp extracted is %s \n", requestloantimestamp);
		//Check it's numeric and not in the future!
		valid_filename = isValidTimestamp(requestloantimestamp);

		if (valid_filename == VALID)
		{
			//printf("Valid timestamp found %s On to part II\n\n", requestloantimestamp);

			// 2nd part: Extract loan offer timestamp
			// Extract the 10 chars that represent the loan offer timestamp
			loanoffertimestamp = (char *)malloc(sizeof(char)*(VALID_LOANOFFER_TIMESTAMP_LENGTH + 1));
			if (loanoffertimestamp == NULL) printf("Error gettting memory\n");
			memset(loanoffertimestamp, '\0', sizeof(char)* (VALID_LOANOFFER_TIMESTAMP_LENGTH + 1));
			extract_loanoffer_timestamp(filename, loanoffertimestamp);
			//printf("Loan Offer Timeststamp extracted is %s\n ", loanoffertimestamp);

			//Check it's numeric and not in the future!
			valid_filename = isValidTimestamp(loanoffertimestamp);

		} //End Get Loan offer timestamp
	}//End Valid File Suffix



	return valid_filename;
}//End Fucntion