#include <ncurses.h>
#include <string.h>
//using namespace std;


int main(int argc, char **argv)
{
    /* NCURSES START*/
    initscr();
    noecho();
    cbreak();

    //get screen size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    //create a window for out input
    WINDOW *menuwin = newwin(6, xMax-12, yMax-8, 5);
    box(menuwin, 0, 0);
    refresh();
    wrefresh(menuwin);

    //makes it so we can use arrow keys
    keypad(menuwin, true);

    char choices[3][5] = {"START", "INFO", "EXIT"};
    int choice;
    int highlight = 0;

    while(1)
    {
        for(int i=0; i<3; i++)
        {
            if(i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i+1, 1, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch(choice)
        {
            case KEY_UP:
                highlight--;
                if(highlight == -1)
                    highlight = 0;
                break;
            case KEY_DOWN:
                highlight++;
                if(highlight == 3)
                    highlight = 2;
                break;
            default:
                break;
        }
        if(choice == 10)
            break;
    }
}
void titleDraw()
{
    printf("\n\n\n\n");
    printf("    #####   ##   #      #    #  ##  ####     ###       #    #     #  #### \n");
    printf("    #       # #  #     # #   # ##   #       #         # #   ##   ##  #    \n");
    printf("    #####   #  # #    #####  ##     ####    #  ##    #####  # # # #  #### \n");
    printf("        #   #   ##   #    #  # ##   #       #   #   #    #  #  #  #  #    \n");
    printf("    #####   #    #  #     #  #  ##  ####     ###   #     #  #     #  #### \n");
}

void menuDraw()
{
    goxoxy(24-2, 12);
    printf(">Game Start");
    gotoxy(24, 13);
    printf("Game Info");
    gotoxy(24, 14);
    printf("Exit");
}
