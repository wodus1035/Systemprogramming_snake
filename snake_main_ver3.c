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
#define SHOOT_CHAR '>'

//snake의 body 좌표 구조체
typedef struct SnakeBody_{
    int x;
    int y;
} SnakeBody;

typedef struct Food_ {
    int x;
    int y;
} Food;

typedef struct Shoot{
    int x;
    int y;
}Shoot;

SnakeBody user_snake[SNAKE_LENGTH];
Food food;
Shoot shoot;

int user_snake_dir;     //user snake's direction
int row, col;
int user_snake_length;

int shoot_row, shoot_col;

void showLoose(void);
void check(void);
void startGame(void);
void FoodPosition(void);
void snakeMove(struct SnakeBody_ *);
//s:computer snake
int moveTime=500;
int set_ticker(int);

void add_shoot(){
    
    int x;
    int y;
    
    y = rand()%shoot_row;
    x = 2;
    
    while((mvinch(y,x) & A_CHARTEXT) != ' '){
        y = rand()%shoot_row;
        x = 2;
    }
    shoot.y = y;
    shoot.x = 2;
   
}

void drawshoot(){
    int i = 0;
    while(shoot.x != -1 && shoot.y != -1){
        move(shoot.y, shoot.x);
        addch(SHOOT_CHAR);
        i++;
    }
}

void shoot_move(){
    shoot.x++;
}

void shoot_hit(){
    shoot_move();
    mvaddch(shoot.y,shoot.x, SHOOT_CHAR);
    if(shoot.x == user_snake[0].x && shoot.y == user_snake[0].y){
        showLoose();
    }else{
        }
}

//e:computer snake


//s:main menu
int select_ = 1;
int selection= 0;
int c;

int startx;
int starty;
int max_x;
int max_y;

char *menus[] = {
    "START",
    "menu1",
    "menu2",
    "menu3",
    "Exit",
};
char *title[] = {
    "    #####      ##   #         #       #  ##     ####            ###          #       #     #     #### \n",
    "    #          # #  #        # #      # ##      #              #            # #      ##   ##     #    \n",
    "    #####      #  # #       #####     ##        ####           #  ##       #####     # # # #     #### \n",
    "        #      #   ##      #    #     # ##      #              #   #      #    #     #  #  #     #    \n",
    "    #####      #    #     #     #     #  ##     ####            ###      #     #     #     #     #### \n"
};

WINDOW *menu_menus, *menu_pictur, *menu_title;

int n_menus = sizeof(menus) / sizeof(char *);
int menu_menus_startx;
int menu_menus_starty;
int menu_menus_max_x;
int menu_menus_max_y;
int menu_pictur_startx;
int menu_pictur_starty;
int menu_pictur_max_x;
int menu_pictur_max_y;
int n_title = sizeof(title) / sizeof(char *);
int menu_title_startx;
int menu_title_starty;
int menu_title_max_x;
int menu_title_max_y;


void _init_ncurses();
void _init_menu_menus();
void _init_menu_title();
void _imp_menu_menus(WINDOW *menu_menus, int select_);
void _imp_menu_pictur(WINDOW *menu_pictur, int select_);
void _imp_menu_title(WINDOW *menu_title, int select_);


void _init_main_menu(){
    _init_menu_menus();
    //_init_menu_pictur();
    _init_menu_title();
    _imp_menu_menus(menu_menus, select_);
    _imp_menu_pictur(menu_pictur, select_);
    _imp_menu_title(menu_title, select_);
}

void _key_selection(){
    while(1)    {
        c = wgetch(menu_menus);
        switch(c)
        {    case KEY_UP:
            if(select_ == 1)
                select_ = n_menus;
            else
                --select_;
            break;
            case KEY_DOWN:
            if(select_ == n_menus)
                select_ = 1;
            else
                ++select_;
            break;
            case 10:
            selection = select_;
            break;
            default:
            refresh();
            break;
        }
        _imp_menu_menus(menu_menus, select_);
        if(selection != 0)
            if(selection == 1) {
                startGame();
            }
        break;
    }
}


void _init_menu_menus(){

    menu_menus = newwin(80, 150, 10, 120);
    keypad(menu_menus, TRUE);
    refresh();
}

/*
void _init_menu_pictur(){
    menu_pictur = newwin(8f0, 120, 10, 0);
    keypad(menu_pictur, TRUE);
    refresh();
}*/

void _init_menu_title(){
    menu_title = newwin(10, 300, 0, 0);
    keypad(menu_title, TRUE);
    refresh();
}

void _imp_menu_menus(WINDOW *menu_menus, int select_)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_menus, 0, 0);
    for(i = 0; i < n_menus; ++i){
        if(select_ == i + 1){
            wattron(menu_menus, A_REVERSE);
            mvwprintw(menu_menus, y, x, "%s", menus[i]);
            wattroff(menu_menus, A_REVERSE);
        }
        else
            mvwprintw(menu_menus, y, x, "%s", menus[i]);
        ++y;
    }
    wrefresh(menu_menus);
}

void _imp_menu_pictur(WINDOW *menu_pictur, int select_)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_pictur, 0, 0);
    /*
    for(i = 0; i < n_pictur; ++i){
        if(select_ == i + 1){
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        }
        else
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        ++y;
    }
     */
    wrefresh(menu_pictur);
}
void _imp_menu_title(WINDOW *menu_title, int select_)
{
    int x, y, i;

    x = 2;
    y = 2;
    box(menu_title, 0, 0);
    for(i = 0; i < n_title; ++i){
        if(select_ == i + 1){
            mvwprintw(menu_title, y, x, "%s", title[i]);
        }
        else
            mvwprintw(menu_title, y, x, "%s", title[i]);
        ++y;
    }
    wrefresh(menu_title);
}

//e:main menu





void initGame()
{
    //snake body init
    for(int i=0; i < SNAKE_LENGTH; i++) {
        user_snake[i].x = -1;
        user_snake[i].y = -1;
        shoot.x = -1;
        shoot.y = -1;
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
    //목표 뱀 길이에 도달하면 게임 클리어
}

//temp
//for test
void sig_func()
{
    endwin();
    exit(1);
}

void startGame()
{
    clear();
    initSnakeLocation();
    drawMap();
    createSnake();
    FoodPosition();
    add_shoot();
    refresh();
    
    char key;
    while(1) {
        timeout(50); //70ms 동안 getch() block시키고 읽을 것이 없으면 return -1을 하고 getch() 종료
        key = getch();
        setDirection(key);
        clear();
        drawMap();
        moveSnake();
        shoot_move();
        shoot_hit();        //check();
        createSnake();
        drawshoot();
        drawFood();
        refresh();
    }
}


int set_ticker(int n_msecs)
{
    struct itimerval new_timeset;
    long n_sec, n_usecs;
 
    n_sec = n_msecs / 1000;
    n_usecs = (n_msecs % 1000) * 1000L;

    new_timeset.it_interval.tv_sec = n_sec;
    new_timeset.it_interval.tv_usec = n_usecs;
    new_timeset.it_value.tv_sec = n_sec;
    new_timeset.it_value.tv_usec = n_usecs;
  
    return setitimer(ITIMER_REAL, &new_timeset, NULL);
}


int main()
{
    signal(SIGINT, sig_func);  //for test
    signal(SIGALRM, add_shoot);            /*SIGALRM을 받으면 cc를 호출*/
    if(set_ticker(moveTime) == -1)      /*timer설정*/
        perror("set_ticker");
    initGame();

    //s:main menu
    _init_main_menu();
    _key_selection();
    
    //e:main menu
    
    
    
    return 0;
}

//추가해야될 기능
//1. 음식 시간 마다 생성 후 시간되면 제거
//2. 컴퓨터 뱀 추가
//3. 메인메뉴 추가
//4. 시그널로 방향 제어?
//5. 길이가 길어질수록 speedup
