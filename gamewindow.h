#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QMainWindow>
#include <QGraphicsView>
#include <court.h>
#include <user.h>
#include <QLabel>
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>





  //Manages the user interface including the main menu, game view, score history,

class GameWindow : public QMainWindow {
    Q_OBJECT

public:
    //defualt constructor
    explicit GameWindow(QWidget *parent = nullptr);

    //constructor for user or guest
    explicit GameWindow(const User &user, QWidget *parent = nullptr);

signals:
    //sent when game window is closed to notify login
    void closed();

protected:
    //handle close windows
    void closeEvent(QCloseEvent *event) override;

    //key press events for handling movement
    void keyPressEvent(QKeyEvent *event) override;

    //key press events for handling movement
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:

    //Flashes score on scoreboard
    void flashScore();

    //switches to game view page
    void showGamePage();

    //Pauses game if its runing and switches to main menue
    void showMainMenu();

    //Shows the score history page
    void showScoreHistory();

    //Logs out the user
    void logoutUser();

    //Starts the game
    void startGame();

    //pauses the game
    void pauseGame();

    //Resets the game
    void resetGame();

    //Updates the displayed scores
    void updateScore(int player1Score, int player2Score);

    //moves player based on keys
    void movePlayer();

    //Updates the countdown timer for each second
    void updateCountdown();

private:
    QGraphicsView *view; // Showing the game scene
    court *scene;   //game logic for rendering the court
    User currentUser; //logged in or guest user
    bool gameRunning; //flag for if the game is currently running

    // Player 1 controls (WASD and spacebar)
    bool player1LeftPressed;  // A
    bool player1RightPressed; // D

    // Player 2 controls (arrow keys and Enter)
    bool player2LeftPressed;  // Left arrow
    bool player2RightPressed; // Right arrow

    QTimer *playerMoveTimer; //timer for continue movement

    // UI components
    QStackedWidget *stackedWidget;
    QWidget *menuPage;
    QWidget *gamePage;
    QWidget *scoreHistoryPage;
    QLabel *welcomeLabel;
    QLabel *birthdayLabel;
    QLabel *userInfoLabel;
    QLabel *currentDateLabel;
    QLabel *profilePicLabel;
    QLabel *player1ScoreLabel;
    QLabel *player2ScoreLabel;
    QLabel *totalScoreLabel;
    QPushButton *playButton;
    QPushButton *scoreHistoryButton;
    QPushButton *logoutButton;
    QTableWidget *scoreHistoryTable;
    QPushButton *backToMenuButton;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *resetButton;
    QLabel *flashLabel;
    QLabel *timerLabel;
    QTimer *countdownTimer;  // ticks every second
    int  remainingSeconds;// seconds left


    //constructs the main menu
    void setupMenuPage();

    //constructs and layout gameplay screens
    void setupGamePage();

    //sets uo the score history page
    void setupScoreHistoryPage();

    //keyboard controlss for players
    void setupPlayerControls();

    //add the saved scores to the score History page
    void updateScoreHistory();

    //shows birthday greeting if today is the users birthday
    void showBirthdayGreeting();

    //update the user avatar
    void updateUserInfo();

    //prepare flashing score sign
    void setupScoreFlash();

};
#endif // GAMEWINDOW_H
