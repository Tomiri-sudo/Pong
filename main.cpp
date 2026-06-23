#include <SDL.h>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Paddle {
    float x, y;
    int w, h;
    float speed;
};

struct Ball {
    float x, y;
    int size;
    float speedX, speedY;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return 1;

    SDL_Window* window = SDL_CreateWindow("Android Pong",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Paddle player1 = { 20, (SCREEN_HEIGHT / 2) - 50, 20, 120, 8.0f };
    Paddle player2 = { SCREEN_WIDTH - 40, (SCREEN_HEIGHT / 2) - 50, 20, 120, 8.0f };
    Ball ball = { SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 15, 6.0f, 6.0f };

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            
            // Touch screen dragging logic for mobile playability
            if (event.type == SDL_FINGERMOTION || event.type == SDL_FINGERDOWN) {
                float touchX = event.tfinger.x * SCREEN_WIDTH;
                float touchY = event.tfinger.y * SCREEN_HEIGHT;
                
                // Left side of screen controls Player 1, Right side controls Player 2
                if (touchX < SCREEN_WIDTH / 2) {
                    player1.y = touchY - (player1.h / 2);
                } else {
                    player2.y = touchY - (player2.h / 2);
                }
            }
        }

        // Keep paddles on screen
        if (player1.y < 0) player1.y = 0;
        if (player1.y > SCREEN_HEIGHT - player1.h) player1.y = SCREEN_HEIGHT - player1.h;
        if (player2.y < 0) player2.y = 0;
        if (player2.y > SCREEN_HEIGHT - player2.h) player2.y = SCREEN_HEIGHT - player2.h;

        // Move Ball
        ball.x += ball.speedX;
        ball.y += ball.speedY;

        if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - ball.size) ball.speedY = -ball.speedY;

        SDL_FRect ballRect = { ball.x, ball.y, (float)ball.size, (float)ball.size };
        SDL_FRect p1Rect = { player1.x, player1.y, (float)player1.w, (float)player1.h };
        SDL_FRect p2Rect = { player2.x, player2.y, (float)player2.w, (float)player2.h };

        if (SDL_HasIntersectionF(&ballRect, &p1Rect)) ball.speedX = std::abs(ball.speedX);
        if (SDL_HasIntersectionF(&ballRect, &p2Rect)) ball.speedX = -std::abs(ball.speedX);

        if (ball.x < 0 || ball.x > SCREEN_WIDTH) {
            ball.x = SCREEN_WIDTH / 2.0f;
            ball.y = SCREEN_HEIGHT / 2.0f;
            ball.speedX = -ball.speedX;
        }

        SDL_SetRenderDrawColor(renderer, 15, 15, 15, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRectF(renderer, &p1Rect);
        SDL_RenderFillRectF(renderer, &p2Rect);
        SDL_RenderFillRectF(renderer, &ballRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

