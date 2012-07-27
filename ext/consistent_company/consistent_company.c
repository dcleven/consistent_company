#include "ruby.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


char * TransformCompany(char * inString);
static int IsCompanyWord(char * inWord);
char * trimwhitespace(char *str);
char * str_replace(char *orig, const char *rep, const char *with);

static VALUE rb_ConsistentCompany_Init(VALUE self)
{
	return self;
}


static VALUE rb_CompanyNamer(VALUE self)
{
	char * pSelf = RSTRING_PTR(self);
	int selfLen = (int)strlen(pSelf)+2;
	int workLen = (int)selfLen;
	char * s = pSelf;
	if (*pSelf == '\0')
		return self;
			
	// calc size of work strings
	// while processing we turn & = AND, + = PLUS
	// and we add space at front and back
	while (s = strpbrk(s, "&+"))
	{	
		workLen +=3; // worst case we add 3 chars
		s++;
	}
	workLen += 90;	// add space front and back
	//////////////
	
	// for company only
	int i;
	char ch;
	int asc;
	int numLefts = 0, numRights = 0;
	int left1 = -1, right1 = -1, left2 = -1, right2 = -1;
	char * workString = malloc(workLen);	// 2 extra chars for TransformCompany
	char * returnString = malloc(workLen);
	char * inString;
	strcpy(workString, pSelf);
	inString = workString;
	
	for( i = 0; inString[i]; i++)
		inString[i] = toupper( inString[i] );
	
	inString = trimwhitespace(inString);
	int len = (int)strlen(inString);
	for (i = 0; i < len; i++)
	{
		if (inString[i] == '(')
		{
			numLefts++;
			if (numLefts == 1)
				left1 = i;
			else if (numLefts == 2)
				left2 = i;
		}
		else if (inString[i] == ')')
		{
			numRights++;
			if (numRights == 1)
				right1 = i;
			else if (numRights == 2)
				right2 = i;
		}
	}

	if (numLefts == 0 || inString[0] == '(')
	{ } // Do Nothing
	else if (numLefts == 1)
	{
		if (right1 > left1)
		{
			// ..(xx)..
			inString[left1++] = ' ';
			memmove(&inString[left1], &inString[right1+1], strlen(inString+right1+1)+1);		
		}
		else
			// ..(xx
			inString[left1] = '\0';
	}
	else if (numLefts == 2)
	{
		if ((left1 < right1) && (right1 < left2) && (left2 < right2))
		{
			// ..(xx)..(xx)..
			inString[left1] = ' ';
			memmove(inString + left1 + 1, inString + right1 + 1, left2-right1-1);
			inString[left1+1+left2-right1-1]  = ' ';
			memmove(inString+left1+1+left2-right1, inString + right2+1, strlen(inString+right2+1)+1);
		}
		else if ((left1 < left2) && (left2 < right1) && (right1 < right2))
		{
			// ..(xx(xx)xx)..
			inString[left1] = ' ';
			memmove(inString+left1+1, inString+right2+1, strlen(inString+right2+1)+1);
		}
		else if ((left1 < right1) && (right1 < left2) && (right2 == -1))
		{
			// ..(xx)..(xx
			inString[left1] = ' ';
			memmove(inString+left1+1, inString+right1+1, left2-right1-1);
			inString[left1+1+left2-right1] = '\0';
		}
		else if ((left1 < left2) && (left2 < right1) && (right2 == -1))
		{
			// ..(xx(xx)xx
			inString[left1] = '\0';
		}
		else if ((right1 == -1) && (right2 == -1))
		{
			// ..(xx(xx
			inString[left1] = '\0';
		}
	}
	char singleCharStr[2];
	singleCharStr[1] = '\0';
	returnString[0] = '\0';
	for (i = 0; i < (int)strlen(inString); i++)
	{
		ch = inString[i];
		asc = (int)ch;

		if ((asc >= 65 && asc <= 90) ||
			(asc >= 48 && asc <= 57) ||
			(asc >= 128))  // A-Z, 0-9, and high order chars
		{
			singleCharStr[0] = ch;
			strcat(returnString, singleCharStr);
		}
		else if (asc == 39) // '
		{ }  // Remove it
		else if (asc == 38 && strlen(returnString) > 0) // &
		{
			if (returnString[strlen(returnString)-1] != ' ')
				strcat(returnString, " AND ");
			else
				strcat(returnString, "AND ");
		}
		else if (asc == 43)  // +
		{
			if (strcmp(returnString, "A") == 0 || strcmp(returnString, "A ") == 0)
				strcpy(returnString, "A PLUS ");
			else if (strlen(returnString) > 0)
			{
				if (returnString[strlen(returnString)-1] != ' ')
					strcat(returnString, " AND ");
				else
					strcat(returnString, "AND ");
			}
		}
		else if (strlen(returnString) > 0 &&
			returnString[strlen(returnString)-1] != ' ')
		{
			strcat(returnString, " ");
		}
	}
	
	str_replace(returnString, " AND ", " & ");
	
	returnString = trimwhitespace(returnString);
	returnString = TransformCompany(returnString);
	VALUE return_value = rb_str_new2(trimwhitespace(returnString));
	free(returnString);
	free(workString);
	return return_value;
} 

/*
TransformCompany
given a string transform typical company name parts to common abbreviations
thereby normailizing the name and making exact matching of different names easier
example:
FIRST FEDERAL SAVINGS becomes 1ST FEDERAL SAVINGS 
*/
char * TransformCompany(char * resultString)
{
	// resultString should have been allocated with 2 extra char for our padding here
	char * buf = malloc(strlen(resultString)+30);
	strcpy(buf, " ");
	strcat(buf,resultString);
	strcat(buf, " ");
	strcpy(resultString, buf);
	free(buf);

	char * spaceLoc;
	char * s = resultString;
	str_replace(s, " THE ", " ");
	str_replace(s, " ONE ", " 1 ");
	str_replace(s, " TWO ", " 2 ");
	str_replace(s, " TO ", " 2 ");
	str_replace(s, " THREE ", " 3 ");
	str_replace(s, " FOUR ", " 4 ");
	str_replace(s, " FOR ", " 4 ");
	str_replace(s, " FIVE ", " 5 ");
	str_replace(s, " SIX ", " 6 ");
	str_replace(s, " SEVEN ", " 7 ");
	str_replace(s, " EIGHT ", " 8 ");
	str_replace(s, " NINE ", " 9 ");
	str_replace(s, " TEN ", " 10 ");
	str_replace(s, " ELEVEN ", " 11 ");

	str_replace(s, " FIRST ", " 1ST ");
	str_replace(s, " SECOND ", " 2ND ");
	str_replace(s, " THIRD ", " 3RD ");
	str_replace(s, " FOURTH ", " 4TH ");
	str_replace(s, " FIFTH ", " 5TH ");
	str_replace(s, " SIXTH ", " 6TH ");
	str_replace(s, " SEVENTH ", " 7TH ");
	str_replace(s, " EIGHTH ", " 8TH ");
	str_replace(s, " NINTH ", " 9TH ");
	str_replace(s, " TENTH ", " 10TH ");
	str_replace(s, " CENTRE ", " CTR ");
	str_replace(s, " CENTER ", " CTR ");
	str_replace(s, " CNTR ", " CTR ");
	str_replace(s, " CENT ", " CTR ");
	str_replace(s, " CENTR ", " CTR ");
	str_replace(s, " AUTOMOTIVE ", " AUTO ");
	str_replace(s, " AUTOMOBILE ", " AUTO ");
	str_replace(s, " AUTOS ", " AUTO ");
	str_replace(s, " AVENUE ", " AVE ");
	str_replace(s, " DRIVE ", " DR ");
	str_replace(s, " PHOTOGRAPHY ", " PHOTO ");
	str_replace(s, " BROTHERS ", " BROS ");
	str_replace(s, " TECHNOLOGY ", " TEC ");
	str_replace(s, " TECH ", " TEC ");
	str_replace(s, " TELEVISION ", " TV ");
	str_replace(s, " INFORMATION ", " INFO ");
	str_replace(s, " SOCIETY ", " SOC ");
	str_replace(s, " DEPARTMENT ", " DEPT ");
	str_replace(s, " REGIONAL ", " REG ");
	str_replace(s, " REGION ", " REG ");
	str_replace(s, " AUTHORITY ", " AUTH ");
	str_replace(s, " NATIONAL ", " NATL ");
	str_replace(s, " INTERNATIONAL ", " INT ");
	str_replace(s, " INTERNATION ", " INT ");
	str_replace(s, " INTL ", " INT ");
	str_replace(s, " MARKETING ", " MKT ");
	str_replace(s, " MKTG ", " MKT ");
	str_replace(s, " MANAGEMENT ", " MGT ");
	str_replace(s, " MGMT ", " MGT ");

	s = trimwhitespace(s);	
	spaceLoc = strstr(s, " ");
	//spaceLoc = resultString.IndexOf(" ");
	if (spaceLoc && strlen(s) > 3) // More than one word and more than 3 chars
	{
		// Check for "A" as the first word, and 
		// make sure that second word is not an initital or the word "PLUS"
		// For example: "A C & R" do not remove "A"; "A TOUCH OF CLASS" remove the "A"
		if (strncmp(s, "A ",  2) == 0 &&
			strncmp(s+2, "&", 1) != 0 &&
			strncmp(s+3, " ", 1) != 0 &&
			strstr(s, "PLUS") != s + 2)
		{
			memmove(s, s+2, strlen(s+2)+1);
		}

		spaceLoc = strrchr(s, ' ');
		//spaceLoc = resultString.LastIndexOf(" ");
		if (spaceLoc)  // Look at the last word
		{
			char * lastWord = malloc(strlen(spaceLoc)+1);
			strcpy(lastWord, spaceLoc + 1);
			if (IsCompanyWord(lastWord))
			{
				*spaceLoc = '\0';
				spaceLoc = strrchr(s, ' ');
				if (spaceLoc)  // Look at the new last word
				{
					free(lastWord);
					lastWord = malloc(strlen(spaceLoc)+1);
					strcpy(lastWord, spaceLoc + 1);
					if (IsCompanyWord(lastWord))
					{
						*spaceLoc = '\0';
					}
				}
			}
			free(lastWord);
		}
		if (s[strlen(s)-1] == '&')
			s[strlen(s)-1] = '\0';
	}
	s = str_replace(s, " ", "");
	return s;
}

/*
IsCompanyWord
returns 1 if the null terminated word passed in inWord 
is a typical Company word. Add more company words here if desired.
return 0 if not a Company word
*/
int IsCompanyWord(char * inWord)
{
	if (strcmp(inWord, "ADV") == 0 ||
		strcmp(inWord, "ADVERTISER") == 0 ||
		strcmp(inWord, "ADV") == 0 ||
		strcmp(inWord, "ADVERTISING") == 0 ||
		strcmp(inWord, "AGCY") == 0 ||
		strcmp(inWord, "AGENCY") == 0 ||
		strcmp(inWord, "AGY") == 0 ||
		strcmp(inWord, "ASC") == 0 ||
		strcmp(inWord, "ASS") == 0 ||
		strcmp(inWord, "ASSN") == 0 ||
		strcmp(inWord, "ASSOC") == 0 ||
		strcmp(inWord, "ASSOCIAT") == 0 ||
		strcmp(inWord, "ASSOCIATES") == 0 ||
		strcmp(inWord, "ASSOCIATION") == 0 ||
		strcmp(inWord, "ATTORNEY") == 0 ||
		strcmp(inWord, "ATTRNY") == 0 ||
		strcmp(inWord, "ATTY") == 0 ||
		strcmp(inWord, "ATY") == 0 ||
		strcmp(inWord, "AUTO") == 0 ||
		strcmp(inWord, "CO") == 0 ||
		strcmp(inWord, "COMP") == 0 ||
		strcmp(inWord, "COMPANIES") == 0 ||
		strcmp(inWord, "COMPANY") == 0 ||
		strcmp(inWord, "CORP") == 0 ||
		strcmp(inWord, "CORPORATION") == 0 ||
		strcmp(inWord, "CT") == 0 ||
		strcmp(inWord, "CONTRA") == 0 ||
		strcmp(inWord, "DEPARTMENT") == 0 ||
		strcmp(inWord, "DEPT") == 0 ||
		strcmp(inWord, "DIR") == 0 ||
		strcmp(inWord, "DIRECT") == 0 ||
		strcmp(inWord, "DIV") == 0 ||
		strcmp(inWord, "DIVISION") == 0 ||
		strcmp(inWord, "GROUP") == 0 ||
		strcmp(inWord, "HOLDINGS") == 0 ||
		strcmp(inWord, "INC") == 0 ||
		strcmp(inWord, "INCORPORATED") == 0 ||
		strcmp(inWord, "INT") == 0 ||
		strcmp(inWord, "LIMITED") == 0 ||
		strcmp(inWord, "LLC") == 0 ||
		strcmp(inWord, "LLP") == 0 ||
		strcmp(inWord, "LOCAL") == 0 ||
		strcmp(inWord, "PVT") == 0 ||
		strcmp(inWord, "LTD") == 0 ||
		strcmp(inWord, "PC") == 0 ||
		strcmp(inWord, "PLC") == 0 ||
		strcmp(inWord, "PROD") == 0 ||
		strcmp(inWord, "PRODS") == 0 ||
		strcmp(inWord, "PRODUCT") == 0 ||
		strcmp(inWord, "PRODUCTIONS") == 0 ||
		strcmp(inWord, "PRODUCTS") == 0 ||
		strcmp(inWord, "TR") == 0 ||
		strcmp(inWord, "TRADE") == 0)
		return 1;
	else
		return 0;
}  // IsCompanyWord

/*
Trim whitespace from front and back of string
return the same ptr as received, move the non-whitespace chars
to the front and trim the end with \0
*/
char * trimwhitespace(char *str)
{
	char *end;
	char *start = str;
	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
	{
		*start = '\0';
	  	return start;
	}
	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;
	memmove(start, str, strlen(str)+1);
	
	return start;
}


// !!!! This ONLY works where rep is longer than with
char * str_replace(char *orig, const char *rep, const char *with)
{
	char * s = orig;
	while (s=strstr(s, rep))
	{
		memmove(s, with, strlen(with));
		memmove(s+strlen(with), s+strlen(rep), strlen(s+strlen(rep))+1);
		s = s + strlen(with)-1;
	}
	return orig;
}


void Init_consistent_company()
{
	VALUE rb_mConsistentCompany = rb_define_module("ConsistentCompany");
	VALUE string   				= rb_define_class("String", rb_cObject);
	rb_define_method(rb_mConsistentCompany, "company_namer", rb_CompanyNamer, 0);
	rb_include_module(string, rb_mConsistentCompany);
}
