#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	return -1;
}





// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	return -1;
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {

	int rowIndex = 0;
	int colIndex = 0;

	int row = 0;
	int col = 0;

	int dong = 0;
	int hosu = 0;

	char password[PASSWD_LEN + 1] = { 0, };
	char context[MAX_MSG_SIZE +1] = { 0, };

	char buff[MAX_MSG_SIZE + 1];

	FILE* fp = fopen(filepath, "r");
	if (fp != NULL) {
		// read system's size from backup file
		if (fscanf(fp, "%d %d", &systemSize[0], &systemSize[1]) == EOF) {
			fclose(fp);
			return -1;
		}

		// read master password from backup file
		if (fscanf(fp, "%s", masterPassword) == EOF) {
			fclose(fp);
			return -1;
		}

		// allocate memory of system
		deliverySystem = (storage_t**) malloc(sizeof(storage_t*) * systemSize[0]);

		// iterate all storage
		for (rowIndex = 0 ; rowIndex < systemSize[0] ; ++rowIndex) {
			
			// allocate memory of row
			*(deliverySystem + rowIndex) = (storage_t*) malloc(sizeof(storage_t) * systemSize[1]);

			for (colIndex = 0 ; colIndex < systemSize[1] ; ++colIndex) {

				// set storage's status to empty
				(*(deliverySystem + rowIndex) + colIndex)->cnt = 0;

				// clear password
				memset((*(deliverySystem + row) + col)->passwd, 0, PASSWD_LEN + 1);
			}
		}

		// read storage data until end of line
		while(fscanf(fp, "%d %d %d %d %s %s",
					&row, &col, &dong, &hosu, password, context) != EOF) {

			// fill member variables
			(*(deliverySystem + row) + col)->building = dong;
			(*(deliverySystem + row) + col)->room = hosu;
			(*(deliverySystem + row) + col)->cnt = 1;

			// copy password
			memset((*(deliverySystem + row) + col)->passwd, 0, PASSWD_LEN + 1);
			strncpy((*(deliverySystem + row) + col)->passwd, password, strlen(password));

			// allocate memory of context using length of context from backup file
			(*(deliverySystem + row) + col)->context = (char*) calloc(sizeof(char), strlen(context) + 1);

			// copy context
			strncpy((*(deliverySystem + row) + col)->context, context, strlen(context));

			// increase size of system
			storedCnt += 1;

			// reset buffer
			memset(password, 0, PASSWD_LEN + 1);
			memset(context, 0, MAX_MSG_SIZE + 1);
		}

		// close file pointer
		fclose(fp);
		return 0;
	} else {
		// return -1 if non-exist
		return -1;
	}
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	int rowIndex = 0;
	int colIndex = 0;

	// iterate all storage
	for (rowIndex = 0 ; rowIndex < systemSize[0] ; ++rowIndex) {
		for (colIndex = 0 ; colIndex < systemSize[1] ; ++colIndex) {

			// if filled
			if (str_checkStorage(rowIndex, colIndex) > 0) {

				// release memory of context
				free((*(deliverySystem + rowIndex) + colIndex)->context);
			}
		}

		// release memory of row
		free(*(deliverySystem + rowIndex));
	}

	// release memory of system
	free(deliverySystem);
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	if (x < 0 || x >= systemSize[0])
	{
		return -1;
	}
	
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}

//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	return -1;
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	return -1;
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	return 0;
}