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
TicketField::TicketField(QString field_name, QString field_value)
{
    this->init(field_name, field_value);
}


TicketField::TicketField(const char *colname, const char *colvalue)
{
    // Temporary placeholders for the field name and field value.

    assert(colname != 0x00); /* This should not happen */

    // Convert to QStrings.

    QString fname = QString(colname);
    QString fvalue = QString(colvalue);

    this->init(fname, fvalue);
}

// The following function does the actual job of initializing the private
// variables of this class.

void TicketField::init(QString field_name, QString field_value)
{

    QString ax = field_name.remove(QChar('"'));
    ax = ax.remove(QChar('\''));
    ax = ax.trimmed();
    assert(ax.size() != 0);  /* Shouldn't happen. */


    QString bx = field_value.remove(QChar('"'));
    bx = bx.remove(QChar('\''));
    bx = bx.trimmed();

    TF_Field_Name  = ax;
    TF_Field_Value = bx;
}



/* *************************************************************
 * ACCESSORS
 * *************************************************************/

QString TicketField::get_Field_Name_QString(void)
{
    return TF_Field_Name;   // assert: TF_Field_Name is in upper case.
}


QString TicketField::get_Field_Value_QString(void)
{
    return TF_Field_Value;
}


// The following obsolete functions just call the function above.
//

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


char *TicketField::get_Field_Value_pchar(char *toag)
{
    strcpy(toag, qPrintable(TF_Field_Value));
    return toag;
}

char *TicketField::getFieldValueString(char *toag)
{
    strcpy(toag, qPrintable(TF_Field_Value));
    return toag;
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

// Like above but checks the field value. The test is
// carried out case insensitive.
//
// Returns 1 if true and 0 otherwise.

int TicketField::has_Field_Value(QString target)
{
     QString ax = (target.trimmed()).toUpper();
     QString bx = TF_Field_Value.toUpper();
     if (ax.compare(bx) == 0) {
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


// First the init() function---which we will be using with the constructors.
//
void Ticket::init(QStringList ticketHeaders, QStringList valuesList)
{
    /* Initialize the field array */
    TK_Next_Available = 0;
    for (int k = 0; k < TICKETS::MAXFIELDS; ++k) {
        TK_Field_Array[k] = 0x00;
    }

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

// Construct a Ticket given a QStringList of field names
// and a QStringList of field values.
//
Ticket::Ticket(QStringList ticketHeaders, QStringList valuesList)
{
    this->init(ticketHeaders, valuesList);
}


// Create a Ticket from a tab delimited char* of field names
// and field values. This function just calls the function above
// after converting the char pointers to QStringLists.


Ticket::Ticket(char *headers, char *inbuf)
{

    QString hqstr  = QString(headers);
    QString fvqstr = QString(inbuf);

    QStringList field_names  =  hqstr.split(QChar('\t'));
    QStringList field_values = fvqstr.split(QChar('\t'));

    this->init(field_names, field_values);
}


// Destructor

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
// Search through all the TicketFields in this ticket and determine
// if the TicketField with column_name has the
// same value with column_value.
//
// The return values are
// 1   -- The column_value matches.
// 0   -- The column_value does not match.
// -1  -- There is no TicketField in this Ticket with the name column_name
{
    QString search_field_name = column_name.trimmed();
    search_field_name = search_field_name.remove(QChar('"'));
    search_field_name = search_field_name.remove(QChar('\''));


    QString search_field_value = column_value.trimmed();
    search_field_value = search_field_value.remove(QChar('"'));
    search_field_value = search_field_value.remove(QChar('\''));

    for (int k = 0; k < TK_Next_Available; ++k) {

        TicketField *currentField = TK_Field_Array[k];    

        if (currentField->has_Field_Name(search_field_name)) {
            if (currentField->has_Field_Value(search_field_value)) {
                return 1;
            }
            else 
                return 0;
        }
    }

    return -1;
}

// Version using char pointers.
int Ticket::is_target_ticket(char *column_name, char *column_value)
{
    return this->is_target_ticket(QString(column_name), QString(column_value));
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
        if (p->has_Field_Name(targ)) {
            return p;
        }
    }

    return (TicketField *) 0x00;
}

// The following obsolete function just calls the above function.

TicketField *Ticket::get_named_field(const char *targ)
{
    return this->get_named_field(QString(targ));
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
    char *retvp;    /* used to squash warnings about unused return values. */

    FILE *dataf = fopen(pathname, "r");



    // First we get the headers.
    retvp = fgets(inbuffer, TICKETS::MAXLINELENGTH, dataf);
    assert(retvp != 0x00);

    QString strbuff = QString(inbuffer);
    TD_ticket_header = strbuff.split(QChar('\t'));


    // Then we get the collection of tickets.
    //
    //
    int tcount = 0;
    while (fgets(inbuffer, TICKETS::MAXLINELENGTH, dataf) != NULL) {
        strbuff = QString(inbuffer);
        QStringList field_values = strbuff.split(QChar('\t'));
        Ticket *temp = new Ticket(TD_ticket_header, field_values);
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

