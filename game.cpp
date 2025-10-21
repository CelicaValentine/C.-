#include "game.h"
#include <iostream>
#include <chrono>

void Game::Init(const std::string& title, int width, int height)
{
    title_ = title;
    width_ = width;
    height_ = height;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        running_ = false;
        return;
    }

    // SDL3: CreateWindow(title, flags, width, height)
    // 传 0 表示无特殊 flags；按需使用 SDL_WINDOW_RESIZABLE / SDL_WINDOW_ALLOW_HIGHDPI 等
    window_ = SDL_CreateWindow(title_.c_str(), 0, width_, height_);
    if (!window_) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        running_ = false;
        return;
    }

    // SDL3: CreateRenderer(window, flags)
    // 传 0 表示默认软件渲染；按需使用 SDL_RENDERER_ACCELERATED 等
    renderer_ = SDL_CreateRenderer(window_, 0);
    if (!renderer_) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window_);
        SDL_Quit();
        running_ = false;
        return;
    }

    running_ = true;
}

void Game::Run()
{
    using clock = std::chrono::steady_clock;
    auto last = clock::now();

    while (running_) {
        auto now = clock::now();
        std::chrono::duration<float> elapsed = now - last;
        last = now;
        float dt = elapsed.count();

        HandleEvents();
        Update(dt);
        Render();

        SDL_Delay(1);   //避免CPU占用过高，当前程序暂停执行的指定毫秒数
    }

    Clean();
}

void Game::HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running_ = false;
        }
        // TODO: 键盘处理：左右、旋转、快速下落等
    }
}

void Game::Update(float /*dt*/)
{
    // TODO: 游戏逻辑：下落计时、碰撞检测、消行、计分等

}

void Game::Render()
{
    if (!renderer_) return;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // TODO: 渲染棋盘、方块、网格、分数等

    SDL_RenderPresent(renderer_);
}

void Game::Clean()
{
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

