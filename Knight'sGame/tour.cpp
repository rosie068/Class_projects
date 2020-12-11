#include <QtGui>
#include "tour.h"

/**
 * @brief Tour main game class
 * Tour generates and adds all funtionality of playing a level of the game, and adds all functionality of playing the game (menu windows, etc.)
 * Tour inherits from QWidget class
 * @author Aviva Prins (and Rosemary He where noted)
 * Aviva Prins is the author of this class and merged Rosemary's code.
 * @date 2/20/2018
 */

/**
 * @brief Tour::Tour
 * Purely for child-parent functionality in MainWindow class.
 * @author Aviva Prins
 */
Tour::Tour() {}

/**
 * @brief Tour::Tour constructs a new game
 * @param menuObject is required to switch between Menu and Tour.
 * @param newLevel specifies the level to be created. Default is level 1 (for testing purposes)
 * @param parent QWidget pointer to parent widget. Default 0 creates a new parent.
 * @author Rosemary He, with help from Aviva Prins
 */
Tour::Tour(QWidget* menuObject, int newLevel, QWidget* parent) : QWidget(parent) {
    //Parent-child menu functionality
    parentWindow = parent;
    menu = menuObject;
    window = new QWidget;

    QMediaPlayer* player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/start.mp3"));
    player->play();
    level = newLevel;

    // Each level has a hard coded solution and other randomly-generated rocks.
    if (newLevel == 1) {
        boardLength = 10;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[99] = Goal;
        // Ensure that there is a solution
        state[12] = Rock;
        state[24] = Rock;
        state[45] = Rock;
        state[66] = Rock;
        state[87] = Rock;
        parent->setWindowTitle("Level 1");
    }

    else if (newLevel == 2) {
        boardLength = 10;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[99] = Goal;
        // Ensure that there is a solution
        state[21] = Rock;
        state[42] = Rock;
        state[34] = Rock;
        state[46] = Rock;
        state[38] = Rock;
        state[59] = Rock;
        state[78] = Rock;
        parent->setWindowTitle("Level 2");
    }

    else if (newLevel == 3) {
        boardLength = 10;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[99] = Goal;
        // Ensure that there is a solution
        state[12] = Rock;
        state[4] = Rock;
        state[16] = Rock;
        state[8] = Rock;
        state[29] = Rock;
        state[48] = Rock;
        state[67] = Rock;
        state[75] = Rock;
        state[87] = Rock;
        parent->setWindowTitle("Level 3");
    }

    else if (newLevel == 4) {
        boardLength = 15;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[224] = Goal;
        // Ensure that there is a solution
        state[17] = Rock;
        state[48] = Rock;
        state[65] = Rock;
        state[82] = Rock;
        state[113] = Rock;
        state[142] = Rock;
        state[159] = Rock;
        state[176] = Rock;
        state[193] = Rock;
        parent->setWindowTitle("Level 4");
    }

    else if (newLevel == 5) {
        boardLength = 15;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[224] = Goal;
        // Ensure that there is a solution
        state[31] = Rock;
        state[60] = Rock;
        state[77] = Rock;
        state[94] = Rock;
        state[111] = Rock;
        state[142] = Rock;
        state[171] = Rock;
        state[188] = Rock;
        state[189] = Rock;
        state[201] = Rock;
        state[206] = Rock;
        state[218] = Rock;
        state[220] = Rock;
        parent->setWindowTitle("Level 5");
    }

    else if (newLevel == 6) {
        boardLength = 15;
        // Initial board value: Lava in all positions
        for (int i = 0; i < boardLength*boardLength; i++) {
            state.push_back(Lava); // All positions are lava initially
        }
        // Place the Knight
        playerPos = 0;
        state[0] = Knight;
        // Place the Goal
        state[224] = Goal;
        // Ensure that there is a solution
        state[17] = Rock;
        state[5] = Rock;
        state[22] = Rock;
        state[34] = Rock;
        state[39] = Rock;
        state[70] = Rock;
        state[101] = Rock;
        state[118] = Rock;
        state[132] = Rock;
        state[149] = Rock;
        state[163] = Rock;
        state[194] = Rock;
        state[207] = Rock;
        parent->setWindowTitle("Level 6");
    }
    // Randomly replace Lava with Rocks
    if (boardLength == 10) {
    for (int i = 0; i < 25 ; i++) { // We want to place no more than 25 Rocks
        int randPos = randInt(1,boardLength*boardLength-1); // Choose the location of the Rocks
        if (state[randPos] == Lava) { // Ensure that we only change Lava into Rocks (and not the Knight or the Goal)
            state[randPos] = Rock;
        }
    }
    }
    else {
        for (int i = 0; i < 60 ; i++) { // We want to place no more than 60 Rocks
            int randPos = randInt(1,boardLength*boardLength-1); // Choose the location of the Rocks
            if (state[randPos] == Lava) { // Ensure that we only change Lava into Rocks (and not the Knight or the Goal)
                state[randPos] = Rock;
            }
        }
    }

    update();
}

/**
 * @brief Tour::displayMenu
 * @param sentinel value 0 indicates jumped in lava, 1 indicates saved the princess.
 * Displays the endMenu at the end of the game.
 * @author Aviva Prins
 * Aviva implemented the end menu
 * Rosemary implemented the "next level" button and implemented audio
 */
void Tour::displayMenu(int sentinel){
    // White background
    QPalette background;
    background.setBrush(QPalette::Background, Qt::white);
    window->setPalette(background);

    // Red text indicating win or loss
    QLabel *label = new QLabel();
    if (sentinel == 0){
        label->setText( "<font size=14><font color= red><b> DUN DUN DUN </b></font>" );
    }
    else if (sentinel == 1) {
        label->setText( "<font size=14><font color= red><b> YOU WON! </b></font>" );
    }
    label->setTextFormat( Qt::RichText );
    label->setAlignment(Qt::AlignCenter);

    // Restart button
    QPushButton *restart = new QPushButton("RESTART");
    QFont font = restart->font();
    font.setStyleStrategy(QFont::ForceOutline);
    font.setPointSize(24);
    QPixmap rock(":/stone.jpg");
    QPalette rockBackground;
    rockBackground.setBrush(restart->backgroundRole(), QBrush(rock));
    rockBackground.setColor(restart->foregroundRole(), Qt::white);
    restart->setFont(font);
    restart->setMinimumSize(200,200);
    rockBackground.setBrush(restart->backgroundRole(), QBrush(rock));
    restart->setFlat(true);
    restart->setAutoFillBackground(true);
    restart->setPalette(rockBackground);
    restart->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (level == 1) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame1()));
    }
    if (level == 2) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame2()));
    }
    if (level == 3) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame3()));
    }
    if (level == 4) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame4()));
    }
    if (level == 5) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame5()));
    }
    if (level == 6) {
        QObject::connect(restart, SIGNAL(released()), menu, SLOT(startGame6()));
    }
    QObject::connect(restart, SIGNAL(released()), this, SLOT(quit()));

    // Next button
    QPushButton *next = new QPushButton("NEXT LEVEL");
    next->setFont(font);
    next->setMinimumSize(200,200);
    rockBackground.setBrush(next->backgroundRole(), QBrush(rock));
    next->setFlat(true);
    next->setAutoFillBackground(true);
    next->setPalette(rockBackground);
    next->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    if (level == 1) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(startGame2()));
    }
    if (level == 2) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(startGame3()));
    }
    if (level == 3) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(startGame4()));
    }
    if (level == 4) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(startGame5()));
    }
    if (level == 5) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(startGame6()));
    }
    if (level == 6) {
        QObject::connect(next, SIGNAL(released()), menu, SLOT(displayMenu()));
    }
    QObject::connect(next, SIGNAL(released()), this, SLOT(quit()));

    // Menu button
    QPushButton *mainMenu = new QPushButton("MENU");
    mainMenu->setFont(font);
    mainMenu->setMinimumSize(200,200);
    rockBackground.setBrush(mainMenu->backgroundRole(), QBrush(rock));
    mainMenu->setFlat(true);
    mainMenu->setAutoFillBackground(true);
    mainMenu->setPalette(rockBackground);
    mainMenu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(mainMenu, SIGNAL(released()), menu, SLOT(displayMenu()));
    QObject::connect(mainMenu, SIGNAL(released()), this, SLOT(quit()));

    // Let's add the appropriate buttons and images!
    QGridLayout *layout = new QGridLayout;
    QLabel* image = new QLabel;
    image->setMinimumSize(250,250);

    if (sentinel == 0) {
        QMediaPlayer* player = new QMediaPlayer;
        player->setMedia(QUrl("qrc:/lava.mp3"));
        player->play();

        window->setWindowTitle("You lost!");
        QPixmap won(":/dead.jpg");
        image->setPixmap(won);
        image->setMaximumWidth(250);
        image->setScaledContents(true);
        layout->addWidget(image,1,0,-1,1);
        layout->addWidget(restart,1,1);
        layout->addWidget(mainMenu,2,1);

    }
    else if (sentinel == 1) {
    QMediaPlayer* player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/cheer.mp3"));
    player->play();

        window->setWindowTitle("Congratulations. You won!");
        QPixmap won(":/youWon.jpeg");
        image->setPixmap(won);
        layout->addWidget(image,1,0,1,-1);
        layout->addWidget(next,2,0);
        layout->addWidget(mainMenu,2,1);
    }

    layout->addWidget(label,0,0,1,-1);

    window->setLayout(layout);
    window->show();
}

/**
 * @brief Tour::quit
 * closes the pop-up window that appears at the end of the game.
 * @author Aviva Prins
 */
void Tour::quit(){
    window->close();
}

/**
 * @brief Tour::mousePressEvent Inherited function from QWidget that handles clicks.
 * @param event
 * Handles the movement of the knight: checks clicked square
 * If the knight can't move there, does nothing.
 * If lava is clicked, ends game.
 * If a viable square is clicked, moves the knight.
 * Checks if the knight wins
 * @author Rosemary He
 */
void Tour::mousePressEvent(QMouseEvent* event) {
    for (int pos = 0; pos < boardLength*boardLength; ++pos) {
        QRect cell = cellRect( pos / boardLength, pos % boardLength);

        if (cell.contains(event->pos())) {
            // 1. Clicked on Lava -> DIE
            if (state[pos] == Lava) {
                displayMenu(0);
            }
            if (boardLength == 10) {
            // Check if the click corresponds to a valid move according to the rules of the knight's tour in chess
            if (pos == playerPos + 12 || pos == playerPos - 12 || pos == playerPos + 21 ||
                    pos == playerPos - 19 ||pos == playerPos + 8 || pos == playerPos - 8 ||
                    pos == playerPos + 19 || pos == playerPos - 21) {


                // 1. Clicked on Lava -> DIE
                if (state[pos] == Lava) {
                    displayMenu(0);
                }

                // 2. Clicked on Goal -> WIN
                else if (state[pos] == Goal) {
                    displayMenu(1);
                }

                // 3. Clicked on Rock -> Move knight
                else if (state[pos] == Rock) {
                    //QMediaPlayer* music = new QMediaPlayer();
                    //music->setMedia(QUrl(":/rock.mp3"));
                    //music -> play();
                    state[playerPos] = Visited;
                    state[pos] = Knight;
                    playerPos = pos;
                }

                // Otherwise, clicked on Visited or Knight (do nothing).
                else {}
                update();
            }
            }
            else if (boardLength == 15) {
            // Check if the click corresponds to a valid move according to the rules of the knight's tour in chess
            if (pos == playerPos + 31 || pos == playerPos - 31 || pos == playerPos + 29 ||
                    pos == playerPos - 29 ||pos == playerPos + 17 || pos == playerPos - 17 ||
                        pos == playerPos + 13 || pos == playerPos - 13) {


                // 1. Clicked on Lava -> DIE
                if (state[pos] == Lava) {
                    displayMenu(0);
                }

                // 2. Clicked on Goal -> WIN
                else if (state[pos] == Goal) {
                    displayMenu(1);
                }

                // 3. Clicked on Rock -> Move knight
                else if (state[pos] == Rock) {
                    state[playerPos] = Visited;
                    state[pos] = Knight;
                    playerPos = pos;
                }

                // Otherwise, clicked on Visited or Knight (do nothing).
                else {}
                update();
            }
            }
        else {
            update();
        }
    }
    else {}
    }
}

/**
 * @brief Tour::paintEvent Inherited function from QWidget that draws the game board after every update().
 * @param event unused parameter, passed in due to inheritance
 * Draws squares corresponding to lava, open tiles, visited tiles (not open), the Knight/player and the Goal tile.
 * @author Rosemary He
 */
void Tour::paintEvent(QPaintEvent* event) {
    setMinimumSize(parentWindow->size());
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Draw Grid
    painter.setPen(QPen(Qt::black,3));

    for (int i = 1; i < boardLength; ++i) {
        painter.drawLine(i* cellWidth(), 0 , i*cellWidth(), height());
        painter.drawLine(0, i*cellHeight(), width(), i*cellHeight());
    }

    // Draw board contents
    painter.setPen(QPen(Qt::black,2));

    for (int pos = 0; pos < boardLength*boardLength; ++pos) {
        QRect cell = cellRect(pos / boardLength , pos % boardLength);

        if (state[pos] == Rock) {
            QPixmap pixmap(":/stone.jpg");
            painter.drawPixmap(cell,pixmap);
        }

        else if (state[pos] == Knight) {
            QPixmap pixmap(":/knight.jpg");
            painter.drawPixmap(cell,pixmap);
        }

        else if (state[pos] == Goal) {
            QPixmap pixmap(":/goal.jpg");
            painter.drawPixmap(cell,pixmap);
        }

        else if (state[pos] == Visited) {
            QPixmap pixmap(":/visitedStone.png");
            painter.drawPixmap(cell,pixmap);
        }
    }
}

/**
 * @brief Tour::cellRect
 * @param row the row # within the game board
 * @param column the column # within the game board
 * @return a rectangle at the (column, row) position
 * This makes coding easier and avoids silly mistakes.
 * @author Rosemary He
 */
QRect Tour::cellRect(int row, int column) const {
    const int HMargin = width() / 1000;
    const int VMargin = height() / 1000;
    return QRect(column * cellWidth() + HMargin, row * cellHeight() + VMargin, cellWidth() - 2 * HMargin, cellHeight() - 2 * VMargin);
}

/**
 * @brief randInt generates a random number.
 * @param low minimum value
 * @param high maximum value
 * @return a random integer between low and high.
 * @author Rosemary He
 */
int Tour::randInt(int low, int high) {
    return qrand() % ((high + 1) - low) + low;
}
