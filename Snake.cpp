#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;

struct SnakeHead{
    int length;
    int x;
    int y;
    char head;
    char movingDirection;
    int score;
    int level;
};

struct food {
    int rowPosition;
    int columnPosition;
    char symbol;
};

bool gameOver = true;

const int width = 80, height = 40;
int prevSnakeSegX, prevSnakeSegY;                               //previous x and y position
void printBoard(SnakeHead &Snake, food &foodItem);
void placeFood(food &foodItem);
void playGame(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily);
void drawDescription(const SnakeHead &Snake, const food &FoodItem);
void movement(SnakeHead &Snake);
void readMovement(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily);
void gotoxy(int x, int y);
void Setup(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily);
bool selfCollision(SnakeHead &snake, vector<int> &tailx, vector<int> &taily);
void createSnakeBody(SnakeHead &Snake, vector<int> &tailx, vector<int> &taily);
void newlevel(SnakeHead &Snake);

int main()
{
    food foodItem;
    SnakeHead Snake;
    vector<int> tailx(20), taily(20);

    playGame(Snake, foodItem, tailx, taily);

    system("pause");
    return 0;
}


void playGame(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily) {
    Setup(Snake, foodItem, tailx, taily);
    printBoard(Snake, foodItem);
    placeFood(foodItem);
    while (gameOver) {
        movement(Snake);
        readMovement(Snake, foodItem, tailx, taily);
        Sleep(150);
    }
}

void Setup(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily) {
    Snake.score = 0;
    Snake.length = 1;
    Snake.x = width/2;
    Snake.y = height/2;
    tailx[0] = Snake.x;
    taily[0] = Snake.y;
    Snake.head = 'o';
    foodItem.symbol = '*';
    Snake.level=1;
}


void printBoard(SnakeHead &Snake, food &foodItem)
{
    for (int i = 0; i < height; i++) {
        if (i > 0 and i < height - 1) {
            cout << "#";
            for (int k = 1; k < width - 1; k++)
                cout << " ";
            cout << "#"<<endl;
            continue;
        }
        for (int j = 0; j < width; j++)
            cout << "#";
        cout << endl;
    }
    drawDescription(Snake, foodItem);
}


void drawDescription(const SnakeHead &Snake, const food &FoodItem)
{
    const int BOX_WIDTH = 29, BOX_LENGTH = 19;

    gotoxy(width + 15, 4); cout << "RULES:";

    gotoxy(width, 1);
    for (int i = 0; i < BOX_WIDTH; i++)
        cout << "-";

    gotoxy(width, BOX_LENGTH);
    for (int k = 0; k < BOX_WIDTH; k++)
        cout << "-";

    gotoxy(width + 5, 6); cout << Snake.head << " -> SnakeHead\n";
    gotoxy(width + 5, 8); cout << FoodItem.symbol << " -> Food\n";
    gotoxy(width + 5, 10); cout << "#  -> Border\n";
    gotoxy(width + 5, 12); cout << "w  -> Move Up\n";
    gotoxy(width + 5, 14); cout << "s  -> Move Down\n";
    gotoxy(width + 5, 16); cout << "a  -> Move Left\n";
    gotoxy(width + 5, 18); cout << "d  -> Move Right";
    gotoxy(width + 5, 20); cout << "level  -> "<<Snake.level;
}


void movement(SnakeHead &SnakeHead)
{
    if (_kbhit()) {
        char input = _getch();


        if (input == 'w' and SnakeHead.movingDirection != 's') SnakeHead.movingDirection  = 'w';

        else if (input == 's' and SnakeHead.movingDirection != 'w') SnakeHead.movingDirection = 's';

        else if (input == 'a' and SnakeHead.movingDirection != 'd') SnakeHead.movingDirection = 'a';

        else if (input == 'd' and SnakeHead.movingDirection != 'a') SnakeHead.movingDirection = 'd';
    }
}


void readMovement(SnakeHead &Snake, food &foodItem, vector<int> &tailx, vector<int> &taily)
{

    (Snake.length == 1) ? gotoxy(tailx[0], taily[0]) : gotoxy(prevSnakeSegX, prevSnakeSegY);
    cout << " ";
    for (int i = 1; i < Snake.length; i++) {
        gotoxy(tailx[i], taily[i]); cout << Snake.head;
    }
    gotoxy(Snake.x, Snake.y); cout << Snake.head;

    createSnakeBody(Snake, tailx, taily);

    if (Snake.movingDirection == 'w') Snake.y--;
    else if (Snake.movingDirection == 's') Snake.y++;
    else if (Snake.movingDirection == 'a') Snake.x--;
    else if (Snake.movingDirection == 'd') Snake.x++;

    if ((Snake.x == 0 or Snake.x == width -1) or (Snake.y == 0 or Snake.y == height - 1) or selfCollision(Snake, tailx, taily)){
        gotoxy(40, 20); cout << "GAME OVER !! \n";
        gotoxy(40,22); cout<<" Press 1 to Play Again. ";
        int input;
        cin>>input;
        if(input == 1)
        {
                system("cls");
                food foodItem1;
                SnakeHead Snake1;
                vector<int> tailx1(20), taily1(20);//
            playGame(Snake1, foodItem1, tailx1, taily1);
        }
        gameOver = false;
    }else if (Snake.y == foodItem.rowPosition and Snake.x == foodItem.columnPosition) {
        placeFood(foodItem);
        newlevel(Snake);
        Snake.score += 1;
        Snake.length += 2;
    }

    gotoxy(width + 5, 40);
    cout << "\nscore: " << Snake.score << endl;
    cout << "Snake Length: " << Snake.length << endl;
}

void placeFood(food &foodItem){
    srand(time(0));
    foodItem.rowPosition = rand() % 18 + 1, foodItem.columnPosition = rand() % (width-2) + 1;

    gotoxy(foodItem.columnPosition, foodItem.rowPosition); cout << foodItem.symbol;
}


void createSnakeBody(SnakeHead &Snake, vector<int> &tailx, vector<int> &taily) {
    int prevHeadX = tailx[0];
    int prevHeadY = taily[0];
    tailx[0] = Snake.x;
    taily[0] = Snake.y;
    for (int i = 1; i < Snake.length; i++) {
        prevSnakeSegX = tailx[i];
        prevSnakeSegY = taily[i];
        tailx[i] = prevHeadX;
        taily[i] = prevHeadY;
        prevHeadX = prevSnakeSegX;
        prevHeadY = prevSnakeSegY;
        tailx.push_back(int());
        taily.push_back(int());
    }
}

void newlevel(SnakeHead &Snake){
    if((Snake.score%4)==0){
        Snake.level++;
    }
}

bool selfCollision(SnakeHead &snake, vector<int> &tailx, vector<int> &taily) {
    int len = snake.length;

    for (int i = 0; i < len; i++)
        if (snake.x == tailx[i] and snake.y == taily[i] and snake.length > 1)
            return true;
    return false;
}

void gotoxy(int x, int y){
    COORD coord;

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
