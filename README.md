# Tic-Tac-Toe Game

A simple client-server Tic-Tac-Toe game implemented using Qt and C++. This project allows two players to compete against each other over a network. 

## Features

- **Client-Server Architecture:** Players can connect to a server to play against each other.
- **Multiplayer Support:** Play with friends or AI on the same network.
- **User-Friendly Interface:** Built with Qt for an intuitive GUI.
- **Game Logic:** Implements classic Tic-Tac-Toe rules.

## Getting Started

### Prerequisites

Make sure you have the following installed:

- [Qt](https://www.qt.io/download) (version X.X.X or later)
- A C++ compiler (e.g., MinGW for Windows)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/tic-tac-toe.git
   cd tic-tac-toe
   ```

2. Open the project in Qt Creator.

3. Build the project using the `Build` menu.

4. Run the server application first and then run the client application.

### Usage

1. Start the server application.
2. Start the client application and enter the server's IP address.
3. Follow the on-screen instructions to play the game.

## Gameplay

- The game is played on a 3x3 grid.
- Players take turns placing their markers (X or O).
- The first player to align three markers horizontally, vertically, or diagonally wins the game.

## Contributing

Contributions are welcome! If you find any bugs or have suggestions for improvements, please create an issue or submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgements

- Thanks to the [Qt](https://www.qt.io/) community for their fantastic framework.
- Inspiration from classic Tic-Tac-Toe gameplay.
