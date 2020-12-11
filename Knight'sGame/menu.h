#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include "tour.h"

#include <QPushButton>
#include <QLabel>
#include <QGridLayout>


class Menu : public QWidget
{
    Q_OBJECT
friend class Tour;
public:
    Menu(QWidget* parent = 0);                      // Displays the menu, and connects the buttons to Tour games
public slots:
    void startGame1();                              // Constructs and starts the first level.
    void startGame2();                              // Constructs and starts the second level.
    void startGame3();                              // Constructs and starts the third level.
    void startGame4();                              // Constructs and starts the fourth level.
    void startGame5();                              // Constructs and starts the fifth level.
    void startGame6();                              // Constructs and starts the sixth level.
    void displayMenu();                             // Displays the menu
    void instructions();                            // Opens a window with instructions
    void closeInstructions();                       // Closes the instructions window
private:
    QLabel* title;                                  // The title, which will appear in the menu
    QLabel* authors;                                // The authors, which will appear in the menu
    QPushButton* level1;                            // When pressed, calls startGame1()
    QPushButton* level2;                            // When pressed, calls startGame2()
    QPushButton* level3;                            // When pressed, calls startGame3()
    QPushButton* level4;                            // When pressed, calls startGame4()
    QPushButton* level5;                            // When pressed, calls startGame5()
    QPushButton* level6;                            // When pressed, calls startGame6()
    QPushButton* instructionsButton;                // When pressed, calls instructions()

    QGridLayout* mainMenu;                          // Combines title, authors, and level buttons into the menu screen.
    QWidget* parent;                                // MainWindow.  Required for proper handling of children.
    Tour* currentGame = new Tour();                 // Initializes the game. Required for proper memory management.
    QWidget* instructionsWindow;                    // Window containing the instructions
};


#endif // MENU_H
