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
	FILE * input;
	FILE * output;
	regex_t nameRegex;
	char nameRegStr[] = "^[A-Z][a-z]{1,49}+\n?$";
	char a[18], b[18];
	char *e;
	long num1=0, num2=0, addResult=-1, multiplyResult=-1;
	int validRegex1 = 1;
	int validRegex2 = 1;
	char numRegStr[] = "^([0-9])+\n?$\0";
	int c;
	regex_t numRegex;
	/*regex_t inputFileRegex;
	regex_t outputFileRegex;*/
	int valid = 1; /*regex validity is backward -> 0 is good, 1 is bad*/	


/**********NAMES**************/
	bzero(firstname, BUFFERSIZE);
	bzero(lastname, BUFFERSIZE);


	/*CREATE/COMPILE REGEX*/
	regcomp(&nameRegex, nameRegStr, REG_EXTENDED|REG_NOSUB);

	while(valid == 1)
	{
		/*GET INPUT*/
		bzero(firstname,51);
		printf("Enter First Name (capital letter followed by lowercase letters: ");
		fgets(firstname,BUFFERSIZE,stdin);
		

		if(strlen(firstname) < 49)
			printf("Press Enter...\n");
		while((c = getchar()) != '\n' && c != EOF){}
	
		/*CHECK AGAINST THE REGEX*/
		/*printf("Your name is: %s\n",firstname);*/	
		valid = regexec(&nameRegex, firstname, (size_t)0, NULL, 0);
		printf("0 = match, 1 = no match: %d\n",valid);
	}
	valid = 1;
	while(valid == 1)
	{
		/*GET INPUT*/
		bzero(lastname,51);
		printf("Enter Last Name (capital letter followed by lowercase letters: ");
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

	password();

	while(addResult == -1 || multiplyResult == -1)
	{
		validRegex1 = 1;
		validRegex2 = 1;

		while(validRegex1 == 1)
		{
			
			validRegex1 = 1;
		
			regcomp(&numRegex, numRegStr, REG_EXTENDED|REG_NOSUB);

			/* get input */
			bzero(a, 18);

			printf("Enter the first number: ");
			fgets(a, 18, stdin);
			if(strlen(a) < 17)
			{
				printf("Press Enter...\n\n");
			}
			/*https://stackoverflow.com/questions/2187474/i-am-not-able-to-flush-stdin*/
			while ((c = getchar()) != '\n' && c != EOF){}

			/*check against the regex */
			printf("Num 1 is: %s\n\n", a);
			validRegex1 = regexec(&numRegex, a, (size_t)0, NULL, 0);
			printf("0 = match, 1 = no match: %d\n\n", validRegex1);
		}

		while(validRegex2 == 1)
		{
			
			validRegex2 = 1;

			regcomp(&numRegex, numRegStr, REG_EXTENDED|REG_NOSUB);
			bzero(b, 18);

			printf("Enter the second number: ");
			fgets(b, 18, stdin);
			if(strlen(b) < 17)
				printf("Press Enter...\n\n");
			while ((c = getchar()) != '\n' && c != EOF){}

			printf("Num 2 is: %s\n\n", b);
			validRegex2 = regexec(&numRegex, b, (size_t)0, NULL, 0);
			printf("0 = match, 1 = no match: %d\n\n", validRegex2);
		}

		num1 = strtol(a, &e, 10);
		num2 = strtol(b, &e, 10);

		addResult = safeAdd(num1, num2);
		multiplyResult = safeMultiply(num1, num2);

		if(addResult == -1)
		{
			printf("Integer Overflow in addition\n\n");
		}

		if(multiplyResult == -1)
		{
			printf("Integer Overflow in multiplication\n\n");
		}

		printf("addition result is: %ld\n\n", addResult);
		printf("multiplication result is: %ld\n\n", multiplyResult);
	}
	regfree(&numRegex);

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

void password(){
    char temp[200] = "";
    char salt[] = "h4ck3r5";
    char password[100];
    char password2[100];
    unsigned long fromFile = 0;
    unsigned long check = 0;
    int c;
    FILE *fp;

    printf("Enter a password: ");

    fgets(password, 99, stdin);
	if(strlen(password) < 98)
	{
	printf("Press Enter...");
	}
	while ((c = getchar()) != '\n' && c != EOF){}

    strcat(temp, salt);
    strcat(temp, password);;
    
    
    fp = fopen("password.txt", "w");
    fprintf(fp, "%lu", hash(temp));
    fclose(fp);
    fp = fopen("password.txt", "r");
    fscanf(fp, "%lu", &fromFile);
    printf("Re-enter your password: ");
    fgets(password2, 99, stdin);
	if(strlen(password2) < 98)
	{
		printf("Press Enter...");
	}
	while ((c = getchar()) != '\n' && c != EOF){}


    bzero(temp, 200);
    strcat(temp, salt);
    strcat(temp, password2);
    check = hash(temp);
    while(check != fromFile){
        printf("Re-enter your password: ");
        fgets(password2, 99, stdin);
	if(strlen(password2) < 98)
	{
		printf("Press Enter...");
	}
	while ((c = getchar()) != '\n' && c != EOF){}
        bzero(temp, 200);
        strcat(temp, salt);
        strcat(temp, password2);
        check = hash(temp);
    }
	printf("Correct Password.\n");
}
