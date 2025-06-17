# 🐍 Retro Snake Game

A modern take on the classic **Snake game**, built in **C++** using the powerful and lightweight [Raylib](https://www.raylib.com) library. Designed with retro visuals, smooth gameplay, and fun enhancements like dynamic difficulty and persistent high score tracking.

---

## 🎮 Features

- ✅ Classic snake controls and movement
- 🍎 Textured food spawning at random positions
- 🧠 Self-collision detection — “**Tail Eat!**”
- 🚧 Wall collision detection — “**Wall Bang!**”
- 💾 **Persistent high score** saved to `highscore.txt`
- ⚡ Snake speed increases every 5 points
- ⏸️ Pause/resume with the spacebar
- 🧼 Clean, grid-aligned layout with boundary box
- 🟩 Colorful, minimal retro theme
- 🎯 Score and high score display

---

## 🛠️ Requirements

### ✅ You need:

- A C++17 or later compiler (e.g., `g++`, `clang++`, MSVC)
- [Raylib](https://www.raylib.com) installed on your system

---

## 💻 Platform Setup Instructions

### 🐧 Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install libraylib-dev build-essential
```

### 🍏 macOS (with Homebrew)

```bash
brew install raylib
```

Make sure you are using the ARM64-native Homebrew on M1/M2/M3 Macs. If you’re unsure, run:

```bash
brew config | grep Architecture
```

### 🪟 Windows (MinGW or MSVC)

    1.	Download the Raylib Windows installer from https://www.raylib.com
    2.	If using MinGW, ensure raylib.dll and .lib files are in your project or compiler path
    3.	If using Visual Studio, link against raylib.lib in project properties

---

## 🚀 How to Build

### ✅ Using clang++ or g++ on Linux/macOS

```bash
clang++ main.cpp -o snake -std=c++17 -lraylib \
-framework OpenGL -framework Cocoa -framework IOKit -framework OpenAL
```

Or on Linux:

```bash
g++ main.cpp -o snake -std=c++17 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

### ✅ On Windows (MinGW)

```bash
g++ main.cpp -o snake.exe -std=c++17 -lraylib -lopengl32 -lgdi32 -lwinmm
```

- Ensure raylib.dll is in the same directory as the snake.exe.

## 🎮 Controls

- ⬆️ Arrow Up : Move Up
- ⬇️ Arrow Down : Move Down
- ⬅️ Arrow Left : Move Left
- ➡️ Arrow Right : Move Right
- Spacebar : Pause/Resume
- Any Arrow : Start Game / Restart after Game Over

## 🗂️ Project Structure

RetroSnake/
├── main.cpp # Game source code
├── highscore.txt # Auto-generated persistent high score file
├── Graphics/
│ └── red_food.png # Food texture image
├── README.md # This file

## 🧠 Gameplay Behavior

    •	The game starts paused. Press any arrow key to begin.
    •	Speed increases every 5 points (interval drops from 0.2s to a minimum of 0.06s).
    •	Hitting a wall ends the game with the message: “Wall Bang!”
    •	Hitting yourself ends the game with the message: “Tail Eat!”
    •	After game over, press any arrow key to restart.

## 🙌 Credits

    •	Built using the awesome Raylib library
    •	Inspired by the classic Nokia Snake Game
    •	Developed by Sanskar Gupta

## 📜 License

This project is open-source and free to use for learning and personal purposes.
You are welcome to modify and distribute this code with proper credit. No commercial use without permission.
