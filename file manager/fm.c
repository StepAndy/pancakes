#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void sig_winch(int signo)
{
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
    nodelay(stdscr, 1);
    while (wgetch(stdscr) != ERR);
    nodelay(stdscr, 0);
}


WINDOW *identify_win(int x, WINDOW *left, WINDOW *right)
{
    if(x < COLS/2) 
        return left;
    else 
        return right;
}


int open_dir(WINDOW *win, struct dirent ***namelist, MEVENT event) 
{                                                                      
    int total_content_amount = sizeof(namelist); //ШТА!!!!
    wprintw(win, "%d\n", total_content_amount);
    wrefresh(win);

    struct dirent **local;
    local = *namelist;
    if(local[event.y]->d_type==4){
        char folder[40];
        strcpy(folder,local[event.y]->d_name);                     
        while(total_content_amount--){                                                    
            free(local[total_content_amount]);
        }          
        total_content_amount = scandir(folder, namelist, NULL, alphasort);
        if(total_content_amount < 0){
            perror("scandir");   
        }
        chdir(folder); 

        return total_content_amount;
    }
}


void print_dir_content(WINDOW *win, struct dirent **namelist,
    int total_content_amount)
{
    wclear(win); 
    wrefresh(win);
    int i = 0;
    wmove(win, 0, 0);
    while(i < total_content_amount){
        if(namelist[i]->d_type == 4) wattron(win, A_BOLD);
        wprintw(win,"type=%i %i %i %s\n",  namelist[i]->d_type,
                total_content_amount, i,namelist[i]->d_name);
        if(namelist[i]->d_type == 4) wattroff(win, A_BOLD);
        i++;
    }
    wmove(win, 0, 0);
    wrefresh(win);
 }


void line_selection(WINDOW *win, struct dirent **namelist)
{
    int x, y;
    getyx(win, y, x);
    wattron(win, A_REVERSE);
    mvwprintw(win, y, x, "type=%i %i",  y,x);/*namelist[y]->d_type,
              namelist[y]->d_name);*/
    wattroff(win, A_REVERSE);
    wrefresh(win);
}

int main(int argc, char const *argv[])
{
    WINDOW          *win;
    WINDOW          *left;
    WINDOW          *right;
    MEVENT          event;
    int             total_content_amount, key;
    int             lcontents_n, rcontents_n;
    char            LCWD[100], RCWD[100];
    struct dirent   **lwin_namelist;    //по необходимости копии директории
    struct dirent   **rwin_namelist;                          

    initscr();
    clear();
    cbreak();   
    refresh();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1); // left & right
    mousemask(ALL_MOUSE_EVENTS, NULL);

    left=derwin(stdscr,LINES,COLS/2,0,0);
    right=derwin(stdscr,LINES,COLS/2,0,COLS/2);

    //////////////////////////////////////////////////////////
    ///////////////////Initial screen/////////////////////////
    //////////////////////////////////////////////////////////
    lcontents_n=scandir(".", &lwin_namelist, NULL, alphasort);
    rcontents_n=scandir(".", &rwin_namelist, NULL, alphasort);
    print_dir_content(left, lwin_namelist, lcontents_n);
    print_dir_content(right, rwin_namelist, rcontents_n);   
    getcwd(LCWD,100);
    getcwd(RCWD,100);
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////
    total_content_amount = lcontents_n;

    while(1){
        MEVENT          event;
        key = wgetch(stdscr);
        getmouse(&event);
        win = identify_win(event.x, left, right);
        if((event.y >= lcontents_n && win == left) || 
           (event.y >= rcontents_n && win == right)) break;
        move(event.y, event.x);
        
        switch(key){
            case KEY_MOUSE:
                
                if(event.bstate & BUTTON1_CLICKED){
                    if(win == left){
                        print_dir_content(left, lwin_namelist, 
                            lcontents_n);
                        line_selection(left, lwin_namelist);
                                                                            /*int y,x;
                                                                            getyx(win,y,x);
                                                                            mvwprintw(win,y,x,"%d %d",y,x);
                                                                            wrefresh(left);*/
                    }
                    else if(win == right){
                        print_dir_content(right, rwin_namelist, 
                            rcontents_n);
                        line_selection(right, rwin_namelist);
                    }

                }

                if((event.bstate & BUTTON1_DOUBLE_CLICKED)){
                    if(win == left){
                        chdir(LCWD);
                        lcontents_n = open_dir(win, &lwin_namelist,
                            event);
                        print_dir_content(left, lwin_namelist, 
                            lcontents_n);
                        getcwd(LCWD, 100);
                        }    
                    else if(win == right){
                        chdir(RCWD);
                        rcontents_n = open_dir(win, &rwin_namelist,
                            event);
                        print_dir_content(right, rwin_namelist,
                            rcontents_n);
                        getcwd(RCWD, 100);
                    }
                }
                break;
            /*case KEY_UP:
            case KEY_DOWN:
            case #DELETE:
            case #ENTER:
            case #TAB:*/
        }    
    }
    
    delwin(left);
    delwin(right);
    getch();
    endwin();

    while(total_content_amount--){
        free(lwin_namelist[total_content_amount]);
        free(rwin_namelist[total_content_amount]);
        }
    free(lwin_namelist);
    free(rwin_namelist);
    
    return 0;
}

/*
< ln,rn : n >
*/