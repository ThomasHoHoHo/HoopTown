#include "gamewindow.h"
#include "userdatabase.h"
#include "user.h"
#include <QCloseEvent>
#include <Qt>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPixmap>
#include <QHeaderView>
#include <QDateTime>
#include <QPushButton>
#include <QCloseEvent>
#include <QRandomGenerator>
#include <QFile>
#include <QPainter>
#include <QTimer>

//default constructor
GameWindow::GameWindow(QWidget *parent): GameWindow(User(), parent) {
    setupMenuPage();
    setupGamePage();
    setupScoreHistoryPage();
    setupScoreFlash();
}

//Constructor for user and guest and intializes the base class, Intializes the player
GameWindow::GameWindow(const User &user, QWidget *parent)
    : QMainWindow(parent), currentUser(user), gameRunning(false),
    player1LeftPressed(false), player1RightPressed(false),
    player2LeftPressed(false), player2RightPressed(false)
{
    //used to switch screens
    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    //set up the pages
    setupMenuPage();
    setupGamePage();
    setupScoreHistoryPage();

    //connect movement keys
    setupPlayerControls();

    //Timer for 1 min game
    remainingSeconds = 60;
    countdownTimer  = new QTimer(this);
    connect(countdownTimer, &QTimer::timeout, this, &GameWindow::updateCountdown);

    //scoreboard flash
    setupScoreFlash();

    //adds game views to the stacked widget
    stackedWidget->addWidget(menuPage);
    stackedWidget->addWidget(gamePage);
    stackedWidget->addWidget(scoreHistoryPage);

    //starts main menu
    showMainMenu();

    // Check for birthday
    if (currentUser.isBirthday()) {
        showBirthdayGreeting();
    }

    // Update user info display
    updateUserInfo();
}

void GameWindow::setupPlayerControls() {
    // Focus policy for key events,, how the window will handle the key inputs
    setFocusPolicy(Qt::StrongFocus);

    // Setup player movement timer for continuous movement
    playerMoveTimer = new QTimer(this);
    connect(playerMoveTimer, &QTimer::timeout, this, &GameWindow::movePlayer);
    playerMoveTimer->start(16); // ~60fps for smooth movement
}


//for player movmement keys
void GameWindow::keyPressEvent(QKeyEvent *event) {
    if (stackedWidget->currentWidget() != gamePage) {
        QMainWindow::keyPressEvent(event);
        return;
    }

    // Player 1 controls (WASD, Space, E to steal, W to jump)
    if (event->key() == Qt::Key_A) {
        player1LeftPressed = true;
    } else if (event->key() == Qt::Key_D) {
        player1RightPressed = true;
    } else if (event->key() == Qt::Key_W) {  // Jump
        if (scene && gameRunning) scene->player1Jump();
    } else if (event->key() == Qt::Key_Space) {
        if (scene && gameRunning) scene->player1Shoot();
    } else if (event->key() == Qt::Key_E) { // Steal
        if (scene && gameRunning) scene->attemptSteal(1);
    }

    // Player 2 controls (Arrows, Enter, M to steal, Up to jump)
    if (event->key() == Qt::Key_Left) {
        player2LeftPressed = true;
    } else if (event->key() == Qt::Key_Right) {
        player2RightPressed = true;
    } else if (event->key() == Qt::Key_Up) {  // Jump
        if (scene && gameRunning) scene->player2Jump();
    } else if (event->key() == Qt::Key_Return) {
        if (scene && gameRunning) scene->player2Shoot();
    } else if (event->key() == Qt::Key_M) { // Steal
        if (scene && gameRunning) scene->attemptSteal(2);
    }

    QMainWindow::keyPressEvent(event);
}

//Event handler for when a key os released
void GameWindow::keyReleaseEvent(QKeyEvent *event) {
    // Player 1 controls
    if (event->key() == Qt::Key_A) {
        player1LeftPressed = false;
    } else if (event->key() == Qt::Key_D) {
        player1RightPressed = false;
    }

    // Player 2 controls
    if (event->key() == Qt::Key_Left) {
        player2LeftPressed = false;
    } else if (event->key() == Qt::Key_Right) {
        player2RightPressed = false;
    }

    QMainWindow::keyReleaseEvent(event);
}

//move players based on direction
void GameWindow::movePlayer() {
    if (!scene || stackedWidget->currentWidget() != gamePage || !gameRunning) return;

    // Move Player 1 based on key presses
    if (player1LeftPressed) {
        scene->movePlayer1Left();
    }

    if (player1RightPressed) {
        scene->movePlayer1Right();
    }

    // Move Player 2 based on key presses
    if (player2LeftPressed) {
        scene->movePlayer2Left();
    }

    if (player2RightPressed) {
        scene->movePlayer2Right();
    }
}

//triggered by start game button
void GameWindow::startGame() {
    //check for valid scene
    if (scene) {
        scene->startGame();
        gameRunning = true;
        if (startButton) startButton->setEnabled(false);
        if (pauseButton) pauseButton->setEnabled(true);
        remainingSeconds = 60;
        timerLabel->setText("01:00");
        countdownTimer->start(1000);
    }
}

//pauses the game
void GameWindow::pauseGame() {
    //check for valid scene
    if (scene) {
        scene->pauseGame();
        gameRunning = false;
        countdownTimer->stop();
        if (startButton) startButton->setEnabled(true);
        if (pauseButton) pauseButton->setEnabled(false);
    }
}

//resets the game
void GameWindow::resetGame() {
    //check for valid scene
    if (scene) {
        scene->resetGame();
        gameRunning = true; // Immediately start a new game
        if (startButton) startButton->setEnabled(false);
        if (pauseButton) pauseButton->setEnabled(true);
        player1ScoreLabel->setText("Player 1: 0");
        player2ScoreLabel->setText("Player 2: 0");
        totalScoreLabel->setText("Total Score: 0");

        remainingSeconds = 60;
        timerLabel->setText("01:00");
        countdownTimer->start(1000);
    }
}

//
void GameWindow::updateScore(int player1Score, int player2Score) {
    //update score label %1 as a placeholder and gets replaced by arg
    player1ScoreLabel->setText(QString("Player 1: %1").arg(player1Score));
    player2ScoreLabel->setText(QString("Player 2: %1").arg(player2Score));
    //add both together for total
    totalScoreLabel->setText(QString("Total Score: %1").arg(player1Score + player2Score));

    // Save total score to user profile if this is a real user
    if (!currentUser.getIsGuest()) {
        currentUser.addScore(player1Score + player2Score);
        UserDatabase::instance()->updateUser(currentUser);
    }
}

void GameWindow::setupGamePage(){
    // create the page container
    gamePage = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(gamePage);

    // the court background
    view = new QGraphicsView(gamePage);
    scene = new court(this);
    connect(scene,&court::pointsScored,this, &GameWindow::flashScore);
    view->setScene(scene);

    // match view to scene size and turn off scrollbars
    view->setFixedSize(scene->sceneRect().size().toSize());
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Connect the score signal from the court to this class
    connect(scene, &court::scoreChanged, this, &GameWindow::updateScore);

    layout->addWidget(view);

    // --- Game instructions ---
    QLabel *instructionsLabel = new QLabel("Player 1: A/D to move, W to jump, SPACE to shoot, E to steal\nPlayer 2: LEFT/RIGHT to move, UP to jump,ENTER to shoot, M to steal");
    instructionsLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(instructionsLabel);

    // --- Score display ---
    QHBoxLayout *scoreLayout = new QHBoxLayout();

    player1ScoreLabel = new QLabel("Player 1: 0", gamePage);
    player1ScoreLabel->setAlignment(Qt::AlignCenter);
    player1ScoreLabel->setStyleSheet("font-size:16px; font-weight:bold; color:cyan;");

    player2ScoreLabel = new QLabel("Player 2: 0", gamePage);
    player2ScoreLabel->setAlignment(Qt::AlignCenter);
    player2ScoreLabel->setStyleSheet("font-size:16px; font-weight:bold; color:red;");

    totalScoreLabel = new QLabel("Total Score: 0", gamePage);
    totalScoreLabel->setAlignment(Qt::AlignCenter);
    totalScoreLabel->setStyleSheet("font-size:18px; font-weight:bold;");

    scoreLayout->addWidget(player1ScoreLabel);
    scoreLayout->addWidget(player2ScoreLabel);
    scoreLayout->addWidget(totalScoreLabel);

    layout->addLayout(scoreLayout);

    timerLabel = new QLabel("01:00", view->viewport());
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet(R"( font-size: 24px; font-weight: bold; color: white;background: rgba(0,0,0,0);  /* transparent */)");
    timerLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    timerLabel->adjustSize();
    // position it at the top‑center of the view
    timerLabel->move(((view->width() - timerLabel->width())/2)-10,70);
    timerLabel->show();

    // --- Game control buttons ---
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    startButton = new QPushButton("Start Game");
    pauseButton = new QPushButton("Pause");
    resetButton = new QPushButton("New Game");

    connect(startButton, &QPushButton::clicked, this, &GameWindow::startGame);
    connect(pauseButton, &QPushButton::clicked, this, &GameWindow::pauseGame);
    connect(resetButton, &QPushButton::clicked, this, &GameWindow::resetGame);

    pauseButton->setEnabled(false); // Initially disabled until game starts

    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(resetButton);

    layout->addLayout(buttonLayout);

    // Back to menu button
    QPushButton *backBtn = new QPushButton("Back to Menu", gamePage);
    connect(backBtn, &QPushButton::clicked, this, &GameWindow::showMainMenu);
    layout->addWidget(backBtn);
}



//Close the game window and let the login know
void GameWindow::closeEvent(QCloseEvent *event){
    emit closed();
    QMainWindow::closeEvent(event);
}

//create sand laysout the score history page
void GameWindow::setupScoreHistoryPage(){
    scoreHistoryPage = new QWidget();
    //vertically stack the elements
    QVBoxLayout *mainLayout = new QVBoxLayout(scoreHistoryPage);

    QLabel *titleLabel = new QLabel("Score History");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");
    titleLabel->setAlignment(Qt::AlignCenter);

    scoreHistoryTable = new QTableWidget();
    scoreHistoryTable->setColumnCount(3);
    scoreHistoryTable->setHorizontalHeaderLabels({"Score", "Date/Time", "Compared to Global Best"});
    scoreHistoryTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    backToMenuButton = new QPushButton("Back to Menu");
    connect(backToMenuButton, &QPushButton::clicked, this, &GameWindow::showMainMenu);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scoreHistoryTable);
    mainLayout->addWidget(backToMenuButton);
}

//updates the user info on the home screen
void GameWindow::updateUserInfo(){
    welcomeLabel->setText("Welcome, " + currentUser.getFullName() + "!");
    userInfoLabel->setText(currentUser.isGuest ? "Playing as Guest" : "Username: " + currentUser.username);

    // Update profile picture if available
    if (!currentUser.profilePicture.isEmpty() && QFile::exists(currentUser.profilePicture)) {
        QPixmap pixmap(currentUser.profilePicture);
        if (!pixmap.isNull()) {
            // Create a circular mask for the profile icon
            QPixmap roundedPixmap(100, 100);
            roundedPixmap.fill(Qt::transparent);

            QPainter painter(&roundedPixmap);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setBrush(Qt::white);
            painter.drawEllipse(0, 0, 100, 100);

            // Set the mask
            pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
            QBitmap mask = roundedPixmap.createMaskFromColor(Qt::transparent);
            pixmap.setMask(mask);

            profilePicLabel->setPixmap(pixmap);
        }
    } else {
        // Default profile picture
        profilePicLabel->setText("No Image");
        profilePicLabel->setAlignment(Qt::AlignCenter);
    }
}

//shows the birthday greeting
void GameWindow::showBirthdayGreeting(){
    birthdayLabel->setText("🎉 Happy Birthday! 🎂");
    birthdayLabel->setVisible(true);

    QMessageBox::information(this, "Happy Birthday!","Happy Birthday, " + currentUser.getFullName() + "!\n\n""We hope you have a fantastic day filled with joy and happiness!\n");
}


void GameWindow::showMainMenu(){
    stackedWidget->setCurrentWidget(menuPage);
    // Pause the game when going back to menu
    if (gameRunning) {
        pauseGame();
    }
}

void GameWindow::showScoreHistory(){
    updateScoreHistory();
    stackedWidget->setCurrentWidget(scoreHistoryPage);
    // Pause the game when viewing score history
    if (gameRunning) {
        pauseGame();
    }
}

void GameWindow::setupMenuPage(){
    menuPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(menuPage);

    QHBoxLayout *userInfoLayout = new QHBoxLayout();

    profilePicLabel = new QLabel();
    profilePicLabel->setFixedSize(100, 100);
    profilePicLabel->setStyleSheet("border: 1px solid #ccc; border-radius: 50px;");
    profilePicLabel->setScaledContents(true);

    QVBoxLayout *infoTextLayout = new QVBoxLayout();
    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    userInfoLabel = new QLabel();
    userInfoLabel->setStyleSheet("font-size: 16px;");

    currentDateLabel = new QLabel(QDate::currentDate().toString("dddd, MMMM d, yyyy"));
    currentDateLabel->setStyleSheet("font-size: 14px; color: #666;");

    birthdayLabel = new QLabel();
    birthdayLabel->setStyleSheet("font-size: 18px; color: #FF5733; font-weight: bold;");
    birthdayLabel->setVisible(false);

    infoTextLayout->addWidget(welcomeLabel);
    infoTextLayout->addWidget(userInfoLabel);
    infoTextLayout->addWidget(currentDateLabel);
    infoTextLayout->addWidget(birthdayLabel);

    userInfoLayout->addWidget(profilePicLabel);
    userInfoLayout->addLayout(infoTextLayout);
    userInfoLayout->addStretch();

    // Game title
    QLabel *gameTitleLabel = new QLabel("Hoop Town");
    gameTitleLabel->setStyleSheet("font-size: 32px; font-weight: bold; color: #FF5733;");
    gameTitleLabel->setAlignment(Qt::AlignCenter);

    // Game description
    QLabel *gameDescription = new QLabel("Two Player Basketball Game\nPlayer 1: A/D to move, W to jump, SPACE to shoot, E to steal\nPlayer 2: LEFT/RIGHT to move, UP to move, ENTER to shoot, E to steal");
    gameDescription->setAlignment(Qt::AlignCenter);
    gameDescription->setStyleSheet("font-size: 14px; margin-bottom: 20px;");

    playButton = new QPushButton("Play Game");
    playButton->setStyleSheet("font-size: 18px; padding: 10px;");
    connect(playButton, &QPushButton::clicked, this, &GameWindow::showGamePage);

    scoreHistoryButton = new QPushButton("Score History");
    scoreHistoryButton->setStyleSheet("font-size: 18px; padding: 10px;");
    connect(scoreHistoryButton, &QPushButton::clicked, this, &GameWindow::showScoreHistory);

    logoutButton = new QPushButton("Logout");
    logoutButton->setStyleSheet("font-size: 18px; padding: 10px;");
    connect(logoutButton, &QPushButton::clicked, this, &GameWindow::logoutUser);

    mainLayout->addLayout(userInfoLayout);
    mainLayout->addWidget(gameTitleLabel);
    mainLayout->addWidget(gameDescription);
    mainLayout->addSpacing(30);
    mainLayout->addWidget(playButton);
    mainLayout->addWidget(scoreHistoryButton);
    mainLayout->addWidget(logoutButton);
    mainLayout->addStretch();
}


void GameWindow::updateScoreHistory(){
    scoreHistoryTable->setRowCount(0);

    int globalBest = UserDatabase::instance()->getGlobalBestScore();

    for (int i = 0; i < currentUser.scoreHistory.size(); ++i) {
        const GameScore &score = currentUser.scoreHistory[i];

        scoreHistoryTable->insertRow(i);

        // Score
        QTableWidgetItem *scoreItem = new QTableWidgetItem(QString::number(score.score));
        scoreItem->setTextAlignment(Qt::AlignCenter);
        scoreHistoryTable->setItem(i, 0, scoreItem);

        // Date/Time
        QTableWidgetItem *dateItem = new QTableWidgetItem(score.playedAt.toString("yyyy-MM-dd hh:mm:ss"));
        dateItem->setTextAlignment(Qt::AlignCenter);
        scoreHistoryTable->setItem(i, 1, dateItem);

        // Comparison with global best
        QString comparisonText;
        if (globalBest == 0) {
            comparisonText = "No global scores yet";
        } else if (score.score >= globalBest) {
            comparisonText = "Global Best!";
        } else {
            int percentage = (score.score * 100) / globalBest;
            comparisonText = QString("%1% of global best").arg(percentage);
        }

        QTableWidgetItem *comparisonItem = new QTableWidgetItem(comparisonText);
        comparisonItem->setTextAlignment(Qt::AlignCenter);
        scoreHistoryTable->setItem(i, 2, comparisonItem);
    }

    // Sort by date descending (newest first)
    scoreHistoryTable->sortItems(1, Qt::DescendingOrder);
}


void GameWindow::showGamePage()
{
    stackedWidget->setCurrentWidget(gamePage);
    // Give focus to the window for keyboard events
    setFocus();

    // Start a new game automatically when showing the game page
    resetGame();
}

void GameWindow::logoutUser()
{
    if (!currentUser.isGuest) {
        UserDatabase::instance()->updateUser(currentUser);
    }

    emit closed();
    close();
}


void GameWindow::setupScoreFlash(){
    flashLabel = new QLabel("SCORE!", view);
    flashLabel->setAttribute(Qt::WA_TransparentForMouseEvents);
    flashLabel->setAlignment(Qt::AlignCenter);
    flashLabel->setStyleSheet(R"(
        font: bold 48px;
        color: white;
        background: transparent;
    )");

    flashLabel->adjustSize();
    int x = ((view->width()  - flashLabel->width()) / 2) - 10;
    int y = 100;
    flashLabel->move(x, y);

    flashLabel->hide();

}


// flashScore already shows then hides after a delay
void GameWindow::flashScore(){
    flashLabel->show();
    QTimer::singleShot(2000, flashLabel, &QLabel::hide);
}

void GameWindow::updateCountdown() {
    if (--remainingSeconds < 0) {
        countdownTimer->stop();

        QMessageBox::information(this, "Time’s up!", "game over.");
        pauseGame();
        return;
    }

    int m = remainingSeconds / 60;
    int s = remainingSeconds % 60;
    timerLabel->setText(QString("%1:%2") .arg(m, 2, 10, QChar('0')).arg(s, 2, 10, QChar('0')));
}
