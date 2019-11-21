#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define SNAKE_CHAR 'O'
#define FOOD_CHAR '$'
#define WALL_CHAR '*'
#define SNAKE_LENGTH 20
#define LOST_MSG "LOOSE"

//snake의 body 좌표 구조체
typedef struct SnakeBody_{
    int x;
    int y;
} SnakeBody;

typedef struct Food_ {
    int x;
    int y;
} Food;

SnakeBody user_snake[SNAKE_LENGTH];
Food food;
int user_snake_dir;     //user snake's direction
int row, col;
int user_snake_length;


void showLoose(void);
void check(void);




void initGame()
{
    //snake body init
    for(int i=0; i < SNAKE_LENGTH; i++) {
        user_snake[i].x = -1;
        user_snake[i].y = -1;
    }
    
    initscr();
    
    getmaxyx(stdscr, row, col); //화면의 크기 설정 (stdscr : 표준화면 크기 )
    noecho();
    cbreak();                 //interrupt 와 flow control key를 제외하고는 입력자료에 어떠한 작용도 x
    curs_set(0);              //curser의 크기 조정 (0에 가까울 수록 커서가 안보임)
    start_color();            /* Start color */
    init_pair(1, COLOR_RED, COLOR_BLACK);   // parameter 1 : color pair
                                        //parameter 2 : word color, 3: block color
    attron(A_BOLD); // word effect
    //color_set(1, NULL);

    clear(); // screen cloear
    
    srand(time(NULL));
}


void initSnakeLocation()
{  //snake 위치 init (처음 화면상에서 보이는 snake 위치)
    user_snake[1].x = col/2;
    user_snake[1].y = row/2;
    user_snake[0].x = col/2 + 1;
    user_snake[0].y = row/2;
    user_snake_length = 2;      //처음 snake길이 2로 설정
    user_snake_dir = RIGHT;
}


void createSnake()
{
    int i = 0;
    while(user_snake[i].x != -1 && user_snake[i].y != -1) {
        move(user_snake[i].y, user_snake[i].x); // (y,x) 좌표로 커서 이동
        addch(SNAKE_CHAR);  //버퍼에 현재 커서의 위치에 snake body 추가
        i++;
    }
}


void drawMap()
{
    //테두리 벽 생성
    for(int i = 0; i < row; i++) {
        mvaddch(i, 0, WALL_CHAR);
        mvaddch(i, col-1, WALL_CHAR);
    }
    
    for(int i = 1; i < col-1; i++) {
        mvaddch(0, i, WALL_CHAR);
        mvaddch(row-1, i, WALL_CHAR);
    }
    
    //맵 안에 장애물 생성
    for (int i = 65; i < 100; i++) {
        mvaddch(row/4, i, WALL_CHAR);
        mvaddch((row/4)*3, i, WALL_CHAR);
    }
    
    //왼쪽 상단 사각형 장애물
    for (int i = 15; i < 25; i++) {
        mvaddch(row/6, i, WALL_CHAR);   //(y,x,char)
        mvaddch((row/6)*2, i, WALL_CHAR);
    }
    for (int i = 8; i < 14; i++) {
        mvaddch(i, row/6+8, WALL_CHAR);
        mvaddch(i, (row/6)*2+10, WALL_CHAR);
    }
    
}


//food
void FoodPosition()
{
    int x,y;
    
    x = rand()%col;
    y = rand()%row;
    
    while((mvinch(y, x) & A_CHARTEXT) == WALL_CHAR) {
        x = rand()%col;
        y = rand()%row;
    }
    
    food.x = x;
    food.y = y;
}


void drawFood()
{
    mvaddch(food.y, food.x, FOOD_CHAR);
}


void snakeMove(struct SnakeBody_ *snk)
{
    for (int i=user_snake_length-1; i>=0; i--) {
        snk[i+1] = snk[i];
    }
}


void snakeHeadMove()
{
    SnakeBody head = user_snake[0];
    snakeMove(user_snake);
    
    switch (user_snake_dir) {
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
    }
    user_snake[0] = head;
}


void moveSnake()
{
    snakeHeadMove();
    //check();
    
    //장애물에 닿으면 게임 종료
    if((mvinch(user_snake[0].y, user_snake[0].x) & A_CHARTEXT) == WALL_CHAR) {
        showLoose();
    }
    
    
    mvaddch(user_snake[0].y, user_snake[0].x, SNAKE_CHAR);
    
    
    if(user_snake[0].x == food.x && user_snake[0].y == food.y) {
        user_snake_length++;
        FoodPosition();
    }
    
    //컴퓨터 뱀에 닿으면 종료
    
}

void setDirection(char c)
{
    switch(c) {
        case 'w':
            user_snake_dir = UP;
            break;
        case 's':
            user_snake_dir = DOWN;
            break;
        case 'd':
            user_snake_dir = RIGHT;
            break;
        case 'a':
            user_snake_dir = LEFT;
            break;
    }
}


void showLoose() {
    clear();
    mvprintw(row/2, col/2-strlen(LOST_MSG)/2, LOST_MSG);
    refresh();
    
    sleep(5);
    
    //수정::main 화면부터 다시 실행
    endwin();
    exit(0);
}


void check()
{
    
}

//temp
//for test
void sig_func()
{
    endwin();
    exit(1);
}



//추가해야될 기능
//1. 음식 시간 마다 생성 후 시간되면 제거
//2. 컴퓨터 뱀 추가
//3. 메인메뉴 추가
//4. 시그널로 방향 제어?
//5. 길이가 길어질수록 speedup
