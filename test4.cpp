/* ***********************************************************************
 * test4.cpp
 *
 * Tests implementation of the TicketDeck class.
 *
 * (c) 2011, robert pascual, victoria, tarlac, philippines
 *
 * Revision History:
 *
 * $Id: test4.cpp,v 1.1 2011/12/14 17:26:51 robert Exp $
 *
 * $Log: test4.cpp,v $
 * Revision 1.1  2011/12/14 17:26:51  robert
 * Initial import into CVS.
 *
 *
 *
 * ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "Valids.hpp"
#include "Tickets.hpp"

int main()
{
    TicketDeck *sample = new TicketDeck("data.txt");
    
    int total = sample->get_num_tickets();

    Ticket *ticketp;
    TicketField *fieldpa, *fieldpb, *fieldpc;
    char bufa[64], bufb[64], bufc[64];

    for (int k = 0; k < total; ++k) {
	ticketp = sample->get_ticket(k);
        fieldpa = ticketp->get_named_field("ID NO.");
	fieldpb = ticketp->get_named_field("FNAME");
    	fieldpc = ticketp->get_named_field("GNAME");
	printf("(%s) - %s, %s\n",
		fieldpa->getFieldValueString(bufa),
		fieldpb->getFieldValueString(bufb),
		fieldpc->getFieldValueString(bufc));

    }
}


