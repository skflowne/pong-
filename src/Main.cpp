// Pong++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "raylib.h"

constexpr auto SCREEN_WIDTH = 1296;
constexpr auto SCREEN_HEIGHT = 1080;

constexpr auto BALL_RADIUS = 12.0f;
constexpr auto INITIAL_BALL_SPEED = 400.0f;
constexpr auto MAX_BALL_SPEED = 800.0f;
constexpr auto ENGLISH_VELOCITY_MODIFIER = 150.f;
constexpr auto BALL_DRAG = 0.1f;

constexpr auto MAX_PLAYER_PADDLE_SPEED = 10.0f;
constexpr auto INITIAL_AI_PADDLE_SPEED = 1600.0f;

constexpr auto PADDLE_SIZE = Vector2(20.0f, 100.f);
constexpr auto PADDLE_MARGIN = 10.0f;

constexpr auto SCORE_FONT_SIZE = 40;
constexpr auto MIDLINE_DASH_SIZE = 20;

//constexpr const char* DVD_LOGO_PATH = "assets/DVD_logo.png";
constexpr char DVD_LOGO_PATH[] = "assets/DVD_logo.png";

struct Math{ 
    inline static float max(float a, float b) {
        return a > b ? a : b;
    }

    inline static float min(float a, float b) {
        return a < b ? a : b;
    }

    inline static float clamp(float x, float min, float max) {
        return Math::min(Math::max(x, min), max);
    }
};



struct GameObject {
    Vector2 position;
    Vector2 velocity;
    Color color;
    std::string name;

    GameObject() :
        position(Vector2(0.0f, 0.0f)),
        velocity(Vector2(0.0f, 0.0f)),
        color(RAYWHITE)
    {};

    GameObject(const Vector2 pos) :
        position(pos),
        velocity(Vector2(0.0f, 0.0f)),
        color(RAYWHITE)
    {};

    GameObject(const Vector2 pos, const Vector2 vel) :
        position(pos),
        velocity(vel),
        color(RAYWHITE)
    {};

    virtual void Update(float dt) {
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
    }

    virtual void Render() = 0;
};

struct Score : GameObject {
    int value;
    int fontSize;
   
    Score() :
        GameObject(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f)),
        value(0),
        fontSize(18)
    {};

    Score(Vector2 pos, int fontSize) :
        GameObject(pos, Vector2(0.0f, 0.0f)),
        value(0),
        fontSize(fontSize)
    {};

    std::string GetText() const {
        return std::to_string(value);
    }

    void Render() {
        DrawText(GetText().data(), position.x, position.y, fontSize, color);
    }

    void operator++(int param) {
        value++;
    }
};

struct Ball : GameObject
{
    float radius;
    float minSpeed;
    float maxSpeed;
    float currentSpeed;
    float drag;

    Ball() : 
        GameObject(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2), Vector2(0.0f, 0.0f)),
        radius(BALL_RADIUS),
        minSpeed(INITIAL_BALL_SPEED - 200.f),
        maxSpeed(MAX_BALL_SPEED),
        currentSpeed(0.0f),
        drag(BALL_DRAG)
    {

    }

    void Update(float dt) override {

        if (position.y - radius <= 0.0f || position.y + radius >= SCREEN_HEIGHT) {
            velocity.y = -velocity.y;
        }

        currentSpeed = Math::min(currentSpeed, maxSpeed);

        position.x += velocity.x * currentSpeed * dt;
        position.y = Math::clamp(position.y + velocity.y * currentSpeed * dt, radius, SCREEN_HEIGHT - radius);

        if (currentSpeed > INITIAL_BALL_SPEED) {
            currentSpeed = Math::max(currentSpeed * (1.0f - drag * dt), INITIAL_BALL_SPEED);
        }

        std::cout << "min: " << minSpeed << "ball speed: " << currentSpeed << std::endl;
    }

    void Render() {
        DrawCircle(position.x, position.y, radius, color);
    }
};

struct Paddle : GameObject {
    Paddle() : GameObject(Vector2(0.0f, 0.0f), Vector2(0.0f, 0.0f))
    {
    };

    Paddle(Vector2 pos) : GameObject(pos, Vector2(0.0f,0.0f)) 
    {
    };

    

    void Render() override {
        DrawRectangle(position.x - PADDLE_SIZE.x / 2, position.y - PADDLE_SIZE.y / 2, PADDLE_SIZE.x, PADDLE_SIZE.y, color);
    }

    virtual void MoveTo(const float to) = 0;
};

struct PlayerPaddle : Paddle
{
    using Paddle::Paddle;

    PlayerPaddle(const Vector2 pos) : Paddle(pos) {};

    void Update(float dt) override {
        position.y = Math::clamp(position.y + velocity.y, PADDLE_SIZE.y / 2, SCREEN_HEIGHT - PADDLE_SIZE.y / 2);
    }

    void MoveTo(const float to) {
        velocity.y = Math::clamp(to - position.y, -MAX_PLAYER_PADDLE_SPEED, MAX_PLAYER_PADDLE_SPEED);
        //std::cout << "player vel:" << velocity.y << std::endl;
    }
};

struct AiPaddle : Paddle {

    float speed;

    AiPaddle() : Paddle(), speed(0.0f) {};
    AiPaddle(const Vector2 pos, float speed) : Paddle(pos), speed(speed) {};

    void MoveTo(const float to) {
        velocity.y = to > position.y ? speed : -speed;
    }
};

Ball ball;
    
float paddleMidY = (float)(SCREEN_HEIGHT / 2);
PlayerPaddle playerPaddle = PlayerPaddle(Vector2(PADDLE_SIZE.x / 2 + PADDLE_MARGIN, paddleMidY));
AiPaddle aiPaddle = AiPaddle(Vector2(SCREEN_WIDTH - PADDLE_SIZE.x / 2 - PADDLE_MARGIN, paddleMidY), INITIAL_AI_PADDLE_SPEED);

Score playerScore = Score(Vector2(SCREEN_WIDTH / 4, SCORE_FONT_SIZE), SCORE_FONT_SIZE);
Score aiScore = Score(Vector2(SCREEN_WIDTH - SCREEN_WIDTH / 4, SCORE_FONT_SIZE), SCORE_FONT_SIZE);

bool roundStarted = false;

void init_round() {
    ball.position = Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
 
    
    ball.currentSpeed = INITIAL_BALL_SPEED;
    
    float horizontal = rand() % 100 >= 50 ? -1.0f : 1.0f;
    float vertical = rand() % 100 >= 50 ? -1.0f : 1.0f;
    ball.velocity = Vector2(horizontal, vertical); //Vector2((rand() % 2) * 0.01f * INITIAL_BALL_SPEED, (rand() % 2) * 0.01f * INITIAL_BALL_SPEED);
    
    roundStarted = true;
}

int main(void)
{
    ball.name = "Ball";

    playerPaddle.name = "Player";
    aiPaddle.name = "AI";

    srand(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong++");

    init_round();
    

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (IsKeyReleased(KEY_BACKSPACE)) {
            init_round();
        }

        // read input
        playerPaddle.MoveTo(GetMousePosition().y);
        aiPaddle.MoveTo(ball.position.y);

        std::cout << "AI paddle vel: " << aiPaddle.velocity.y << std::endl;

        // simulate
        float ballToPlayerDist = (ball.position.x - ball.radius) - (playerPaddle.position.x + PADDLE_SIZE.x / 2);
        float ballToAIDist = (aiPaddle.position.x - PADDLE_SIZE.x / 2) - (ball.position.x + ball.radius);

        if (roundStarted) {
            if (ballToPlayerDist <= 0.0f) {
                if (ball.position.y >= playerPaddle.position.y - PADDLE_SIZE.y / 2
                    && ball.position.y <= playerPaddle.position.y + PADDLE_SIZE.y / 2) {

                    ball.position.x = (playerPaddle.position.x + PADDLE_SIZE.x / 2) + ball.radius;
                    ball.velocity.x = -ball.velocity.x;
                    ball.currentSpeed += (ball.velocity.y * playerPaddle.velocity.y) * abs(playerPaddle.velocity.y) * ENGLISH_VELOCITY_MODIFIER;
                
                } else if (ball.position.x <= 0.0f) {
                    roundStarted = false;
                    aiScore++;
                    std::cout << "AI scores!" << std::endl;
                }
            }

            if(ballToAIDist <= 0.0f){
                if (ball.position.y >= aiPaddle.position.y - PADDLE_SIZE.y / 2
                    && ball.position.y <= aiPaddle.position.y + PADDLE_SIZE.y / 2) {
                
                    ball.position.x = (aiPaddle.position.x - PADDLE_SIZE.x / 2) - ball.radius;
                    ball.velocity.x = -ball.velocity.x;
                
                }
                else if (ball.position.x >= SCREEN_WIDTH) {
                    roundStarted = false;
                    playerScore++;
                    std::cout << "Player scores!" << std::endl;
                }
            }
        }

        // move stuff
        ball.Update(dt);
        playerPaddle.Update(dt);
        aiPaddle.Update(dt);


        // render stuff
        BeginDrawing();
        ClearBackground(BLACK);
        ball.Render();
        playerPaddle.Render();
        aiPaddle.Render();
        playerScore.Render();
        aiScore.Render();

        

        for (int i = MIDLINE_DASH_SIZE; i < SCREEN_HEIGHT; i += MIDLINE_DASH_SIZE * 2) {
            DrawLine(SCREEN_WIDTH / 2, i, SCREEN_WIDTH / 2, i + MIDLINE_DASH_SIZE, RAYWHITE);
        }

        EndDrawing();

        if (!roundStarted) {
            init_round();
        }
    }

    CloseWindow();

    return 0;
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
