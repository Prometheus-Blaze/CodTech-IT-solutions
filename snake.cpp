#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <ctime>

using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int SIZE = 20; // Grid size

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
    SnakeSegment(int x, int y) : x(x), y(y) {}
};

class SnakeGame {
    RenderWindow window;
    vector<SnakeSegment> snake;
    Direction dir;
    int fruitX, fruitY;
    int score = 0;
    int speed = 150; // initial delay in ms
    Clock clock;
    Time lastMove;

    RectangleShape segmentShape;
    RectangleShape fruitShape;
    Font font;
    Text scoreText;

    SoundBuffer eatBuffer;
    Sound eatSound;

public:
    SnakeGame() : window(VideoMode(WIDTH, HEIGHT), "Snake Game") {
        window.setFramerateLimit(60);
        dir = RIGHT;

        snake.push_back(SnakeSegment(5, 5));
        placeFruit();

        segmentShape.setSize(Vector2f(SIZE - 2, SIZE - 2));
        segmentShape.setFillColor(Color::Green);

        fruitShape.setSize(Vector2f(SIZE - 2, SIZE - 2));
        fruitShape.setFillColor(Color::Red);

        font.loadFromFile("arial.ttf");
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setPosition(10, 10);
        scoreText.setFillColor(Color::White);

        eatBuffer.loadFromFile("eat.wav");
        eatSound.setBuffer(eatBuffer);
    }

    void placeFruit() {
        fruitX = rand() % (WIDTH / SIZE);
        fruitY = rand() % (HEIGHT / SIZE);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            if (clock.getElapsedTime().asMilliseconds() - lastMove.asMilliseconds() > speed) {
                update();
                lastMove = clock.getElapsedTime();
            }
            render();
        }
    }

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up) && dir != DOWN) dir = UP;
        else if (Keyboard::isKeyPressed(Keyboard::Down) && dir != UP) dir = DOWN;
        else if (Keyboard::isKeyPressed(Keyboard::Left) && dir != RIGHT) dir = LEFT;
        else if (Keyboard::isKeyPressed(Keyboard::Right) && dir != LEFT) dir = RIGHT;
    }

    void update() {
        // Move snake
        SnakeSegment head = snake[0];
        switch (dir) {
            case UP: head.y--; break;
            case DOWN: head.y++; break;
            case LEFT: head.x--; break;
            case RIGHT: head.x++; break;
        }

        // Check collision
        if (head.x < 0 || head.x >= WIDTH / SIZE || head.y < 0 || head.y >= HEIGHT / SIZE)
            reset();

        for (int i = 1; i < snake.size(); ++i) {
            if (snake[i].x == head.x && snake[i].y == head.y)
                reset();
        }

        // Eat fruit
        if (head.x == fruitX && head.y == fruitY) {
            snake.insert(snake.begin(), head);
            score++;
            speed = max(50, speed - 5); // Increase difficulty
            placeFruit();
            eatSound.play();
        } else {
            // move normally
            snake.pop_back();
            snake.insert(snake.begin(), head);
        }
    }

    void reset() {
        snake.clear();
        snake.push_back(SnakeSegment(5, 5));
        dir = RIGHT;
        score = 0;
        speed = 150;
    }

    void render() {
        window.clear();

        // Draw snake
        for (auto &seg : snake) {
            segmentShape.setPosition(seg.x * SIZE, seg.y * SIZE);
            window.draw(segmentShape);
        }

        // Draw fruit
        fruitShape.setPosition(fruitX * SIZE, fruitY * SIZE);
        window.draw(fruitShape);

        // Draw score
        scoreText.setString("Score: " + to_string(score));
        window.draw(scoreText);

        window.display();
    }
};

int main() {
    srand(time(0));
    SnakeGame game;
    game.run();
    return 0;
}
