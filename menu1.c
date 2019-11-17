#include <stdio.h>
#include <ncurses.h>


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

int num_menus = 3;
char *grups[] = {
    "START",
    "MENU 01",
    "MENU 02",
    "MENU 03",
    "MENU 04",
    "Exit",
};

char *picture[] = {
    "OOOOOOOO\n",
    "OOOOOOOO\n",
    "OOOOOOOO\n",
    "OOOOOOOO\n",
    "OOOOOOOO\n",
    "OOOOOOOO\n",
    "OOOOOOOO\n"};

char *title[] = {
    "    #####    ##   #       #     #  ##   ####         ###        #     #     #   #### \n",
    "    #        # #  #      # #    # ##    #           #          # #    ##   ##   #    \n",
    "    #####    #  # #     #####   ##      ####        #  ##     #####   # # # #   #### \n",
    "        #    #   ##    #    #   # ##    #           #   #    #    #   #  #  #   #    \n",
    "    #####    #    #   #     #   #  ##   ####         ###    #     #   #     #   #### \n" };
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
            //        mvprintw(15, 0, "Menu seleccionat: %d", menu_seleccionat);refresh();
            _imp_menu(menu[menu_seleccionat], seleccionat);
            if(eleccio != 0) break;
        }  
        refresh();
        getchar();
        clrtoeol();
        refresh();
        endwin();
        return 0;
    }

void _init_menus(){
    //Init menu grups   

    menu[0].menu = &grups;
    menu[0].n_op = sizeof(grups) / sizeof(char *);
    menu[0].startx = 90;
    menu[0].starty = 10;
    menu[0].max_x = 150;
    menu[0].max_y = 30;
    menu[0].wmenu = newwin(menu[0].max_y, menu[0].max_x, menu[0].starty, menu[0].startx);
    keypad(menu[0].wmenu, TRUE);

    menu[1].menu = &title;
    menu[1].n_op = sizeof(title) / sizeof(char *);
    menu[1].startx = 0;
    menu[1].starty = 0;
    menu[1].max_x = 150;
    menu[1].max_y = 10;
    menu[1].wmenu = newwin(menu[1].max_y, menu[1].max_x, menu[1].starty, menu[1].startx);
    //keypad(menu[1].wmenu, TRUE);
    
    menu[2].menu = &picture;
    menu[2].n_op = sizeof(picture) / sizeof(char *);
    menu[2].startx = 0;
    menu[2].starty = 10;
    menu[2].max_x = 90;
    menu[2].max_y = 30;
    menu[2].wmenu = newwin(menu[2].max_y, menu[2].max_x, menu[2].starty, menu[2].startx);
    //keypad(menu[1].wmenu, TRUE);
    
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
