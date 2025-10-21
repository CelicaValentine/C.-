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
    // �� 0 ��ʾ������ flags������ʹ�� SDL_WINDOW_RESIZABLE / SDL_WINDOW_ALLOW_HIGHDPI ��
    window_ = SDL_CreateWindow(title_.c_str(), 0, width_, height_);
    if (!window_) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        running_ = false;
        return;
    }

    // SDL3: CreateRenderer(window, flags)
    // �� 0 ��ʾĬ�������Ⱦ������ʹ�� SDL_RENDERER_ACCELERATED ��
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

        SDL_Delay(1);   //����CPUռ�ù��ߣ���ǰ������ִͣ�е�ָ��������
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
        // TODO: ���̴������ҡ���ת�����������
    }
}

void Game::Update(float /*dt*/)
{
    // TODO: ��Ϸ�߼��������ʱ����ײ��⡢���С��Ʒֵ�

}

void Game::Render()
{
    if (!renderer_) return;

    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // TODO: ��Ⱦ���̡����顢���񡢷�����

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

