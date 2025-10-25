
#define _CRT_SECURE_NO_WARNINGS  // 解决sprintf安全警告
#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <iostream>
#include<easyx.h>

using namespace std;

// 定义常量（方块大小、游戏区域尺寸等）
#define BLOCK_SIZE 20                       // 每个小方块的像素大小
#define WIDTH 10                            // 游戏区域列数（横向方块数）
#define HEIGHT 20                           // 游戏区域行数（纵向方块数）
#define WIN_WIDTH BLOCK_SIZE * (WIDTH + 6)  // 窗口宽度（含右侧信息区）
#define WIN_HEIGHT BLOCK_SIZE * HEIGHT      // 窗口高度
#define GRAY RGB(192, 192, 192)             // 定义灰色常量

// 定义方块颜色（对应不同形状）
// 例子:将文本颜色设置为蓝色（索引3）,SetTextColor(hdc, blockColor[3]);
COLORREF blockColor[] = {
    RGB(0, 0, 0),      // 0-黑色
    RGB(255, 0, 0),    // 1-红色
    RGB(0, 255, 0),    // 2-绿色
    RGB(0, 0, 255),    // 3-蓝色
    RGB(255, 255, 0),  // 4-黄色
    RGB(255, 165, 0),  // 5-橙色
    RGB(128, 0, 128),  // 6-紫色
    RGB(0, 255, 255),   // 7-青色
    RGB(255,255,255),   // 8-白色
    RGB(240,240,240),    //9-浅灰色背景
    RGB(160, 82, 45)    //10-牛皮色背景
};


//************游戏的全局变量****************//
typedef struct KeyConfig {
    char keyrotate; // 旋转
    char keyleft;   // 左移
    char keyright;  // 右移
    char keydown;   // 下移
    char keydrop;   // 快速落地
    char keyquit;   // 退出游戏
    char keypause;  // 暂停游戏
}Key; Key keyset;
ExMessage msg = { 0 };  //建立外设输入的结构体存储


// 定义7种俄罗斯方块形状（4x4矩阵）
int blockShape[7][4][4][4] = {
    // I型：4种旋转状态（横/竖）
        {
            {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // 状态0：竖条
            {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：横条
            {{1,0,0,0}, {1,0,0,0}, {1,0,0,0}, {1,0,0,0}},  // 状态2：竖条（同状态0）
            {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：横条（同状态1）
        },
    // L型：4种旋转状态（右、下、左、上）
    {
        {{1,0,0,0}, {1,0,0,0}, {1,1,0,0}, {0,0,0,0}},  // 状态0：右L
        {{1,1,1,0}, {1,0,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：下L
        {{1,1,0,0}, {0,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态2：左L
        {{0,0,1,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：上L
    },
    // 反L型：4种旋转状态（左、下、右、上）
    {
        {{0,1,0,0}, {0,1,0,0}, {1,1,0,0}, {0,0,0,0}},  // 状态0：左反L
        {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1：下反L
        {{1,1,0,0}, {1,0,0,0}, {1,0,0,0}, {0,0,0,0}},  // 状态2：右反L
        {{1,1,1,0}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3：上反L
    },
    // 方形：4种状态相同（旋转后不变）
    {
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态1
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2
        {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}}   // 状态3
    },
    // T型：4种旋转状态（上、右、下、左）
    {
        {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：上T
        {{0,1,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态1：右T
        {{0,0,0,0}, {1,1,1,0}, {0,1,0,0}, {0,0,0,0}},  // 状态2：下T
        {{0,1,0,0}, {0,1,1,0}, {0,1,0,0}, {0,0,0,0}}   // 状态3：左T
    },
    // Z型：2种旋转状态（右、左，重复两次）
    {
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：右Z
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}},  // 状态1：左Z
        {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2：右Z（同状态0）
        {{0,1,0,0}, {1,1,0,0}, {1,0,0,0}, {0,0,0,0}}   // 状态3：左Z（同状态1）
    },
    // 反Z型：2种旋转状态（左、右，重复两次）
    {
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态0：左反Z
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}},  // 状态1：右反Z
        {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}},  // 状态2：左反Z（同状态0）
        {{1,0,0,0}, {1,1,0,0}, {0,1,0,0}, {0,0,0,0}}   // 状态3：右反Z（同状态1）
    }
};
// 游戏区域数组,0表示空白，非0表示有方块
int gameArea[HEIGHT][WIDTH] = { 0 };
// 当前方块信息,currShape:方块类型,currRot:旋转状态,currX:横坐标,currY:纵坐标
int currShape, currRot, currX, currY;
// 下一个方块类型
int nextShape;
// 游戏数据
int score = 0;
int gameTime = 0;   //游戏游玩时长
time_t startTime;   // 游戏开始时间

int speed = 1000;   //初始速度
int iscore = 0;     //速度挡位，初始为变速
int colorcode = 0;  //背景颜色编号标识

bool modifykeyflag = false; //键位修改标识


// 函数声明
void initGame();    //初始化游戏
void createBlock(int& shape, int& rot, int& x, int& y); //创建新方块
void drawBlock(int shape, int rot, int x, int y);   //绘制方块
void drawGameArea();    //绘制游戏区域
void drawInfo();    //绘制右侧信息区
bool checkCollision(int shape, int rot, int x, int y); //检测碰撞
void rotateBlock(); //旋转方块
bool moveBlock(int dx, int dy); //移动方块
void dropBlock();   //快速落地
void eliminateLines();  //消除满行
void updateTime();  //  更新时间
bool isGameOver(int shape, int rot, int x, int y);  //检查游戏结束
void gameOver();    //处理游戏结束
void Gamepaused(); //游戏暂停方法
bool checkAnyKeyPressed(); //检测键盘输入任意键
void handleInput(); //处理键盘输入
void Menusq(); //菜单页面
void Setting();
void drawTextBorder(int x, int y, const wchar_t* text, COLORREF textColor, COLORREF bgColor);
//void Confirm();
void SpeedChoice();
void Speed();
void SetColor();
void colormodle();
//bool modifykey(char* key, char mfkey); //键位修改


/*————————————————————————————游戏的线程————————————————————————————————————————*/
int main() {
    initgraph(WIN_WIDTH, WIN_HEIGHT);   // 初始化图形窗口
    BeginBatchDraw();//开启双缓冲
    //char* ch=NULL;
    //ch=& keyset.keyquit;
    //modifykey(ch,'G');
    Menusq();

    while (true)
    {
        handleInput(); // 处理键盘输入
        static DWORD lastDropTime = 0;  // 记录上次自动下落时间
        Speed();
        if (GetTickCount() - lastDropTime > speed) {
            if (!moveBlock(0, 1)) { // 无法下移，固定方块
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (blockShape[currShape][currRot][i][j]) { // 固定方块到游戏区域
                            int x = currX + j;
                            int y = currY + i;
                            if (y >= 0) gameArea[y][x] = currShape + 1; // 使用非0值表示有方块
                        }
                    }
                }
                eliminateLines();   // 消除满行

                if (isGameOver(currShape, currRot, currX, currY)) { // 检测新生成的当前方块是否碰撞（游戏结束判定）
                    gameOver();
                    break;
                }
                // 生成新方块
                currShape = nextShape;
                currRot = 0;               // 新方块初始旋转状态
                currX = WIDTH / 2 - 2;    // 新方块X初始位置
                currY = -4;              // 新方块Y初始位置
                createBlock(nextShape, currRot, currX, currY);
            }
            lastDropTime = GetTickCount();  // 更新下落时间
        }

        //更新显示
        updateTime();
        cleardevice();  //清屏
        drawGameArea();
        drawBlock(currShape, currRot, currX, currY);
        drawInfo();
        FlushBatchDraw();   // 刷新绘图，将双缓冲中的内容显示到屏幕上
        Sleep(10);   // 减少CPU占用
    }
    closegraph();   // 关闭图形窗口
}

//初始化游戏
void initGame() {
    srand((unsigned int)time(NULL));
    startTime = time(NULL);
    // 初始化按键配置
    keyset.keyrotate = 'W';
    keyset.keyleft = 'A';
    keyset.keyright = 'D';
    keyset.keydown = 'S';
    keyset.keydrop = ' ';
    keyset.keyquit = 'Q';
    keyset.keypause = 'P';

    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            gameArea[i][j] = 0;
    int tempX, tempY;  // 临时变量,防止createBlock修改下一个currX,currY
    tempX = currX;
    tempY = currY;
    createBlock(currShape, currRot, tempX, tempY);  // 创建当前方块
    createBlock(nextShape, currRot, currX, currY);  // 创建下一个方块
}

//创建新方块
void createBlock(int& shape, int& rot, int& x, int& y) {
    shape = rand() % 7;   // 随机选择方块形状
    rot = 0;             // 初始旋转状态
    x = WIDTH / 2 - 2;      // 初始位置在顶部中央
    y = -4;            // 初始位置在顶部外
}

//绘制方块
void drawBlock(int shape, int rot, int x, int y) {  // x,y为方块左上角在游戏区域中的坐标,rot为旋转状态,shape为方块形状
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[shape][rot][i][j] && (y + i >= 0)) {
                int drawX = (x + j) * BLOCK_SIZE;
                int drawY = (y + i) * BLOCK_SIZE;
                setfillcolor(blockColor[shape + 1]);                    // shape从0开始，颜色从1开始
                solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);// 绘制方块
                setlinecolor(blockColor[0]);                // 设置边框颜色
                rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);    // 绘制边框
            }
        }
    }
}

//绘制游戏区域
void drawGameArea() {
    setlinecolor(RED);  // 设置边框颜色
    rectangle(0, 0, WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE);   // 绘制边框

    // 绘制网格和已固定的方块
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int x = j * BLOCK_SIZE;
            int y = i * BLOCK_SIZE;
            setlinecolor(LIGHTGRAY);    // 设置网格线颜色
            line(x, 0, x, HEIGHT * BLOCK_SIZE);     // 绘制竖线,line(x1, y1, x2, y2),x1=x2时为竖线,y1=y2时为横线
            line(0, y, WIDTH * BLOCK_SIZE, y);      // 绘制横线,x1表示起点横坐标,y1表示起点纵坐标,x2表示终点横坐标,y2表示终点纵坐标

            if (gameArea[i][j] != 0) {
                setfillcolor(blockColor[gameArea[i][j]]);                      // 设置方块颜色
                solidrectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1); // 绘制方块
                setlinecolor(RED);                                         // 设置边框颜色
                rectangle(x, y, x + BLOCK_SIZE - 1, y + BLOCK_SIZE - 1);    // 绘制边框
            }
        }
    }
}

//绘制右侧信息区
void drawInfo() {

    int infoX = WIDTH * BLOCK_SIZE + 10;  // 右侧信息区起始X坐标
    settextcolor(blockColor[9]);          // 设置文本颜色
    setbkmode(TRANSPARENT);     // 设置文本背景透明

    // 绘制分数（使用宽字符数组和swprintf_s）
    WCHAR scoreText[20];                // 宽字符数组
    swprintf_s(scoreText, L"分数: %d", score);  // 直接格式化宽字符串
    outtextxy(infoX, 20, scoreText);    // 绘制分数

    // 绘制游戏时间
    WCHAR timeText[20];          // 宽字符数组
    swprintf_s(timeText, L"时间: %ds", gameTime);  // 直接格式化宽字符串
    outtextxy(infoX, 60, timeText);     // 绘制时间

    // *****绘制下一个方块提示*****
    outtextxy(infoX, 100, L"下一个:");  // 直接使用宽字符串前缀L

    // 绘制下一个方块
    int nextX = infoX + (BLOCK_SIZE * 4 - BLOCK_SIZE * 2) / 2;
    int nextY = 120;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (blockShape[nextShape][0][i][j]) {
                int drawX = nextX + j * BLOCK_SIZE;
                int drawY = nextY + i * BLOCK_SIZE;
                setfillcolor(blockColor[nextShape + 1]);                            //方块颜色
                solidrectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);//绘制方块
                setlinecolor(blockColor[0]);                                            //边框颜色
                rectangle(drawX, drawY, drawX + BLOCK_SIZE - 1, drawY + BLOCK_SIZE - 1);//绘制边框
            }
        }
    }

    // 绘制操作提示（直接使用宽字符串前缀L),outtextxy(x, y, L"文本内容")函数支持宽字符串的输出
    outtextxy(infoX, 220, L"操作:");
    outtextxy(infoX, 240, L"W - 旋转");
    outtextxy(infoX, 260, L"A - 左移");
    outtextxy(infoX, 280, L"D - 右移");
    outtextxy(infoX, 300, L"S - 下移");
    outtextxy(infoX, 320, L"空格 - 落地");
    outtextxy(infoX, 340, L"Q - 退出");
}


//检测碰撞
bool checkCollision(int shape, int rot, int x, int y) {
    // 遍历4x4方块矩阵的每个格子（俄罗斯方块标准用4x4矩阵存储形状）
    for (int i = 0; i < 4; i++) {       // i：方块矩阵的行（对应游戏区域的Y方向）
        for (int j = 0; j < 4; j++) {   // j：方块矩阵的列（对应游戏区域的X方向）
            // 只处理方块实际存在的格子（blockShape为1的部分）
            if (blockShape[shape][rot][i][j] == 1) {
                // 计算当前格子在游戏区域中的实际坐标
                int gridX = x + j;  // 转换为游戏区域的列坐标
                int gridY = y + i;  // 转换为游戏区域的行坐标

                // 1. 左右边界碰撞：超出游戏区域左右边缘（完全禁止）
                if (gridX < 0 || gridX >= WIDTH) {
                    return true;  // 左边越界或右边越界，碰撞
                }

                // 2. 底部边界碰撞：超出游戏区域下边缘（落地判定）
                if (gridY >= HEIGHT) {
                    return true;  // 底部越界，碰撞（无法继续下移）
                }

                // 4. 与已固定方块的碰撞：当前格子在游戏区域内（gridY >=0），且已有方块
                if (gridY >= 0 && gameArea[gridY][gridX] != 0) {
                    return true;        // 与已固定的方块重叠，碰撞
                }
            }
        }
    }
    // 所有格子都无碰撞
    return false;
}

//旋转方块
void rotateBlock() {
    int newRot = (currRot + 1) % 4;                        // 计算新的旋转状态(循环)
    if (!checkCollision(currShape, newRot, currX, currY)) // 无碰撞，更新旋转状态
        currRot = newRot;
}

//移动方块
bool moveBlock(int dx, int dy) {
    int newX = currX + dx;
    int newY = currY + dy;
    if (!checkCollision(currShape, currRot, newX, newY)) {  // 无碰撞，更新位置
        currX = newX;
        currY = newY;
        return true;
    }
    return false;
}

//快速落地
void dropBlock() {
    while (moveBlock(0, 1));
}

//消除满行
void eliminateLines() {
    int lineCount = 0;  // 记录消除的行数
    for (int i = HEIGHT - 1; i >= 0; i--) {     // 从底部开始检查每一行
        bool isFull = true;                     // 检查变量，当前行是否满
        for (int j = 0; j < WIDTH; j++) {      //每列检查是否都有方块
            if (gameArea[i][j] == 0) {
                isFull = false;
                break;
            }
        }

        if (isFull) {       // 如果当前行满，消除该行
            lineCount++;
            for (int k = i; k > 0; k--)
                for (int j = 0; j < WIDTH; j++)
                    gameArea[k][j] = gameArea[k - 1][j];
            for (int j = 0; j < WIDTH; j++)
                gameArea[0][j] = 0;
            i++;
        }
    }

    switch (lineCount) {    // 根据消除行数更新分数
    case 1: score += 10; break;
    case 2: score += 30; break;
    case 3: score += 50; break;
    case 4: score += 80; break; // 四行一次性消除，得分最高
    }
}

// 更新时间
void updateTime() {
    gameTime = (int)(time(NULL) - startTime);
}

//检查游戏结束
bool isGameOver(int shape, int rot, int x, int y) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (blockShape[shape][rot][i][j] == 1) {
                int gridY = y + i;  // 转换为游戏区域的行坐标
                if (gridY < 0) {
                    return true;
                }
            }
    return false;
}

//处理游戏结束
void gameOver() {
    cleardevice();  // 清屏
    while (1) {
        settextcolor(RED);  // 设置文本颜色
        settextstyle(40, 0, L"宋体");  // 宽字符字体名，结束标题
        outtextxy(WIN_WIDTH / 2 - 120, WIN_HEIGHT / 2 - 60, L"游戏结束！");

        WCHAR scoreText[20];  // 宽字符数组,分数显示
        swprintf_s(scoreText, L"最终分数: %d", score);
        outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, scoreText);

        settextstyle(20, 0, L"宋体");  // 宽字符字体名
        outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 + 40, L"按任意键退出...");
        FlushBatchDraw();   // 刷新绘图，将双缓冲中的内容显示到屏幕上
        if (checkAnyKeyPressed())
            break;  //检测按键，退出循环然后关闭窗口 
        Sleep(100);   // 减少CPU占用
    }
    cleardevice();
    closegraph();
    exit(0);
}

//游戏暂停方法
void Gamepaused() {
    cleardevice();// 清屏
    settextcolor(blockColor[9]);                       // 设置文本颜色
    settextstyle(40, 0, L"宋体");             // 宽字符字体名，暂停标题
    outtextxy(WIN_WIDTH / 2 - 80, WIN_HEIGHT / 2 - 60, L"游戏暂停");
    settextstyle(20, 0, L"宋体");              // 宽字符字体名
    outtextxy(WIN_WIDTH / 2 - 100, WIN_HEIGHT / 2, L"按暂停键继续...");
    FlushBatchDraw();// 刷新绘图，将双缓冲中的内容显示到屏幕上
    while (true) {                        //在暂停中等待按键触发
        if (peekmessage(&msg, EX_KEY))
            if (msg.message == WM_KEYDOWN && msg.vkcode == keyset.keypause) { 
                break;
            }
            else if (msg.message == WM_KEYUP) { Sleep(16); }
    }
    startTime = time(NULL) - gameTime;  //调整时间，删除暂停时间
}

//检测键盘输入任意键位
bool checkAnyKeyPressed() {
    for (int key = 8; key <= 255; ++key) { //遍历一遍键位
        if (GetAsyncKeyState(key) & 0x8000) {
            return true;  // 有任意键被按下
        }
    }
    return false;
}

//处理键盘输入，《模板非黑框的键盘输入方式》
void handleInput() {

    if (peekmessage(&msg, EX_KEY))
    {
        if (msg.message == WM_KEYDOWN) 
        {
            if (msg.vkcode == keyset.keyrotate)
            {
                rotateBlock();
            } // 旋转方块
            else if (msg.vkcode == keyset.keydown)
            {
                moveBlock(0, 1);
            }  // 下移方块
            else if (msg.vkcode == keyset.keyleft)
            {
                moveBlock(-1, 0);
            } // 左移方块
            else if (msg.vkcode == keyset.keyright)
            {
                moveBlock(1, 0);
            } // 右移方块
            else if (msg.vkcode == keyset.keydrop)
            {
                dropBlock();
            }// 快速落地
            else if (msg.vkcode == keyset.keyquit)
            {
                gameOver();
            }  // 退出游戏
            else if (msg.vkcode == keyset.keypause)
            {
                Gamepaused();
            }   // 暂停游戏
        }
        else if (msg.message == WM_KEYUP) { Sleep(16); }
    }
}

//键位修改
//bool modifykey(char* key, char mfkey) {
//
//    while (GetAsyncKeyState(key);  //等待松开暂停键
//    while (true)                        //在暂停中等待按键触发
//        if (GetAsyncKeyState(keyset.keypause) & 0x8000)
//            break;
//    if (mfkey == keyset.keyrotate || mfkey == keyset.keydown ||
//        mfkey == keyset.keyleft || mfkey == keyset.keyright ||
//        mfkey == keyset.keydrop || mfkey == keyset.keypause ||
//        mfkey == keyset.keyquit)    //检测键位是否占用
//        return false;
//    *key = mfkey;   //修改键位
//    return true;
//}

//游戏菜单界面
void Menusq() {
    cleardevice();       //清屏
    int selected = 0;   // 当前选中项的判断值 
    settextstyle(25, 0, L"宋体");
    const wchar_t* menuItems[3] = { L"开始游戏", L"退出游戏", L"设置" };  // 菜单选项文本（按顺序存储，方便循环绘制）
    int itemY[3] = { WIN_HEIGHT/2-20, WIN_HEIGHT/2+20, WIN_HEIGHT/2+60 }; // 每个选项的Y坐标
    if (msg.message == WM_KEYUP) { Sleep(100); }    //防止返回游戏菜单连触发
    while (true) {
        settextcolor(BROWN);    //字体颜色
        settextstyle(35, 0, L"宋体");
        outtextxy(WIN_WIDTH/2-100, WIN_HEIGHT/2-100,L"俄罗斯方块");

        for (int i = 0; i < 3; i++) {

            settextcolor(WHITE); // 未选中的文字为白色
            outtextxy(WIN_WIDTH / 2 - 100, itemY[i], menuItems[i]); //通过i找到要渲染文字的坐标
        }
        //绘制当前选中项的边框和高亮文字
        settextcolor(YELLOW);   // 选中的文字为黄色
        drawTextBorder(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected], RED, BLACK);
        outtextxy(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected]);
        FlushBatchDraw();   //渲染缓冲画面
        //处理按键输入（0:开始, 1:退出, 2:设置）
        if (peekmessage(&msg, EX_KEY))
        {
            if (msg.message == WM_KEYDOWN)
            {
                if(msg.vkcode == VK_UP) //上键处理
                    selected = (selected - 1 + 3) % 3; // 循环上移（0→2）
                else if(msg.vkcode == VK_DOWN)  //下键处理
                    selected = (selected + 1 + 3) % 3; // 循环下移（2→0）
                else if(msg.vkcode == VK_RETURN)    //回车处理_选项
                    switch (selected) {
                    case 0:// 开始游戏
                        initGame(); // 初始化游戏数据（生成初始方块、清空地图、初始化分数和时间）   
                        return; 
                    case 1:// 退出
                        exit(0); 
                    case 2://设置
                        Setting();  //调用设置方法
                        cleardevice();  //清屏
                        break;
                    }
            }
            else if (msg.message == WM_KEYUP) { Sleep(100); }
        }
        Sleep(20);  //防止高刷新
    }
}

// 绘制文本边框，保存当前绘图状态（避免影响后续绘图）
void drawTextBorder(int x, int y, const wchar_t* text, COLORREF textColor, COLORREF bgColor) {
      
    COLORREF oldTextColor = gettextcolor();  // 保存原文本色  
    int oldBkMode = getbkmode();             // 保存原背景模式  
    COLORREF oldBkColor = getbkcolor();      // 保存原背景色  

    // 设置背景色和文本色  
    setbkmode(OPAQUE);                       // 开启不透明背景  
    setbkcolor(bgColor);                     // 设置背景色  
    settextcolor(textColor);                 // 设置文本前景色  

    // 绘制文本  
    outtextxy(x, y, text);

    // 恢复原绘图状态  
    settextcolor(oldTextColor);
    setbkmode(oldBkMode);
    setbkcolor(oldBkColor);
}

//设置下落速度
void SpeedChoice()
{
    int selected = 0; // 当前选中项（0:开始, 1:退出, 2:设置）
    initGame();
    cleardevice();
    settextcolor(WHITE);
    settextstyle(20, 0, L"宋体");
    outtextxy(WIN_WIDTH / 2 - 150, WIN_HEIGHT / 2 - 100, L"速度设置");
    settextstyle(25, 0, L"宋体");
    // 菜单选项文本（按顺序存储，方便循环绘制）
    const wchar_t* menuItems[4] = { L"变速", L"慢速", L"中等", L"快速" };
    int itemY[4] = { WIN_HEIGHT / 2 - 20, WIN_HEIGHT / 2 + 40, WIN_HEIGHT / 2 + 80 ,WIN_HEIGHT / 2 + 100 }; // 每个选项的Y坐标
    while (1) {
        //绘制所有菜单选项（未选中状态）
        for (int i = 0; i < 4; i++) {

            settextcolor(WHITE); // 未选中文字为
            outtextxy(WIN_WIDTH / 2 - 100, itemY[i], menuItems[i]);//通过i找到要渲染文字的坐标
        }
        //绘制当前选中项的边框和高亮文字
        settextcolor(YELLOW); // 选中文字为黄色
        drawTextBorder(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected], RED, BLACK);
        outtextxy(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected]);
        FlushBatchDraw();

        if (peekmessage(&msg, EX_KEY))
        {
            if (msg.message == WM_KEYDOWN)
            {
                if (msg.vkcode == VK_UP) //上键处理
                    selected = (selected - 1 + 4) % 4;  // 循环上移
                else if (msg.vkcode == VK_DOWN)  //下键处理
                    selected = (selected + 1 + 4) % 4; // 循环下移
                else if (msg.vkcode == VK_RETURN)  //回车处理_选项
                        switch (selected) 
                        {
                            case 0:
                                iscore = 0;
                                return; // 开始游戏
                            case 1:
                                iscore = 1;
                                return; // 退出
                            case 2:
                                iscore = 2;
                                return; // 进入设置后返回菜单需清屏
                            case 3:
                                iscore = 3;
                                return;
                        }
            }
            else if (msg.message == WM_KEYUP) { Sleep(100); }
        }


        ////处理按键输入
        //if (GetAsyncKeyState(VK_UP) & 0x8000) {
        //    selected = (selected - 1 + 3) % 4; // 循环上移（0→2）
        //    Sleep(150); // 防止连跳
        //}
        //else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        //    selected = (selected + 1) % 4; // 循环下移（2→0）
        //    Sleep(150);
        //}
        //else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        //    switch (selected) {
        //    case 0:
        //        iscore = 0;
        //        return; // 开始游戏
        //    case 1:
        //        iscore = 1;
        //        return; // 退出
        //    case 2:
        //        iscore = 2;
        //        return; // 进入设置后返回菜单需清屏
        //    case 3:
        //        iscore = 3;
        //        return;
        //    }
        //}

        Sleep(10);
    }
}

//修改下落速度的参数方法
void Speed()
{
    switch (iscore)
    {
    case 0:     //变速方法，越来越快
        iscore = score;
        switch (iscore / 100)
        {
        case 0:speed = 1000; break;
        case 1:speed = 900; break;
        case 2:speed = 800; break;
        case 3:speed = 700; break;
        case 4:speed = 600; break;
        case 5:speed = 500; break;
        case 6:speed = 400; break;
        case 7:speed = 300; break;
        case 8:speed = 200; break;
        case 9:speed = 100; break;
        case 10:speed = 50; break;
        default:speed = 25; break;
        }
        break;
    case 1:speed = 1000; break;     //慢 1s
    case 2:speed = 600; break;      //平 0.6s
    case 3:speed = 300; break;      //快 0.3s
    }
}


//设置界面
void Setting()
{
	if (msg.message == WM_KEYUP) { Sleep(100); }    //防止连跳
    Sleep(100);
    int selected = 0; // 当前选中项判断值（0:退出, 1:设置速度, 2:设置键位，3：设置背景颜色，4：设置音乐）
    cleardevice();  //清屏
    settextcolor(WHITE);    //字体颜色
    settextstyle(20, 0, L"宋体");
    outtextxy(WIN_WIDTH / 2 - 150, 0, L"设置");
    settextstyle(25, 0, L"宋体");
    // 菜单选项文本（按顺序存储，方便循环绘制）
    const wchar_t* menuItems[5] = { L"退出", L"设置速度", L"设置键位" , L"设置背景色" , L"设置音乐" };
    int itemY[5] = { WIN_HEIGHT / 2 - 80 , WIN_HEIGHT / 2 - 40,WIN_HEIGHT / 2, WIN_HEIGHT / 2 + 40, WIN_HEIGHT / 2 + 80, }; // 每个选项的Y坐标
    while (true) {
        setbkmode(OPAQUE);          // 开启透明背景  
        setbkcolor(blockColor[colorcode]);
        // 1. 绘制所有菜单选项（未选中状态）
        for (int i = 0; i < 5; i++) {

            settextcolor(WHITE); // 未选中文字为
            outtextxy(WIN_WIDTH / 2 - 100, itemY[i], menuItems[i]);//通过i找到要渲染文字的坐标
        }
        // 2. 绘制当前选中项的边框和高亮文字
        settextcolor(YELLOW); // 选中文字为黄色
        drawTextBorder(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected], RED, BLACK);
        outtextxy(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected]);

        if (peekmessage(&msg, EX_KEY))  //键盘的输入处理
        {
            if (msg.message == WM_KEYDOWN)
            {
                if (msg.vkcode == VK_UP) //上键处理
                    selected = (selected - 1 + 5) % 5; // 循环上移
                else if (msg.vkcode == VK_DOWN)  //下键处理
                    selected = (selected + 1) % 5; // 循环下移
                else if (msg.vkcode == VK_RETURN)    //回车处理_选项
                    switch (selected) 
                    {
                        case 0:
                            return;
                        case 1:
                            SpeedChoice();
                            return;
                        case 2:
                            return;
                        case 3:
                            colormodle();
                            return;
                        case 4:
                            return;
                    }
            }
            else if (msg.message == WM_KEYUP) { Sleep(100); }
        }



        //if (GetAsyncKeyState(VK_UP) & 0x8000) {
        //    selected = (selected - 1 + 5) % 5; // 循环上移（0→2）
        //    Sleep(150); // 防止连跳
        //}
        //else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        //    selected = (selected + 1) % 5; // 循环下移（2→0）
        //    Sleep(150);
        //}

        //else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        //    switch (selected) {
        //    case 0:
        //        return; 
        //    case 1:
        //        SpeedChoice();
        //        return;
        //    case 2:
        //        return;
        //    case 3:
        //        colormodle();
        //        return;
        //    case 4:
        //        return;


        FlushBatchDraw();   //渲染缓冲数据
        Sleep(16);          //降低刷新
    }
}

//背景颜色设置
void SetColor(){
        cleardevice();
        int selected = 0; // 当前选中项（0:黑色, 1:浅灰色, 2:咖啡色, 3:ihuf）
        settextstyle(15, 0, L"宋体");
        const wchar_t* menuItems[4] = { L"黑色", L"浅灰色", L"咖啡色", L"ihuf" };
        int itemY[4] = { WIN_HEIGHT / 2 - 40, WIN_HEIGHT / 2, WIN_HEIGHT / 2 + 40, WIN_HEIGHT / 2 + 80 };

        while (true) {
            cleardevice();
            settextcolor(RED);
            settextstyle(20, 0, L"宋体");
            outtextxy(WIN_WIDTH / 2 - 150, WIN_HEIGHT / 2 - 100, L"选择颜色");

                for (int i = 0; i < 4; i++) {   //绘制所有菜单选项（未选中的）
                settextcolor(WHITE);
                outtextxy(WIN_WIDTH / 2 - 100, itemY[i], menuItems[i]);
                }

                settextcolor(YELLOW);
                drawTextBorder(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected], RED, BLACK);
                outtextxy(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected]);
                FlushBatchDraw();   //渲染缓冲内容

                //处理按键输入
                if (peekmessage(&msg, EX_KEY))  //键盘的输入处理
                {
                    if (msg.message == WM_KEYDOWN)
                    {
                        if (msg.vkcode == VK_UP) //上键处理
                            selected = (selected - 1 + 4) % 4; // 循环上移
                        else if (msg.vkcode == VK_DOWN)  //下键处理
                            selected = (selected + 1 + 4) % 4; // 循环下移
                        else if (msg.vkcode == VK_RETURN)    //回车处理_选项
                            switch (selected) 
                            {
                                case 0: colorcode = 0; return;
                                case 1: colorcode = 9; return;
                                case 2: colorcode = 10; return;
                                case 3: colorcode = 11; return;
                            }
                    }
                    else if (msg.message == WM_KEYUP) { Sleep(100); }
                }

                //// 处理按键
                //if (GetAsyncKeyState(VK_UP) & 0x8000) {
                //selected = (selected - 1 + 4) % 4;
                //Sleep(150);
                //}
                //else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
                //selected = (selected + 1) % 4;
                //Sleep(150);
                //}
                //else if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                //    switch (selected) {
                //    case 0: colorcode = 0; return;
                //    case 1: colorcode = 9; return;
                //    case 2: colorcode = 10; return;
                //    case 3: colorcode = 11; return;
                //    }
                //    Sleep(150);
                //   }
        }
    }

//颜色设置
void colormodle()
{
    cleardevice();   //清屏
    int selected = 0; // 当前选中项 0：设置背景色 、 1：设置方块色
    settextstyle(30, 0, L"宋体");
    // 菜单选项文本（按顺序存储，方便循环绘制）
    const wchar_t* menuItems[2] = { L"设置背景色", L"设置方块色" };
    int itemY[2] = { WIN_HEIGHT / 2 - 40, WIN_HEIGHT / 2 };
    while (true) {
        settextcolor(WHITE);
        settextstyle(20, 0, L"宋体");
        outtextxy(WIN_WIDTH / 2 - 150, WIN_HEIGHT / 2 - 100, L"");
        // 1. 绘制所有菜单选项（未选中状态）
        for (int i = 0; i < 2; i++) {
            settextcolor(WHITE); // 未选中文字为白
            outtextxy(WIN_WIDTH / 2 - 100, itemY[i], menuItems[i]);//通过i找到要渲染文字的坐标，依次显示文字
        }
        // 2. 绘制当前选中项的边框和高亮文字
        settextcolor(YELLOW); // 选中文字为黄色
        drawTextBorder(WIN_WIDTH/2-100,itemY[selected],menuItems[selected],RED,BLACK);
        outtextxy(WIN_WIDTH / 2 - 100, itemY[selected], menuItems[selected]);

        // 3. 处理按键输入
        if (peekmessage(&msg, EX_KEY))  //键盘的输入处理
        {
            if (msg.message == WM_KEYDOWN)
            {
                if (msg.vkcode == VK_UP) //上键处理
                    selected = (selected - 1 + 2) % 2; // 循环上移
                else if (msg.vkcode == VK_DOWN)  //下键处理
                    selected = (selected + 1 + 2) % 2; // 循环下移
                else if (msg.vkcode == VK_RETURN)    //回车处理_选项
                    switch (selected)
                    {
                        case 0: SetColor(); return; // 开始游戏
                        case 1:colorcode = 1; return;
                    }
            }
            else if (msg.message == WM_KEYUP) { Sleep(100); }
        }
        
        //if (_kbhit()) {
        //    int key = _getch(); // 用int接收方向键的双字节扫描码
        //    if (key == 0xE0) { // 方向键前缀码
        //        key = _getch(); // 获取实际方向键扫描码
        //        switch (key) {
        //        case 0x48: // 上箭头
        //            selected = (selected - 1 + 2) % 2; // 循环上移（0→2）

        //            break;
        //        case 0x50: // 下箭头
        //            selected = (selected + 1) % 2; // 循环下移（2→0）
        //            break;

        //        }
        //    }
        //    else if (key == 13) {
        //        switch (selected) {
        //        case 0: SetColor(); return; // 开始游戏
        //        case 1:colorcode = 1; return;
        //        }
        //    }
        //}
        FlushBatchDraw();
        Sleep(10);
    }
}
