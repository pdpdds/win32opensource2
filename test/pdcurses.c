#include <curses.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
    char inp[60]; 

    initscr();
    start_color();
    scrollok(stdscr, TRUE);

    init_pair(1, COLOR_WHITE + 8, COLOR_BLUE);
    bkgd(COLOR_PAIR(1));

    addstr("This is a demo of ");
    attron(A_UNDERLINE);
    addstr("PDCurses");
    attroff(A_UNDERLINE);
    addstr(".\nYour comments here: ");
    getnstr(inp, 59);
    addstr("Press any key to exit.");

    getch();
    endwin();

    return 0;
}