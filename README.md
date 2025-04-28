# 🏀 Hoop Town 🏀  
1v1 Basketball Arcade Game with Qt GUI

Project Owner: Thomas Ho

## Project Overview:
HoopTown is a fast-paced 2D basketball game featuring local multiplayer competition. Developed using modern C++ and Qt framework, this project emphasizes: responsive physics-based gameplay, split-screen competitive mechanics, user profile system with score tracking, custom animation systems.

## Key Gameplay Features:
- Timed jump shots & dunk mechanics
- Ball stealing system
- Real-time score tracking
- 2-minute match timer
- Crowd reaction animations

## Tech Stack:
- Game Engine: C++17, Qt 6.5
- Data Storage: JSON (Qt JSON module)
- Authentication: SHA-256 password hashing
- Build System: CMake 3.20+

## Product Backlog Implementation

### Backlog 1: User Management System 
**Features**:
- Secure authentication flow (JWT-like token system)
- Profile persistence with JSON storage
- Birthday recognition system
- Score history visualization

**Implementation**:
```bnf
// User profile serialization
void User::saveToJson(QJsonObject &json) const {
    json["firstName"] = firstName;
    json["lastName"] = lastName;
    json["dob"] = dob.toString(Qt::ISODate);
    json["scores"] = QJsonArray::fromVariantList(scoreHistory);
}
```

**Data Management (JSON structure):
```bnf
{
  "users": [
    {
      "username": "player23",
      "passwordHash": "a7f5f...",
      "profile": {
        "firstName": "LeBron",
        "lastName": "James",
        "dob": "1984-12-30",
        "bestScore": 42
      },
      "matches": [
        {
          "date": "2024-03-15T15:22:00Z",
          "score": 21,
          "opponent": "CPU",
          "result": "win"
        }
      ]
    }
  ]
}
```
