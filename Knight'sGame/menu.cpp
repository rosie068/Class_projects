#include "menu.h"

/**
 * @brief Menu::Menu
 * @param parent is the MainWindow
 * Creates the main menu layout
 * @author Aviva Prins and Rosemary He
 */
Menu::Menu(QWidget* parent) : QWidget(parent) {
    this->parent = parent;
    qsrand(time(nullptr));
    instructionsWindow = new QWidget;

    // Widget settings
    parent->setWindowTitle("Knight's Tour");
    setMinimumSize(1000,800);

    // Title and font settings
    title = new QLabel("Knight's Tour");
    title->setTextFormat(Qt::RichText);
    title->setAlignment(Qt::AlignCenter);
    QFont font = title->font();
    font.setStyleStrategy(QFont::ForceOutline);
    font.setPointSize(72);
    title->setFont(font);
    title->hasScaledContents();
    title->setMinimumSize(200,50);

    // Font settings, continued.
    QPalette fontPalette = title->palette();
    fontPalette.setColor(title->backgroundRole(), Qt::white);
    fontPalette.setColor(title->foregroundRole(), Qt::white);
    title->setPalette(fontPalette);

    // Authors settings
    authors = new QLabel("Aviva Prins and Rosemary He");
    authors->setTextFormat(Qt::RichText);
    authors->setAlignment(Qt::AlignCenter);
    font.setPointSize(40);
    authors->setFont(font);
    authors->hasScaledContents();
    authors->setMinimumSize(200,50);
    authors->setPalette(fontPalette);

    // Button background settings
    QPixmap rock(":/stone.jpg");
    QPalette rockBackground;

    // Level 1 button settings
    level1 = new QPushButton("Level 1", this);
    font.setPointSize(24);
    fontPalette.setColor(title->backgroundRole(), Qt::white);
    fontPalette.setColor(title->foregroundRole(), Qt::white);
    level1->setFont(font);
    level1->setMinimumSize(200,50);
    rockBackground.setBrush(level1->backgroundRole(), QBrush(rock));
    rockBackground.setColor(level1->foregroundRole(), Qt::white);
    level1->setFlat(true);
    level1->setAutoFillBackground(true);
    level1->setPalette(rockBackground);
    level1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level1, SIGNAL(released()), this, SLOT(startGame1()));

    // Level 2 button settings
    level2 = new QPushButton("Level 2", this);
    level2->setFont(font);
    level2->setMinimumSize(200,50);
    rockBackground.setBrush(level2->backgroundRole(), QBrush(rock));
    level2->setFlat(true);
    level2->setAutoFillBackground(true);
    level2->setPalette(rockBackground);
    level2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level2, SIGNAL(released()), this, SLOT(startGame2()));

    // Level 3 button settings
    level3 = new QPushButton("Level 3", this);
    level3->setFont(font);
    level3->setMinimumSize(200,50);
    rockBackground.setBrush(level3->backgroundRole(), QBrush(rock));
    level3->setFlat(true);
    level3->setAutoFillBackground(true);
    level3->setPalette(rockBackground);
    level3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level3, SIGNAL(released()), this, SLOT(startGame3()));

    // Level 4 button settings
    level4 = new QPushButton("Level 4", this);
    level4->setFont(font);
    level4->setMinimumSize(200,50);
    rockBackground.setBrush(level4->backgroundRole(), QBrush(rock));
    level4->setFlat(true);
    level4->setAutoFillBackground(true);
    level4->setPalette(rockBackground);
    level4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level4, SIGNAL(released()), this, SLOT(startGame4()));

    // Level 5 button settings
    level5 = new QPushButton("Level 5", this);
    level5->setFont(font);
    level5->setMinimumSize(200,50);
    rockBackground.setBrush(level5->backgroundRole(), QBrush(rock));
    level5->setFlat(true);
    level5->setAutoFillBackground(true);
    level5->setPalette(rockBackground);
    level5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level5, SIGNAL(released()), this, SLOT(startGame5()));

    // Level 6 button settings
    level6 = new QPushButton("Level 6", this);
    level6->setFont(font);
    level6->setMinimumSize(200,50);
    rockBackground.setBrush(level6->backgroundRole(), QBrush(rock));
    level6->setFlat(true);
    level6->setAutoFillBackground(true);
    level6->setPalette(rockBackground);
    level6->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(level6, SIGNAL(released()), this, SLOT(startGame6()));

    // Instructions button
    instructionsButton = new QPushButton("Instructions", this);
    instructionsButton->setFont(font);
    instructionsButton->setMinimumSize(200,50);
    rockBackground.setBrush(instructionsButton->backgroundRole(), QBrush(rock));
    instructionsButton->setFlat(true);
    instructionsButton->setAutoFillBackground(true);
    instructionsButton->setPalette(rockBackground);
    instructionsButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(instructionsButton, SIGNAL(released()), this, SLOT(instructions()));

    // Now that we've made everything beautiful, let's arrange them!
    // Sets the layout of the buttons
    QGridLayout* levelLayout = new QGridLayout();
    levelLayout->addWidget(level1,1,1);
    levelLayout->addWidget(level2,1,2);
    levelLayout->addWidget(level3,1,3);
    levelLayout->addWidget(level4,2,1);
    levelLayout->addWidget(level5,2,2);
    levelLayout->addWidget(level6,2,3);
    levelLayout->addWidget(instructionsButton, 3, 1, 1, -1);
    levelLayout->setVerticalSpacing(25);
    levelLayout->setHorizontalSpacing(50);

    // Create mainMenu and show it
    mainMenu = new QGridLayout(this);
    mainMenu->setContentsMargins(25,25,25,25);
    mainMenu->addWidget(title,1,1,1,-1);
    mainMenu->addWidget(authors,2,1,1,-1);
    mainMenu->addLayout(levelLayout,3,1,3,2);
    this->setLayout(mainMenu);

}

/**
 * @brief Menu::startGame1
 * Starts the first game.
 * @author Aviva Prins
 */
void Menu::startGame1() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 1, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::startGame2
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::startGame2() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 2, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::startGame3
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::startGame3() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 3, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::startGame4
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::startGame4() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 4, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::startGame5
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::startGame5() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 5, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::startGame6
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::startGame6() {
    currentGame->close();
    Tour* gameNew = new Tour(this, 6, parent);
    currentGame = gameNew;
    gameNew->show();
    this->close();
}

/**
 * @brief Menu::displayMenu
 * Starts the game.
 * @author Aviva Prins
 */
void Menu::displayMenu() {
    parent->setWindowTitle("Knight's Tour");
    this->show();
    currentGame->close();
}

/**
 * @brief Menu::instructions
 * Displays the instructions window
 * @author Aviva Prins
 */
void Menu::instructions() {
    instructionsWindow->setWindowTitle("Instructions");
    instructionsWindow->setMinimumWidth(700);

    // Background settings
    QPixmap lava(":/lava.png");
    QPalette background;
    background.setBrush(QPalette::Window, lava);
    instructionsWindow->setPalette(background);

    QPixmap rock(":/stone.jpg");
    QPalette rockBackground;

    QLabel* title = new QLabel("The floor is lava! Save the princess before it is too late.");
    title->setWordWrap(true);
    QLabel* paragraph = new QLabel("Welcome, brave knight. Your quest is to save the princess by hopping from rock to rock. Reach her before she sinks! You can move according to the rules of the knight in chess.");
    paragraph->setWordWrap(true);

    QFont font = title->font();
    font.setStyleStrategy(QFont::ForceOutline);
    QPalette fontPalette = title->palette();
    fontPalette.setColor(title->backgroundRole(), Qt::white);
    fontPalette.setColor(title->foregroundRole(), Qt::white);
    title->setPalette(fontPalette);

    font.setPointSize(36);
    title->setFont(font);
    title->hasScaledContents();
    title->setMinimumSize(200,50);
    font.setPointSize(24);
    paragraph->setFont(font);
    paragraph->setPalette(fontPalette);

    // Close
    QPushButton* closeWindow = new QPushButton("GOT IT", instructionsWindow);

    closeWindow->setFont(font);
    closeWindow->setMinimumSize(200,50);
    fontPalette.setColor(closeWindow->backgroundRole(), Qt::white);
    fontPalette.setColor(closeWindow->foregroundRole(), Qt::white);
    closeWindow->setFont(font);
    closeWindow->setPalette(fontPalette);
    closeWindow->setFlat(true);
    closeWindow->setAutoFillBackground(true);
    rockBackground.setBrush(closeWindow->backgroundRole(), QBrush(rock));
    rockBackground.setColor(closeWindow->foregroundRole(), Qt::white);
    closeWindow->setPalette(rockBackground);
    closeWindow->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QObject::connect(closeWindow, SIGNAL(released()), this, SLOT(closeInstructions()));

    QLabel* image = new QLabel;
    QPixmap moves(":/knightSchematic.png");
    image->setPixmap(moves);
    image->setMaximumWidth(200);
    image->setScaledContents(false);
    image->setAlignment(Qt::AlignCenter);

    QGridLayout* layout = new QGridLayout;
    layout->addWidget(title, 1, 1,1,-1);
    layout->addWidget(paragraph, 2, 1);
    layout->addWidget(image, 2, 2);
    layout->addWidget(closeWindow, 4,1,1,-1);
    instructionsWindow->setLayout(layout);

    instructionsWindow->show();
}

/**
 * @brief Menu::closeInstructions
 * Closes the instructions window and deletes all its contents
 * @author Aviva Prins
 */
void Menu::closeInstructions(){
    delete instructionsWindow;
    instructionsWindow = new QWidget;
}


