#ifndef COURT_H
#define COURT_H
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsPixmapItem>
#include <QVariantAnimation>
#include <QTimer>

class court : public QGraphicsScene {
    Q_OBJECT
public:
    explicit court(QObject *parent = nullptr);
    void startGame();
    void pauseGame();
    void resetGame();
    int getScore() const;
    int getPlayer1Score() const;
    int getPlayer2Score() const;

    // Player 1 controls
    void movePlayer1Left();
    void movePlayer1Right();
    void player1Jump();
    void player1Shoot();

    // Player 2 controls
    void movePlayer2Left();
    void movePlayer2Right();
    void player2Jump();
    void player2Shoot();

    // Steal
    void attemptSteal(int playerNum);

signals:
    void scoreChanged(int player1Score, int player2Score);
    void player1ScoreChanged(int score);
    void player2ScoreChanged(int score);
    void pointsScored();

private slots:
    void animateCrowd();

private:
    QGraphicsPixmapItem *leftHoopItem;
    QGraphicsPixmapItem *rightHoopItem;
    QPointF leftHoopPos;
    QPointF rightHoopPos;
    QGraphicsPixmapItem *ball;
    QGraphicsPixmapItem *player1;
    QGraphicsPixmapItem *player2;


    //crowd
    QVector<QGraphicsPixmapItem*> crowdItems;
    QVector<qreal> crowdY;
    void setupCrowd();

    QTimer *gameTimer;
    bool isGameStarted;

    int player1Score;
    int player2Score;
    int activePlayer;

    // Jump mechanics
    qreal player1SpeedY;
    qreal player2SpeedY;
    bool player1IsJumping;
    bool player2IsJumping;

    // Ball movement
    qreal ballSpeedX;
    qreal ballSpeedY;
    bool ballInHand;


    // Game configuration
    int courtWidth;
    int courtHeight;
    int playerWidth;
    int playerHeight;
    int ballSize;

    void setupPlayers();
    void setupBall();
    void updateGame();
    void moveBall();
    void updatePlayers();
    void checkCollisions();
    void resetBall();
    void attachBallToPlayer(int playerNum);
};
#endif // COURT_H
