/* ***********************************************************************
 * test2.cp
 *
 * Exercises the functions in Valids.cpp
 *
 * (c) 2011, robert pascual, victoria, tarlac, philippines
 *
 * Revision History:
 *
 * $Id: test2.cpp,v 1.1 2011/12/07 04:56:37 robert Exp $
 *
 * $Log: test2.cpp,v $
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

    // Now we dump the contents of the tickets.
    //
    //
    int numf;
    Ticket *pHolder;              // Ticket place holder.
    TicketField *tempTF;
    char ax[64], bx[64];


    for (int k = 0; k < tcount; ++k) {
	pHolder = tickets_array[k];
        numf = pHolder->getNumFields();
	for (int j = 0; j < numf;++j) {
            tempTF = pHolder->get_field(j);
	    printf("%s : %s\n",
		        tempTF->getFieldName(ax), 
			tempTF->getFieldValueString(bx));
	}
	printf("\n\n");
    }
}


