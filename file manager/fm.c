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

#define TAB 9
#define ENTER 10
#define EDITORS_PATH "/home/andrew/Рабочий стол/sandbox/text_editor/txteditor"
#define MX_PTH_LNGTH 150
#define L_SLCTD_NAME lwin_namelist[event.y]->d_name
#define L_SLCTD_TYPE lwin_namelist[event.y]->d_type
#define R_SLCTD_NAME rwin_namelist[event.y]->d_name
#define R_SLCTD_TYPE rwin_namelist[event.y]->d_type

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
    int total_content_amount = sizeof(namelist);                    //ERROR!!!!Sizeof []
    wprintw(win, "%d\n", total_content_amount);
    wrefresh(win);

    struct dirent **local;
    local = *namelist;
    if(local[event.y]->d_type==4){
        char folder[MX_PTH_LNGTH];
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
    mvwprintw(win, y, x, "type=%i %s", namelist[y]->d_type,
    namelist[y]->d_name);/*namelist[y]->d_type,
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
    int             x, y;
    int             total_content_amount, key;
    int             lcontents_n, rcontents_n;
    char            LCWD[MX_PTH_LNGTH],
                    RCWD[MX_PTH_LNGTH];
    struct dirent   **lwin_namelist;    //по необходимости копии директории
    struct dirent   **rwin_namelist;                          

    initscr();
    clear();
    cbreak();   
    refresh();
    signal(SIGWINCH, sig_winch);
    keypad(stdscr, 1); // left & right
    mousemask(ALL_MOUSE_EVENTS, NULL);

    ///////////////////////////////WINDOWS//////////////////////////////////////////

    left=derwin(stdscr,LINES,COLS/2,0,0);                           
    right=derwin(stdscr,LINES,COLS/2,0,COLS/2);                     

    ////////////////////////////////////////////////////////////////////////////////
    ///////////////////Initial screen///////////////////////// : 2 structs, 2 paths
    ////////////////////////////////////////////////////////////////////////////////
    lcontents_n=scandir(".", &lwin_namelist, NULL, alphasort);
    rcontents_n=scandir(".", &rwin_namelist, NULL, alphasort);
    print_dir_content(left, lwin_namelist, lcontents_n);
    print_dir_content(right, rwin_namelist, rcontents_n);   
    getcwd(LCWD,100);
    getcwd(RCWD,100);
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    
    total_content_amount = lcontents_n; // [dsmthg wth] total_content_amount !!![]

    while(1){
        MEVENT          event;
  
        keypad(stdscr, 1); 
        key = wgetch(stdscr);
        getmouse(&event);
        //win = identify_win(event.x, left, right);
        if((event.y >= lcontents_n && win == left) || 
           (event.y >= rcontents_n && win == right)) break;
        move(event.y, event.x);
        getyx(stdscr, y, x);
        
        win = identify_win(x, left, right);
        /*mvwprintw(win, 19, 0, "y=%i x=%i", y, x);
        if(win==left && key == '\t') {move(y, COLS/2); mvwprintw(win, 20, 0, "key=%i", key);}
        if(win==right && key == '\t') {move(y,0);}
        mvwprintw(win, 20, 0, "key=%i", key);
        wrefresh(left);
        wrefresh(right);*/
        
        switch(key){
            case KEY_MOUSE:
                //////////////////////////////////////////////
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
                break;
                }
                //////////////////////////////////////////////
                if((event.bstate & BUTTON1_DOUBLE_CLICKED)){
                    if(win == left){
                        if(strstr(L_SLCTD_NAME,".txt") != 0){
                            execl(EDITORS_PATH, "txteditor",L_SLCTD_NAME
                                ,NULL);
                        }
                        else if(L_SLCTD_TYPE == 8){
                            char elf_path[MX_PTH_LNGTH]="";
                            strcpy(elf_path,LCWD);
                            strcat(elf_path,"/");
                            strcat(elf_path,L_SLCTD_NAME);
                            mvprintw(22,0,"elf_path=%s",elf_path);
                            refresh();
                            execl(elf_path,L_SLCTD_NAME, NULL);
                        }
                        else{
                            chdir(LCWD);
                            lcontents_n = open_dir(win, &lwin_namelist,
                            event);
                            print_dir_content(left, lwin_namelist, 
                            lcontents_n);
                            getcwd(LCWD, 100);
                        
                        }
                        /*chdir(LCWD);
                        lcontents_n = open_dir(win, &lwin_namelist,
                            event);
                        print_dir_content(left, lwin_namelist, 
                            lcontents_n);
                        getcwd(LCWD, 100);*/
                        }    
                    else if(win == right){
                        chdir(RCWD);
                        rcontents_n = open_dir(win, &rwin_namelist,
                            event);
                        print_dir_content(right, rwin_namelist,
                            rcontents_n);
                        getcwd(RCWD, 100);
                    }
                break;
                }
            case KEY_UP:
                getyx(stdscr, y, x);
                move(y-1, COLS/2);
                break;
            case KEY_DOWN:
                getyx(stdscr, y, x);
                move(y+1, COLS/2);
                break;
            case ENTER:

            case TAB:
                if(win == left){
                    getyx(left, y, x);
                    wmove(right, y, 0);
                } 
                if(win == right){
                    getyx(right, y, x);
                    wmove(left, y, 0); 
                }
                break;
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