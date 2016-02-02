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

namespace TICKETS {
    const int MAXFIELDS     = 512;
    const int MAXNAMELEN    = 512;
    const int MAXFIELDLEN   = 512;
    const int MAXTICKETS    = 1024;
    const int MAXLINELENGTH = 1024;
}


// The class TicketField encapsulates information about
// a single field in the valids ticket. Examples of fields
// would be SEX, STATUS, etc. We made the following decisions
// about this class:
//
// (a) fieldName shall be case-insensitive
// (b) All values will, in the meantime, be of type char *. Even so,
//     we shall implement a getIntValue() and getFloatValue() method
//     on the fieldValue member of this class.
//
class TicketField {
    public:
	// Default constructor. 
	TicketField(void);

	// Constructor that includes the fieldName and the
	// fieldValue.
        TicketField(char *, char *);

	// Destructor.
	~TicketField();

	// The mutators.
        void setFieldName(char *);
	void setFieldValueString(char *);
	void setFieldValueFloat(double);
	void setFieldValueInt(int);

	// The accessors.
	//
        char *getFieldName(char *);
	char *getFieldValueString(char *);
	double getFieldValueFloat();
	int getFieldValueInt();

    private:
        char *fieldName;
        char *fieldValue;
};

// A Ticket consists of a collection of TicketFields.
class Ticket {

    public:
	Ticket(void);

	// Construct a new ticket from the given headers
	// and the buffer containing a tab-delimited field
	// of values.
	Ticket(char **headers, char *inbuf);

	// Destructor.
	//
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

	// Indicates whether this Ticket has the value or
	// content column_value for its column_name.
	//
	// Returns 1 if true and 0 otherwise.
	//
	int is_target_ticket(char *column_name, char *column_value);

    private:
        TicketField *fieldArray[TICKETS::MAXFIELDS];
	int next_available;   // Index of next available slot in fieldArray.
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
	char *TD_ticket_header[TICKETS::MAXFIELDS];
	int TD_num_tickets;
};

// Functions from parser.cpp
//
//
int is_a_field_delimiter(char);
char **get_field_values(char *, char **);
char **get_fieldnames(char *, char **);

#endif

