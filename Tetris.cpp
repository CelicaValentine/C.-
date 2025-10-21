// tetris.cpp
#include "tetris.h"
#include <cstdlib>
#include <ctime>

// Block类构造函数实现：随机初始化方块
Block::Block() {
    std::srand(std::time(0));  // 初始化随机数种子
    shapeIndex = std::rand() % SHAPES.size();  // 随机选择形状
    color = COLORS[shapeIndex];                // 匹配颜色
    rotation = 0;                              // 初始旋转状态
    // 初始位置：顶部居中
    x = GRID_COLUMNS / 2 - SHAPES[shapeIndex][0].size() / 2;
    y = 0;
}

// 实现：获取当前旋转状态的形状（90度顺时针旋转）
std::vector<std::vector<int>> Block::getCurrentShape() {
    auto& original = SHAPES[shapeIndex];
    int rows = original.size();
    int cols = original[0].size();

    if (rotation == 0) return original;  // 0度旋转直接返回原形状

    // 旋转逻辑：行列转置并反转行
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = original[i][j];
        }
    }
    return rotated;
}

// GameArea类构造函数实现：初始化空网格
GameArea::GameArea() {
    grid.resize(GRID_ROWS, std::vector<int>(GRID_COLUMNS, -1));
}

// 实现：碰撞检测（边界+已有方块）
bool GameArea::isCollision(Block& block) {
    auto shape = block.getCurrentShape();
    int rows = shape.size();
    int cols = shape[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j] == 1) {  // 仅检测方块的有效格子
                int gridX = block.x + j;
                int gridY = block.y + i;
                // 检查是否超出边界或与已有方块重叠
                if (gridX < 0 || gridX >= GRID_COLUMNS ||
                    gridY >= GRID_ROWS ||
                    (gridY >= 0 && grid[gridY][gridX] != -1)) {
                    return true;  // 碰撞
                }
            }
        }
    }
    return false;  // 无碰撞
}

// 实现：将方块固定到网格
void GameArea::lockBlock(Block& block) {
    auto shape = block.getCurrentShape();
    int rows = shape.size();
    int cols = shape[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j] == 1) {
                int gridX = block.x + j;
                int gridY = block.y + i;
                if (gridY >= 0) {  // 忽略超出顶部的部分
                    grid[gridY][gridX] = block.shapeIndex;
                }
            }
        }
    }
}

// 实现：绘制游戏区域和已固定的方块
void GameArea::render(SDL_Renderer* renderer) {
    // 绘制网格边框
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect border = {
        GAME_AREA_X - 1, GAME_AREA_Y - 1,
        GRID_COLUMNS * GRID_SIZE + 2,
        GRID_ROWS * GRID_SIZE + 2
    };
    SDL_RenderDrawRect(renderer, &border);

    // 绘制已固定的方块
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            if (grid[i][j] != -1) {
                SDL_Color color = COLORS[grid[i][j]];
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect rect = {
                    GAME_AREA_X + j * GRID_SIZE,
                    GAME_AREA_Y + i * GRID_SIZE,
                    GRID_SIZE - 1, GRID_SIZE - 1  // 留空隙
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}