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

WINDOW *identify_win(int x,WINDOW *left,WINDOW *right)
{
    if(x<COLS/2) return left;
    else return right;
}

int open_dir(struct dirent ***namelist,MEVENT event,WINDOW *win) 
{                                                                      
    wclear(win); 
    wrefresh(win);
    int n;
    struct dirent **local;
    local = *namelist;
    if(local[event.y]->d_type==4){
        char folder[40];
        strcpy(folder,local[event.y]->d_name);                     
        while(n--){                                                    
            free(local[n]);
        }          
        n = scandir(folder, namelist, NULL, alphasort);
        chdir(folder);                                                 
        return n;
    }    
}

void print_dir_content(int n,struct dirent **namelist,WINDOW *win)
{
    if (n < 0)
        perror("scandir");
    else{
        int i=0;
        wmove(win,0,0);
        while(i < n){
            if(namelist[i]->d_type==4){
                wattron(win,A_BOLD);
            }
            wprintw(win,"type=%i %i %i %s\n",  namelist[i]->d_type,n, i,namelist[i]->d_name);
            if(namelist[i]->d_type==4){
                wattroff(win,A_BOLD);
            }
            i++;
        }
    }
    wrefresh(win);
 }

int main(int argc, char const *argv[])
{
    WINDOW          *win;
    WINDOW          *left;
    WINDOW          *right;
    MEVENT          event;
    int             i,n,key;
    int             ln,rn;
    char            LCWD[100],RCWD[100];
    struct dirent   **lwin_namelist;
    struct dirent   **rwin_namelist;                          

    initscr();
    clear();
    cbreak();   
    refresh();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1); // left & right
    mousemask(ALL_MOUSE_EVENTS, NULL);

    i=0;
    
    left=derwin(stdscr,LINES,COLS/2,0,0);
    right=derwin(stdscr,LINES,COLS/2,0,COLS/2);

    ln=scandir(".", &lwin_namelist, NULL, alphasort);
    rn=scandir(".", &rwin_namelist, NULL, alphasort);
    print_dir_content(ln,lwin_namelist,left);
    print_dir_content(rn,rwin_namelist,right);   
    getcwd(LCWD,100);getcwd(RCWD,100);

    while(1){
        key=wgetch(stdscr);
        getmouse(&event);
        if(event.y>=n)break;
        move(event.y,event.x);
        win=identify_win(event.x,left,right);
        switch(key){
            case KEY_MOUSE:
                /*if(event.bstate & BUTTON1_PRESSED){

                }*/
                if(event.bstate & BUTTON1_DOUBLE_CLICKED){
                    if(win==left){
                        chdir(LCWD);
                        n=open_dir(&lwin_namelist,event,win);
                        print_dir_content(n,lwin_namelist,left);
                        getcwd(LCWD,100);
                        }    
                    else if(win==right){
                        chdir(RCWD);
                        n=open_dir(&rwin_namelist,event,win);
                        print_dir_content(n,rwin_namelist,right);
                        getcwd(RCWD,100);
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

    while(n--){
        free(lwin_namelist[n]);
        free(rwin_namelist[n]);
        }
    free(lwin_namelist);
    free(rwin_namelist);
    
    return 0;
}

/*
< ln,rn : n >
*/