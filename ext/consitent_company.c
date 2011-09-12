#include "ruby.h"
#include "pair.h"
#include <ctype.h>
#include "common.h"
#include <string.h>
#include <stdlib.h>


char * TransformCompany(char * inString);
static int IsCompanyWord(char * inWord);
char *trimwhitespace(char *str);
char *str_replace(char *orig, const char *rep, const char *with);

static VALUE rb_CompanyNamer_Init(VALUE self)
{
	return self;
}

//char * BaseString(char * inString)
static VALUE rb_MakeName(VALUE self, VALUE string)
{
	char * inString = RSTRING_PTR(string);
	
	// for company only
	int i;
	char ch;
	int asc;
	int numLefts = 0, numRights = 0;
	int left1 = -1, right1 = -1, left2 = -1, right2 = -1;
	char returnString[500]  = "";

	// inString = inString.ToUpper().Trim();
	for( i = 0; inString[i]; i++)
		inString[i] = toupper( inString[i] );
	
	inString = trimwhitespace(inString);
	unsigned long len = strlen(inString);
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
			strcpy(&inString[left1], &inString[right1+1]);		
			
			//inString = inString.Substring(0, left1) + " " + inString.Substring(right1 + 1);
		}
		else
			// ..(xx
			inString[left1] = '\0';
			//inString = inString.Substring(0, left1);
	}
	else if (numLefts == 2)
	{
		if ((left1 < right1) && (right1 < left2) && (left2 < right2))
		{
			// ..(xx)..(xx)..
			inString[left1] = ' ';
			strncpy(inString + left1 + 1, inString + right1 + 1, left2-right1-1);
			inString[left2] = ' ';
			strcpy(inString+left2+1, inString + right2+1);
			//inString = inString.Substring(0, left1) + " " +
			//	inString.Substring(right1 + 1, left2 - right1 - 1) + " " +
			//	inString.Substring(right2 + 1);
		}
		else if ((left1 < left2) && (left2 < right1) && (right1 < right2))
		{
			// ..(xx(xx)xx)..
			inString[left1] = ' ';
			strcpy(inString+left1+1, inString+right2+1);
			//inString = inString.Substring(0, left1) + " " + inString.Substring(right2 + 1);
		}
		else if ((left1 < right1) && (right1 < left2) && (right2 == -1))
		{
			// ..(xx)..(xx
			inString[left1] = ' ';
			strncpy(inString+left1+1, inString+right1+1, left2-right1-1);
			//inString = inString.Substring(0, left1) + " " +
			//	inString.Substring(right1 + 1, left2 - right1 - 1);
		}
		else if ((left1 < left2) && (left2 < right1) && (right2 == -1))
		{
			// ..(xx(xx)xx
			inString[left1] = '\0';
			//inString = inString.Substring(0, left1);
		}
		else if ((right1 == -1) && (right2 == -1))
		{
			// ..(xx(xx
			inString[left1] = '\0';
			//inString = inString.Substring(0, left1);
		}
	}
	char singleCharStr[2];
	singleCharStr[1] = '\0';
	returnString[0] = '\0';
	for (i = 0; i < strlen(inString); i++)
	{
		ch = inString[i];
		asc = (int)ch;

		if ((asc >= 65 && asc <= 90) ||
			(asc >= 48 && asc <= 57) ||
			(asc >= 128))  // A-Z, 0-9, and high order chars
		{
			singleCharStr[0] = ch;
			strcat(returnString, singleCharStr);
			//returnString = returnString + ch;
		}
		else if (asc == 39) // '
		{ }  // Remove it
		else if (asc == 38 && strlen(returnString) > 0) // &
		{
			if (returnString[strlen(returnString)-1] != ' ')
				strcat(returnString, " AND ");
			else
				strcat(returnString, "AND ");
			//if (returnString.Substring(returnString.Length - 1) != " ")
			//	returnString = returnString + " AND ";
			//else
			//	returnString = returnString + "AND ";
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

			//if ((returnString == "A") || (returnString == "A "))
			//	returnString = "A PLUS ";
			//else if (returnString.Length > 0)
			//{
			//	if (returnString.Substring(returnString.Length - 1) != " ")
			//		returnString = returnString + " AND ";
			//	else
			//		returnString = returnString + "AND ";
			//}
		}
		else if (strlen(returnString) > 0 &&
			returnString[strlen(returnString)-1] != ' ')
		{
			strcat(returnString, " ");
		}
		//else if ((returnString.Length > 0) &&
		//	(returnString.Substring(returnString.Length - 1) != " "))
		//	returnString = returnString + " ";
	}
	char * p;
	str_replace(returnString, " AND ", " & ");
	trimwhitespace(returnString);
	//returnString = returnString.Replace(" AND ", " & ").Trim();
	
	strcpy(returnString, TransformCompany(returnString));
	
	return rb_str_new2(trimwhitespace(returnString));

}  // BaseString

char * TransformCompany(char * resultString)
{
	char buf[500] = " ";
	strcat(buf,resultString);
	strcat(buf, " ");
	strcpy(resultString, buf);
	char lastWord[500] = "";
	char * spaceLoc;
	char * s = resultString;

	//StringBuilder sb = new StringBuilder(resultString);
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
	str_replace(s, " AUTOMOTIVE ", " AUTO ");
	str_replace(s, " AUTOMOBILE ", " AUTO ");
	str_replace(s, " AUTOS ", " AUTO ");
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
	str_replace(s, " MARKETING ", " MKTG ");
	str_replace(s, " MANAGEMENT ", " MGT ");
	str_replace(s, " MGMT ", " MGT ");

	s = trimwhitespace(s);
	spaceLoc = strstr(s, " ");
	//spaceLoc = resultString.IndexOf(" ");
	if (spaceLoc && strlen(resultString) > 3)
	//if (spaceLoc >= 0 && resultString.Length > 3)  // More than one word and more than 3 chars
	{
		// Check for "A" as the first word, and 
		// make sure that second word is not an initital or the word "PLUS"
		// For example: "A C & R" do not remove "A"; "A TOUCH OF CLASS" remove the "A"
		if (strncmp(resultString, "A ",  3) == 0 &&
			strncmp(resultString+2, "&", 1) != 0 &&
			strncmp(resultString+3, " ", 1) != 0 &&
			strstr(resultString, "PLUS") != resultString + 2)
		{
			strcpy(resultString, resultString+2);
		}
		//if (resultString.Substring(0, 2) == "A " &&
		//	resultString.Substring(2, 1) != "&" &&
		//	resultString.Substring(3, 1) != " " &&
		//	resultString.IndexOf("PLUS") != 2)
		//	resultString = resultString.Substring(2);

		spaceLoc = strrchr(resultString, ' ');
		//spaceLoc = resultString.LastIndexOf(" ");
		if (spaceLoc)  // Look at the last word
		{
			strcpy(lastWord, spaceLoc + 1);
			//lastWord = resultString.Substring(spaceLoc + 1);
			if (IsCompanyWord(lastWord))
			{
				*spaceLoc = '\0';
				//resultString = resultString.Substring(0, spaceLoc);
				spaceLoc = strrchr(resultString, ' ');
				//spaceLoc = resultString.LastIndexOf(" ");
				if (spaceLoc)  // Look at the new last word
				{
					strcpy(lastWord, spaceLoc + 1);
					//lastWord = resultString.Substring(spaceLoc + 1);
					if (IsCompanyWord(lastWord))
					{
						*spaceLoc = '\0';
						//resultString = resultString.Substring(0, spaceLoc);
					}
				}
			}
		}
		if (resultString[strlen(resultString)-1] == '&')
			resultString[strlen(resultString)-1] = '\0';
		//if (resultString.Substring(resultString.Length - 1) == "&")
		//	resultString = resultString.Substring(0, resultString.Length - 1);
	}

	str_replace(resultString, " ", "");
	return resultString;
}

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


char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

// !!!! This ONLY works where rep is longer than with
char *str_replace(char *orig, const char *rep, const char *with)
{
	char * s = orig;
	while (s=strstr(s, rep))
	{
		strncpy(s, with, strlen(with));
		strcpy(s+strlen(with), s+strlen(rep));
		s = s + strlen(with);
	}
	return orig;
}


void Init_consistent_company()
{
    rb_require("consistent_company/version");
    rb_mConsistentCompany = rb_define_module("ConsistentCompany");

	rb_cCompanyNamer = rb_define_class_under(rb_mConsistentCompany, "CompanyNamer", rb_cObject);
	rb_define_method(rb_cCompanyNamer, "initialize", rb_CompanyNamer_Init, 0);
	rb_define_method(rb_cCompanyNamer, "make", rb_MakeName, 1);
}