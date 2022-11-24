#define _POSIX_C_SOURCE 200200L
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <curses.h>
#include <time.h>
#include <string.h>

void game(int difficulty, int* score, char name[100]);
bool game_over(int rows, int columns, char field[rows][columns+1]);
void game_field(int difficulty, int length, int height, int* score);
void draw_border(int difficulty, int* leingth, int* height);
bool check(int rows, int columns, char field[rows][columns+1]);
bool block_move(int rows, int columns, char field[rows][columns+1], char block[4][9], int it, int jt);
void update(int rows, int columns, char field[rows][columns+1], int color[rows][columns+1], int paint);
int can_move(int rows, int columns, char field[rows][columns+1], int it, int jt);
void generate_block(char block[4][9], char randblock);
void color();
void check_rows(int rows, int columns, char field[rows][columns+1], int color[rows][columns+1], int* score);
void turn(char block[4][9], char randblock, int i);

int main(int argc, char *argv[])
{
    FILE *lb= fopen("leaderboard.txt", "a+");
    if(argc!=2)
    {
        printf("Wrong amount of arguments\n");
        return EXIT_FAILURE;
    }
    if(strcmp(argv[1], "easy") && strcmp(argv[1], "normal") && strcmp(argv[1], "hard"))
    {
        printf("Wrong difficulty\n");
        return EXIT_FAILURE;
    }
    int difficulty;
    if(!strcmp(argv[1], "easy"))
        difficulty=1;
    if(!strcmp(argv[1], "normal"))
        difficulty=2;
    if(!strcmp(argv[1], "hard"))
        difficulty=3;
    int score=0;
    char name[100];
    game(difficulty, &score, name);
    char sscore[10];
    sprintf(sscore, "%d", score);
    fputs(name, lb);
    fputc(' ', lb);
    fputs(sscore, lb);
    fputs("\n", lb);
    //^EXIT_FAILURE
    return EXIT_SUCCESS;
}

bool game_over(int rows, int columns, char field[rows][columns+1])
{
    for(int j=0;j<columns;j++)
    {
        if(field[0][j]!=' ')
            return true;
    }
    return false;
}

void color()
{
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED); 
    init_pair(2, COLOR_BLACK, COLOR_GREEN); 
    init_pair(3, COLOR_BLACK, COLOR_YELLOW); 
    init_pair(4, COLOR_BLACK, COLOR_BLUE); 
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA); 
    init_pair(6, COLOR_BLACK, COLOR_CYAN); 
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
}

void draw_border(int difficulty, int *length, int *height)
{ 
    move(3, COLS/2-35); attron(COLOR_PAIR(7));
    printw("          "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("        "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("          "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("      "); attroff(COLOR_PAIR(7));
    printw("      "); attron(COLOR_PAIR(7));
    printw("      "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("        ");  
    move(4, COLS/2-31); 
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("              "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("      "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("      "); attron(COLOR_PAIR(7));
    printw("  ");
    move(5, COLS/2-31); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));      //T
    printw("        "); attron(COLOR_PAIR(7));
    printw("        "); attroff(COLOR_PAIR(7));//E
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));      //T
    printw("        "); attron(COLOR_PAIR(7));
    printw("      "); attroff(COLOR_PAIR(7));  //R
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));      //I
    printw("      "); attron(COLOR_PAIR(7));
    printw("        ");                        //S
    move(6, COLS/2-31); 
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("              "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("      "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("            "); attron(COLOR_PAIR(7));
    printw("  ");
    move(7, COLS/2-31); 
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("        "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("        "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("  "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("      "); attroff(COLOR_PAIR(7));
    printw("    "); attron(COLOR_PAIR(7));
    printw("        ");
    attroff(COLOR_PAIR(7));
    if(difficulty==1)
    {
        *length=6;
        *height=12;
    }
    if(difficulty==2)
    {
        *length=12;
        *height=20;
    }
    if(difficulty==3)
    {
        *length=10;
        *height=24;
    }
    attron(COLOR_PAIR(7));
    for(int i=0;i<=*length;i++)
    {
        mvprintw(11, COLS/2+i, " ");
        mvprintw(11, COLS/2-i, " ");
    }
    for(int j=0;j<(*height+2);j++)
    {
        mvprintw(j+11, COLS/2+*length, " ");
        mvprintw(j+11, COLS/2-*length-1, " ");
        mvprintw(j+11, COLS/2-*length-2, " ");
        mvprintw(j+11, COLS/2+*length+1, " ");
    }
    for(int i=0;i<=*length;i++)
    {
        mvprintw(*height+12, COLS/2+i, " ");
        mvprintw(*height+12, COLS/2-i, " ");
    }
    attroff(COLOR_PAIR(7));
    refresh();
}

void generate_block(char block[4][9], char randblock)
{
    if(randblock=='Z')
    {
        block[1][2]='#';
        block[1][3]='#';

        block[1][4]='#';
        block[1][5]='#';

        block[2][4]='#';
        block[2][5]='#';

        block[2][6]='#';
        block[2][7]='#';
    }
    if(randblock=='L')
    {
        block[0][2]='#';
        block[0][3]='#';

        block[1][2]='#';
        block[1][3]='#';

        block[2][2]='#';
        block[2][3]='#';

        block[3][2]='#';
        block[3][3]='#';

        block[3][4]='#';
        block[3][5]='#';

    }
    if(randblock=='I')
    {
        block[0][2]='#';
        block[0][3]='#';
        
        block[1][2]='#';
        block[1][3]='#';
        
        block[2][2]='#';
        block[2][3]='#';
        
        block[3][2]='#';
        block[3][3]='#';
    }
    if(randblock=='S')
    {
        block[1][2]='#';
        block[1][3]='#';

        block[1][4]='#';
        block[1][5]='#';

        block[2][0]='#';
        block[2][1]='#';

        block[2][2]='#';
        block[2][3]='#';
    }
    if(randblock=='T')
    {
        block[2][0]='#';
        block[2][1]='#';

        block[2][2]='#';
        block[2][3]='#';

        block[1][2]='#';
        block[1][3]='#';

        block[2][4]='#';
        block[2][5]='#';
    }
    if(randblock=='O')
    {
        block[1][2]='#';
        block[1][3]='#';
        
        block[1][4]='#';
        block[1][5]='#';
        
        block[2][2]='#';
        block[2][3]='#';
        
        block[2][4]='#';
        block[2][5]='#';
    }
}

bool check(int rows, int columns, char field[rows][columns+1])
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;j++)
        {
            if(field[i][j]=='#')
            {
                if(i==(rows-1))
                    return false;
                if(field[i+1][j]=='/')
                    return false;
            } 
        }
    }
    return true;
}

bool block_move(int rows, int columns, char field[rows][columns+1], char block[4][9], int it, int jt)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;j++)
        {
            if(field[i][j]!='/')
                field[i][j]=' ';
        }
    }
    for(int i=it;i<it+4;i++)
    {
        for(int j=jt;j<jt+8;j++)
        {
            if(field[i%rows][j%columns]!='/')
                field[i%rows][j%columns]=block[i-it][j-jt];
            if(field[i%rows][j%columns]=='/' && !it)
            {
                return false;
            }
        }
    }
    return true;
}

void update(int rows, int columns, char field[rows][columns+1], int color[rows][columns+1], int paint)
{
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;j++)
        {
            if(field[i][j]=='#')
            {
                field[i][j]='/';
                color[i][j]=paint;
            }
        }
    }
}

int can_move(int rows, int columns, char field[rows][columns+1], int it, int jt)
{
    for(int i=it; i<it+4; i++)
    {
        for(int j=jt; j<jt+8; j++)
        {
            if(field[i][j]=='#')
            {
                if(j==0)
                    return -1;
                if(j==(columns-1))
                    return 1;
                else
                {
                    if(field[i][j-1]=='/')
                        return -1;
                    if(field[i][j+1]=='/')
                        return 1;
                }
            }
        }
    }
    return 0;
}

void check_row(int rows, int columns, char field[rows][columns+1], int color[rows][columns+1], int* score)
{
    int row=0;
    for(int i=0;i<rows;i++)
    {
        bool r=1;
        for(int j=0;j<columns;j++)
        {
            if(field[i][j]!='/')
            {
                r=0;
                break;
            }
        }
        if(r)
        {
            *score+=columns;
            row=i;
            break;
        }
    }
    for(int i=row;i>0;i--)
    {
        for(int j=0;j<columns;j++)
        {
            field[i][j]=field[i-1][j];
            color[i][j]=color[i-1][j];
            attron(COLOR_PAIR(color[i][j]));
            mvprintw(12+i, j+(COLS-columns)/2, " ");
            attroff(COLOR_PAIR(color[i][j]));
        }
    }
}

void turn(char block[4][9], char randblock, int i)
{
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<8;j++)
            block[i][j]=' ';
    }
    if(randblock=='Z')
    {
        if(!(i%2))
        {
            block[1][2]='#';
            block[1][3]='#';

            block[1][4]='#';
            block[1][5]='#';

            block[2][4]='#';
            block[2][5]='#';

            block[2][6]='#';
            block[2][7]='#';
        }
        else
        {
            block[1][4]='#';
            block[1][5]='#';

            block[2][4]='#';
            block[2][5]='#';

            block[2][2]='#';
            block[2][3]='#';

            block[3][2]='#';
            block[3][3]='#';
        }
    }
    if(randblock=='L')
    {
        i%=4;
        if(i==0)
        {    
            block[0][2]='#';
            block[0][3]='#';

            block[1][2]='#';
            block[1][3]='#';

            block[2][2]='#';
            block[2][3]='#';

            block[3][2]='#';
            block[3][3]='#';

            block[3][4]='#';
            block[3][5]='#';
        }
        if(i==1)
        {    
            for(int j=0;j<8;j++)
                block[2][j]='#';
    
            block[3][0]='#';
            block[3][1]='#';
        }
        if(i==2)
        {
            for(int j=0;j<4;j++)
                block[j][4]='#';
            for(int j=0;j<4;j++)
                block[j][5]='#';

            block[0][2]='#';
            block[0][3]='#';
        }
        if(i==3)
        {
            for(int j=0;j<8;j++)
                block[3][j]='#';
    
            block[2][6]='#';
            block[2][7]='#';
        }
    }
    if(randblock=='I')
    {
        i=i%4;
        if(i==0)
        {
            for(int j=0;j<4;j++)
                block[j][2]='#';
            for(int j=0;j<4;j++)
                block[j][3]='#';
        }
        if(i==1)
        {
            for(int j=0;j<8;j++)
                block[1][j]='#';
        }
        if(i==2)
        {
            for(int j=0;j<4;j++)
                block[j][4]='#';
            for(int j=0;j<4;j++)
                block[j][5]='#';
        }
        if(i==3)
        {
            for(int j=0;j<8;j++)
                block[2][j]='#';
        }
    }
    if(randblock=='S')
    {
        if(i%2)
        {
            block[1][2]='#';
            block[1][3]='#';

            block[2][2]='#';
            block[2][3]='#';

            block[2][4]='#';
            block[2][5]='#';

            block[3][4]='#';
            block[3][5]='#';
        }
        else
        {
            block[1][2]='#';
            block[1][3]='#';

            block[1][4]='#';
            block[1][5]='#';

            block[2][0]='#';
            block[2][1]='#';

            block[2][2]='#';
            block[2][3]='#';
        }
    }
    if(randblock=='T')
    {
        i%=4;
        block[2][0]='#';
        block[2][1]='#';

        block[2][2]='#';
        block[2][3]='#';

        block[1][2]='#';
        block[1][3]='#';

        block[2][4]='#';
        block[2][5]='#';
            
        block[3][2]='#';
        block[3][3]='#';
        if(i==0)
        {
            block[3][2]=' ';
            block[3][3]=' ';
        }
        if(i==1)
        {
            block[2][0]=' ';
            block[2][1]=' ';
        }
        if(i==2)
        {
            block[1][2]=' ';
            block[1][3]=' ';
        }
        if(i==3)
        {
            block[2][4]=' ';
            block[2][5]=' ';
        }
    }
    if(randblock=='O')
    {  
        block[1][2]='#';
        block[1][3]='#';
        
        block[1][4]='#';
        block[1][5]='#';
        
        block[2][2]='#';
        block[2][3]='#';
        
        block[2][4]='#';
        block[2][5]='#';
    }
}

void game_field(int difficulty, int length, int height, int* score)
{  
    srand(time(NULL));
    //Clears buffer and allows CTRL+C
    cbreak();
    //getch() is not visible
    noecho();
    //Allows input from keyboad
    keypad(stdscr, TRUE);
    //Visibility of cursor
    curs_set(FALSE);
    //Allows getch() to work without ENTER
    nodelay(stdscr, TRUE);
    int speed=6-difficulty;
    struct timespec ts={
    .tv_sec=0,
    .tv_nsec=0.01*5000000000L*speed
    };
    int tm=0, trn=0;
    int rows=height;
    int columns=length*2;
    char field[rows][columns+1];
    int color[rows][columns+1];
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<columns;j++)
        {
            field[i][j]=' ';
            color[i][j]=8;
        }
        field[i][columns]='\0';
        color[i][columns]='\0';
    }
    char blocks[]={"ZLISTO"};
    bool end=0;
    while(!game_over(rows, columns, field) && !end)
    {
        char randblock=blocks[rand()%6];
        char block[4][9]=
        {
            "        ",
            "        ",
            "        ",
            "        "
        };
        int paint=rand()%6+1;
        generate_block(block, randblock);
        int it=0,jt=columns/2-4,line;
        bool mv=true;
        mvprintw(12, 30, "YOUR SCORE IS: %d", *score);
        while(true)
        {
            if(!block_move(rows, columns, field, block, it, jt))
            {
                end=true;
                break;
            }
            check_row(rows, columns, field, color, &*score);
            mvprintw(12, 30, "YOUR SCORE IS: %d", *score);
            for(int i=0;i<rows;i++)
            {
                for(int j=0;j<columns;j++)
                {
                    if(field[i][j]==' ')
                    {
                        attron(COLOR_PAIR(8));
                        mvprintw(12+i, j+(COLS-columns)/2, " ");
                        attroff(COLOR_PAIR(8));
                    }
                    if(field[i][j]=='#')
                    {
                        attron(COLOR_PAIR(paint));
                        mvprintw(12+i, j+(COLS-columns)/2, " ");
                        attroff(COLOR_PAIR(paint));
                    }
                }
            }
            refresh();
            ts.tv_nsec=0.1 * 1000000000L*speed;
            nanosleep(&ts, NULL);
            int input = getch();
            switch(input)
            {   
                case KEY_LEFT:
                {
                    if(can_move(rows, columns, field, it, jt)!=-1)
                    {
                        if(mv)
                        {
                            line=it;
                            it--;
                            mv=false;
                        }
                        jt-=2;
                    }
                    break;
                }
                case KEY_RIGHT:
                {
                    if(can_move(rows, columns, field, it, jt)!=1)
                    {
                        if(mv)
                        {
                            line=it;
                            it--;
                            mv=false;
                        }
                        jt+=2;
                    }
                    break;
                }
                case KEY_DOWN:
                {
                    if(!(tm%2))
                        speed/=2;
                    else
                        speed*=2;
                    tm++;
                    break;
                }
                case KEY_UP:
                {
                    if(mv)
                    {
                        line=it;
                        it--;
                        mv=false;
                    }
                    trn++;
                    turn(block, randblock, trn);
                    break;
                }
            }
            ts.tv_nsec=0.1 * 1000000000L;
            nanosleep(&ts, NULL);
            if(check(rows, columns, field))
                it++;
            else
                break;
            if(line<it)
                mv=true;
        }
        update(rows, columns, field, color, paint); 
    }
    //move(12, COLS/2-length+1);
}

void game(int difficulty, int* score, char name[100])
{
    initscr();
    color();
    int length, height;
    mvprintw(8, COLS/2-7, "YOUR NICKNAME: ");
    scanw("%[^\n]", name);
    mvprintw(8, COLS/2-7, "                                                                                                                                          ");
    draw_border(difficulty, &length, &height);
    game_field(difficulty, length, height, &*score);
    nodelay(stdscr, FALSE);
    mvprintw(40, COLS/2-18, "GAME OVER   PRESS ANY BUTTON TO EXIT");
    getch();
    endwin();
}   









