#pragma once
#include <SDL3/SDL.h>
#include <string>

class Game {
public:
    static Game& GetInstance() {
        static Game instance;
        return instance;
    }

    // �������ڽӿ�
    void Init(const std::string& title, int width, int height);
    void Run();
    void HandleEvents();
    void Update(float dt);
    void Render();
    void Clean();

    bool IsRunning() const noexcept { return running_; }

    ~Game() = default; // ������������Ϊ public��������ʿ�������

private:
    Game() = default;

    // ��ֹ�������ƶ�
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;

    // SDL �� Handles ��״̬
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;

    // ����״̬������
    bool running_ = false;
    int width_ = 800;
    int height_ = 600;
    std::string title_;
};