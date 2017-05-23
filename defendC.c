#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <stdio.h>
#include <unistd.h>
#include <limits.h>

#define BUFFERSIZE 51

/*Functions*/
char * prompt_and_read(const char * prompt);
int validextension(char type, char * file);
unsigned long hash(char *str);
long safeMultiply(long a, long b);
long safeAdd(long a, long b);
void password();


int main()
{

	char * pathnameInput;
	char * pathnameOutput;
	int theBool = 0;
	char firstname [BUFFERSIZE];
	char lastname [BUFFERSIZE];
	char a[19], b[19];
	char *e;
	long num1, num2;
	long addResult;
	long multiplyResult;
	FILE * input;
	FILE * output;
	regex_t nameRegex;
	char nameRegStr[] = "^[A-Z][a-z]{1,49}$\0";
	/*regex_t numRegex;
	regex_t inputFileRegex;
	regex_t outputFileRegex;*/
	int c;
	int valid = 1; /*regex validity is backward -> 0 is good, 1 is bad*/	


/**********NAMES**************/
	bzero(firstname, BUFFERSIZE);
	bzero(lastname, BUFFERSIZE);

	/*
	printf("Enter First Name: ");
	fgets(firstname, BUFFERSIZE, stdin);

	if(firstname[BUFFERSIZE-1] != '\0')
	{
		printf("Name too long");
		while((c = getchar()) != '\n' && c != EOF);
	}
	*/

	/*CREATE/COMPILE REGEX*/
	regcomp(&nameRegex, nameRegStr, REG_EXTENDED|REG_NOSUB);

	while(valid == 1)
	{
		/*GET INPUT*/
		bzero(firstname,51);
		printf("Enter First Name (capital letter followed by lowercase letters: ");
		fgets(firstname,50,stdin);
		

		if(strlen(firstname) < 49)
			printf("Press Enter...\n");
		while((c = getchar()) != '\n' && c != EOF){}
	
		/*CHECK AGAINST THE REGEX*/
		/*printf("Your name is: %s\n",firstname);*/	
		valid = regexec(&nameRegex, firstname, (size_t)0, NULL, 0);
		/*printf("0 = match, 1 = no match: %d\n",valid);*/
	}
	valid = 1;
	while(valid == 1)
	{
		/*GET INPUT*/
		bzero(lastname,51);
		printf("Enter First Name (capital letter followed by lowercase letters: ");
		fgets(lastname,50,stdin);
		

		if(strlen(lastname) < 49)
			printf("Press Enter...\n");
		while((c = getchar()) != '\n' && c != EOF){}
	
		/*CHECK AGAINST THE REGEX*/
		/*printf("Your name is: %s\n",lastname);*/	
		valid = regexec(&nameRegex, lastname, (size_t)0, NULL, 0);
		/*printf("0 = match, 1 = no match: %d\n",valid);*/
	}

	/*
	printf("Enter Last Name: ");
	fgets(lastname, BUFFERSIZE, stdin);
	*/


	/*FREE THE REGEX*/
	regfree(&nameRegex);
	printf("Enter the first number: ");
	fgets(a, 19, stdin);
	printf("Enter in the second number: ");
	fgets(b, 19, stdin); /*19 is the max amount of digits that a long can be */


	/*converts the two string numbers into longs */
	num1 = strtol(a, &e, 10);
	num2 = strtol(b, &e, 10);

	password();

	addResult = safeAdd(num1, num2);
	multiplyResult = safeMultiply(num1, num2);
	while(addResult == -1)
	{
		printf("Invalid addition, Integer Overflow.\n");
		printf("Please enter in valid numbers");
		printf("Enter the first number: ");
		fgets(a, 19, stdin);
		printf("Enter in the second number: ");
		fgets(b, 19, stdin); /*19 is the max amount of digits that a long can be */

		num1 = strtol(a, &e, 10);
		num2 = strtol(b, &e, 10);

		addResult = safeAdd(num1, num2);
	}

	while(multiplyResult == -1)
	{
		printf("Invalid addition, Integer Overflow.\n");
		printf("Please enter in valid numbers");
		printf("Enter the first number: ");
		fgets(a, 19, stdin);
		printf("Enter in the second number: ");
		fgets(b, 19, stdin); /*19 is the max amount of digits that a long can be */

		num1 = strtol(a, &e, 10);
		num2 = strtol(b, &e, 10);

		multiplyResult = safeMultiply(num1, num2);
	}


	while(theBool != 1)
	{
		pathnameInput = prompt_and_read("Enter a input file name, must be a local path: ");
		theBool = validextension('i',pathnameInput);
		input = fopen(pathnameInput,(void*)'r');

		if(!input)
			theBool = 0;		
	}/*end of while*/

	theBool = 0;
	while(theBool != 1)
	{
		pathnameOutput = prompt_and_read("Enter a output file name, must be a local path: ");
		theBool = validextension('o',pathnameOutput);
		output = fopen(pathnameInput,(void*)'w');
		if(!output)
			theBool = 0;
	}

	return 1;
}/*end of main*/

/********************FUNCTIONS***********************/

char * prompt_and_read(const char * prompt)
{
	char * response;
	char * bufsize;

	printf("%s", prompt);
	asprintf(&bufsize, "%%%us", BUFSIZ);

	if((response = malloc(BUFSIZ + 1)) == NULL)
	{
		fprintf(stderr,"out of memory\n");
		exit(1);
	}
	scanf(bufsize, response);
	free(bufsize);
	return response;
}/*end of prompt_and_read*/

int validextension(char type, char * file)
{
	char * validFile = NULL;
	printf("file inside the extension %s\n",file);
	validFile = strstr(file,".txt");

	if(validFile == NULL)
	{
		printf("can't find the txt extension.\n");
		return 0;
	}
	else
	{
		validFile = strstr(file,"/");
		/*printf("found the txt\n");*/
		printf("this is inside the return from / %s\n",validFile);
	
		if(validFile == NULL)
		{

			if((access( file, F_OK ) != -1) && type == 'o')
			{
				printf("File already exist or is a directory\n");
				return 0;
			}/*end of if file exists*/

			/*file doesn't exist*/
			return 1;
		}
		else
		{
			printf("got path the / \n");
			return 0;
		}
	}/*end of else*/
	return 1;
}


/*https://stackoverflow.com/questions/7666509/hash-function-for-string*/
unsigned long hash(char *str)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}


long safeAdd(long a, long b)
{
	if(a > 0 && b > LONG_MAX - a)
	{
		return -1;
	}
	else if(a < 0 && b < LONG_MIN - a)
	{
		return -1;
	}
	return a + b;
}

long safeMultiply(long a, long b)
{
	if(a > LONG_MAX / b)
	{
		return -1;
	}
	else if((a < LONG_MIN / b))
	{
		return -1;
	}

	else if((a == -1) && (b == INT_MIN))
	{
		return -1;
	}

	else if((b == -1) && (a == INT_MIN))
	{
		return -1;
	}
	/*else*/
	return a * b;
}

void password()
{
	char temp[200] = "";
	char salt[] = "h4ck3r5";
	char password[100];
	char password2[100];
	unsigned long fromFile = 0;
	unsigned long check;
	FILE *fp;

	printf("Enter a password: ");
	fgets(password, 99, stdin);
	strcat(temp, salt);
	strcat(temp, password);

	fp = fopen("password.txt", "w");
	fprintf(fp, "%lu", hash(temp));
	fclose(fp);
	fp = fopen("password.txt", "r");
	fscanf(fp, "%lu", &fromFile);
	printf("Re-enter your password: ");
	fgets(password2, 99, stdin);
	bzero(temp, 200);
	strcat(temp, salt);
	strcat(temp, password2);

	check = hash(temp);
	while(check != fromFile)
	{
		printf("Re-enter your password: ");
		fgets(password2, 99, stdin);
		bzero(temp, 200);
		strcat(temp, salt);
		strcat(temp, password2);
		check = hash(temp);
	}
}

