// tetris.cpp
#include "tetris.h"
#include <cstdlib>
#include <ctime>

// Block�๹�캯��ʵ�֣������ʼ������
Block::Block() {
    std::srand(std::time(0));  // ��ʼ�����������
    shapeIndex = std::rand() % SHAPES.size();  // ���ѡ����״
    color = COLORS[shapeIndex];                // ƥ����ɫ
    rotation = 0;                              // ��ʼ��ת״̬
    // ��ʼλ�ã���������
    x = GRID_COLUMNS / 2 - SHAPES[shapeIndex][0].size() / 2;
    y = 0;
}

// ʵ�֣���ȡ��ǰ��ת״̬����״��90��˳ʱ����ת��
std::vector<std::vector<int>> Block::getCurrentShape() {
    auto& original = SHAPES[shapeIndex];
    int rows = original.size();
    int cols = original[0].size();

    if (rotation == 0) return original;  // 0����תֱ�ӷ���ԭ��״

    // ��ת�߼�������ת�ò���ת��
    std::vector<std::vector<int>> rotated(cols, std::vector<int>(rows, 0));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = original[i][j];
        }
    }
    return rotated;
}

// GameArea�๹�캯��ʵ�֣���ʼ��������
GameArea::GameArea() {
    grid.resize(GRID_ROWS, std::vector<int>(GRID_COLUMNS, -1));
}

// ʵ�֣���ײ��⣨�߽�+���з��飩
bool GameArea::isCollision(Block& block) {
    auto shape = block.getCurrentShape();
    int rows = shape.size();
    int cols = shape[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j] == 1) {  // ����ⷽ�����Ч����
                int gridX = block.x + j;
                int gridY = block.y + i;
                // ����Ƿ񳬳��߽�������з����ص�
                if (gridX < 0 || gridX >= GRID_COLUMNS ||
                    gridY >= GRID_ROWS ||
                    (gridY >= 0 && grid[gridY][gridX] != -1)) {
                    return true;  // ��ײ
                }
            }
        }
    }
    return false;  // ����ײ
}

// ʵ�֣�������̶�������
void GameArea::lockBlock(Block& block) {
    auto shape = block.getCurrentShape();
    int rows = shape.size();
    int cols = shape[0].size();

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (shape[i][j] == 1) {
                int gridX = block.x + j;
                int gridY = block.y + i;
                if (gridY >= 0) {  // ���Գ��������Ĳ���
                    grid[gridY][gridX] = block.shapeIndex;
                }
            }
        }
    }
}

// ʵ�֣�������Ϸ������ѹ̶��ķ���
void GameArea::render(SDL_Renderer* renderer) {
    // ��������߿�
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_Rect border = {
        GAME_AREA_X - 1, GAME_AREA_Y - 1,
        GRID_COLUMNS * GRID_SIZE + 2,
        GRID_ROWS * GRID_SIZE + 2
    };
    SDL_RenderDrawRect(renderer, &border);

    // �����ѹ̶��ķ���
    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLUMNS; j++) {
            if (grid[i][j] != -1) {
                SDL_Color color = COLORS[grid[i][j]];
                SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
                SDL_Rect rect = {
                    GAME_AREA_X + j * GRID_SIZE,
                    GAME_AREA_Y + i * GRID_SIZE,
                    GRID_SIZE - 1, GRID_SIZE - 1  // ����϶
                };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}