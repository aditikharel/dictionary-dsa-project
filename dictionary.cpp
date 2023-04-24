
#include<cstdio>
#include<cctype>
#include<algorithm>
#include<string.h>
#include<thread>
#include<stdlib.h>
#include<windows.h>
#include<mmsystem.h>
#include<iostream>
#include"trie.h"

#define max_recentwords 5

void display_menu();
void search_word(TrieNode *);
void add_word(TrieNode *, std::string word = "0");
void print_recentwords(TrieNode *, std::string word = "0");
TrieNode *delete_word(TrieNode *);

bool isValidWord(std::string);

int main()
{
 
    TrieNode *root = getNode();

    //reads from the words.txt and meanings.txt files until end of file is reached
    while(getline(in1, s1))
    {
        getline(in2, s2);
        //inserts the given word and its meaning in the trie structure
        insert(root, s1, s2);
    }
    
    in1.close();
    in2.close();

    
    int choice = 0;
    //infinite loop until user chooses to exit
    while(choice != 5)
    {
        system("cls");

        SetConsoleTextAttribute(hConsole, 11);
        animate("\t\t\t\t\tWelcome to Dictionary!\n\n");
        SetConsoleTextAttribute(hConsole, 15);

        display_menu();
        std::cin>>choice;
        std::cin.clear();
        std::cin.ignore(2147483647,'\n');

        switch(choice)
        {
            case 1:
                search_word(root);
                break;

            case 2:
                add_word(root);
                break;
            
            case 3:
                root = delete_word(root);
                break;
            
            case 4:
                print_recentwords(root);
                break;

            case 5:
                SetConsoleTextAttribute(hConsole, 11);
                animate("Thank you for using this application!\nSee you next time!");
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


//Menu
void display_menu(){
    animate("\t\t\t\t\t      Main Menu\n");
    animate("Enter your choice: ");
    animate("1. Search a word.");
    animate("2. Add a word.");
    animate("3. Delete a word.");
    animate("4. Search History.");
    animate("5. Exit the application");
    animate("\n\nEnter your choice: ");
}


//searches for the word in trie structure
void search_word(TrieNode *root)
{
    system("cls");

    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t\t    Search a word!\n");
    SetConsoleTextAttribute(hConsole, 15);

    animate("Enter word to be searched: ");
    std::string key;
    std::cin>>key;
    std::cin.clear();
    std::cin.ignore(2147483647,'\n');

    //converts the given word to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    //Valid word means all alphabets
    if(isValidWord(key))
    {
        static int numRecent =1;
        char ** recent=(char**)malloc(max_recentwords * sizeof(char*));

        if (numRecent <= max_recentwords)
        {
            FILE* fp = fopen("recent.txt","a");
            const char* recent_str = key.c_str();
            recent[numRecent-1] = (char*) malloc ((strlen(recent_str)+1) * sizeof(char));
            strcpy(recent[numRecent-1], recent_str);
            fprintf(fp, "%s\n", recent[numRecent-1]);
            fclose(fp);
        }
        
        search_and_suggest(root, key);

        //iWhen no words are found for suggestions
        if(count == 0)
        {
            SetConsoleTextAttribute(hConsole, 12);
            animate("Sorry, no suggestions found!\n");
            SetConsoleTextAttribute(hConsole, 15);
        }
        
        char c = 'n';
        
        //When word is not found, allows users to add 
        if(wordfound == false)
        {
            animate("\nThink we missed this word?");
            animate("Would you like to add this word to the dictionary along with it's meaning?(y/n)");
            std::cin>>c;
            std::cin.clear();
            std::cin.ignore(2147483647,'\n');
            if(c == 'y' || c == 'Y')
                add_word(root, key);
        }
        
        //To ask if user wants to search
        if(c == 'n' || c == 'N' || wordfound == true){
            animate("Would you like to search another word?(y/n)");
            std::cin>>c;
            std::cin.clear();
            std::cin.ignore(2147483647,'\n');
            
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

    //Error message for invalid word
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


//wordis set to zero when no argument passed
void add_word(TrieNode *root, std::string word)
{
    if(word == "0")
        system("cls");

    std::ofstream out1;
    std::ofstream out2;

    out1.open("words.txt", std::ios_base::app);
    out2.open("meanings.txt", std::ios_base::app);
    
    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t    Add a new word to the dictionary!\n");
    SetConsoleTextAttribute(hConsole, 15);
    
    //When no key passed
    if(word == "0")
    {
        animate("Enter the word to be added: ");
        std::cin>>word;
        std::cin.clear();
        std::cin.ignore(2147483647,'\n');

        //converts it to lower case
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
    }


    //checks if given word is valid
    if(isValidWord(word))
    {
        //adds key to word file
        out1<<word<<"\n";

        //ask for meaning from user
        animate(("Enter the meaning of " + word + ": "));
        std::string meaning;
        std::getline(std::cin, meaning);
        
        //adds meaning to meaning file
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

    //Error message 
    else
    {
        SetConsoleTextAttribute(hConsole, 12);
        animate("Error! Word should only contain letters from the English alphabet!\n");
        SetConsoleTextAttribute(hConsole, 15);
        animate("Press Enter to continue");
        getchar();
        add_word(root);
    }

}


TrieNode *delete_word(TrieNode *root)
{ 
    system("cls");

    SetConsoleTextAttribute(hConsole, 11);
    animate("\t\t\t\t    Delete a word from the dictionary\n");
    SetConsoleTextAttribute(hConsole, 15);

    animate("Enter the word to be deleted: ");
    std::string key;
    std::cin>>key;
    std::cin.clear();
    std::cin.ignore(2147483647,'\n');

    //transforms the word to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    //checks if word is valid
    if(isValidWord(key))
    {   
        wordfound = false;

        //calls the function to remove the word from the trie structure
        root = remove(root, key);
        
        //When word is found and deleted from Trie
        if(wordfound)
        {
            //new file tempwords.txt is created,all words except the one to be deleted are added to this
            //the words.txt is deleted and tempwords.txt is renamed
            //same for meanings.txt
  
            std::ofstream temp("tempwords.txt");
            in1.open("words.txt");
            std::string s;
            
            //pos is the position of the word
            int pos = 0;
            bool found = false;

            //entering each word except the word to be deleted
            while(std::getline(in1, s))
            {
                if (s == key)
                    found = true;
                
                else
                {
                    temp<<s<<"\n";
                    
                    //position stops incrementing if the word is found
                    if(found == false)
                        pos++;
                }
            }

            in1.close();
            temp.close();

            //deleting and renaming the files
            remove("words.txt");
            rename("tempwords.txt", "words.txt");

            //opening meanings.txt file and creating a temporary file for meanings
            temp.open("tempmeanings.txt");
            in1.open("meanings.txt");

            //entering each word's meaning except for the word to be deleted
            while(std::getline(in1, s))
            {
                //checks if position of the meaning to be deleted is reached
                if(pos == 0)
                {
                    pos--;
                    continue;
                }
                temp<<s<<"\n";
                pos--;
            }

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

    //Error message for invalid word
    else
    {
        SetConsoleTextAttribute(hConsole, 12);
        animate("Error! Word should only contain letters from the English alphabet!\n");
        SetConsoleTextAttribute(hConsole, 15);
        animate("Press Enter to continue");
        getchar();
        delete_word(root);
    }
    
    return root;
}


//To check if word is valid
bool isValidWord(std::string key)
{
    for(int i = 0; i < key.size(); i++)
    {
        //checks if character is an alphabet
        if(isalpha(key[i]) == false)
            return false;
    }
    return true;
}



void print_recentwords(TrieNode *root, std::string word)
{
    if(word == "0")
         system("cls");
    
    const int MAX_WORD_LENGTH = 100;
    char ch;

    FILE *fp = fopen("recent.txt", "r");
    if (fp == NULL) 
    {
        printf("Failed to open file\n");
        exit(1);
    }

    char str[MAX_WORD_LENGTH];
    char *words[max_recentwords];
    int count = 0;
    while (fscanf(fp, "%s", str) == 1) 
    {
        if (count < max_recentwords)
        {
            words[count] = strdup (str);
            count++;
        }
        else
        {
            free(words[0]);
            for (int i = 0; i < max_recentwords - 1; i++) 
            {
                words[i] = words[i + 1];
            }
            words[max_recentwords - 1] = strdup(str);
        }
        
    }
    fclose(fp);

    printf("Recently Searched Words:\n\n");
    for (int i = count-1; i >= 0; i--) 
    {
        printf("%s\n", words[i]);
    }

    for (int i = 0; i < max_recentwords; i++) 
    {
        free(words[i]);
    }
     printf("\n\nPress Enter key to continue...\n");
    while ((ch = getchar()) != '\n')
    {
        putchar(fgetc(fp));
    }

}

