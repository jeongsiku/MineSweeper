#include <iostream>
#include <Windows.h>
#include <conio.h>

#define left 75
#define right 77
#define up 72
#define down 80

#define emptyPic 9
#define tilePic 0
#define minePic 11
#define flagPic 10

void setPos(int x, int y);
void draw();
void flag();
int dig(int curY, int curX);
int check(int x, int y);
void emptyCheck();
void mineDraw();
int sec;
int table[20][20];
int minePos[20][20];

int curX; int curY;
int gameEnd = 0;
int mineScore = 0;
int mineSetCount = 40; // 이 곳에 지뢰갯수 입력.30~40

void setMine()
{
    srand((unsigned int)time(NULL));
    int mineArr[400]={0,};
    int count = 0;
    int mPos = 0;
    while (count < mineSetCount) // 지뢰갯수
    {
        mPos = rand() % 400;
        if (mineArr[mPos] == 0)
        {
            mineArr[mPos] = 1;
            count++;
        }
        else
            continue;
    }
    for (int i = 0; i < 400; i++)
    {
        minePos[i / 20][i % 20] = mineArr[i];
    }
}

void mineView() 
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (minePos[i][j] == 1)
            {
                setPos(j*2,i);
                printf("MM");
            }
        }
    }
} 

void init()
{
    setMine();
    setPos(45, 5);
    printf("깃발은 f \n");
    setPos(45, 6);
    printf("땅파기는 스페이스바 \n");
    curY = 10;
    curX = 10;
}

void input()
{
    if (_kbhit())
    {
        int key = _getch();
        if (key == 224)
        {
            int key = _getch();
            switch (key)
            {
            case left:
                if (curX == 0)
                    break;
                curX--;
                draw();
                break;
            case right:
                if (curX == 19)
                    break;
                curX++;
                draw();
                break;
            case up:
                if (curY == 0)
                    break;
                curY--;
                draw();
                break;
            case down:
                if (curY == 19)
                    break;
                curY++;
                draw();
                break;
            
            }
        }
        switch (key)
        {
        case 102: // F
            flag();
            draw();
            break;
        case 32: // 스페이스 바
            dig(curY, curX);
            emptyCheck();
            draw();
            break;
        }
        
    }
}
void flag()
{
    if (table[curY][curX] == 10)
    {
        table[curY][curX] = 0;
        if (mineScore >0 )
            mineScore--;
    }
    else
    {
        table[curY][curX] = 10;
        if (mineScore < mineSetCount)
            mineScore++;
    }
}
int dig(int curY, int curX)
{
    if (minePos[curY][curX] == 1) // 밝고 죽은 것
    {
        mineDraw();
        gameEnd = 1;
        return 0;
    }
    else
    {
        int digResult=check(curY, curX);
        if (digResult==0)
        {
            for (int r = -1; r <= 1; r++)
            {
                if (curY + r < 0 || 19 < curY + r)continue;
                for (int c = -1; c <= 1; c++)
                {
                    if (curX + c < 0 || 19 < curX + c)continue;
                    if (r == 0 && c == 0)continue;
                    check(curY + r, curX + c);
                }
            }
        }
    }
    return 0;
}
void mineDraw()
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (minePos[i][j] == 1)
                table[i][j] = minePic;
        }
    }
}
void emptyCheck()
{
    int digcount = 0;
    do {
        digcount = 0;
        for (int r = 0; r < 20; r++)
        {
            for (int c = 0; c < 20; c++)
            {
                if (table[r][c] == emptyPic)
                {
                    for (int rr = -1; rr <= 1; rr++)
                    {
                        if (r + rr < 0 || 19 < r + rr)continue;
                        for (int cc = -1; cc <= 1; cc++)
                        {
                            if (c + cc < 0 || 19 < c + cc)continue;
                            if (table[r + rr][c + cc] == tilePic)
                            {
                                dig(r + rr, c + cc);
                                digcount++;
                            }
                                
                        }

                    }
                }

            }
        }
    } while (digcount != 0);
    
}

int check(int row, int col)
{
    int mineCheck = 0;
    for (int r = -1; r <= 1; r++)
    {
        if (row + r < 0 || 19 < row + r )continue;
        for (int c = -1; c <= 1; c++)
        {
            if (col + c < 0 || 19 < col + c )continue;
            if (r == 0 && c == 0)continue;
            
            if (minePos[row +r][col +c] == 1)
                mineCheck++;
        }

    }
    table[row][col] = mineCheck;
    if (mineCheck == 0)
    {
        table[row][col] = 9;
        return mineCheck;
    }
    return mineCheck;
}
void setPos(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void draw()
{
    setPos(0, 0);
    for (int i = 0; i < 20; i++)    {
        for (int j = 0; j < 20; j++) {
            switch (table[i][j])
            {
            case 0:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    7);
                printf("■");
                break;
            case 1:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    9);
                printf("01");
                break;
            case 2:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    10);
                printf("02");
                break;
            case 3:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    11);
                printf("03");
                break;
            case 4:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    12);
                printf("04");
                break;
            case 5:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    13);
                printf("05");
                break;
            case 6:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    14);
                printf("06");
                break;
            case 7:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    1);
                printf("07");
                break;
            case 8:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    2);
                printf("08");
                break;
            case 9:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    15);
                printf("  ");
                break;
			case 10:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    13);
				printf("※");
				break;
            case 11:
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                    12);
                printf("⊙");
                break;

            }
            
        } printf("\n");
    }
    setPos(curX*2, curY);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
        14);
    printf("▣");
    setPos(45, 3);
    printf("남은 지뢰는 %d 개입니다. \n", mineSetCount-mineScore);
   
    //mineView();
}

int clear()
{
    if (mineScore == mineSetCount)
    {
        int clearCheck = 0;
        for (int r = 0; r < 20; r++)
        {
            for (int c = 0; c < 20; c++)
            {
                if (minePos[r][c] == 1 && table[r][c] == flagPic)
                    clearCheck++;
            }
        }
        if (clearCheck == mineSetCount)
        {
            return 1;
        }
    }
        
    return 0;
}

void update()
{

}

void main()
{
    init();
    draw();
    while (1)
    {
        update();
        input();
        
        if (clear())
        {
            mineDraw();
            draw();
            setPos(curX * 2, curY);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                12);
            printf("⊙");
            setPos(45, 8);
            printf("지뢰를 모두 찾았습니다!! \n\n\n\n\n\n\n\n\n\n\n");
            return;
        }
        if (gameEnd == 1)
        {
            
            draw();
            setPos(45, 8);
            printf("지뢰를 밟았습니다.\n\n\n\n\n\n\n\n\n\n\n");
            return;
        }
        
    }
    
}

