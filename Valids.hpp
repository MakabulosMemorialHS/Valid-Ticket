/* **********************************************************************
 * Valids.hpp
 *
 * Declarations for the classes used by the Valids
 * program.
 *
 * robert pascual (c) 2011
 *
 * Revision History:
 *
 * $Id: Valids.hpp,v 1.5 2011/12/15 13:56:53 robert Exp $
 *
 * $Log: Valids.hpp,v $
 * Revision 1.5  2011/12/15 13:56:53  robert
 * Added more manifest constants.
 *
 * Revision 1.4  2011/12/14 19:36:08  robert
 * Now can print the headers of the Valids. Valids are
 * currently at one page per student but this will be improved.
 *
 * Revision 1.3  2011/12/12 11:05:20  robert
 * Initial import of the newer version of these files.
 *
 *
 * **********************************************************************/

#ifndef VALIDS_HPP
#define VALIDS_HPP

#ifndef DEBUG_MODE
    #include <QtGui>
    #include <QDialog>

    #include "ui_Valids.h"
#endif    // DEBUG_MODE

// Set the default location of data.txt
//
#ifdef linux
    #define DEFAULT_FILE  "/home/robert/developer/Valids/data.txt"
#endif

// The following MANIFEST constants control the layout of the tickets,
// the memory allocated to various dynamic variables, and
// others.
//
namespace VALIDS {
    const int PATHNAME_MAXLEN = 1024;
    const int FIELDLEN_MAXLEN = 64;
    const int FIELDNAME_MAXLEN = 64;
    const int PAPER_ABSCISSA_ORIGIN = 50;
    const int PAPER_ORDINATE_ORIGIN = 50;
    const int BASELINE_SKIP = 15;
    const int VALIDS_PER_VPAGE = 8;    // Valids per vertical page
    const int LETTERWIDTH = 10;
    const int TEXTWIDTH = 330;
    const int TEXTHEIGHT = 310;
    const int HORIZ_SKIP = 400;        // Horizontal skip for the next
                                       // ticket on the same vertical
				       // position.

    const QString FONT_FAMILY("Courier");
    const int FONT_SIZE = 12;
}

#ifndef DEBUG_MODE

class agent_set_params_dialog : public QDialog,
                                public Ui::Set_Params_Dialog
{
    Q_OBJECT

    public:
        agent_set_params_dialog(QWidget *parent = 0);

    private slots:
        void invoke_fileDialog();
	void accept(void);

    private:

	// Comparisons should be case insensitive.
	//
	char fileName_str[VALIDS::PATHNAME_MAXLEN]; 
	char fieldName_str[VALIDS::FIELDNAME_MAXLEN]; 
	char fieldValue_str[VALIDS::FIELDLEN_MAXLEN];
};
#endif     // DEBUG_MODE

#endif

