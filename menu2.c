#include <stdio.h>
#include <ncurses.h>

#define WIDTH 30
#define HEIGHT 10

int startx;
int starty;
int max_x;
int max_y;

char *grups[] = {
            "AULA 01",
            "AULA 02",
            "AULA 03",
            "AULA 04",
            "Exit",
          };

char *ordres[] = {
            "PING",
            "HALT",
            "INFO",
            "Exit",
          };

WINDOW *menu_grups, *menu_ordres;

int n_grups = sizeof(grups) / sizeof(char *);
int menu_grups_startx;
int menu_grups_starty;
int menu_grups_max_x;
int menu_grups_max_y;
int n_ordres = sizeof(ordres) / sizeof(char *);
int menu_ordres_startx;
int menu_ordres_starty;
int menu_ordres_max_x;
int menu_ordres_max_y;


void _init_ncurses();
void _init_menu_grups();
void _init_menu_ordres();
void _imp_menu_grups(WINDOW *menu_grups, int seleccionat);
void _imp_menu_ordres(WINDOW *menu_ordres, int seleccionat);

int main(){

   
    int seleccionat = 1;
    int eleccio = 0;
    int c;

    _init_ncurses();

    startx = (80 - WIDTH) / 2;
    starty = (24 - HEIGHT) / 2;

    _init_menu_grups();
    _init_menu_ordres();
//mvprintw(10,10,"Punt 3");getchar();
    _imp_menu_grups(menu_grups, seleccionat);
    _imp_menu_ordres(menu_ordres, seleccionat);
   
    while(1)    {
        c = wgetch(menu_grups);
        switch(c)
        {    case KEY_UP:
                if(seleccionat == 1)
                    seleccionat = n_grups;
                else
                    --seleccionat;
                break;
            case KEY_DOWN:
                if(seleccionat == n_grups)
                    seleccionat = 1;
                else
                    ++seleccionat;
                break;
            case 10:
                eleccio = seleccionat;
                mvprintw(24, 0, "Has premut enter = %3d Es pto imprimir com: '%c'", c, c);
                break;
            default:
                mvprintw(24, 0, "Has premut el carècter = %3d Es pto imprimir com: '%c'", c, c);
                refresh();
                break;
        }
        _imp_menu_grups(menu_grups, seleccionat);
        _imp_menu_ordres(menu_ordres, seleccionat);
        if(eleccio != 0)    /* User did a choice come out of the infinite loop */
            break;
    }   
    mvprintw(23, 0, "Has elegit %d amb l'elecció %s\n", eleccio, grups[eleccio - 1]);
    clrtoeol();
    refresh();
    endwin();
    return 0;
}

void _init_menu_grups(){

    menu_grups_startx = 0;
    menu_grups_starty = 0;
    menu_grups_max_x = WIDTH;
    menu_grups_max_y = HEIGHT;

    menu_grups = newwin(menu_grups_max_y, menu_grups_max_x, menu_grups_starty, menu_grups_startx);
    keypad(menu_grups, TRUE);
    refresh();
}

void _init_menu_ordres(){
    menu_ordres = newwin(HEIGHT, WIDTH, starty, startx);
    keypad(menu_ordres, TRUE);
    refresh();
}

void _imp_menu_grups(WINDOW *menu_grups, int seleccionat)
{
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu_grups, 0, 0);
    for(i = 0; i < n_grups; ++i){   
        if(seleccionat == i + 1){
            wattron(menu_grups, A_REVERSE);
            mvwprintw(menu_grups, y, x, "%s", grups[i]);
            wattroff(menu_grups, A_REVERSE);
        }
        else
            mvwprintw(menu_grups, y, x, "%s", grups[i]);
        ++y;
    }
    wrefresh(menu_grups);
}

void _imp_menu_ordres(WINDOW *menu_ordres, int seleccionat)
{
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu_ordres, 0, 0);
    for(i = 0; i < n_ordres; ++i){   
        if(seleccionat == i + 1){
            wattron(menu_ordres, A_REVERSE);
            mvwprintw(menu_ordres, y, x, "%s", ordres[i]);
            wattroff(menu_ordres, A_REVERSE);
        }
        else
            mvwprintw(menu_ordres, y, x, "%s", ordres[i]);
        ++y;
    }
    wrefresh(menu_ordres);
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
