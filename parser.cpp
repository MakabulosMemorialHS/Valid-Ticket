/* ***********************************************************************
 * parser.cpp
 *
 * This is the parser module.
 *
 * (c) 2011, robert pascual, victoria, tarlac, philippines
 *
 * Revision History:
 *
 * $Id: parser.cpp,v 1.3 2011/12/14 19:45:25 robert Exp $
 *
 * $Log: parser.cpp,v $
 * Revision 1.3  2011/12/14 19:45:25  robert
 * Minor change only. Define'd out the main() function so I
 * can use this module with outer test modules with their own
 * main() functions.
 *
 * Revision 1.2  2011/12/12 11:20:34  robert
 * Among other things, I made changes to reflect that Valids.hpp and
 * Valids.cpp have been modified and many of their functionality moved
 * to Tickets.hpp and Ticket.cpp.
 *
 * Revision 1.1.1.1  2011/11/30 16:51:51  robert
 * Initial import into CVS.
 *
 *
 *
 * ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "Tickets.hpp"

namespace VALIDS {
    enum PARSERSTATE {
	START = 0, READY, RECOGNIZED, HALT
    };
}


// The next function returns a positive integer if c is a delimiter.
// The accepted delimiters are given by the following defined string.
//
//
#define DELIMITERS "\t"

int is_a_field_delimiter(char c)
{
    const char *dlptr = DELIMITERS;

    while (*dlptr != 0x00) {
	if (c == *dlptr++) return 1;
    }
    return 0;
}

// Given a line, this function will extract the fields from
// that line and place them in the array of pointers to char field_values.
// The line should be in tab delimited format.
//
char **get_field_values(char *inbuffer, char **field_items)
{
    char colname[TICKETS::MAXNAMELEN];
    char *cnpointer;
    char *readpointer;
    char c;
    char *newstring;


    // First, we make sure inbuffer does not have a '\r' char
    // by scanning inbuffer and changing the '\r' into a null.
    //
    int buflen = strlen(inbuffer);
    for (int k = buflen-1; k >= 0; --k) {
	if (inbuffer[k] == '\r') {
	    inbuffer[k] = 0x00;
	}
    }

#if (0)
    // Maybe I should remove the following routine because
    // the code calling this function knows better if field_items
    // is properly initialized and its size. The routine below
    // could lead to subtle bugs if field_items is not exactly equal to
    // TICKETS::MAXFIELDS.
    //
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
	field_items[k] = 0x00;
    }
#endif

    readpointer = inbuffer;   // Point to beginning of string to process.

    int column_count = 0;
    cnpointer = colname;
    VALIDS::PARSERSTATE state = VALIDS::START;
    while (1) {
	switch(state) {
	    case VALIDS::START:
                cnpointer = colname;
		*cnpointer = 0x00;         // Clear the buffer.
		state = VALIDS::READY;
		break;
	    case VALIDS::READY:
		c = *readpointer++;      // Read a character
		if (is_a_field_delimiter(c)) { // Check if delimiter
                    state = VALIDS::RECOGNIZED;
		    *cnpointer = 0x00;
		}
		else if (c == 0x00 || c == '\n') {
		    state = VALIDS::HALT;
		    *cnpointer = 0x00;
		}
		else {
		    *cnpointer++ = c;
		    *cnpointer = 0x00;
		}
		break;
	    case VALIDS::RECOGNIZED:
	    case VALIDS::HALT:
                newstring = new char[TICKETS::MAXNAMELEN];
                field_items[column_count] = newstring;
                *cnpointer = 0x00;
		strcpy(newstring, colname);
		column_count++;
		if (state != VALIDS::HALT) {
		    state=VALIDS::START;
		}
		else return field_items;
		break;
	}
#ifdef DEBUG_MODE	
	printf ("state = %d, colname = %s\n", state, colname);
#endif
    }
}


// Get the names of the fields from the tab-delimited file.
// Make sure that **field_items is at least large enough to handle
// the maximum possible number of fields. For safety, use TICKETS::MAXFIELDS
// as the size for field_items.
//
char **get_fieldnames(char *inbf, char **fields)
{
    return get_field_values(inbf, fields);
}



#if(0)
int main()
{
    char inbuffer[512];
    char *headers[512];
    char *items[512];

    // Initialize the headers and items arrays.
    //
    for (int k = 0; k < 512; ++k) {
	headers[k] = 0x00;
	items[k] = 0x00;
    }

    fgets(inbuffer, 512, stdin);
    get_fieldnames(inbuffer,headers);
    for (int k = 0; k < 512 && headers[k] != 0x00; ++k)
        printf("%d %s\n", k, headers[k]);
    while (fgets(inbuffer,512,stdin) != NULL) {
	get_field_values(inbuffer, items);
	for (int k = 0; k < 512 && items[k] != 0x00; ++k) {
	    printf("%d %s\n", k, items[k]);
	}
    }
}

#endif

