// ************************************************************************** //
//
//  Steca: stress and texture calculator
//
//! @file      gui/steca.cpp
//! @brief     Implements the main program
//!
//! @homepage  https://github.com/scgmlz/Steca
//! @license   GNU General Public License v3 or higher (see COPYING)
//! @copyright Forschungszentrum Jülich GmbH 2016-2018
//! @authors   Scientific Computing Group at MLZ (see CITATION, MAINTAINER)
//
// ************************************************************************** //

#include "../manifest.h"
#include "console.h"
#include "mainwin.h"
#include "cfg/msg_handler.h"
#include "session.h"
#include <iostream>
#define OPTPARSE_IMPLEMENTATION
#define OPTPARSE_API static
#include "optparse.h"
#include <QApplication>
#include <QStyleFactory>

const char* version =
#include "../VERSION"
    ;

class MainWin* gMainWin; //!< global, for message handling
class Session* gSession; //!< global, for data handling

int main(int argc, char* argv[]) {

    struct optparse options;
    optparse_init(&options, argv);
    int opt;
    while ((opt = optparse(&options, "hvc")) != -1) {
        switch (opt) {
        case 'h':
            std::cout << APPLICATION_CLAIM << "\n\n"
                      << "Usage: " << APPLICATION_NAME << " [options]\n\n"
                      << "Options:\n"
                      << "  -h  Print this message.\n"
                      << "  -v  Print " << APPLICATION_NAME << " version.\n"
                      << "  -c  Read commands from console instead of starting the GUI.\n";
            exit(0);
        case 'v':
            std::cout << APPLICATION_NAME << " version " << version << "\n";
            exit(0);
        }
    }
    if (optparse_arg(&options)) {
        std::cerr << "Unexpected command-line argument(s) given\n";
        exit(-1);
    }

    QApplication app(argc, argv);

    app.setApplicationName(APPLICATION_NAME);
    app.setApplicationVersion(version);
    app.setOrganizationName(ORGANIZATION_NAME);
    app.setOrganizationDomain(ORGANIZATION_DOMAIN);

#if defined(Q_OS_OSX)
    app.setStyle(QStyleFactory::create("Macintosh"));
#elif defined(Q_OS_WIN)
    app.setStyle(QStyleFactory::create("Fusion"));
#else
    app.setStyle(QStyleFactory::create("Fusion"));
#endif

    qInstallMessageHandler(messageHandler);

    gSession = Session::instance();

    gMainWin = MainWin::instance();
    gMainWin->show();
    qDebug() /* qInfo() TODO restore */ << "Welcome to Steca";

    Console console;
    QObject::connect(&console, &Console::transmitLine, gMainWin, &MainWin::execCommand);

    return app.exec();
}
