#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "menu.h"
#include "tour.h"

namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 * Handles menu and tour so that they will appear in the same window (i.e. will be the parent of Menu and Tour objects)
 * @author Aviva Prins
 * @date 3/5/2018
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:


private:
    Ui::MainWindow *ui;
    QWidget* parent;
};

#endif // MAINWINDOW_H
