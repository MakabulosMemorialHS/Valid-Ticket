/* ***********************************************************************
 * main.cpp
 *
 * robert pascual
 *
 * The main source code.
 *
 * Revision History:
 *
 * $Id: main.cpp,v 1.1 2011/12/12 11:05:50 robert Exp $
 *
 * $Log: main.cpp,v $
 * Revision 1.1  2011/12/12 11:05:50  robert
 * Initial import into CVS.
 *
 *
 * *********************************************************************/
#include <QApplication>
#include <QDialog>
#include "Tickets.hpp"
#include "Valids.hpp"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    agent_set_params_dialog *app_dialog = new agent_set_params_dialog();
    app_dialog->show();

    return app.exec();
}

