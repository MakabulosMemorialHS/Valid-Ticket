/* **********************************************************************
 * Tickets.cpp
 *
 * Implementation of the classes declared in Tickets.hpp.
 *
 * Revision History
 *
 * $Id: Tickets.cpp,v 1.3 2011/12/15 13:50:06 robert Exp $
 *
 * $Log: Tickets.cpp,v $
 * Revision 1.3  2011/12/15 13:50:06  robert
 * -- Added function is_target_ticket().
 * -- replaced all strcmp() with strcasecmp().
 *
 * Revision 1.2  2011/12/14 19:34:50  robert
 * Made dozens of unnamed improvements.
 *
 * Revision 1.1  2011/12/12 11:04:51  robert
 * Initial import into CVS. These versions are the original
 * Valids.hpp and Valids.cpp files. Those files were copied
 * to Tickets.hpp and Tickets.cpp because the Tickets
 * functionality is general and could be useable in other
 * contexts.
 *
 *
 *
 ***********************************************************************/
#include <stdio.h>
#include <string.h>
#include "Tickets.hpp"

// Create a ticket from the given headers and the
// data from inbuf. inbuf is assumed to be a tab delimited
// list of items.
// 
Ticket::Ticket(char **headers, char *inbuf)
{
    char *fieldnames[TICKETS::MAXFIELDS];

    // First we properly initialize fieldnames and fieldArray.
    //
    next_available = 0;
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
	fieldnames[k] = 0x00;
	fieldArray[k] = 0x00;
    }

    get_field_values(inbuf, fieldnames);

    /* Assert: fieldnames is no longer empty. */


    for (int k = 0; headers[k] != 0x00 && k < TICKETS::MAXFIELDS; ++k) {
	if (fieldnames[k] == 0x00) break;
        TicketField *temp = new TicketField(headers[k], fieldnames[k]);
        this->addField(temp);
    }
	        
    // Destroy the memory pointed to by the elements of
    // fieldnames. Prevents memory leakages.
    //
    for (int k = 0; fieldnames[k] != 0x00 && k < TICKETS::MAXFIELDS; ++k) {
	delete fieldnames[k];
    }
}

Ticket::~Ticket(void)
{
    for (int k = 0; k < next_available; ++k) {
	delete fieldArray[k];
    }
}

// Add the named TicketField to this Ticket.
//
void Ticket::addField(TicketField *f)
{
    this->fieldArray[next_available++] = f;
}


// Return with the number of fields in this ticket.
int Ticket::getNumFields(void)
{
    return next_available;
}


int Ticket::is_target_ticket(char *column_name, char *column_value)
{
    char buffer[TICKETS::MAXFIELDLEN];

    TicketField *tf = this->get_named_field(column_name);
    if (tf == 0x00) return 0;
    tf->getFieldValueString(buffer);

    if (strcasecmp(buffer, column_value) == 0) return 1;
    else return 0;
}


// Return with a pointer to the TicketField whose
// fieldName value is given by targ.
//
TicketField *Ticket::get_named_field(const char *targ)
{
    TicketField *p;
    char tp[TICKETS::MAXFIELDLEN];

    for (int k = 0; k < TICKETS::MAXFIELDS 
	              && this->fieldArray[k] != 0x00; ++k) {
	p = this->fieldArray[k];
        if (strcasecmp(p->getFieldName(tp), targ) == 0) 
	    return p;
    }
    return (TicketField *) 0x00;
}


// Get the indexed TicketField for this Ticket.
//
TicketField *Ticket::get_indexed_field(int idx)
{
    if (idx < next_available && idx >= 0) return this->fieldArray[idx];
    else return (TicketField *) 0x00;
}


TicketField::TicketField(char *colname, char *colvalue)
{
    int alen, blen;

    if (colname != 0x00) {  // There is a valid column name given...
	alen = strlen(colname);
	fieldName = new char[alen+1];
	strcpy(fieldName, colname);
	if (colvalue != 0x00) { // If theres a valid column value...
	    blen = strlen(colvalue);
	    fieldValue = new char[blen+1];
	    strcpy(fieldValue, colvalue);
	}
	else {
	    *fieldValue = 0x00;
	}
    }
    else {
	*fieldName = 0x00;
	*fieldValue = 0x00;
    }
}

TicketField::~TicketField(void)
{
    if (*fieldName != 0x00) delete fieldName;
    if (*fieldValue != 0x00) delete fieldValue;
}



// Get the field name and place a copy in targ
// Make sure targ points to a location with enough space
// for the fieldname.
//
char *TicketField::getFieldName(char *targ)
{
    strcpy(targ, fieldName);
    return targ;
}


// Get the field value and place a copy in toag
// Make sure targ points to a location with enough space
// for the fieldname.
char *TicketField::getFieldValueString(char *toag)
{
    strcpy(toag, fieldValue);
    return toag;
}



/* *******************************************************************
 * Methods for class TicketDeck.
 *
 * *******************************************************************/

// Constructor:
//

TicketDeck::TicketDeck(char *pathname)
{
    char inbuffer[TICKETS::MAXLINELENGTH];
    char *items[512];

    FILE *dataf = fopen(pathname, "r");

    // Initialize the headers and items arrays.
    //
    for (int k = 0; k < 512; ++k) {
	TD_ticket_header[k] = 0x00;
	items[k] = 0x00;
    }


    // First we get the headers.
    fgets(inbuffer, TICKETS::MAXLINELENGTH, dataf);
    get_fieldnames(inbuffer,TD_ticket_header);


    // Then we get the collection of tickets.
    //
    //
    int tcount = 0;
    while (fgets(inbuffer, TICKETS::MAXLINELENGTH, dataf) != NULL) {
        Ticket *temp = new Ticket(TD_ticket_header, inbuffer);
	TD_ticket_array[tcount++] = temp;
    }
    TD_ticket_array[tcount] = 0x00;   // Terminate with a sentinel value.
    TD_num_tickets = tcount;


    // Close the data file.
    //
    fclose(dataf);
    
}


TicketDeck::~TicketDeck(void)
{
    for (int k = 0; k < TD_num_tickets; ++k) {
	delete TD_ticket_array[k];
    }
}



Ticket *TicketDeck::get_ticket(int indx)
{
    return TD_ticket_array[indx];
}




int TicketDeck::get_num_tickets(void)
{
    return TD_num_tickets;
}

