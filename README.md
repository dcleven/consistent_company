# consistent_company

A Ruby C Extension that takes a Company Name string and replaces it with a normalized compressed version.
The new name is not meant for human consumption, but for comparing other like names to find matches.
This was written with the Adverting domain in mind. So many of the common advertiser/agency abbreviations are
handled well. Although it can work with other domains you may find need to enhance the specific company name 
identifiers in IsCompanyName and TransformCompany.

Tested with over 600,000 names from 100's of sources where an overlap of names is expected with a match rate of 40%.

## Install:
gem install consistent_company

## Usage:
A company_namer method is added to the string class.

	>> require 'consistent_company'
	=> true
	>> 'First Federal Savings'.company_namer
	=> '1STFEDERALSAVINGS'

## Development:


## CompanyNamer Function Transformations
This function performs a sequence of changes to its input parameter, which should be the name of an advertiser or agency.

1.	It transforms all alpha characters to upper case.

2.	It loops through the string, finding and tracking open and close parenthesis characters.
If it finds any of the following patterns, the parentheses and the text contained within them is removed 
and replaced with a single space. 
The “..” strings indicate other characters not within the parentheses; the “xx” characters represent any characters within the parentheses.
All other patterns of parenthesis characters are left intact.
..(xx)..	..(xx	..(xx)..(xx)..	..(xx(xx)xx)..	..(xx)..(xx	..(xx(xx)xx	..(xx(xx

3.	It loops through the string performing character-level transformations:

	a.	Characters in the ranges A-Z, 0-9, and high-order characters (ASCII value 128 or greater) are kept  unchanged.

	b.	Single tic-marks (‘) are removed

	c.	An ampersand character (&) in the middle of the name is converted to “AND”, with surrounding spaces as they were

	d.	A plus sign (+) is converted to “PLUS” if it is preceded only by “A” (taking account of spacing); otherwise it is converted to “AND”

	e.	All other characters are converted to spaces

	f.	All the above transformations ensure that there are no multiple spaces in the result

4.	It replaces all instances of “ AND ” (the word “and” surrounded by spaces) with “ & ”, and removes leading and trailing spaces.

5.	It passes the string through TransformCompany and then returns the result.

## TransformCompany Function Transformations
This function transforms elements of the company name at the word level, 
removing some and making variants into a common form. 
It also changes commonly used long words into shorter versions, to speed the hashing process.

1.	It removes the word “THE”

2.	It transforms the first eleven number words into numeric form: “THREE” becomes “3”

3.	It transforms the first ten ordinals into numeric form: “SIXTH” becomes “6TH”

4.	It transforms common words to a common abbreviation. "CENTER" becomes CTR

5.	Leading and trailing spaces are removed

6.	If the string has more than one word and more than three characters:

	a.	If the leading word is “A” and the second word is neither an initial or “PLUS”, remove the leading “A”.Example: “A B & X” keeps its “A”, as does “A PLUS”, but “A TOUCH OF CLASS” does not.
	
	b. If the last word in the name is a “company word” as defined by IsCompanyWord, then remove it and repeat the check and removal on the resulting last word. 
	
	c.	If the last character in the name is now “&”, remove it

7.	Remove all spaces from the name and return .

## IsCompanyWord Function
The following words are recognized as commonly-used in company names without adding to the distinguishing characteristics of the name.

ADV, ADVERTISER, ADV, ADVERTISING, AGCY, AGENCY, AGY, ASC, ASS, ASSN, ASSOC, ASSOCIAT, ASSOCIATES, ASSOCIATION, ATTORNEY, ATTRNY, ATTY, ATY, AUTO, CO, COMP, COMPANIES, COMPANY, CORP, CORPORATION, CT, CONTRA, DEPARTMENT, DEPT, DIR, DIRECT, DIV, DIVISION, GROUP, HOLDINGS, INC, INCORPORATED, INT, LIMITED, LLC, LLP, LOCAL, LTD, PC, PLC, PROD, PRODS, PRODUCT, PRODUCTIONS, PRODUCTS, TR, TRADE

== Contributing to consistent_company
 
* Check out the latest master to make sure the feature hasn't been implemented or the bug hasn't been fixed yet
* Check out the issue tracker to make sure someone already hasn't requested it and/or contributed it
* Fork the project
* Start a feature/bugfix branch
* Commit and push until you are happy with your contribution
* Make sure to add tests for it. This is important so I don't break it in a future version unintentionally.
* Please try not to mess with the Rakefile, version, or history. If you want to have your own version, or is otherwise necessary, that is fine, but please isolate to its own commit so I can cherry-pick around it.

== Copyright

Copyright (c) 2011 Doug Cleven. See LICENSE.txt for
further details.

