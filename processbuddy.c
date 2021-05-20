#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include <experimental/filesystem>
#include <iostream>
#include <dirent.h>

int main()
{
    WINDOW *mainwin;
    // using std::experimental::filesystem::directory_iterator;
    /*  Initialize ncurses  */
    int y = 33;
    if ((mainwin = initscr()) == NULL)
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    /*  Display "Hello, world!" in the centre of the
	screen, call refresh() to show our changes, and
	sleep() for a few seconds to get the full screen effect  */
    // for (const auto &file : directory_iterator(path))
    //     std::cout << file.path() << std::endl;

    DIR *d;
    struct dirent *dir;
    int maxy = getmaxy(mainwin);
    int maxx = getmaxx(mainwin);
    d = opendir("/proc");
    int height = 0;
    int x = 20;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            //printf("%s\n", dir->d_name);
            mvaddstr(height, x, dir->d_name);
            height += 1;
            if (height == 30)
            {
                height = 0;
                x += 10;
            }
        }
        closedir(d);
    }
    refresh();
    sleep(3);

    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}