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
#include <assert.h>
#include <QString>
#include "Tickets.hpp"

// ============================================================================
//
//     Class TicketField
//
// ============================================================================


/* ****************************************************************
 * CONSTRUCTORS
 * ****************************************************************/

TicketField::TicketField(const char *colname, const char *colvalue)
{
    // Temporary placeholders for the field name and field value.

    assert(colname != 0x00); /* This should not happen */

    // Convert to QStrings.

    QString fname = QString(colname);
    QString fvalue = QString(colvalue);


    // Now we remove double and single quotes.

    fname.remove(QChar('"'));    // Remove the double quotes.
    fvalue.remove(QChar('"'));    // Remove the double quotes.
    
    fname.remove(QChar('\''));   // Remove the single quotes.
    fvalue.remove(QChar('\''));   // Remove the single quotes.

    TF_Field_Name = (fname.trimmed()).toUpper();
    TF_Field_Value = fvalue.trimmed();
}


TicketField::TicketField(QString field_name, QString field_value)
{

    QString ax = field_name.remove(QChar('"'));
    ax = ax.remove(QChar('\''));
    ax = field_name.trimmed();
    assert(ax.size() != 0);  /* Shouldn't happen. */


    QString bx = field_value.remove(QChar('"'));
    bx = bx.remove(QChar('"'));

    TF_Field_Name = (ax.trimmed()).toUpper();
    TF_Field_Value = bx.trimmed();
}


/* *************************************************************
 * ACCESSORS
 * *************************************************************/

char *TicketField::get_Field_Name_pchar(char *targ)
// Get the field name and place a copy in targ
// Make sure targ points to a location with enough space
// for the fieldname.
//
// The field names when retrieved will always default to
// upper case.
{
    strcpy(targ, qPrintable(TF_Field_Name.toUpper()));
    return targ;
}


// The version using QStrings
//
QString TicketField::get_Field_Name_QString(void)
{
    return TF_Field_Name;   // assert: TF_Field_Name is in upper case.
}


// Get the field value and place a copy in toag
// Make sure targ points to a location with enough space
// for the fieldname.
//
// This version returns a pointer to a char.
//
char *TicketField::get_Field_Value_pchar(char *toag)
{
    strcpy(toag, qPrintable(TF_Field_Value));
    return toag;
}


// This version returns a QString.
//
QString TicketField::get_Field_Value_QString(void)
{
    return TF_Field_Value;
}


// This function compares target with the TF_Field_Name
// of the current TicketField.
//
// It returns 1 if target and TF_Field_Name are lexically the
// same and 0 otherwise.
//
// When comparing the field names we do a non-case-sensitive
// comparison.

int TicketField::has_Field_Name(QString target)
{
     QString ax = (target.trimmed()).toUpper();

     if (ax.compare(TF_Field_Name) == 0) {
         return 1;
     }
     else return 0;
}



// =========================================================================
//
//                              Class Ticket
//
// A Ticket consists of a collection of TicketFields. The order of the
// TicketFields in a Ticket should not be important. When retrieving
// TicketFields, we always search on the Field Name and not the
// particular location of the TicketField.
//
// =========================================================================



// Construct a Ticket given the field names (which is in headers) and a tabbed
// delimited file containing the field values. We assume that the locations
// in inbuf are important as they shall be read sequentially and also
// assigned sequentially the field names encoded in headers.
//
Ticket::Ticket(char **headers, char *inbuf)
{

    /* Initialize the field array */
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
        TK_Field_Array[k] = (TicketField *) 0x00;
    }

    QStringList qstrlistHeaders;
    QString instr = QString(inbuf);

    for (int k = 0; headers[k] != 0x00; ++k) {
        QString content = QString(headers[k]);    // Get content of the header
        content = content.remove(QChar('"'));     // Remove double quotes, if any.
        content = content.remove(QChar('\''));    // Remove single quotes, if any.
        
        qstrlistHeaders.append(content.trimmed());   // Trim and append to qstrlistHeaders.
    }

    QString tkcontent = inbuf;    // assert: inbuf is now in content.
    QStringList valuesList = tkcontent.split(QChar('\t'));

    /* Now we create the TicketFields for this Ticket
       We only create as many fields as there are Headers. 
       If we ran out of data then we put in empty strings
       in the Field Values. */

    int max_headers = qstrlistHeaders.size();
    int max_values  = valuesList.size();

    for (int k = 0; k < max_headers && k < TICKETS::MAXFIELDS; ++k) {
       if (k >= max_values) {
           TK_Field_Array[k] = new TicketField(qstrlistHeaders.at(k), QString(""));
       }
       else {
           TK_Field_Array[k] = new TicketField(qstrlistHeaders.at(k), valuesList.at(k));
       }
    }
    TK_Next_Available = max_headers;
}


// The following constructor has the same function as the previous constructor
// but this time we already have a QStringList of ticketHeaders.
// The routine is almost the same as previous.
//
Ticket::Ticket(QStringList ticketHeaders, char *tabbed_buffer)
{
    /* Initialize the field array */
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
        TK_Field_Array[k] = 0x00;
    }

    QString tkcontent = tabbed_buffer;    // assert: inbuf is now in content.
    QStringList valuesList = tkcontent.split(QChar('\t'));

    /* Now we create the TicketFields for this Ticket
       We only create as many fields as there are Headers. 
       If we ran out of data then we put in empty strings
       in the Field Values. */

    int max_headers = ticketHeaders.size();
    int max_values  = valuesList.size();

    for (int k = 0; k < max_headers && k < TICKETS::MAXFIELDS; ++k) {
       if (k >= max_values) {
           TK_Field_Array[k] = new TicketField(ticketHeaders.at(k), QString(""));
       }
       else {
           TK_Field_Array[k] = new TicketField(ticketHeaders.at(k), valuesList.at(k));
       }
    }
    TK_Next_Available = max_headers;
}


 

Ticket::~Ticket(void)
{
    for (int k = 0; k < TK_Next_Available; ++k) {
	delete TK_Field_Array[k];
    }
}


// Add the named TicketField to this Ticket.
//
void Ticket::addField(TicketField *f)
{
    this->TK_Field_Array[TK_Next_Available++] = f;
}

// Return with the number of fields in this ticket.
int Ticket::getNumFields(void)
{
    return TK_Next_Available;
}


int Ticket::is_target_ticket(QString column_name, QString column_value)
// Determine if the TicketField with column_name has the
// same value with column_value.
//
// The return values are
// 1   -- The column_value matches.
// 0   -- The column_value does not match.
// -1  -- There is no TicketField in this Ticket with the name column_name
{
    TicketField *currentField;

    QString search_field_name = column_name.trimmed();
    search_field_name = search_field_name.remove(QChar('"'));
    search_field_name = search_field_name.remove(QChar('\''));


    QString search_field_value = column_value.trimmed();
    search_field_value = search_field_value.remove(QChar('"'));
    search_field_value = search_field_value.remove(QChar('\''));

    QString current_field_name, current_field_value;

    int k;

    for (k = 0; k < TK_Next_Available; ++k) {

        /* Get a TicketField from this Ticket */
        currentField = TK_Field_Array[k];    

        /* Get the Field Name of this TicketField */
        current_field_name = currentField->get_Field_Name_QString();

        /* Compare the Field Name of this TicketField with the Field Name
           being searched. */
        if (current_field_name.compare(search_field_name) == 0) {

            /* Found a match. Now check if the Field Value is what
               we are also looking for. */
            current_field_value = currentField->get_Field_Value_QString();

            if (current_field_value.compare(search_field_value) == 0) {
                return 1;
            }
            else {
                return 0;
            }
        }
    }

    if (k == TK_Next_Available) {
        return -1;
    }
}


// Return with a pointer to the TicketField whose
// fieldName value is given by targ or 0x00 if not found.
//
TicketField *Ticket::get_named_field(QString targ)
{
    QString targ2 = targ.remove(QChar('"'));
    targ2 = (targ2.remove(QChar('\''))).trimmed();

    for (int k = 0; k < TK_Next_Available
	              && this->TK_Field_Array[k] != 0x00; ++k) {
	TicketField *p = TK_Field_Array[k];
        QString ax = p->get_Field_Name_QString();

        if (ax.compare(targ2) == 0) return p;
    }
    return (TicketField *) 0x00;
}

// Get the indexed TicketField for this Ticket.
//
TicketField *Ticket::get_indexed_field(int idx)
{
    if (idx < TK_Next_Available && idx >= 0) return this->TK_Field_Array[idx];
    else return (TicketField *) 0x00;
}




// =========================================================================
//
//     Class TicketDeck.
//
// A collection of Tickets we call a TicketDeck. In production use, a TicketDeck
// consists of the rows of tuples stored in a tab-delimited text file.
//
// The tab-delimited text file which we are working with have the following
// properties.
//
// (a) The first row in the file are the headers.
// (b) Each row thereafter represent individual tickets
//     
// =========================================================================

// Constructor:
//

TicketDeck::TicketDeck(char *pathname)
{
    char inbuffer[TICKETS::MAXLINELENGTH];
    char *items[512];

    FILE *dataf = fopen(pathname, "r");

    // Initialize the headers and items arrays.
    //
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
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

