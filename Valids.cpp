/* **********************************************************************
 * Valids.cpp
 *
 * Defines the classes required by the GUI frontend.
 *
 * Revision History:
 *
 * $Id: Valids.cpp,v 1.5 2011/12/18 13:27:44 robert Exp $
 *
 * $Log: Valids.cpp,v $
 * Revision 1.5  2011/12/18 13:27:44  robert
 * Corrected spelling error in the search for L FEE. Added a period.
 *
 * Revision 1.4  2011/12/15 13:52:00  robert
 * Working version. This program works quite well under Linux and will
 * soon be ported to Windows to evaluation.
 *
 * Revision 1.3  2011/12/14 19:36:08  robert
 * Now can print the headers of the Valids. Valids are
 * currently at one page per student but this will be improved.
 *
 * Revision 1.2  2011/12/12 11:05:20  robert
 * Initial import of the newer version of these files.
 *
 *
 * *********************************************************************/
#include <string.h>
#include "Tickets.hpp"
#include "Valids.hpp"

// Comment out the following define for the production version.
//
// #define DEBUG_MODE

agent_set_params_dialog::agent_set_params_dialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);

    fileName_str[0] = 0x00;
    fieldName_str[0] = 0x00;
    fieldValue_str[0] = 0x00;

    connect(file_dialog_button, SIGNAL(clicked()), 
	    this, SLOT(invoke_fileDialog()));

}

void agent_set_params_dialog::invoke_fileDialog(void)
{
    QString homeDir = QDir::homePath();


    // Usage notes: separate the file filters by spaces. For example
    // if you want the user to be able to see only *.txt and *.csv files.
    QString fileName = 
	QFileDialog::getOpenFileName(this, tr("Select File"),
             homeDir, 
	     tr("Data Files (*.txt *.csv);; All Files (*.*)"));
    // strcpy(data_file, fileName.toAscii());
    //

    (this->target_file_name)->setText(fileName);

    // printf("%s\n", data_file);
}



// Now comes the function that does the one true reason we
// have this program. This is the function that prints the actual
// valids. It is rather long.
//
//
void agent_set_params_dialog::accept(void)
{


    QString axe = target_file_name->text();
    QString axf = target_field_name->text();
    QString axg = target_field_value->text();

    // Convert QString objects to NULL terminated
    // character arrays.
    //
    strcpy(fileName_str, axe.toAscii());
    strcpy(fieldName_str, axf.toAscii());
    strcpy(fieldValue_str, axg.toAscii());

#ifdef DEBUG_MODE
    printf("I got the following values.\n"
	   "File to process : %s\n"
	   "Field to read   : %s\n"
	   "Value to detect : %s\n", 
	          fileName_str, fieldName_str, fieldValue_str);
#endif

    // Open the data file and create a TicketDeck
    // from the contents of the datafile.
    //
    TicketDeck *ticketdeck = new TicketDeck(fileName_str);


    // Prepare the printer.
    //
    QPrinter pageOut;
    QPrintDialog printDialog(&pageOut, this);
    QFont default_font(VALIDS::FONT_FAMILY, VALIDS::FONT_SIZE);

    // Set the defaults for the printer.
    //
    pageOut.setFullPage(1);
    pageOut.setPaperSize(QPrinter::Legal);



    if (printDialog.exec()) {
	QPainter painter(&pageOut);
	QRect rect = painter.viewport();
	painter.setFont(default_font);

        // Now process the TicketDeck
	//

	// The character buffers that will be used.
	// Declared like below to make it easier to copy
	// and paste if we need more.
	//
	char buffer_A[TICKETS::MAXFIELDLEN];
	char buffer_B[TICKETS::MAXFIELDLEN];
	char buffer_C[TICKETS::MAXFIELDLEN];
	char buffer_D[TICKETS::MAXFIELDLEN];
	char buffer_E[TICKETS::MAXFIELDLEN];
	char buffer_F[TICKETS::MAXFIELDLEN];

	char fullname_buffer[TICKETS::MAXFIELDLEN];
	char guardian_name[TICKETS::MAXFIELDLEN];

        Ticket *ticketp;	
        int count = ticketdeck->get_num_tickets();
      
	// The pointers to the ticket fields.
	//
	TicketField *field_1, *field_2, *field_3;
	TicketField *field_4, *field_5, *field_6;


	int page_vpos;     // Vertical position of ticket in page.
	                   // This is either 0 for first, 1 for
			   // second, etc.
			   //
	int page_hpos;     // Horizontal position. 0 for the left position
	                   // and 1 for the right position.
			   //
	int ticket_page_index;  // 0 for the first ticket in the page.
        int ticket_proc_count = 0;  // count of processed tickets.

	for (int k = 0; k < count; ++k) {

	    // Get a ticket from the ticketdeck.
	    //

            ticketp = ticketdeck->get_ticket(k);

            // Is this the right ticket?
	    //
            // Insert routine to vet the ticket here.
	    //
            if (!(ticketp->is_target_ticket(fieldName_str, fieldValue_str))) {
		continue;
	    }


	    // Compute the vertical position of this valid ticket.
	    //
	    ticket_page_index = ticket_proc_count % VALIDS::VALIDS_PER_VPAGE;

	    if (ticket_page_index % 2 == 0) {
		page_vpos = ticket_page_index / 2;
                page_hpos = 0;
	    }
	    else {
		page_vpos = ticket_page_index / 2;
		page_hpos = 1;
	    }



	    // Get pointers to the interesting fields.
	    //
            field_1 = ticketp->get_named_field("ID NO.");
            field_2 = ticketp->get_named_field("GNAME");
            field_3 = ticketp->get_named_field("FNAME");
            field_4 = ticketp->get_named_field("GU.G.NAME");
            field_5 = ticketp->get_named_field("GU.F.NAME");
            field_6 = ticketp->get_named_field("BRGY");

	    // Get the values for the interesting
	    // fields. Don't get confused.
	    //
	    if (field_1 != 0x00) field_1->getFieldValueString(buffer_A);
	    if (field_2 != 0x00) field_2->getFieldValueString(buffer_B);
	    if (field_3 != 0x00) field_3->getFieldValueString(buffer_C);
	    if (field_4 != 0x00) field_4->getFieldValueString(buffer_D);
	    if (field_5 != 0x00) field_5->getFieldValueString(buffer_E);
	    if (field_6 != 0x00) field_6->getFieldValueString(buffer_F);

            // Compose the fullname of the student.
	    //
	    strcpy(fullname_buffer, buffer_B);
	    strcat(fullname_buffer, " ");
	    strcat(fullname_buffer, buffer_C);

	    // Compose the guardian name
	    //
	    strcpy(guardian_name, buffer_D);
	    strcat(guardian_name, " ");
	    strcat(guardian_name, buffer_E);

	    

            // This section prints personal information about
	    // the student.
	    //
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN 
			         + page_vpos * VALIDS::TEXTHEIGHT,
			     tr(buffer_A));
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + VALIDS::BASELINE_SKIP, 
			     tr(fullname_buffer));
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 2*VALIDS::BASELINE_SKIP, 
			     tr(guardian_name));
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 3*VALIDS::BASELINE_SKIP, 
			     tr(buffer_F));

	    // Now compose the section and status information.
	    //
            if ((field_1 = ticketp->get_named_field("SECT")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");

	    if ((field_2 = ticketp->get_named_field("STATUS")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");

	    // We position the section and the status so that
	    // it is flush right on the TEXTWIDTH. Note the
	    // assumption that each letter is LETTERWIDTH wide.
	    //
	    //
            int posXA, posXB;   // Adjustments to make the section and
	                        // status flush right. These variables
				// will be reused.

	    posXA = VALIDS::TEXTWIDTH - strlen(buffer_A)*VALIDS::LETTERWIDTH;
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN + posXA
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT,
			     tr(buffer_A));

	    posXB = VALIDS::TEXTWIDTH - strlen(buffer_B)*VALIDS::LETTERWIDTH;
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN + posXB
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 3*VALIDS::BASELINE_SKIP, 
			     tr(buffer_B));

	    // Print the string Payments Received so it is in the middle
	    // of the TEXTWIDTH. Note computation of posXA. Good thing 
	    // you are good with algebra.
	    //
	    strcpy(buffer_F, ">> Payments Received <<");
	    posXA = (VALIDS::TEXTWIDTH 
		     - strlen(buffer_F)*VALIDS::LETTERWIDTH)/2;
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN + posXA
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 5*VALIDS::BASELINE_SKIP, 
			     tr(buffer_F));

            // Now print out in the payments made.
	    //
	    // This is the first line
	    //
            if ((field_1 = ticketp->get_named_field("DATE-1")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("AMT-1")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");
            if ((field_3 = ticketp->get_named_field("REM-1")) != 0x00)
	        field_3->getFieldValueString(buffer_C);
            else strcpy(buffer_C, "");

	    sprintf(buffer_D, "%-11s %12s %-8s", buffer_A, buffer_B, buffer_C);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 6*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));

	    // This is the second line
	    //
            if ((field_1 = ticketp->get_named_field("DATE-2")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("AMT-2")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");
            if ((field_3 = ticketp->get_named_field("REM-2")) != 0x00)
	        field_3->getFieldValueString(buffer_C);
            else strcpy(buffer_C, "");

	    sprintf(buffer_D, "%-11s %12s %-8s", buffer_A, buffer_B, buffer_C);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 7*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));

	    // This is the third line
	    //
            if ((field_1 = ticketp->get_named_field("DATE-3")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("AMT-3")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");
            if ((field_3 = ticketp->get_named_field("REM-3")) != 0x00)
	        field_3->getFieldValueString(buffer_C);
            else strcpy(buffer_C, "");

	    sprintf(buffer_D, "%-11s %12s %-8s", buffer_A, buffer_B, buffer_C);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 8*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));


	    // The separator between the two payments reports.
	    // The routine was copy-pasted from the routine to
	    // print "Payments Received."
	    //
	    strcpy(buffer_F, ">>>>--->>oOo<<---<<<<");
	    posXA = (VALIDS::TEXTWIDTH 
		     - strlen(buffer_F)*VALIDS::LETTERWIDTH)/2;
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN + posXA
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 9*VALIDS::BASELINE_SKIP, 
			     tr(buffer_F));


            // Now for the miscellaneous payments.
	    //
	    //
	    // Ms Mak and AF
	    //
            if ((field_1 = ticketp->get_named_field("MS MAK")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("AF")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");

	    sprintf(buffer_D, "MS %13s AF %13s", buffer_A, buffer_B);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 10*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));

	    // TP and TAPSA
	    //
            if ((field_1 = ticketp->get_named_field("T-PAPER")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("TAPSA")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");

	    sprintf(buffer_D, "TP %13s Tapsa %10s", buffer_A, buffer_B);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 11*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));

	    // INS. and L. FEE
	    //
            if ((field_1 = ticketp->get_named_field("INS.")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");
            if ((field_2 = ticketp->get_named_field("L. FEE")) != 0x00)
	        field_2->getFieldValueString(buffer_B);
            else strcpy(buffer_B, "");

	    sprintf(buffer_D, "INS %12s LFEE %11s", buffer_A, buffer_B);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 12*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));

	    // And Finally, REM-4 
	    //
            if ((field_1 = ticketp->get_named_field("REM-4")) != 0x00)
	        field_1->getFieldValueString(buffer_A);
            else strcpy(buffer_A, "");

	    strcpy(buffer_B, "");  // Won't have any contents.

	    sprintf(buffer_D, "RM %13s %13s", buffer_A, buffer_B);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 13*VALIDS::BASELINE_SKIP, 
			     tr(buffer_D));





	    // At the very bottom of the Valid we put in the contents of
	    // COMMENT-5
	    //
            field_1 = ticketp->get_named_field("COMMENT-5");
	    if (field_1 == 0x00) strcpy(buffer_A, "Not Available");
	    else field_1->getFieldValueString(buffer_A);
            painter.drawText(VALIDS::PAPER_ABSCISSA_ORIGIN
		                 + page_hpos * VALIDS::HORIZ_SKIP,
		             VALIDS::PAPER_ORDINATE_ORIGIN
			         + page_vpos * VALIDS::TEXTHEIGHT
			         + 15*VALIDS::BASELINE_SKIP, 
			     tr(buffer_A));

            ticket_proc_count++;    // Increment count of tickets
	                            // processed.

	    if (ticket_page_index == VALIDS::VALIDS_PER_VPAGE - 1) 
		pageOut.newPage();
	}

        	
#if (0)

	painter.drawText(0, 50, tr("This is a test."));
	painter.drawText(80, 80, tr("This should be in the middle."));

#endif

	// Uncomment the following line if you want the
	// application to close after printing.
	//
	// #define CLOSE_AFTER_PRINTING
	
#ifdef CLOSE_AFTER_PRINTING

        this->done(0);

#else
        
	strcpy(fieldValue_str, "");
        target_field_value->setText("");

#endif

    }
}


