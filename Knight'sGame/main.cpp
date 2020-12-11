#include "mainwindow.h"
#include "menu.h"
#include "tour.h"
#include <QApplication>

/**
 * @brief Knight's Tour
 * @author Aviva Prins and Rosemary He
 * @date 2/20/2018
 *
 * "I pledge that I have neither given nor received unauthorized assistance on this assignment."
 */

int main( int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow* game = new MainWindow();
    game->show();
    return a.exec();
}
