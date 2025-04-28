#include "loginwindow.h"
#include "userdatabase.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPixmap>
#include <QRegularExpression>
#include <QDate>

LoginWindow::LoginWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Game Login");
    resize(400, 500);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setupLoginPage();
    setupSignupPage();

    stackedWidget->addWidget(loginPage);
    stackedWidget->addWidget(signupPage);

    showLoginPage();
}

LoginWindow::~LoginWindow()
{
}

void LoginWindow::setupLoginPage()
{
    loginPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(loginPage);

    QLabel *titleLabel = new QLabel("Game Login");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");

    QFormLayout *formLayout = new QFormLayout();

    usernameLoginEdit = new QLineEdit();
    passwordLoginEdit = new QLineEdit();
    passwordLoginEdit->setEchoMode(QLineEdit::Password);

    formLayout->addRow("Username:", usernameLoginEdit);
    formLayout->addRow("Password:", passwordLoginEdit);

    loginButton = new QPushButton("Login");
    loginButton->setObjectName("primaryButton");
    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);

    signupPageButton = new QPushButton("Sign Up");
    connect(signupPageButton, &QPushButton::clicked, this, &LoginWindow::showSignupPage);

    playAsGuestButton = new QPushButton("Play as Guest");
    connect(playAsGuestButton, &QPushButton::clicked, this, &LoginWindow::onPlayAsGuestClicked);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(signupPageButton);
    mainLayout->addWidget(playAsGuestButton);
    mainLayout->addStretch();
}

void LoginWindow::setupSignupPage()
{
    signupPage = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(signupPage);

    QLabel *titleLabel = new QLabel("Sign Up");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 20px;");

    QFormLayout *formLayout = new QFormLayout();

    firstNameEdit = new QLineEdit();
    lastNameEdit = new QLineEdit();
    dobEdit = new QDateEdit(QDate::currentDate());
    dobEdit->setDisplayFormat("dd/MM/yyyy");
    dobEdit->setCalendarPopup(true);
    dobEdit->setMaximumDate(QDate::currentDate());
    dobEdit->setDate(QDate::currentDate().addYears(-18)); // Default to 18 years ago

    genderComboBox = new QComboBox();
    genderComboBox->addItem("Select Gender (Optional)");
    genderComboBox->addItem("Male");
    genderComboBox->addItem("Female");
    genderComboBox->addItem("Other");
    genderComboBox->addItem("Prefer not to say");

    usernameSignupEdit = new QLineEdit();
    passwordSignupEdit = new QLineEdit();
    passwordSignupEdit->setEchoMode(QLineEdit::Password);
    connect(passwordSignupEdit, &QLineEdit::textChanged, this, &LoginWindow::validatePassword);

    passwordStrengthLabel = new QLabel("");

    // Profile picture section
    QHBoxLayout *profileLayout = new QHBoxLayout();
    profileImageLabel = new QLabel();
    profileImageLabel->setFixedSize(100, 100);
    profileImageLabel->setStyleSheet("border: 1px solid #ccc;");
    profileImageLabel->setAlignment(Qt::AlignCenter);
    profileImageLabel->setText("No Image");

    browseImageButton = new QPushButton("Browse...");
    connect(browseImageButton, &QPushButton::clicked, this, &LoginWindow::onBrowseImageClicked);

    profileLayout->addWidget(profileImageLabel);
    profileLayout->addWidget(browseImageButton);
    profileLayout->addStretch();

    formLayout->addRow("First Name:", firstNameEdit);
    formLayout->addRow("Last Name:", lastNameEdit);
    formLayout->addRow("Date of Birth:", dobEdit);
    formLayout->addRow("Gender:", genderComboBox);
    formLayout->addRow("Profile Picture:", profileLayout);
    formLayout->addRow("Username:", usernameSignupEdit);
    formLayout->addRow("Password:", passwordSignupEdit);
    formLayout->addRow("", passwordStrengthLabel);

    signupButton = new QPushButton("Create Account");
    signupButton->setObjectName("primaryButton");
    connect(signupButton, &QPushButton::clicked, this, &LoginWindow::onSignupClicked);

    backToLoginButton = new QPushButton("Back to Login");
    connect(backToLoginButton, &QPushButton::clicked, this, &LoginWindow::showLoginPage);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(signupButton);
    mainLayout->addWidget(backToLoginButton);
}

void LoginWindow::showLoginPage()
{
    stackedWidget->setCurrentWidget(loginPage);
}

void LoginWindow::showSignupPage()
{
    stackedWidget->setCurrentWidget(signupPage);
}

void LoginWindow::onLoginClicked()
{
    QString username = usernameLoginEdit->text().trimmed();
    QString password = passwordLoginEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login Error", "Please enter both username and password.");
        return;
    }

    User user;
    bool success = UserDatabase::instance()->authenticateUser(username, password, user);

    if (success) {
        this->hide();
        startGame(user);
    } else {
        QMessageBox::warning(this, "Login Error", "Invalid username or password.");
    }
}

void LoginWindow::onSignupClicked()
{
    if (validateSignupForm()) {
        User newUser;
        newUser.firstName = firstNameEdit->text().trimmed();
        newUser.lastName = lastNameEdit->text().trimmed();
        newUser.dateOfBirth = dobEdit->date();

        int genderIndex = genderComboBox->currentIndex();
        if (genderIndex > 0) { // If a gender was selected
            newUser.gender = genderComboBox->currentText();
        }

        newUser.username = usernameSignupEdit->text().trimmed();
        newUser.password = passwordSignupEdit->text();
        newUser.profilePicture = profileImagePath;

        bool success = UserDatabase::instance()->addUser(newUser);

        if (success) {
            QMessageBox::information(this, "Signup Success", "Account created successfully!");
            showLoginPage();
        } else {
            QMessageBox::warning(this, "Signup Error", "Username already exists. Please choose a different username.");
        }
    }
}

void LoginWindow::onPlayAsGuestClicked()
{
    User guestUser;
    guestUser.firstName = "Guest";
    guestUser.lastName = "User";
    guestUser.username = "guest_" + QString::number(QDateTime::currentDateTime().toSecsSinceEpoch());
    guestUser.isGuest = true;

    this->hide();
    startGame(guestUser);
}

void LoginWindow::onBrowseImageClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Select Profile Picture", "", "Images (*.png *.jpg *.jpeg)");

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            profileImagePath = filePath;
            profileImageLabel->setPixmap(pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            QMessageBox::warning(this, "Image Error", "Could not load the selected image.");
        }
    }
}

void LoginWindow::validatePassword(const QString &password)
{
    QRegularExpression uppercaseRegex("[A-Z]");
    QRegularExpression lowercaseRegex("[a-z]");
    QRegularExpression numberRegex("[0-9]");

    bool hasUppercase = uppercaseRegex.match(password).hasMatch();
    bool hasLowercase = lowercaseRegex.match(password).hasMatch();
    bool hasNumber = numberRegex.match(password).hasMatch();
    bool hasMinLength = password.length() >= 8;

    QString strengthText;
    QString colorStyle;

    if (password.isEmpty()) {
        strengthText = "";
    } else if (hasUppercase && hasLowercase && hasNumber && hasMinLength) {
        strengthText = "Password strength: Strong";
        colorStyle = "color: green;";
    } else if ((hasUppercase || hasLowercase) && (hasNumber || hasMinLength)) {
        strengthText = "Password strength: Moderate";
        colorStyle = "color: orange;";
    } else {
        strengthText = "Password strength: Weak";
        colorStyle = "color: red;";
    }

    passwordStrengthLabel->setText(strengthText);
    passwordStrengthLabel->setStyleSheet(colorStyle);
}

bool LoginWindow::validateSignupForm()
{
    if (firstNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter your first name.");
        return false;
    }

    if (lastNameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter your last name.");
        return false;
    }

    if (usernameSignupEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a username.");
        return false;
    }

    QString password = passwordSignupEdit->text();
    QRegularExpression uppercaseRegex("[A-Z]");
    QRegularExpression lowercaseRegex("[a-z]");
    QRegularExpression numberRegex("[0-9]");

    bool hasUppercase = uppercaseRegex.match(password).hasMatch();
    bool hasLowercase = lowercaseRegex.match(password).hasMatch();
    bool hasNumber = numberRegex.match(password).hasMatch();
    bool hasMinLength = password.length() >= 8;

    if (!hasMinLength || !hasUppercase || !hasLowercase || !hasNumber) {
        QMessageBox::warning(this, "Password Error",
                             "Password must be at least 8 characters long and contain at least:\n"
                             "- One uppercase letter\n"
                             "- One lowercase letter\n"
                             "- One number");
        return false;
    }

    return true;
}

void LoginWindow::startGame(const User &user)
{
    GameWindow *gameWindow = new GameWindow(user,this);
    gameWindow->setAttribute(Qt::WA_DeleteOnClose);
    gameWindow->show();

    // Close login window when user closes game window
    connect(gameWindow, &::GameWindow::close, this, &LoginWindow::show);
}
