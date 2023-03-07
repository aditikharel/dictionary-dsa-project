//By Atharva Shekatkar
//20BDS0067

//This file is a utility header for animating the given text to make the program look better


#include<iostream>
#include<thread>
#include<stdlib.h>
#include<windows.h>
#include<mmsystem.h>

#pragma comment(lib, "Winmm.lib")

int x = -1;

//sound flag is used to check whether user wants sound or not 
//bool sound = false;

//cursorshow flag is used to check whether currently cursor is being displayed or not
bool cursorshow = true;

//SetConsoleTextAttribute sets the color of the output text to the specified color
//SetConsoleCursorPosition sets the position of the cursor on the screen

//utility function to toggle visibility of cursor inside the running program
bool toggle_cursor_visibility(bool showCursor){

    if(showCursor == true)
        showCursor = false;
    else  
        showCursor = true;

    HANDLE std_out = GetStdHandle(STD_OUTPUT_HANDLE);   // Get standard output
    CONSOLE_CURSOR_INFO cursorInfo;                   
    GetConsoleCursorInfo(std_out, &cursorInfo);           // Get cursorinfo from output
    cursorInfo.bVisible = showCursor;                 // Set flag visible.
    SetConsoleCursorInfo(std_out, &cursorInfo);         //set cursor info
    return showCursor;
}

//loading animation for the beginning of the program
//we make the cursor invisible when there is an output being printed on the screen 
//or when loading animation plays
//the cursor is made visible after this again
void load_animation(){
    HANDLE std_out_1 = GetStdHandle(STD_OUTPUT_HANDLE);

    //clears screen
    system("cls");

    //toggle cursor visibility
    cursorshow = toggle_cursor_visibility(cursorshow);
    std::string s = "Loading /";
    std::cout<<s;
    int i = 0;

    //looping for showing the loading animation
    while(i < 40){
        std::cout<<"\b";
        if(i % 4 == 1)
            std::cout<<"-";
        else if(i % 4 == 2)
            std::cout<<"\\";
        else if(i % 4 == 3)
            std::cout<<"|";
        else
            std::cout<<"/";
        i++;
        //sleep function for 130 milliseconds
     //   std::this_thread::sleep_for(std::chrono::milliseconds(130));
    }
    
    //deleting the characters displayed one by one
    i = 0;
    while(i < s.size()){
        std::cout<<"\b \b";
        i++;
        //std::this_thread::sleep_for(std::chrono::milliseconds(60));
    }

    i = 0;
    s = "Load complete.\nPress Enter to continue.";
    SetConsoleTextAttribute(std_out_1, 10);

    //displays the given string one by one
    while(i < s.size()){
        std::cout<<s[i];
       // std::this_thread::sleep_for(std::chrono::milliseconds(60));
        i++;
    }
    
    cursorshow = toggle_cursor_visibility(cursorshow);
    getchar();

    cursorshow = toggle_cursor_visibility(cursorshow);

    //the coordinates to which the cursor position is to be set
    COORD coord;
    coord.X = 24;
    coord.Y = 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    
    i = 0;
    //deletes previously displayed characters one by one 
    while(i < s.size()){
        if(i == 24){
            coord.X = 14;
            coord.Y = 0;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        }

        std::cout<<"\b \b";
        //std::this_thread::sleep_for(std::chrono::milliseconds(60));
        i++;
    }

    cursorshow = toggle_cursor_visibility(cursorshow);
    SetConsoleTextAttribute(std_out_1, 15);

}

//animates the given string by displaying each character one by one
void animate(std::string s){

    //x being even or odd determines which beep sound will be played
    x++;
    cursorshow = toggle_cursor_visibility(cursorshow);
    
    //if user wants sound to be played, then the following block is executed
    // if(sound){
    //     if(x % 2 != 0)
    //         PlaySoundA("Computer_beep_sound.wav", NULL, SND_ASYNC | SND_FILENAME);
    //     else
    //         PlaySoundA("Computer_beep_sound_2.wav", NULL, SND_ASYNC | SND_FILENAME);
    // }

    //displays the character of the string s one by one
    for(int i = 0; i < s.size(); i++){
        std::cout<<s[i]<<std::flush;

        //checks if the output has reached the end of screen
        if((i >= 98) && (i % 98 == 0))
            std::cout<<std::endl;
        
        //sleeps for 20 miliseconds
       // std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    std::cout<<std::endl;
    
    cursorshow = toggle_cursor_visibility(cursorshow);
}