#ifndef TOUR_H
#define TOUR_H
/**
 * @brief Tour main game class
 * Tour generates and adds all funtionality of playing a level of the game, and adds all functionality of playing the game (menu windows, etc.)
 * Tour inherits from QWidget class
 * @author Aviva Prins and Rosemary He
 * @date 2/20/2018
 */

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QStyle>
#include <QDesktopWidget>
#include <QLabel>
#include <QMediaPlayer>

class QRect;
class QSize;

class Tour : public QWidget {
    Q_OBJECT

public:
    Tour();
    Tour(QWidget* menuObject, int newLevel = 1, QWidget* parent = 0);    // Constructor creates a new game. Contains level layouts corresponding to newLevel's value.

protected:
    void mousePressEvent(QMouseEvent* event);       // Inherited function from QWidget that handles clicks. Here, it handles the game logic of moving the knight, including checking for win condition
    void paintEvent(QPaintEvent *event);            // Inherited function from QWidget that draws the game board after every update(). Here, it draws the board (replaces open squares with visited).

private slots:
     void displayMenu(int sentinel);                // Uses endMenu to display the choices Restart or Menu
     void quit();

private:
    // Private data fields
    QVector<QChar> state;                          // Stores the game layout.
    int level = 1;                                 // Stores the current level #. See above for corresponding level layout, QString level#.
    int playerPos;                                 // Stores the player's position.

    // Constructor Values
    enum { Lava = 'L', Visited = 'x', Rock = 'R', Goal = 'G', Knight = 'K' }; // See state, above.
    int boardLength = 10;                          // All boards are square. boardLength stores the length of one side.

    // Behind the scenes: key functions for the construction of the board
    QRect cellRect(int row, int col) const;         // Used to generate each cell on the board for easier coding.
    int cellWidth() const {return parentWindow->width()/boardLength; }    // Dynamic width allows resizing the window mid-game.
    int cellHeight() const {return parentWindow->height()/boardLength; }  // Dynamic height allows resizing the window mid-game.
    QSize sizeHint() const {return parentWindow->size();}                 // Uses the parent window to deterimine the size (although rescaling is allowed).
    int randInt(int low, int high);                 // Random integer generator, used in board construction (for random tiles)

    // Behind the scenes: child and parent handling
    QWidget* parentWindow;                          // Corresponds to MainWindow
    QWidget* menu;                                  // Corresponds to Menu
    QWidget* window;                                // Corresponds to the end game popup window
};

#endif // TOUR_H
