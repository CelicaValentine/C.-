
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
