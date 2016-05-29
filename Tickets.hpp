/* *************************************************************************
 * Tickets.hpp
 *
 * Declaration of Ticket class. A Ticket is a collection of
 * fields and their values which together form a ticket. A Ticket
 * is different from a database because a Ticket does not have
 * links or relations between them.
 *
 * This class was used in the creation of the Valids program.
 *
 * robert pascual (c) 2011
 * Victoria, Tarlac
 *
 *
 * Revision History:
 *
 * $Id: Tickets.hpp,v 1.3 2011/12/15 13:50:06 robert Exp $
 *
 * $Log: Tickets.hpp,v $
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
 * ************************************************************************/
#ifndef TICKETS_HPP
#define TICKETS_HPP

#include <QString>
#include <QStringList>

namespace TICKETS {
    const int MAXFIELDS     = 1024;
    const int MAXNAMELEN    = 1024;
    const int MAXFIELDLEN   = 1024;
    const int MAXTICKETS    = 2048;
    const int MAXLINELENGTH = 2048;
}


// The class TicketField encapsulates information about
// a single field in the valids ticket. Examples of fields
// would be SEX, STATUS, etc. A TicketField is just an ordered pair
// where the first member is the Field Name and the second member
// is the Field Value.
//
// We made the following decisions
// about this class:
//
// This is a refactoring: In this version of the Ticket and TicketField
// class:
//
// (a) Field Names shall not be case-insensitive. They will be stored as upper
//     case values and a case insensitive search will always be carried out.
//     There are some very subtle bugs
//     introduced by having the fieldName be case sensitive. Those bugs are very hard
//     to find.
//
// (b) Field Names and Field values will be of type QString. The QString class is a thoroughly
//     debugged string class. Let us take advantage of the work done by others.
//
// (c) We shall also implement methods called getIntValue() and
//     getFloatValue() which will work as intended. For example, it shall
//     return reasonable values if confronted with a field that cannot
//     be converted to either a double or an int.
//
// (d) Field Values will be stored as is. No conversion to upper or lower case QStrings
//     shall be made. When a search is carried out, the default search shall be
//     case insensitive but a function shall also be provided to make the search
//     case sensitive.
//

class TicketField {
    public:

        // ======================================================
        // CONSTRUCTORS.
        // ======================================================

	TicketField(void);
        TicketField(const char *, const char *);
        TicketField(QString, QString);


        // ======================================================
	// DESTRUCTOR.
        // ======================================================
 
	~TicketField();


        // ======================================================
	// MUTATORS and ACCESSORS
        // ======================================================

        QString get_Field_Name_QString(void);
        QString get_Field_Value_QString(void);

        int has_Field_Name(QString);       // Check if TicketField has this particular FieldName.
        int has_Field_Value(QString);


        // ====================================================
        // OBSOLETE Functions.
        // Do not use with new programs.
        // ===================================================

        char *get_Field_Name_pchar(char *); // Obsolete
	char *get_Field_Value_pchar(char *); // Obsolete


        // ========================================================
        // UNIMPLEMENTED Functions.
        // The following functions are presently unimplemented.
        // Unless these functions are needed somehow they would
        // probably remain unimplemented.
        // ========================================================
	double get_Field_Value_Float();
	double get_Field_Value_Double();
	int    get_Field_Value_Int();
        void setFieldName(QString); 
	void setFieldValueString(QString); 
	void setFieldValueFloat(double);
	void setFieldValueInt(int);



    private:
        QString TF_Field_Name;
        QString TF_Field_Value;
        void init(QString, QString);   // Only used privately
};


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

class Ticket {

    public:

	Ticket(void);



	// Construct a new ticket from the given headers
	// and the buffer containing a tab-delimited field
	// of values. Overloaded functions.

        Ticket(QStringList headers, QStringList buff);
	Ticket(char *headers, char *inbuf);   // Obsolete. Do not use
                                               // with new programs.


	// Destructor.
	
	~Ticket();

	void addField(TicketField *);

	// Accessor functions
	//
	//
	// Get a pointer to the indexed field.
	TicketField *get_indexed_field(int);

        // Get the number of fields in this ticket.
	int getNumFields(void);

	// Return with a pointer to the TicketField
	// whose fieldName is given.
	//
	TicketField *get_named_field(const char *);
        TicketField *get_named_field(QString);

	// Indicates whether this Ticket has the value or
	// content column_value for its column_name.
	//
	// Returns 1 if true and 0 otherwise.
	//
	int is_target_ticket(char *column_name, char *column_value);
        int is_target_ticket(QString column_name, QString column_value);

    private:
        TicketField *TK_Field_Array[TICKETS::MAXFIELDS];
	int TK_Next_Available;   // Index of next available slot in fieldArray.
        void init(QStringList, QStringList);
};


// A collection of tickets from a single file we shall call a TicketDeck.
// This is actually just a structure with fancy class-oriented
// methods.
//
class TicketDeck
{
    public:
	// Constructors.
	//
	// Create a TicketDeck structure from the given path name.
	//
	TicketDeck(char *pathname);

	// Destructor
	//
	~TicketDeck(void);


	// Fill argv with the headers of this TicketData.
	//
	char **getHeaders(char **argv);


	// Return with a pointer to a ticket. The ticket
	// is identified through its index.
	//
        Ticket *get_ticket(int index);


        // Return with number of tickets in the deck.
	//
        int get_num_tickets(void);


    private:
	Ticket *TD_ticket_array[TICKETS::MAXTICKETS];
	QStringList TD_ticket_header;
	int TD_num_tickets;
};

// Functions from parser.cpp
//
//
int is_a_field_delimiter(char);
char **get_field_values(char *, char **);
char **get_fieldnames(char *, char **);

#endif

