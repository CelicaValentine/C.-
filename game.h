#pragma once
#include <SDL3/SDL.h>
#include <string>

class Game {
public:
    static Game& GetInstance() {
        static Game instance;
        return instance;
    }

    // 生命周期接口
    void Init(const std::string& title, int width, int height);
    void Run();
    void HandleEvents();
    void Update(float dt);
    void Render();
    void Clean();

    bool IsRunning() const noexcept { return running_; }

    ~Game() = default; // 将析构函数设为 public，避免访问控制问题

private:
    Game() = default;

    // 禁止拷贝与移动
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    // SDL 句 Handles 与状态
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    // 运行状态与配置
    bool running_ = false;
    int width_ = 800;
    int height_ = 600;
    std::string title_;
};