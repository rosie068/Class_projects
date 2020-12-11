#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * Constructor
 * @param parent
 * @author Aviva Prins
 * @date 3/5/2018
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Background settings
    QPixmap lava(":/lava.png");
    QPalette background;
    background.setBrush(QPalette::Window, lava);
    this->setPalette(background);

    // Initialize and show
    ui->setupUi(this);
    this->parent = parent;
    Menu* game = new Menu(this);
    setCentralWidget(game);
    this->show();

}

/**
 * @brief MainWindow::~MainWindow
 * Destructor.
 * @author Aviva Prins
 */
MainWindow::~MainWindow()
{
    delete ui;
}
