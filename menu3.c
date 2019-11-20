#include <stdio.h>
#include <ncurses.h>

#define WIDTH          20
#define HEIGHT         10
#define SEP             2

#define MAX_MENUS      10
#define MAX_OP_MENU    10

#define ENTER          10

int startx;
int starty;
int max_x;
int max_y;

typedef struct menus {
    WINDOW *wmenu;
    int startx;
    int starty;
    int max_x;
    int max_y;
    char **menu;
    int n_op;
} t_menu;

t_menu menu[MAX_MENUS];

//Definim el nostres menus. Aquí un exemple.
int num_menus = 3;

char *grups[] = {
            "Grups",
            "AULA 01",
            "AULA 02",
            "AULA 03",
            "AULA 04",
            "Exit",
          };
         
char *ordres[] = {
            "Ordres",
            "PING",
            "HALT",
            "INFO",
            "Exit",
          };

char *menu3[] = {
            "one",
            "two",
            "three",
            "four",
            "Exit",
          };
         
char *list_menu[] = {
                            &grups,
                            &ordres,
                            &menu3,
                        };
int n_op_menu[] = {6,5,5,};


//int n_grups = sizeof(grups) / sizeof(char *);

void _init_ncurses();
void _init_menus();
void _imp_menus();
void _imp_menu(t_menu menu, int seleccionat);

int main(){
int menu_seleccionat = 0;
int seleccionat = 1;
int eleccio = 0;
int c;

    _init_ncurses();
    _init_menus();
    _imp_menus();

    while(1){
        c = wgetch(menu[menu_seleccionat].wmenu);
        switch(c){
                case KEY_UP:
                if(seleccionat == 1)
                    seleccionat = menu[menu_seleccionat].n_op;
                else
                    --seleccionat;
                break;
            case KEY_DOWN:
                if(seleccionat == menu[menu_seleccionat].n_op)
                    seleccionat = 1;
                else
                    ++seleccionat;
                break;
            case KEY_LEFT:
                if (( seleccionat == 1 ) && ( menu_seleccionat > 0 )) menu_seleccionat--;
                break;
            case KEY_RIGHT:
                if ( ( seleccionat == 1 ) && ( menu_seleccionat < num_menus - 1 )) menu_seleccionat++;
                break;
            case ENTER:
                eleccio = seleccionat;
                break;
            default:
                refresh();
                break;
        }
            mvprintw(15, 0, "Menu seleccionat: %d", menu_seleccionat);refresh();
        _imp_menu(menu[menu_seleccionat], seleccionat);
        if(eleccio != 0) break;
    } 
    mvprintw(16, 0, "Has escollit %d amb l'elecció %s\n", eleccio, menu[menu_seleccionat].menu[eleccio - 1]);refresh();getchar();
    clrtoeol();
    refresh();
    endwin();
    return 0;
}

void _init_menus(){
int i;
//Init menu grups  

for ( i = 0; i < num_menus; i++){

    menu[i].menu = list_menu[i];
    menu[i].n_op = n_op_menu[i];
    menu[i].startx = i * WIDTH + (i * SEP);
    menu[i].starty = 0;
    menu[i].max_x = WIDTH;
    menu[i].max_y = HEIGHT;
    menu[i].wmenu = newwin(menu[i].max_y, menu[i].max_x, menu[i].starty, menu[i].startx);
    keypad(menu[i].wmenu, TRUE);
}
refresh();
}

void _imp_menus(){
int i;
    for ( i = 0; i < num_menus; i ++ )    _imp_menu(menu[i], 0);
}

void _imp_menu(t_menu menu, int seleccionat){
    int x, y, i; 

    x = 2;
    y = 2;
    box(menu.wmenu, 0, 0);
    for(i = 0; i < menu.n_op; ++i){ 
        if(seleccionat == i + 1){
            wattron(menu.wmenu, A_REVERSE);
            mvwprintw(menu.wmenu, y, x, "%s", menu.menu[i]);
            wattroff(menu.wmenu, A_REVERSE);
        }
        else
            mvwprintw(menu.wmenu, y, x, "%s", menu.menu[i]);
        ++y;
    }
    wrefresh(menu.wmenu);
}

void _init_ncurses(){
   initscr();
   getmaxyx(stdscr, max_y, max_x);
   clear();
   noecho();
   cbreak();
   curs_set(FALSE);
   refresh();
 }

