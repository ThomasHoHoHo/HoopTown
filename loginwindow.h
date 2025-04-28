#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDateEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include "gamewindow.h"
#include "user.h"

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void showLoginPage();
    void showSignupPage();
    void onLoginClicked();
    void onSignupClicked();
    void onPlayAsGuestClicked();
    void onBrowseImageClicked();
    void validatePassword(const QString &password);

private:
    QStackedWidget *stackedWidget;

    // Login page widgets
    QWidget *loginPage;
    QLineEdit *usernameLoginEdit;
    QLineEdit *passwordLoginEdit;
    QPushButton *loginButton;
    QPushButton *signupPageButton;
    QPushButton *playAsGuestButton;

    // Signup page widgets
    QWidget *signupPage;
    QLineEdit *firstNameEdit;
    QLineEdit *lastNameEdit;
    QDateEdit *dobEdit;
    QComboBox *genderComboBox;
    QLineEdit *usernameSignupEdit;
    QLineEdit *passwordSignupEdit;
    QLabel *passwordStrengthLabel;
    QLabel *profileImageLabel;
    QString profileImagePath;
    QPushButton *browseImageButton;
    QPushButton *signupButton;
    QPushButton *backToLoginButton;

    void setupLoginPage();
    void setupSignupPage();
    bool validateSignupForm();
    void startGame(const User &user);
};

#endif // LOGINWINDOW_H
