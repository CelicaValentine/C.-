// tetris.h
#ifndef TETRIS_H  // 防止头文件重复包含
#define TETRIS_H

#include <SDL3/SDL.h>
#include <vector>

// 游戏常量（全局可见）
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 30;
const int GRID_COLUMNS = 10;
const int GRID_ROWS = 20;
const int GAME_AREA_X = 50;
const int GAME_AREA_Y = 50;

// 方块形状和颜色（全局常量，供Block和GameArea使用）
const std::vector<std::vector<std::vector<int>>> SHAPES = {
    {{1, 1, 1, 1}},                                  // I型
    {{1, 1}, {1, 1}},                                // O型
    {{1, 1, 1}, {0, 1, 0}},                          // T型
    {{1, 1, 1}, {1, 0, 0}},                          // L型
    {{1, 1, 1}, {0, 0, 1}},                          // J型
    {{1, 1, 0}, {0, 1, 1}},                          // S型
    {{0, 1, 1}, {1, 1, 0}}                           // Z型
};

const std::vector<SDL_Color> COLORS = {
    {0, 255, 255, 255},    // I型：青色
    {255, 255, 0, 255},    // O型：黄色
    {128, 0, 128, 255},    // T型：紫色
    {255, 165, 0, 255},    // L型：橙色
    {0, 0, 255, 255},      // J型：蓝色
    {0, 255, 0, 255},      // S型：绿色
    {255, 0, 0, 255}       // Z型：红色
};

// 方块类声明
class Block {
public:
    int x, y;                  // 网格坐标（列, 行）
    int shapeIndex;            // 形状索引
    int rotation;              // 旋转状态（0-3）
    SDL_Color color;           // 颜色

    Block();  // 构造函数声明
    std::vector<std::vector<int>> getCurrentShape();  // 获取当前旋转状态的形状
};

// 游戏区域类声明
class GameArea {
private:
    std::vector<std::vector<int>> grid;  // 网格状态（-1为空）
public:
    GameArea();  // 构造函数声明
    bool isCollision(Block& block);      // 碰撞检测
    void lockBlock(Block& block);        // 固定方块到网格
    void render(SDL_Renderer* renderer); // 绘制游戏区域
};

#endif  // TETRIS_H
