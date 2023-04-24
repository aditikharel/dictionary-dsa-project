
#include<string>
#include<fstream>
#include<iostream>


//to check whether the word to be searched/deleted is found or not
bool wordfound = true;

//to check whether word is being searched or 
//searching is completed and need to start looking for suggestions
bool searching = true;

//count of suggestion words
int count = 0;

//gets standard output
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


std::ifstream in1("words.txt");
std::ifstream in2("meanings.txt");
std::string s1, s2;

const int NUM_OF_ALPHABET = 26;


//Trie Node structure
class TrieNode
{
    public:
        TrieNode *children[NUM_OF_ALPHABET];
        bool isEndOfWord;
        std::string word;
        std::string meaning;
};

void animate(std::string s)
{
 
    //displays character of string
    for(int i = 0; i < s.size(); i++)
    {
        std::cout<<s[i]<<std::flush;

        //checks if output has reached the end of screen
        if((i >= 98) && (i % 98 == 0))
            std::cout<<std::endl;
        
    }
    std::cout<<std::endl;
 
}

//returns new trie node initialized to NULL
TrieNode *getNode(void)
{
    TrieNode *ptr = new TrieNode; 
    
    ptr->isEndOfWord = false;
    ptr->word = "0";
    ptr->meaning = "0";

    //initialize each child pointer to NULL
    for(int i = 0; i < NUM_OF_ALPHABET; i++)
        ptr->children[i] = NULL;
    
    return ptr;
}

//If key is part of a bigger word and is already present, it marks the required node as end of word
void insert(TrieNode *root, std::string key, std::string mean)
{
    TrieNode *ptr = root;
    
    for(int i = 0; i < key.length(); i++)
    {
        //gets the position of the node to go to next for insertion of next character
        int index = key[i] - 'a';

        //initializes a new node at the required position if it does not exist
        if(!ptr->children[index])
            ptr->children[index] = getNode();
        
        ptr = ptr->children[index];
    }

    ptr->isEndOfWord = true;
    ptr->word = key;
    ptr->meaning = mean;

}

//checks if the node has no children. 
//If it is empty, returns true 
bool isEmpty(TrieNode *root)
{
    //checks if each child node is not NULL
    for(int i = 0; i < NUM_OF_ALPHABET; i++)
    {
        if(root->children[i])
            return false;
    }
    return true;
}

TrieNode *remove(TrieNode *root, std::string key, int depth = 0)
{

    //When word not found
    if(!root || ((depth == key.size()) && (root->isEndOfWord == false)))
    {
        SetConsoleTextAttribute(hConsole, 12);
        animate("No such word found!");
        animate("Please check if you're writing the correct word!\n");
        SetConsoleTextAttribute(hConsole, 15);
        return NULL;
    }
    
    //executed once depth equal to length of key
    if(depth == key.size())
    {
        //if word is found, the current node is no longer the end of a word
        if(root->isEndOfWord)
        {
            root->isEndOfWord = false;
            root->meaning = "0";
            root->word = "0";
            
            wordfound = true;

            SetConsoleTextAttribute(hConsole, 10);
            animate("Word found and deleted successfully!\n");
            SetConsoleTextAttribute(hConsole, 15);
        }
        
        //if current node has no children, then delete the current node and set the value to NULL
        if(isEmpty(root))
        {
            delete(root);
            root = NULL;
        }

        return root;
    }

    //if not last character, recursively call the function for the next character
    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);

    //if only child of root got deleted, and it is not the end of word, delete node
    if(isEmpty(root) && root->isEndOfWord == false)
    {
        delete root;
        root = NULL;
    }

    return root;
}


//recursive function for searching the trie structure and for autocompleting words
//autocomplete works regardless of the fact whether the word exists in the trie or not
void search_and_suggest(TrieNode *root, std::string key, int depth = 0)
{

    //searching operation is done until the depth equal to length of key is reached, then find words for suggest

    //searching and wordfound flags are set to true until we reach the depth= length os key ow untill NULL node
    if(depth <= key.size() && searching)
    {
        if(!root || (depth == key.size() && root->isEndOfWord == false))
        {
            SetConsoleTextAttribute(hConsole, 12);
            animate("No such word found!\n");
            wordfound = false;
            SetConsoleTextAttribute(hConsole, 15);
            animate("Did you mean: ");
            searching = false;

            //for key of length greater than 1, return function
            //for key of length 1, it will go for depth 0 then suggest all words in dictionary
            //Example can be ther, the ,there
            if(key.size() > 1)
                return;   
        }
        
        //When word found
        else if(depth == key.size() && root->isEndOfWord)
        {
            searching = false;

            SetConsoleTextAttribute(hConsole, 10);
            animate("Word found!!\n");
            SetConsoleTextAttribute(hConsole, 15);
            
            animate((root->word + ": " + root->meaning + "\n"));

            animate("See these other suggestions:");
     
            if(key.size() > 1)
                return;
            
        }

        //Till depth reached, recursively calls the function for next letter

        else
        {
            int index = key[depth] - 'a';
            search_and_suggest(root->children[index], key, depth + 1);
        }
    }

    
    //suggestion part
    int length = key.size();

    
    //Reduce size of length if length of key is greater than 1 
    //only for suggestions if their length is greater than or equal to one less than length of key 
    if(key.size() > 1)
        length -= 1;

    
    //for depth >= length and if the searching part is completed: searching = false
    //only showing 6 suggestions
    if((searching == false) && (depth >= length) && (count < 6))
    {
        //for null node
        if(!root)
            return;

        //if given node is the end of a word and if this word is not key word
        if(root->isEndOfWord && root->word != key)
        {
            animate((root->word + "\n"));
            //counter for suggestions
            count++;
        }


        //iterate for each non-NULL node starting from node at the depth == length    
        for(int i = 0; i < NUM_OF_ALPHABET; i++)
        {
            if(root->children[i])
                search_and_suggest(root->children[i], key, depth + 1);
        }
    }
    return;
}
