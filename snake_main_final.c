#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <curses.h>
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
#define SNAKE_CHAR 'O'
#define FOOD_CHAR '$'
#define WALL_CHAR '*'
#define SNAKE_LENGTH 20
#define LOST_MSG "LOOSE"
#define LEFT_BULLET_CHAR '>'
#define RIGHT_BULLET_CHAR '<'
#define MAX_STAGE 10
#define FOOD_MAX 3

//snake의 body 좌표 구조체
typedef struct SnakeBody_{
    int x;
    int y;
} SnakeBody;

typedef struct Food_ {
    int x;
    int y;
} Food;

typedef struct Bullet{	//Bullet coordinate
	int x;
	int y;
}Bullet;

SnakeBody user_snake[SNAKE_LENGTH];
Food food[FOOD_MAX];
Bullet left_Bullet[MAX_STAGE];
Bullet right_Bullet[MAX_STAGE];


int user_snake_dir;     //user snake's direction
int row, col;
int user_snake_length;
int score = 0;


void showLoose(void);
void check(void);
void startGame();



//s: Bullet configuration

int move_time = 80;
int Bullet_stage = 1;

void bullet_Position(){ 	//Bullet_postion set_up
    int y;
    
    for(int i = 0; i < Bullet_stage; i++) {
        left_Bullet[i].x = 2;  //벽 넘어서 생성
        right_Bullet[i].x = col-2;
        
        //left Bullet
        y = rand()%row;
        while((mvinch(y, left_Bullet[i].x) & A_CHARTEXT) == WALL_CHAR || y == 0)
            y = rand()%row;
        
        left_Bullet[i].y = y;
        
        //right Bullet
        y = rand()%row;
        while((mvinch(y, right_Bullet[i].x) & A_CHARTEXT) == WALL_CHAR || y == 0)
            y = rand()%row;
        
        right_Bullet[i].y = y;
    }
	
    if(Bullet_stage < MAX_STAGE) {
        Bullet_stage++; //총알 수 증가
        move_time -= 5;//속도 향상 시키기
        set_ticker(move_time);
    }
}

void bullet_Position_set(){
	//for(int i=0;i<Bullet_stage;i++)
        bullet_Position();
}

void draw_Bullet(){	//left_Bullet draw
	for(int i = 0; i < Bullet_stage; i++){
        if(left_Bullet[i].y == 0) left_Bullet[i].y+=2;
        if(right_Bullet[i].y == 0) right_Bullet[i].y+=2;
        
        mvaddch(left_Bullet[i].y,left_Bullet[i].x,LEFT_BULLET_CHAR);
        mvaddch(right_Bullet[i].y,right_Bullet[i].x,RIGHT_BULLET_CHAR);
	}
}

void bullet_move(){	//buelln move
    for(int i = 0; i < Bullet_stage; i++) {
        if(left_Bullet[i].x < col-2) {   //끝에 도달하지 않았다면 다음 위치로 이동
            left_Bullet[i].x++;
            right_Bullet[i].x--;
        }
        else { //끝에 도달했다면
            //총알 제거
            for(int i=0;i<Bullet_stage;i++) {
                mvaddch(left_Bullet[i].y,left_Bullet[i].x, ' ');
                mvaddch(right_Bullet[i].y,right_Bullet[i].x, ' ');

                left_Bullet[i].y = -1;
                left_Bullet[i].x = -1;
                right_Bullet[i].y = -1;
                right_Bullet[i].x = -1;
            }
            
            score = score + 100;//score///////////////////for///////Bullet
            printw("score: %d", score);
            
            //새로운 총알 생성
            bullet_Position_set();
        }
        draw_Bullet();

    }
}

void Bullet_hit(){		//if Bullet hit snake then showloose
	
	for(int i = 0; i < SNAKE_LENGTH; i++){
		if((mvinch(user_snake[i].y, user_snake[i].x) & A_CHARTEXT) == LEFT_BULLET_CHAR || (mvinch(user_snake[i].y, user_snake[i].x) & A_CHARTEXT) == RIGHT_BULLET_CHAR )
            showLoose();
	}
}

//e: Bullet configuration


//s:main menu

int startx;
int starty;
int max_x;
int max_y;
char *pictur[] = {

    "OOOO\n"
        "OOOO\n"
        "OOOO\n"
};
char *menus[] = {
    "START",
    "Exit",
};
char *title[] = {
    "    #####      ##   #         #       #  ##     ####            ###          #       #     #     #### \n",
    "    #          # #  #        # #      # ##      #              #            # #      ##   ##     #    \n",
    "    #####      #  # #       #####     ##        ####           #  ##       #####     # # # #     #### \n",
    "        #      #   ##      #    #     # ##      #              #   #      #    #     #  #  #     #    \n",
    "    #####      #    #     #     #     #  ##     ####            ###      #     #     #     #     #### \n"
};

WINDOW *menu_menus, *menu_title;
WINDOW *menu_pictur;

int n_menus = sizeof(menus) / sizeof(char *);
int n_title = sizeof(title) / sizeof(char *);
int n_pictur = sizeof(pictur)/ sizeof(char *);

void _init_menu_menus();
void _init_menu_title();
void _imp_menu_menus(WINDOW *menu_menus, int select_);
void _imp_menu_pictur(WINDOW *menu_pictur, int select_);
void _imp_menu_title(WINDOW *menu_title, int select_);


void _key_selection(){
    int select_ =1;
    int selection =0;
    int c;
    _init_menu_menus();
    //_init_menu_pictur();
    _init_menu_title();
    _imp_menu_menus(menu_menus, select_);
    _imp_menu_pictur(menu_pictur, select_);
    _imp_menu_title(menu_title, select_);
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
            if(selection != 0){
                if(selection == 1)
                    startGame();
            
                if(selection == 2){///////////////////////
                    endwin();
                    exit(0);/////////////////
                }
            }
    }
}

void _init_menu_menus(){

    menu_menus = newwin(row, col, row/5, col*2/3);/////////////////////////////
    keypad(menu_menus, TRUE);
    refresh();
}


void _init_menu_pictur(){
    menu_pictur = newwin(row, col*2/3, row/5, 0);////////////////////////////
    refresh();
}

void _init_menu_title(){
    menu_title = newwin(row/5, col, 0, 0);////////////////////////////////////
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

    for(i = 0; i < n_pictur; ++i){
        if(select_ == i + 1){
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        }
        else
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        ++y;
    }

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


void drawMap()/////////////////////////////////////////////////////////////////
{
    for(int i=0; i<row/2; i++) {
        addch(' ');
    }
    printw("score : %d", score);
    //테두리 벽 생성
    for(int i = 0; i < row; i++) {
        mvaddch(i-1, 0, WALL_CHAR);
        mvaddch(i-1, col-1, WALL_CHAR);
    }

    for(int i = 1; i < col-1; i++) {
        mvaddch(1, i-1, WALL_CHAR);
        mvaddch(row-1, i-1, WALL_CHAR);
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
    while (1) {
    
        for(int i=0;i<FOOD_MAX;i++){
            x = rand()%col;
            y = rand()%row;

            while((mvinch(y, x) & A_CHARTEXT) == WALL_CHAR) {
                x = rand()%col;
                y = rand()%row;
            }

            food[i].x = x;
            food[i].y = y;
        }
        sleep(10);
    }
}


void drawFood()
{
    
    for(int i=0; i<FOOD_MAX;i++){
        mvaddch(food[i].y, food[i].x, FOOD_CHAR);
        if(user_snake[0].x == food[i].x && user_snake[0].y == food[i].y){
            mvaddch(food[i].y,food[i].x, " ");
            food[i].x = -1;
            food[i].y = -1;
        }
    }
        score = score + 1;
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
    
    //장애물에 닿으면 게임 종료
    if((mvinch(user_snake[0].y, user_snake[0].x) & A_CHARTEXT) == WALL_CHAR) {
        showLoose();
    }
    
    //총알에 닿으면 종료
    if((mvinch(user_snake[0].y, user_snake[0].x) & A_CHARTEXT) == LEFT_BULLET_CHAR || (mvinch(user_snake[0].y, user_snake[0].x) & A_CHARTEXT) == RIGHT_BULLET_CHAR) {
        showLoose();
    }

    mvaddch(user_snake[0].y, user_snake[0].x, SNAKE_CHAR);


  for(int i=0;i<FOOD_MAX;i++){
    if(user_snake[0].x == food[i].x && user_snake[0].y == food[i].y) {
        user_snake_length++;
	score+=1000;
    }
  }
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

    Bullet_stage = 1;
    score = 0;
    move_time = 80;
    //수정::main 화면부터 다시 실행
    for(int i=0; i<Bullet_stage; i++) {
        mvaddch(left_Bullet[i].y, left_Bullet[i].x, ' ');
        mvaddch(right_Bullet[i].y, right_Bullet[i].x, ' ');
        
        left_Bullet[i].y = -1;
        left_Bullet[i].x = -1;
        right_Bullet[i].y = -1;
        right_Bullet[i].x = -1;
    }
    clear();
    _key_selection();
    refresh();
    endwin();
    exit(0);
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
    pthread_t food_thread;
    pthread_create(&food_thread, NULL, FoodPosition,NULL);
    
    clear();
    initSnakeLocation();
    drawMap();
    createSnake();
    refresh();
    bullet_Position_set();
    
    if(set_ticker(move_time) == -1)      /*timer설정*/
        perror("set_ticker");
    
    char key;
    while(1) {
        timeout(50); //70ms 동안 getch() block시키고 읽을 것이 없으면 return -1을 하고 getch() 종료
        Bullet_hit();
        key = getch();
        setDirection(key);
        clear();
        drawMap();
        moveSnake();
        createSnake();
        drawFood();
        draw_Bullet();
        
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
    //signal(SIGALRM, SIG_IGN);
    signal(SIGALRM, bullet_move);            /*SIGALRM을 받으면 draw_Bullet를 호출*/
    
    initGame();

    //s:main menu
    /*_init_main_menu();*/
    _key_selection();
//    clrtoeol();

    //e:main menu



    return 0;
}

//추가해야될 기능
//1. 음식 시간 마다 생성 후 시간되면 제거
//2. 컴퓨터 뱀 추가
//3. 메인메뉴 추가
//4. 시그널로 방향 제어?
//5. 길이가 길어질수록 speedup
