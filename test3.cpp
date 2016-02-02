/* ***********************************************************************
 * test3.cpp
 *
 * Exercises the functions in Valids.cpp
 *
 * (c) 2011, robert pascual, victoria, tarlac, philippines
 *
 * Revision History:
 *
 * $Id: test3.cpp,v 1.1 2011/12/07 04:56:37 robert Exp $
 *
 * $Log: test3.cpp,v $
 * Revision 1.1  2011/12/07 04:56:37  robert
 * Initial import.
 *
 *
 * ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "Valids.hpp"

int main()
{
    char inbuffer[512];
    char *headers[512];
    char *items[512];
    Ticket *tickets_array[512];   // I know there are less than this number
                                  // of students.



    // Initialize the headers and items arrays.
    //
    for (int k = 0; k < 512; ++k) {
	headers[k] = 0x00;
	items[k] = 0x00;
    }


    // First we get the headers.
    fgets(inbuffer, 512, stdin);
    get_fieldnames(inbuffer,headers);

    // Now we create the Tickets.
    //
    int tcount = 0;
    while (fgets(inbuffer, 512, stdin) != NULL) {
        Ticket *temp = new Ticket(headers, inbuffer);
	tickets_array[tcount++] = temp;
    }
    tickets_array[tcount] = 0x00;   // Terminate with a sentinel value.

    printf("Done with creating the ticket array.\n");


    // Now we look for all GNAME, FNAME, and STATUS Fields and dump them.
    // We also find the field with index 0 and dump its contents.
    //
    TicketField *a, *b, *c, *d;
    char as[VALIDS::MAXFIELDLEN], 
	 at[VALIDS::MAXFIELDLEN],
	 ar[VALIDS::MAXFIELDLEN],
	 aq[VALIDS::MAXFIELDLEN];
        
    for (int k = 0; k < tcount; ++k) {
        a = tickets_array[k]->get_named_field("GNAME");
        b = tickets_array[k]->get_named_field("FNAME");
        c = tickets_array[k]->get_named_field("STATUS");
        d = tickets_array[k]->get_indexed_field(0);

	printf("(%s) %s, %s --- %s\n",
		d->getFieldValueString(ar),
		b->getFieldValueString(as),
		a->getFieldValueString(at),
		c->getFieldValueString(aq));
    }

}


