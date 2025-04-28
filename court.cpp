#include "court.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QBrush>
#include <QPen>
#include <QRandomGenerator>
#include <QTimer>

//Z value is important for these, we will want the ball to be infront of the backboard,and the hoop infront of the ball
// 0 = Background
// 1 = BackBoard and scoreboard
// 2 = Players
// 3 = Ball
// 4 = Hoop

court::court(QObject *parent)
    : QGraphicsScene(parent), isGameStarted(false), player1Score(0), player2Score(0),
    activePlayer(1), ballInHand(true)
{
    // 1) load & scale the master canvas
    QPixmap master(":/images/Background.png");
    const int maxHeight = 600;
    int w = qRound(master.width() * (double)maxHeight / master.height());
    int h = maxHeight;
    QPixmap courtImg = master.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // Store court dimensions
    courtWidth = w;
    courtHeight = h;
    playerWidth = 50;  // Adjust based on your player sprites
    playerHeight = 100; // Adjust based on your player sprites
    ballSize = 30;

    //make the window the court size
    setSceneRect(0, 0, w, h);

    //court is at z 0
    auto *backgroundItem = addPixmap(courtImg);
    backgroundItem->setZValue(0);

    // target size based on the background
    QSize targetSize(w, h);

    // Back boards are at 1
    QPixmap leftBB(":/images/LeftBackBoard.png");
    QPixmap rightBB(":/images/RightBackBoard.png");
    auto *leftBackBoardItem = addPixmap(leftBB.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    auto *rightBackBoardItem = addPixmap(rightBB.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    leftBackBoardItem->setZValue(1);
    rightBackBoardItem->setZValue(1);

    //hoop is at 4
    QPixmap leftHoop(":/images/LeftHoop.png");
    QPixmap rightHoop(":/images/RightHoop.png");
    leftHoopItem = addPixmap(leftHoop.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    rightHoopItem = addPixmap(rightHoop.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    // make the hoop’s shape only include non‑transparent pixels:
    leftHoopItem ->setShapeMode(QGraphicsPixmapItem::MaskShape);
    rightHoopItem->setShapeMode(QGraphicsPixmapItem::MaskShape);
    leftHoopItem->setZValue(4);
    rightHoopItem->setZValue(4);

    // Store hoop positions for collision detection
    leftHoopPos = leftHoopItem->pos();
    rightHoopPos = rightHoopItem->pos();


    //scoreboard is at 1
    QPixmap scoreboard(":/images/ScoreBoard.png");
    auto *scoreboardItem = addPixmap(scoreboard.scaled(targetSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    scoreboardItem->setZValue(1);

    // Setup players and ball
    setupPlayers();
    setupBall();

    // Setup game timer
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &court::updateGame);
    gameTimer->setInterval(16); // ~60fps for smooth movement

    setupCrowd();
    connect(this, &court::pointsScored,this, &court::animateCrowd);
}



void court::setupCrowd(){
    const QStringList facePaths = { ":/images/Crowd1.png",":/images/Crowd2.png",":/images/Crowd3.png", ":/images/Crowd4.png",":/images/Crowd5.png"};

    int n = facePaths.size();
    for (int i = 0; i < n; ++i) {
        QPixmap face(facePaths[i]);

        face = face.scaledToHeight(80, Qt::SmoothTransformation);

        auto *item = addPixmap(face);
        item->setOpacity(0.85);
        item->setZValue(0.5);

        qreal centerX  = sceneRect().width() / 2.0;
        qreal spacing  = face.width() + 10;
        qreal x = centerX + (i - (n-1)/2.0) * spacing - face.width()/2.0;
        qreal y = sceneRect().height() * 0.5;
        item->setPos(x, y);

        crowdItems << item;
        crowdY << y;
    }

}


void court::animateCrowd(){
    auto *animate = new QVariantAnimation(this);
    animate->setDuration(500);
    animate->setEasingCurve(QEasingCurve::InOutQuad);
    animate->setStartValue(0.0);
    animate->setKeyValueAt(0.5, -20.0);
    animate->setEndValue(0.0);

    connect(animate, &QVariantAnimation::valueChanged, this, [=](const QVariant &v){
        qreal offset = v.toReal();
        for (int i = 0; i < crowdItems.size(); ++i) {
            crowdItems[i]->setY(crowdY[i] + offset);
        }
    });

    connect(animate, &QVariantAnimation::finished, animate, &QObject::deleteLater);
    animate->start();
}



void court::setupPlayers() {
    //randomly select payer images
    const QStringList spritePaths = {
        ":/images/Player1.png",
        ":/images/Player2.png",
        ":/images/Player3.png"
    };
    // 1) make a list [0,1,2] and shuffle it
    std::array<int, 3> idxs = {0,1,2};
    std::shuffle(idxs.begin(), idxs.end(),*QRandomGenerator::global());
    // 2) grab the first two picks
    int image1 = idxs[0];
    int image2 = idxs[1];
    // Load player sprites
    QPixmap player1Img(spritePaths[image1]);
    QPixmap player2Img(spritePaths[image2]);
    // Double the player size
    playerWidth = 100;
    playerHeight = 200;
    // Scale if needed
    player1Img = player1Img.scaled(playerWidth, playerHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    player2Img = player2Img.scaled(playerWidth, playerHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //Flip Player 2 Head
    player2Img = player2Img.transformed(
        QTransform().scale(-1, 1),
        Qt::SmoothTransformation
        );
    // Create player items
    player1 = addPixmap(player1Img);
    player2 = addPixmap(player2Img);
    // Set positions - player 1 on left side, player 2 on right side
    player1->setPos(courtWidth * 0.25 - playerWidth/2, courtHeight - playerHeight - 10);
    player2->setPos(courtWidth * 0.75 - playerWidth/2, courtHeight - playerHeight - 10);
    // Set Z values for players (behind ball, in front of background)
    player1->setZValue(2);
    player2->setZValue(2);
}

void court::attachBallToPlayer(int playerNum) {
    if (playerNum == 1) {
        // Attach to player 1
        QPointF player1Pos = player1->pos();
        ball->setPos(player1Pos.x() + playerWidth/2 - ballSize/2,
                     player1Pos.y() - ballSize/2);
        activePlayer = 1;
    } else {
        // Attach to player 2
        QPointF player2Pos = player2->pos();
        ball->setPos(player2Pos.x() + playerWidth/2 - ballSize/2,
                     player2Pos.y() - ballSize/2);
        activePlayer = 2;
    }

    ballInHand = true;
    ballSpeedX = 0;
    ballSpeedY = 0;
}

void court::setupBall() {
    ballSize = 60;
    // Load basketball image
    QPixmap ballImg(":/images/Basketball.png");
    ballImg = ballImg.scaled(ballSize, ballSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // Create ball item
    ball = addPixmap(ballImg);
    ball->setZValue(3); // Ball is at z-index 3 (in front of players, behind hoop)
    // Position the ball with player 1 initially
    attachBallToPlayer(1);
    // Reset ball speed
    ballSpeedX = 0;
    ballSpeedY = 0;
}


void court::resetBall() {
    // After a basket, give ball to the other player
    attachBallToPlayer(activePlayer == 1 ? 2 : 1);
}

void court::startGame() {
    if (!isGameStarted) {
        isGameStarted = true;
        attachBallToPlayer(1); // Start with player 1
        player1Score = 0;
        player2Score = 0;
        emit scoreChanged(player1Score, player2Score);
        emit player1ScoreChanged(player1Score);
        emit player2ScoreChanged(player2Score);
        gameTimer->start();
    }
}

void court::updateGame() {
    moveBall();
    updatePlayers(); // Handle jump physics
}

void court::pauseGame() {
    if (isGameStarted) {
        gameTimer->stop();
        isGameStarted = false;
    }
}

void court::resetGame() {
    // Stop game timer
    gameTimer->stop();
    isGameStarted = false;

    // Reset player positions to initial spots
    player1->setPos(courtWidth * 0.25 - playerWidth/2, courtHeight - playerHeight - 10);
    player2->setPos(courtWidth * 0.75 - playerWidth/2, courtHeight - playerHeight - 10);

    // Reset ball to player 1
    attachBallToPlayer(1);

    // Reset scores
    player1Score = 0;
    player2Score = 0;
    emit scoreChanged(player1Score, player2Score);
    emit player1ScoreChanged(player1Score);
    emit player2ScoreChanged(player2Score);

    // Restart game
    startGame();
}

void court::player1Shoot() {
    if (isGameStarted && ballInHand && activePlayer == 1) {
        // Set initial ball velocity for a shot
        ballSpeedX = QRandomGenerator::global()->bounded(3, 8); // Shoot toward right hoop
        ballSpeedY = -14; // Strong upward movement

        ballInHand = false;
    }
}

void court::player2Shoot() {
    if (isGameStarted && ballInHand && activePlayer == 2) {
        // Set initial ball velocity for a shot
        ballSpeedX = QRandomGenerator::global()->bounded(-8, -3); // Shoot toward left hoop
        ballSpeedY = -14; // Strong upward movement

        ballInHand = false;
    }
}

void court::movePlayer1Left() {
    if (isGameStarted) {
        QPointF pos = player1->pos();
        if (pos.x() > 0) {
            player1->moveBy(-10, 0);

            // Move the ball with player if held
            if (ballInHand && activePlayer == 1) {
                ball->moveBy(-10, 0);
            }
        }
    }
}

void court::movePlayer1Right() {
    if (isGameStarted) {
        QPointF pos = player1->pos();
        if (pos.x() + playerWidth < courtWidth) {
            player1->moveBy(10, 0);

            // Move the ball with player if held
            if (ballInHand && activePlayer == 1) {
                ball->moveBy(10, 0);
            }
        }
    }
}

void court::movePlayer2Left() {
    if (isGameStarted) {
        QPointF pos = player2->pos();
        if (pos.x() > 0) {
            player2->moveBy(-10, 0);

            // Move the ball with player if held
            if (ballInHand && activePlayer == 2) {
                ball->moveBy(-10, 0);
            }
        }
    }
}

void court::movePlayer2Right() {
    if (isGameStarted) {
        QPointF pos = player2->pos();
        if (pos.x() + playerWidth < courtWidth) {
            player2->moveBy(10, 0);

            // Move the ball with player if held
            if (ballInHand && activePlayer == 2) {
                ball->moveBy(10, 0);
            }
        }
    }
}

void court::moveBall() {
    if (isGameStarted && !ballInHand) {
        // Apply gravity
        ballSpeedY += 0.5;


        // Move the ball according to its speed
        ball->moveBy(ballSpeedX, ballSpeedY);

        // Check for collisions
        checkCollisions();

        // Check if ball is out of bounds
        if (ball->pos().y() > courtHeight + 100) {
            resetBall();
        }
    }
}

//Steal
void court::attemptSteal(int playerNum) {
    if (!isGameStarted || !ballInHand || playerNum == activePlayer) return;

    QGraphicsPixmapItem* thief = (playerNum == 1) ? player1 : player2;
    QGraphicsPixmapItem* target = (activePlayer == 1) ? player1 : player2;

    // Calculate distance between player centers
    QPointF thiefPos = thief->pos();
    QPointF targetPos = target->pos();
    qreal dx = (thiefPos.x() + playerWidth/2) - (targetPos.x() + playerWidth/2);
    qreal dy = (thiefPos.y() + playerHeight/2) - (targetPos.y() + playerHeight/2);
    qreal distance = sqrt(dx*dx + dy*dy);

    // Steal if within 50 pixels
    if (distance < 50) {
        attachBallToPlayer(playerNum);
    }
}

//Jump mechanics
// Jump mechanics
void court::player1Jump() {
    if (!player1IsJumping && player1->y() == courtHeight - playerHeight - 10) {
        player1SpeedY = -14.14; // Jump ~200 pixels high
        player1IsJumping = true;
    }
}

void court::player2Jump() {
    if (!player2IsJumping && player2->y() == courtHeight - playerHeight - 10) {
        player2SpeedY = -14.14; // Jump ~200 pixels high
        player2IsJumping = true;
    }
}

// Update player positions
void court::updatePlayers() {
    // Player 1 jump
    if (player1IsJumping) {
        player1SpeedY += 0.5; // Gravity
        QPointF prevPos = player1->pos();
        player1->setY(player1->y() + player1SpeedY);

        // Move ball with player if holding
        if (ballInHand && activePlayer == 1) {
            ball->setPos(ball->x(), ball->y() + player1SpeedY);
        }

        // Land detection
        if (player1->y() >= courtHeight - playerHeight - 10) {
            player1->setY(courtHeight - playerHeight - 10);
            player1SpeedY = 0;
            player1IsJumping = false;
        }
    }

    // Player 2 jump (same logic)
    if (player2IsJumping) {
        player2SpeedY += 0.5;
        QPointF prevPos = player2->pos();
        player2->setY(player2->y() + player2SpeedY);

        if (ballInHand && activePlayer == 2) {
            ball->setPos(ball->x(), ball->y() + player2SpeedY);
        }

        if (player2->y() >= courtHeight - playerHeight - 10) {
            player2->setY(courtHeight - playerHeight - 10);
            player2SpeedY = 0;
            player2IsJumping = false;
        }
    }
}

void court::checkCollisions() {
    // Get ball position
    QPointF ballPos = ball->pos();
    QRectF ballGlobalRect(ballPos.x(), ballPos.y(), ballSize, ballSize);

    // Check for collision with walls
    if (ballPos.x() <= 0 || ballPos.x() + ballSize >= courtWidth) {
        // Bounce off the walls
        ballSpeedX = -ballSpeedX * 0.8;
    }

    // Check for collision with ceiling
    if (ballPos.y() <= 0) {
        ballSpeedY = -ballSpeedY * 0.8; // Bounce with some energy loss
    }

    // Check for collision with the floor
    if (ballPos.y() + ballSize >= courtHeight) {
        ballSpeedY = -ballSpeedY * 0.8; // Bounce with some energy loss

        // If the ball is moving slowly after hitting the floor, reset it
        if (qAbs(ballSpeedY) < 2.0) {
            resetBall();
        }


    }


    if (ballSpeedY > 0) {
        qreal leftY  = leftHoopItem->y();
        qreal rightY = rightHoopItem->y();
        if (ball->collidesWithItem(leftHoopItem)) {
            player2Score += 2;
            emit player2ScoreChanged(player2Score);
            emit scoreChanged(player1Score, player2Score);
            emit pointsScored();
            resetBall();
            return;
        }
        if (ball->collidesWithItem(rightHoopItem)) {
            player1Score += 2;
            emit player1ScoreChanged(player1Score);
            emit scoreChanged(player1Score, player2Score);
            emit pointsScored();
            resetBall();
            return;
        }
    }
}

int court::getScore() const {
    return player1Score + player2Score;
}

int court::getPlayer1Score() const {
    return player1Score;
}

int court::getPlayer2Score() const {
    return player2Score;
}
