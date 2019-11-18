#include <stdio.h>
#include <ncurses.h>

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
char *pictur[] = {
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
    "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n",
};char *title[] = {
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
int n_pictur = sizeof(pictur) / sizeof(char *);
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
void _init_menu_pictur();
void _init_menu_title();
void _imp_menu_menus(WINDOW *menu_menus, int select);
void _imp_menu_pictur(WINDOW *menu_pictur, int select);
void _imp_menu_title(WINDOW *menu_title, int select);
int main(){


    int select = 1;
    int selection= 0;
    int c;

    _init_ncurses();


    _init_menu_menus();
    _init_menu_pictur();
    _init_menu_title();
    _imp_menu_menus(menu_menus, select);
    _imp_menu_pictur(menu_pictur, select);
    _imp_menu_title(menu_title, select);
    while(1)    {
        c = wgetch(menu_menus);
        switch(c)
        {    case KEY_UP:
            if(select == 1)
                select = n_menus;
            else
                --select;
            break;
            case KEY_DOWN:
            if(select == n_menus)
                select = 1;
            else
                ++select;
            break;
            case 10:
            selection = select;
            break;
            default:
            refresh();
            break;
        }
        _imp_menu_menus(menu_menus, select);
        if(selection != 0)    
            /* Menu Select Option Here */
            break;
    }   
    mvprintw(23, 0, "Has %d selection %s\n", selection, menus[selection- 1]);
    clrtoeol();
    refresh();
    endwin();
    return 0;
}

void _init_menu_menus(){

    menu_menus = newwin(80, 150, 10, 120);
    keypad(menu_menus, TRUE);
    refresh();
}

void _init_menu_pictur(){
    menu_pictur = newwin(80, 120, 10, 0);
    keypad(menu_pictur, TRUE);
    refresh();
}

void _init_menu_title(){
    menu_title = newwin(10, 300, 0, 0);
    keypad(menu_title, TRUE);
    refresh();
}

void _imp_menu_menus(WINDOW *menu_menus, int select)
{
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu_menus, 0, 0);
    for(i = 0; i < n_menus; ++i){   
        if(select == i + 1){
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

void _imp_menu_pictur(WINDOW *menu_pictur, int select)
{
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu_pictur, 0, 0);
    for(i = 0; i < n_pictur; ++i){   
        if(select == i + 1){
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        }
        else
            mvwprintw(menu_pictur, y, x, "%s", pictur[i]);
        ++y;
    }
    wrefresh(menu_pictur);
}
void _imp_menu_title(WINDOW *menu_title, int select)
{
    int x, y, i;   

    x = 2;
    y = 2;
    box(menu_title, 0, 0);
    for(i = 0; i < n_title; ++i){   
        if(select == i + 1){
            mvwprintw(menu_title, y, x, "%s", title[i]);
        }
        else
            mvwprintw(menu_title, y, x, "%s", title[i]);
        ++y;
    }
    wrefresh(menu_title);
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
