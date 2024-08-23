// Pong++.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "raylib.h"

constexpr auto SCREEN_WIDTH = 1296;
constexpr auto SCREEN_HEIGHT = 1080;
constexpr auto BALL_RADIUS = 4.0f;
constexpr auto INITIAL_BALL_SPEED = 100.0f;
//constexpr const char* DVD_LOGO_PATH = "assets/DVD_logo.png";
constexpr char DVD_LOGO_PATH[] = "assets/DVD_logo.png";

struct Ball
{
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color color;

    Ball() : 
        position(Vector2(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)),
        velocity(Vector2(0.0f, 0.0f)),
        radius(BALL_RADIUS),
        color(RAYWHITE)
    {

    }
};

Ball ball;

void init_round() {
    ball = Ball();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong++");

    init_round();
    ball.velocity = Vector2(INITIAL_BALL_SPEED, INITIAL_BALL_SPEED);

    //Image dvdLogo = LoadImage(DVD_LOGO_PATH);
    Texture2D dvdLogoTex = LoadTexture(DVD_LOGO_PATH);
    

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();
        ball.position.x = ball.position.x + ball.velocity.x * dt;
        ball.position.y = ball.position.y + ball.velocity.y * dt;

        if (ball.position.y <= 0.0f || ball.position.y + dvdLogoTex.height >= SCREEN_HEIGHT) {
            ball.velocity.y = -ball.velocity.y;
        }

        if (ball.position.x <= 0.0f || ball.position.x + dvdLogoTex.width >= SCREEN_WIDTH) {
            ball.velocity.x = -ball.velocity.x;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        //DrawCircle(ball.position.x, ball.position.y, ball.radius, ball.color);
        DrawTexture(dvdLogoTex, ball.position.x, ball.position.y, RAYWHITE);
        EndDrawing();
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
