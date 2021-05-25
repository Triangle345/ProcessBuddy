#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
//#include <experimental/filesystem>
#include <iostream>
#include <dirent.h>
#include <string.h>
#include <menu.h>
#include <vector>
#include <memory>
#include "Module.h"

MENU* CreateMenu(const std::vector<std::string> & items)
{
    MENU * menu = {0};

    auto my_items = (ITEM **)calloc(items.size() + 1, sizeof(ITEM *));
    for(int i = 0; i < items.size();i++)
        my_items[i] = new_item(items.at(i).c_str(), "");
    my_items[items.size()] = (ITEM *)NULL;
    menu = new_menu((ITEM **)my_items);
    int ret = menu_opts_off(menu, O_ONEVALUE);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    set_menu_fore(menu, COLOR_PAIR(1));
    set_menu_mark(menu, NULL);
    //TODO: may need to string tokenize
    set_menu_format(menu, 100, 6);

    return menu;
}

int main()
{
    WINDOW *mainwin;
    // using std::experimental::filesystem::directory_iterator;
    /*  Initialize ncurses  */
    if ((mainwin = initscr()) == NULL)
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }
    cbreak();
	noecho();
    start_color();

	keypad(stdscr, TRUE);
    DIR *d;
    struct dirent *dir;
    int n_choices = 50;
    int maxx, maxy = 0;
    getmaxyx(mainwin, maxy,maxx);
    d = opendir("/proc");
    int x = 0;
    int y = 0;
    MENU* my_menu = {0};
    std::vector<char> buf(100);
     
    std::vector<std::string> processOut;
    long long numProcesses = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            
            if (dir->d_name[0] != '0' && atoll(dir->d_name) == 0)
            {
                continue;
            }
            //mvaddstr(y, x, dir->d_name);
            y += 1;
            if (y > maxy-5)
            {
                y = 0;
                x += 15;
            }
            processOut.push_back(dir->d_name);
            numProcesses++;
        }
        closedir(d);
        
    }

    //item_opts_off(my_items[3], O_SELECTABLE);
    my_menu = CreateMenu(processOut);
    post_menu(my_menu);
    //free_item();
    //free_menu();
    //menu_unpost();

    //  char buf[100] = {0};
    //  sprintf(buf, "x:%d y:%d", maxx/2, maxy/2);
    //  mvaddstr(maxy/2,maxx/2, buf);
    //  sprintf(buf, "x:%d y:%d", 1, 1);
    //  mvaddstr(1,1, buf);
    refresh();
    
    int c = 0;
    while((c = getch()) != KEY_F(1))
    { 
        switch(c)
        { 
            case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
            break;
            case 's':
                menu_driver(my_menu, REQ_TOGGLE_ITEM);
            break;
            // ENTER
            case 10:
                auto pid = std::string(item_name(current_item(my_menu)));
                wclear(mainwin);
                unpost_menu(my_menu);
                //TODO: need to free the menu properly.. maybe create a class or something
                // for(int i = 0; i < processOut.size();i++)
                //     free_item(my_items[i]);
                // my_items[numProcesses] = (ITEM *)NULL;
                free_menu(my_menu);

                
                refresh();
                mvaddstr(maxy/2,maxx/2, pid.c_str());
                
                sprintf(buf.data(), "/proc/%s/maps", pid.c_str());
                FILE * f = fopen(buf.data(), "r");
                if (!f){
                    perror(buf.data());
                    exit(errno);
                }

                char* read_cnt = 0;
                std::vector<std::unique_ptr<Module>> modules;
                while (read_cnt != NULL || modules.size() == 0)
                {
                    
                    char modules_line[300] = {0};
                    read_cnt = fgets(modules_line,300, f);
                    modules.push_back(std::make_unique<Module>(modules_line));
                }
                
                std::vector<std::string> modulesOut;
                auto tmpheader = Module::getHeader();
                modulesOut.push_back(tmpheader);
                int max = 5;
                for (auto & module : modules)
                {
                    auto tmp = module->toString();
                    modulesOut.push_back(tmp);
                }
                MENU * my_menu_modules = CreateMenu(modulesOut);
                post_menu(my_menu_modules);
                refresh();
            break;
        }
                        

        sleep(.2);
    } 
    //sleep(3);

    /*  Clean up after ourselves  */

    delwin(mainwin);
    endwin();
    refresh();

    return EXIT_SUCCESS;
}