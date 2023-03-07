//By Atharva Shekatkar
//20BDS0067

//this is the main file from which the program will be run
 
#include"trie.h"
#include<cstdio>
#include<cctype>
#include<algorithm>

//prototype declarations
void display_menu();
void search_word(TrieNode *);
void add_word(TrieNode *, std::string word = "0");
TrieNode *delete_word(TrieNode *);
//void toggle_sound();
//void changefont(int, int);
bool isValidWord(std::string);

//Main function
int main(){
    
    //calls load animation function
    load_animation();

    //changes the font size for the program
    //changefont(0, 32);

    //initializes root node
    TrieNode *root = getNode();

    //reads from the words.txt and meanings.txt files until end of file is reached
    while(getline(in1, s1)){
        getline(in2, s2);
        
        //inserts the given word and its meaning in the trie structure
        insert(root, s1, s2);
    }
    
    //closing the files
    in1.close();
    in2.close();

    
    int choice = 0;
    //infinite loop until user chooses to exit
    while(choice != 5){
        //clears screen
        system("cls");

        SetConsoleTextAttribute(hConsole, 11);
        animate("\t\t\t\t\tWelcome to Dictionary!\n\n");
        SetConsoleTextAttribute(hConsole, 15);

        display_menu();
        std::cin>>choice;
        std::cin.clear();
        std::cin.ignore(2147483647,'\n');
       // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch(choice){
            case 1:
                search_word(root);
                break;

            case 2:
                add_word(root);
                break;
            
            case 3:
                root = delete_word(root);
                break;
            
            // case 4:
            //     toggle_sound();
            //     break;
            
            case 4:
                SetConsoleTextAttribute(hConsole, 11);
                animate("Thank you for using this application!\nSee you next time!");
               // std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                SetConsoleTextAttribute(hConsole, 15);
                break;
            
            default:
                SetConsoleTextAttribute(hConsole, 11);
                animate("Please enter a valid option!");
                SetConsoleTextAttribute(hConsole, 15);
                animate("Press Enter to continue");
                getchar();

        }
    }

    return 0;

}

//displays the menu 
void display_menu(){
    animate("\t\t\t\t\t      Main Menu\n");
    animate("Enter your choice: ");
    animate("1. Search a word.");
    animate("2. Add a word.");
    animate("3. Delete a word.");
    //animate("4. Toggle the beep sound effect on/off");
    animate("4. Exit the application");
    animate("\n\nEnter your choice: ");
}


//searches for the word in trie structure
void search_word(TrieNode *root){
    //clear screen
    system("cls");

    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t\t    Search a word!\n");
    SetConsoleTextAttribute(hConsole, 15);

    //gets the word to be searched from the user
    animate("Enter word to be searched: ");
    std::string key;
    std::cin>>key;
    std::cin.clear();
    std::cin.ignore(2147483647,'\n');
    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //conveerts the given word to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    //checks given word is valid, that is, only contains alphabets
    if(isValidWord(key)){
        
        //calls the function to search the given word in the trie structure and suggest words based on it
        search_and_suggest(root, key);

        //if no words are found for suggestions, then the following block is executed
        if(count == 0){
            SetConsoleTextAttribute(hConsole, 12);
            animate("Sorry, no suggestions found!\n");
            SetConsoleTextAttribute(hConsole, 15);
        }
        
        char c = 'n';
        
        //if given word is not found, a chance is given to add the word to the dictionary
        if(wordfound == false){
            animate("\nThink we missed this word?");
            animate("Would you like to add this word to the dictionary along with it's meaning?(y/n)");
            std::cin>>c;
            std::cin.clear();
            std::cin.ignore(2147483647,'\n');
            //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            //checks if user chooses to add the given word
            if(c == 'y' || c == 'Y')
                //calls funtion to add given word to the dictionary
                add_word(root, key);
        }
        
        //checks if user wants to search another word in the dictionary 
        if(c == 'n' || c == 'N' || wordfound == true){
            animate("Would you like to search another word?(y/n)");
            std::cin>>c;
            std::cin.clear();
            std::cin.ignore(2147483647,'\n');
       //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            //if user wants to search another word, recursively calls itself again
            if(c == 'y'){
                searching = true;
                wordfound = true;
                count = 0;
                search_word(root);
            }
        }

        searching = true;
        wordfound = true;
        count = 0;
    }

    //displays error message for invalid word
    else{
        SetConsoleTextAttribute(hConsole, 12);
        animate("Error! Word should only contain letters from the English alphabet!\n");
        SetConsoleTextAttribute(hConsole, 15);
        animate("Press Enter to continue");
        getchar();
        search_word(root);
    }

    return;
}

//function to add a word to the dictionary
//word is by default set to "0" if no argument is passed
void add_word(TrieNode *root, std::string word){
    
    //if no word is passed, clears screen
    if(word == "0")
        system("cls");

    std::ofstream out1;
    std::ofstream out2;

    //opens words and meanings text files to write the new word and it's meaning to them
    out1.open("words.txt", std::ios_base::app);
    out2.open("meanings.txt", std::ios_base::app);
    
    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t    Add a new word to the dictionary!\n");
    SetConsoleTextAttribute(hConsole, 15);
    
    //if no key is passed, gets the word to be added from the user
    if(word == "0"){
        animate("Enter the word to be added: ");
        std::cin>>word;
        std::cin.clear();
        std::cin.ignore(2147483647,'\n');
  //      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //converts it to lower case
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    }

    //checks if given word is valid
    if(isValidWord(word)){

        
        
        //adds key to words.txt
        out1<<word<<"\n";

        //gets meaning of the word from the user
        animate(("Enter the meaning of " + word + ": "));
        std::string meaning;
        std::getline(std::cin, meaning);
        
        //adds meaning to meaning.txt
        out2<<meaning<<"\n";

        insert(root, word, meaning);
    
        out1.close();
        out2.close();

        SetConsoleTextAttribute(hConsole, 10);
        animate("\nWord added Successfully!");
        
        SetConsoleTextAttribute(hConsole, 11);
        
        animate("Press Enter to continue");
        
        SetConsoleTextAttribute(hConsole, 15);
        getchar();
    }

    //displays error message for incorrect word
    else{
        SetConsoleTextAttribute(hConsole, 12);
        animate("Error! Word should only contain letters from the English alphabet!\n");
        SetConsoleTextAttribute(hConsole, 15);
        animate("Press Enter to continue");
        getchar();
        add_word(root);
    }

}


//used to toggle the beep typing sound on or off
// void toggle_sound(){
//     system("cls");
//     if(sound == true)
//         sound = false;
//     else
//         sound = true;
//     SetConsoleTextAttribute(hConsole, 11);
//     animate("Typing sound toggled!");
//     animate("Press Enter to continue");
//     SetConsoleTextAttribute(hConsole, 15);
//     getchar();
// }

//function to delete a word from the trie structure as well as the text files 
//if the word is found
TrieNode *delete_word(TrieNode *root){
    //clears screen
    system("cls");

    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t    Delete a word from the dictionary\n");
    SetConsoleTextAttribute(hConsole, 15);

    //getting the word to be deleted from the user
    animate("Enter the word to be deleted: ");
    std::string key;
    std::cin>>key;
    std::cin.clear();
    std::cin.ignore(2147483647,'\n');
   // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //transforms the word to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    //checks if word is valid
    if(isValidWord(key)){   

        wordfound = false;

        //calls the function to remove the word from the trie structure
        root = remove(root, key);
        
        //if word is found and deleted from the trie structure, then the following block is executed
        if(wordfound){
            //what essentially happens here is that a new file called tempwords.txt is created
            //all words other than the word to be deleted are added to this temporary file.
            //the file words.txt is deleted and tempwords.txt is renamed to words.txt
            //similar process is followed for the meanings file


            std::ofstream temp("tempwords.txt");
            in1.open("words.txt");
            std::string s;
            
            //pos stores the position (that is, the line) at which the word occurs
            int pos = 0;
            bool found = false;

            //entering each word except the word to be deleted
            while(std::getline(in1, s)){
                if (s == key)
                    found = true;
                
                else{
                    temp<<s<<"\n";
                    
                    //position stops incrementing if the word is found
                    if(found == false)
                        pos++;
                }
            }

            //closing the files
            in1.close();
            temp.close();

            //deleting and renaming the files
            remove("words.txt");
            rename("tempwords.txt", "words.txt");

            //opening meanings.txt file and creating a temporary file for meanings
            temp.open("tempmeanings.txt");
            in1.open("meanings.txt");

            //entering each word's meaning sequentially except for the word to be deleted
            while(std::getline(in1, s)){
                //checks if position of the meaning to be deleted is reached
                if(pos == 0){
                    pos--;
                    continue;
                }
                temp<<s<<"\n";
                pos--;
            }

            //closing the filess
            in1.close();
            temp.close();

            //removing and renaming the files
            remove("meanings.txt");
            rename("tempmeanings.txt", "meanings.txt");

        }
        

        SetConsoleTextAttribute(hConsole, 11);
        animate("Press Enter to continue");
        getchar();
        SetConsoleTextAttribute(hConsole, 15);
        wordfound = true;
    }

    //displays error message if word is not valid
    else{
        SetConsoleTextAttribute(hConsole, 12);
        animate("Error! Word should only contain letters from the English alphabet!\n");
        SetConsoleTextAttribute(hConsole, 15);
        animate("Press Enter to continue");
        getchar();
        delete_word(root);
    }
    
    
    return root;
}

//utulity function for windows to change font
// void changefont(int width, int height){
//     CONSOLE_FONT_INFOEX font_info;
//     font_info.cbSize = sizeof(font_info);
//     font_info.nFont = 0;
//     font_info.dwFontSize.X = width;                   // Width of each character in the font
//     font_info.dwFontSize.Y = height;                  // Height
//     font_info.FontFamily = FF_DONTCARE;
//     font_info.FontWeight = FW_NORMAL;
//     std::wcscpy(font_info.FaceName, L"Consolas"); // Choose your font
//     SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &font_info);
// }

//checks if word passed to it is valid
bool isValidWord(std::string key){
    for(int i = 0; i < key.size(); i++){
        //checks if the character is an alphabet
        if(isalpha(key[i]) == false)
            return false;
    }
    return true;
}