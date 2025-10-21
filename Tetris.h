// tetris.h
#ifndef TETRIS_H  // ��ֹͷ�ļ��ظ�����
#define TETRIS_H

#include <SDL3/SDL.h>
#include <vector>

// ��Ϸ������ȫ�ֿɼ���
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 30;
const int GRID_COLUMNS = 10;
const int GRID_ROWS = 20;
const int GAME_AREA_X = 50;
const int GAME_AREA_Y = 50;

// ������״����ɫ��ȫ�ֳ�������Block��GameAreaʹ�ã�
const std::vector<std::vector<std::vector<int>>> SHAPES = {
    {{1, 1, 1, 1}},                                  // I��
    {{1, 1}, {1, 1}},                                // O��
    {{1, 1, 1}, {0, 1, 0}},                          // T��
    {{1, 1, 1}, {1, 0, 0}},                          // L��
    {{1, 1, 1}, {0, 0, 1}},                          // J��
    {{1, 1, 0}, {0, 1, 1}},                          // S��
    {{0, 1, 1}, {1, 1, 0}}                           // Z��
};

const std::vector<SDL_Color> COLORS = {
    {0, 255, 255, 255},    // I�ͣ���ɫ
    {255, 255, 0, 255},    // O�ͣ���ɫ
    {128, 0, 128, 255},    // T�ͣ���ɫ
    {255, 165, 0, 255},    // L�ͣ���ɫ
    {0, 0, 255, 255},      // J�ͣ���ɫ
    {0, 255, 0, 255},      // S�ͣ���ɫ
    {255, 0, 0, 255}       // Z�ͣ���ɫ
};

// ����������
class Block {
public:
    int x, y;                  // �������꣨��, �У�
    int shapeIndex;            // ��״����
    int rotation;              // ��ת״̬��0-3��
    SDL_Color color;           // ��ɫ

    Block();  // ���캯������
    std::vector<std::vector<int>> getCurrentShape();  // ��ȡ��ǰ��ת״̬����״
};

// ��Ϸ����������
class GameArea {
private:
    std::vector<std::vector<int>> grid;  // ����״̬��-1Ϊ�գ�
public:
    GameArea();  // ���캯������
    bool isCollision(Block& block);      // ��ײ���
    void lockBlock(Block& block);        // �̶����鵽����
    void render(SDL_Renderer* renderer); // ������Ϸ����
};

#endif  // TETRIS_H
