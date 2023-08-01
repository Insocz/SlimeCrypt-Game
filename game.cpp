#include <ncurses.h>
#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

class Map_manager{
    public:
        void creGrid(string file_name,int wight,char (&map_grid)[32][128]);        
        void renGrid(char grid[][128],int y_pos,int x_pos,int y,int x,WINDOW * win);
        void clsGrid(int y_pos,int x_pos,int y,int x,WINDOW * win);
};

void Map_manager::creGrid(string file_name,int wight,char (&map_grid)[32][128]){
    ifstream file(file_name);

    if (!file.is_open()) {
        endwin();

        cout << "Error map level file missing\n";        
        exit(1);
    }

    int i = 0;
    int j = 0;
    char ch;

    while (file.get(ch)) {
        map_grid[i][j] = ch;
        j++;
        if (j == wight-1) {
            i++;
            j = 0;
        }
    }
    file.close();

    /* for showing the map_grid
    for(int h = 0;h <= 32;h++){
        for(int w = 0;w <= 128;w++){
            mvwprintw(win,h+1,w+1,"%c",map_grid[h][w]);
        }
    }
    */
}

void Map_manager::renGrid(char grid[][128],int y_pos,int x_pos,int y,int x,WINDOW * win){

    /*
    y
          333333
        2222222222
    - 11111111111111
      00000pley00000
    + 11111pley11111
      22222222222222
        3333333333
          444444

    x
    ------- ++++++++++
          101234
        3210123456
      54321012345678
    - 54321pley45678 +
    - 54321pley45678 +
      54321012345678
        3210123456
          101234

    */

    wattron(win,A_REVERSE);

    for(int i = 0;i<=y+1;i++){
        for(int j = 0;j<=x;j++){
            
            wattron(win,A_REVERSE);
            
            if(i != 0 || j != 0){
                if(grid[(y_pos-1)-i][(x_pos-1)-j] != ' '){
                    wattroff(win,A_REVERSE);
                }
                mvwprintw(win,y_pos-i,x_pos-j,"%c",grid[(y_pos-1)-i][(x_pos-1)-j]);
            }
        }

        for(int j = 0;j<=x+4;j++){
            
            wattron(win,A_REVERSE);
            
            if(i != 0 || j > 4){
                if(grid[(y_pos-1)-i][(x_pos-1)+j] != ' '){
                    wattroff(win,A_REVERSE);
                }

                mvwprintw(win,y_pos-i,x_pos+j,"%c",grid[(y_pos-1)-i][(x_pos-1)+j]);
            }
        }
    }

    for(int i = 1;i<=y+2;i++){
        for(int j = 0;j<=x;j++){
            
            wattron(win,A_REVERSE);
            
            if(i != 1 || j != 0){
                if(grid[(y_pos-1)+i][(x_pos-1)-j] != ' '){
                    wattroff(win,A_REVERSE);
                }

                mvwprintw(win,y_pos+i,x_pos-j,"%c",grid[(y_pos-1)+i][(x_pos-1)-j]);
            }            
        }
    
        for(int j = 0;j<=x+4;j++){
            
            wattron(win,A_REVERSE);

            if(i != 1 || j > 4){
                if(grid[(y_pos-1)+i][(x_pos-1)+j] != ' '){
                    wattroff(win,A_REVERSE);
                }

                mvwprintw(win,y_pos+i,x_pos+j,"%c",grid[(y_pos-1)+i][(x_pos-1)+j]);
            }
        }
    }

    wattroff(win,A_REVERSE);

    box(win,0,0);
    refresh();
    wrefresh(win);
}

void Map_manager::clsGrid(int y_pos,int x_pos,int y,int x,WINDOW * win){

    for(int i = 0;i<=y+1;i++){
        for(int j = 0;j<=x;j++){
            if(i != 0 || j != 0){
                mvwprintw(win,y_pos-i,x_pos-j," ");
            }
        }

        for(int j = 0;j<=x+4;j++){
            if(i != 0 || j > 4){
                mvwprintw(win,y_pos-i,x_pos+j," ");
            }
        }
    }

    for(int i = 1;i<=y+2;i++){
        for(int j = 0;j<=x;j++){
            if(i != 1 || j != 0){
                mvwprintw(win,y_pos+i,x_pos-j," ");
            }            
        }
    
        for(int j = 0;j<=x+4;j++){
            if(i != 1 || j > 4){
                mvwprintw(win,y_pos+i,x_pos+j," ");
            }
        }
    }
}

class Window_manager{
    public:
        WINDOW * creWin(int heigh,int weight,int y,int x,int box_1,int box_2,bool key,bool boxik);
        void delWin(WINDOW * win);
        string pouse_win(string * arra,int len,int y,int x);
        string wpouse_ver(WINDOW * win,string * arra,int len,int y);
        void renPouse_ver(WINDOW * win,string * arra,int len,int y);
        
};

WINDOW * Window_manager::creWin(int heigh,int weight,int y,int x,int box_1,int box_2,bool key,bool boxik){
    WINDOW * win = newwin(heigh,weight,y,x);
    
    if (boxik){
        box(win,box_1,box_2);
    }
    refresh();
    wrefresh(win);

    keypad(win,key);
    
    return win;
}

void Window_manager::delWin(WINDOW * win){
    wclear(win);
    refresh();
    wrefresh(win);
    delwin(win);
}

string Window_manager::pouse_win(string * arra,int len,int y,int x){
    int high = 0;
    int x_size;
    int y_size;
    int c;

    getmaxyx(stdscr,y_size,x_size);

    Window_manager Window_class;

    WINDOW * pouse_win = Window_class.creWin(y,x,y_size/2-y/2,x_size/2-x/2,0,0,true,true);

    while (1)
    {
        for(int i = 0;i<len;i++){
            if(i == high){
                wattron(pouse_win,A_REVERSE);
            }         
        
            mvwprintw(pouse_win,i+1,2,"%s",arra[i].c_str());
            wattroff(pouse_win,A_REVERSE);
        }
        
        c = wgetch(pouse_win);

        switch (c)
        {
        case KEY_UP:
            high--;
            if (high == -1){
                high = len-1;
            }
            break;
        
        case KEY_DOWN:
            high++;
            if (high == len){
                high = 0;
            }
            break;

        default:
            break;

        }
        if (c == 10){break;}
    }

    Window_class.delWin(pouse_win);

    return arra[high];
}

string Window_manager::wpouse_ver(WINDOW * win,string * arra,int len,int y){
    int high = 0;
    int c;
    int size = 0;

    while (1)
    {
        for(int i = 0;i<len;i++){
            if(i == high){
                wattron(win,A_REVERSE);
            }         
        
            mvwprintw(win,y,2+size,"%s",arra[i].c_str());
            wattroff(win,A_REVERSE);
        
            size += arra[i].size() + 1;
        }
        
        size = 0;

        c = wgetch(win);

        switch (c)
        {
        case KEY_LEFT:
            high--;
            if (high == -1){
        
                high = len-1;
            }
            break;
        case KEY_RIGHT:
            high++;
            if (high == len){
                high = 0;
            }
            break;

        default:
            break;

        }
        if (c == 10){break;}
    }

    for(int i = 0;i<len;i++){   

        wattroff(win,A_REVERSE);
        mvwprintw(win,2,2+size,"%s",arra[i].c_str());
    
        size += arra[i].size() + 1;
    }

    refresh();
    wrefresh(win);

    return arra[high];
}

void Window_manager::renPouse_ver(WINDOW * win,string * arra,int len,int y){
            
    int size = 0;

    for(int i = 0;i<len;i++){
        mvwprintw(win,y,2+size,"%s",arra[i].c_str());

        size += arra[i].size() + 1;
    }

    wrefresh(win);
}

class Player_manager{
    public:
        void setPla(int y,int x,WINDOW * win,bool up);
        void movPla(char& c,int& y_pos,int& x_pos,WINDOW * win,bool& up,char grid[][128]);
        bool isBlo(char grid[][128],int y_pos,int x_pos,char c);
};

void Player_manager::setPla(int y,int x,WINDOW * win,bool up){
    if (up){
        mvwprintw(win,y,x,".---.");
        mvwprintw(win,y+1,x,"|___|");
    }
    else{
        mvwprintw(win,y,x," ___ ");
        mvwprintw(win,y+1,x,"|___|");
    }
}

void Player_manager::movPla(char& c,int& y_pos,int& x_pos,WINDOW * win,bool& up,char grid[][128]){

    c = getch();

    switch (c)
    {
    case 'w':
        if (y_pos != 1){
                if (isBlo(grid,y_pos,x_pos,c)){
                    mvwprintw(win,y_pos+1,x_pos,"     ");
                    y_pos--;
                }
            }
        break;
    
    case 's':
        if (y_pos != 32){
            if (isBlo(grid,y_pos,x_pos,c)){
                    mvwprintw(win,y_pos,x_pos,"     ");
                    y_pos++;
                }
            }
        break;

    case 'a':
        if (x_pos != 1){
            if (isBlo(grid,y_pos,x_pos,c)){
                    mvwprintw(win,y_pos,x_pos+4," ");
                    mvwprintw(win,y_pos+1,x_pos+4," ");
                    x_pos--;
                }
            }
        break;

    case 'd':
        if (x_pos != 128){
            if (isBlo(grid,y_pos,x_pos,c)){
                    mvwprintw(win,y_pos,x_pos," ");
                    mvwprintw(win,y_pos+1,x_pos," ");
                    x_pos++;
                }
            }
        break;

    default:
        break;
    }

    setPla(y_pos,x_pos,win,up);

    up = !up;

    if (c != ERR){
        if (c == 'w' || c == 's'){usleep(200000);}
        else {usleep(150000);}
        flushinp();
    }
}

bool Player_manager::isBlo(char grid[][128], int y_pos, int x_pos, char c) {
    switch (c) {
    case 'w':
        y_pos--;
        // Adjust x_pos to check five consecutive spaces
        for (int i = 0; i < 5; i++) {
            if (' ' != grid[(y_pos-1)][(x_pos-1)+i]) {
                return false; // obstacle found, return false
            }
        }
        return true; // no obstacles found, return true
        break;

    case 's':
        y_pos += 2;
        // Adjust x_pos to check five consecutive spaces
        for (int i = 0; i < 5; i++) {
            if (' ' != grid[(y_pos-1)][(x_pos-1)+i]) {
                return false; // obstacle found, return false
            }
        }
        return true; // no obstacles found, return true
        break;

    case 'a':
        x_pos--;
        if (' ' != grid[(y_pos-1)][(x_pos-1)] || ' ' != grid[(y_pos-1)+1][x_pos]) {
            return false; // obstacle found, return false
        }
        return true; // no obstacles found, return true
        break;

    case 'd':
        x_pos += 5;
        if (' ' != grid[(y_pos-1)][(x_pos-1)] || ' ' != grid[(y_pos-1)+1][x_pos]) {
            return false; // obstacle found, return false
        }
        return true; // no obstacles found, return true
        break;

    default:
        break;
    }

    return false; // For safety, return false in case of invalid input
}

class Settings_manager{
    public:
        void setting_set();
        void setting_apply(int speed,int y_view,int x_view);
        void speed_get(int& speed);
        void speed_write(int speed);
        void viewDis_get(int& y_view,int& x_view);
        void viewDis_write(int y_view,int x_view);

};

void Settings_manager::setting_set(){

    Window_manager Window_class;

    int x_size;
    int y_size;
    int c;
    int len = 6;
    int high = 0;

    int speed_num; 
    speed_get(speed_num);

    int y_view;
    int x_view;
    viewDis_get(y_view,x_view);

    getmaxyx(stdscr,y_size,x_size);

    WINDOW * win = Window_class.creWin(20,30,y_size/2-20/2,x_size/2-30/2,0,0,true,true);

    while (1)
    {
        
        string settings_list[6] {"Speed: "+to_string(speed_num)+"ms <>","View: y-"+to_string(y_view)+" <>","      x-"+to_string(x_view)+" <>","","Apply","Exit"};
        mvwprintw(win,1,2,"                ");
        mvwprintw(win,2,2,"                ");
        mvwprintw(win,3,2,"                ");

        for(int i = 0;i<len;i++){
            if(i == high){
                wattron(win,A_REVERSE);
            }         
        
            mvwprintw(win,i+1,2,"%s",settings_list[i].c_str());
            wattroff(win,A_REVERSE);
        }
        
        c = wgetch(win);

        switch (c)
        {
        case KEY_UP:
            high--;
            if (high == -1){
                high = len-1;
            }
            break;
        
        case KEY_DOWN:
            high++;
            if (high == len){
                high = 0;
            }
            break;

        case KEY_LEFT:
            if (high == 0 && speed_num != 100){
                speed_num -= 10;
            }
            else if(high == 1 && y_view != 0){
                y_view -= 1;
            }
            else if(high == 2 && x_view != 0){
                x_view -= 1;
            }
            break;

        case KEY_RIGHT:
            if (high == 0 && speed_num != 3000){
                speed_num += 10;
            }
            else if(high == 1 && y_view != 20){
                y_view += 1;
            }
            else if(high == 2 && x_view != 40){
                x_view += 1;
            }
            break;

        default:
            break;

        }
        if (c == 10 && settings_list[high] == "Apply"){setting_apply(speed_num,y_view,x_view);break;}
        if (c == 10 && settings_list[high] == "Exit"){break;}
    }

    Window_class.delWin(win);
}

void Settings_manager::setting_apply(int speed,int y_view,int x_view){
    
    speed_write(speed);
    viewDis_write(y_view,x_view);
}

void Settings_manager::speed_get(int& speed){

    ifstream speed_file("settings/speed.txt");
    if (speed_file.is_open()) {
        speed_file >> speed;
        speed_file.close();
    }
    else{
        endwin();

        cout << "Error settings speed file missing\n";        
        exit(1);
    }
}

void Settings_manager::speed_write(int speed){
    
    ofstream speed_file("settings/speed.txt");
    if (speed_file.is_open()) {
        speed_file << speed;
        speed_file.close();
    }
    else{
        endwin();

        cout << "Error settings speed file missing\n";        
        exit(1);
    }
}

void Settings_manager::viewDis_get(int& y_view,int& x_view){
    ifstream view_file("settings/view.txt");
    if (view_file.is_open()) {
        view_file >> y_view >> x_view;
        view_file.close();
    }
    else{
        endwin();

        cout << "Error settings speed file missing\n";        
        exit(1);
    }   
}

void Settings_manager::viewDis_write(int y_view,int x_view){
    ofstream view_file("settings/view.txt");
    if (view_file.is_open()) {
        view_file << y_view << "\n" << x_view;
        view_file.close();
    }
    else{
        endwin();

        cout << "Error settings speed file missing\n";        
        exit(1);
    }   
}

class Inventory_manager{
    public:
        void show_inv();
        void set_inv(WINDOW * win);

    private:
        Window_manager Window_class;
};

void Inventory_manager::show_inv(){

    int y_size,x_size;

    getmaxyx(stdscr,y_size,x_size);

    WINDOW * win =  Window_class.creWin(32,80,y_size/2-29/2-2,x_size/2-80/2,0,0,true,true);

    wattron(win,A_REVERSE);

    for(int i = 1;i<80;i++){
        mvwprintw(win,1,i,"-");
    }

    mvwprintw(win,1,2,"Inventory");

    wattroff(win,A_REVERSE);

    for(int i = 1;i<50;i++){
        for(int j = 2;j<31;j+=4){
            mvwprintw(win,j,i,"-");
        }
    }
    
    for(int i = 1;i<50;i+=8){
        for(int j = 2;j<31;j+=1){
            mvwprintw(win,j,i,"|");
        }
    }

    box(win,0,0);
    
    set_inv(win);

    Window_class.delWin(win);
    
}

void Inventory_manager::set_inv(WINDOW * win){

// 6 x 3
// 2 x 3

int x = 2;
int y = 3;
int c;

while(1){
    wattron(win,A_REVERSE);

    for(int i=0;i<=2;i++){
        for(int j=0;j<=6;j++){
            mvwprintw(win,y+i,x+j," ");
        }
    } 

    wattroff(win,A_REVERSE);

    refresh();
    wrefresh(win);

    c = wgetch(win);

    for(int i=0;i<=2;i++){
        for(int j=0;j<=6;j++){
            mvwprintw(win,y+i,x+j," ");
        }
    } 

    switch (c)
    {
    case KEY_UP:
        if (y != 3){
            y -= 4;
        }
        break;
    
    case KEY_DOWN:
        if (y != 27){
            y += 4;
        }
        break;

    case KEY_RIGHT:
        if(x != 42){
            x += 8;
        }
        break;

    case KEY_LEFT:
        if (x != 2){
            x -= 8;
        }
        break;

    default:
        break;
    }

    if (c == 10){
        break;
    }

}

}

int main(){

    //pre set start
        initscr();
    
        curs_set(0);
        noecho();
        cbreak();
    
        Window_manager Window_class;
        Map_manager Map_class;
        Player_manager Player_class;
        Settings_manager Settings_class;
        Inventory_manager Inventory_class;

    //per set end

    //set vars start
        int x_size;
        int y_size;
        string pouse_output;
        string start_menu[6] {"New game","Continue","Settings","Help","","Exit"};
        string help_menu[9] {"","The original settings is how the game","should be played but you can change it.","","Move: WSAD","Menu: arrows left,right/esc","Settings: arrows up,down,left,right","","Exit"};
        string esc_menu[5] {"Inventory","Resume","Settings","Help","Exit"};
        int y_pos = 12;
        int x_pos = 12;
        int y_view = 2;
        int x_view = 5;
        char c;
        bool up = true;
        int speed;
        char map_grid[32][128];
    //set vars end

    //code start

        //code setup start

            Settings_class.viewDis_get(y_view,x_view);
            Settings_class.speed_get(speed);
            timeout(speed);

            getmaxyx(stdscr,y_size,x_size);
            
            if(y_size < 36 && x_size < 132+5+6){
                endwin();
            
                cout << "Error too small window use y >= 36 x >= 143\n";

                return -1;
            }

            if(!has_colors()){
                endwin();

                cout << "Error terminal doesnt have colors\n";

                return -1;
            }
        
            //main win
            WINDOW * main_win = Window_class.creWin(34,130,y_size/2-17,x_size/2-65,0,0,true,true);

            //stat win 
            WINDOW * stat_win = Window_class.creWin(5,30,y_size/2-17-5,x_size/2-65,0,0,true,true);
        
            //menu win
            WINDOW * menu_win = Window_class.creWin(5,130,y_size/2+17,x_size/2-65,0,0,true,true);
        //code setup end

        //code main start
            mvprintw(y_size/2-22,x_size/2-10,"%s","________________________________  ____   ____  __  _   ____   ______");
            mvprintw(y_size/2-21,x_size/2-10,"%s"," ___   __     __   __  __   ___  |   _| | __ | \\ \\| | | __ | |__  __|");
            mvprintw(y_size/2-20,x_size/2-10,"%s","|  _| |  |   |  | |  \\/  | | __| |  |   |    /  \\   | |   _|   |  |");
            mvprintw(y_size/2-19,x_size/2-10,"%s"," \\ \\  |  |_  |  | |      | | __| |  |_  |_|\\_\\   \\__| |__|     |__|");
            mvprintw(y_size/2-18,x_size/2-10,"%s","|___| |____| |__| |__||__| |___| |____| _____________________________");

            while (1){
                pouse_output = Window_class.pouse_win(start_menu,6,20,30);
            
                if (pouse_output == "New game"){
                    break;
                }
                else if (pouse_output == "Continue"){
                    break;
                }   
                else if (pouse_output == "Settings"){
                    Settings_class.setting_set();
                    Settings_class.speed_get(speed);
                    timeout(speed);
                    Settings_class.viewDis_get(y_view,x_view);
                }
                else if (pouse_output == "Help"){
                    while(1){
                        pouse_output = Window_class.pouse_win(help_menu,9,12,45);
                        if(pouse_output == "Exit"){
                            break;
                        }
                    }
                }
                else if (pouse_output == "Exit"){
                    endwin();
                    return 0;
                }
            }

            Map_class.creGrid("lv1.txt",130,map_grid);
            Map_class.renGrid(map_grid,y_pos,x_pos,y_view,x_view,main_win);
            Player_class.setPla(y_pos,x_pos,main_win,up);
            Window_class.renPouse_ver(menu_win,esc_menu,5,2);            

            refresh();
            wrefresh(main_win);

            while (1){
                Map_class.clsGrid(y_pos,x_pos,y_view,x_view,main_win);
                Player_class.movPla(c,y_pos,x_pos,main_win,up,map_grid);
                Map_class.renGrid(map_grid,y_pos,x_pos,y_view,x_view,main_win);
                
                if (c == 27){
                    
                    pouse_output = Window_class.wpouse_ver(menu_win,esc_menu,5,2);
                    
                    if (pouse_output == "Settings"){
                        Settings_class.setting_set();
                        Settings_class.speed_get(speed);
                        timeout(speed);
                        Map_class.clsGrid(y_pos,x_pos,y_view,x_view,main_win);
                        Settings_class.viewDis_get(y_view,x_view);        
                    }

                    else if (pouse_output == "Inventory"){
                        Inventory_class.show_inv();
                    }

                    else if (pouse_output == "Help"){
                        while(1){
                            pouse_output = Window_class.pouse_win(help_menu,9,12,45);
                            if(pouse_output == "Exit"){
                                break;
                            }
                        }
                    }

                    else if (pouse_output == "Exit"){
                        endwin();
                        return 0;
                    }

                    Map_class.renGrid(map_grid,y_pos,x_pos,y_view,x_view,main_win);
                    Player_class.setPla(y_pos,x_pos,main_win,up);
                
                }
            }

        //code main end

    //code end

    //end start
       endwin();
    //end end
 
    return 0;
}