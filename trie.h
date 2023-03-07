//By Atharva Shekatkar
//20BDS0067

#include<string>
#include<fstream>
#include"animation.h"

//wordfound flag is used to check whether the word to be searched/deleted is found or not
bool wordfound = true;

//searching flag is used to display whether we are searching for a given word or if we are 
//done searching and need to start looking for suggestion words
bool searching = true;

//to keep count of the suggestion words displayed
int count = 0;

//gets standard output
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


//words.txt and meanings.txt are the files containing words and meanings for the dictionary respectively
std::ifstream in1("words.txt");
std::ifstream in2("meanings.txt");
std::string s1, s2;

//number of english alphabets
const int NUM_OF_ALPHABET = 26;

//Structure of a Trie node
class TrieNode{
    public:
        TrieNode *children[NUM_OF_ALPHABET];
        bool isEndOfWord;
        std::string word;
        std::string meaning;
};


//returns new trie node initialized to NULL
TrieNode *getNode(void){
    TrieNode *ptr = new TrieNode; 
    
    ptr->isEndOfWord = false;
    ptr->word = "0";
    ptr->meaning = "0";

    //initializes each child pointer to NULL
    for(int i = 0; i < NUM_OF_ALPHABET; i++)
        ptr->children[i] = NULL;
    
    return ptr;
}

//inserts key into tree.
//If key is part of a bigger word and is already present, it marks the required node as end of word
void insert(TrieNode *root, std::string key, std::string mean){
    TrieNode *ptr = root;
    
    for(int i = 0; i < key.length(); i++){
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

//checks if the node has no children. If it is empty, returns true else returns false
bool isEmpty(TrieNode *root){
    //iteratively checks if each child node is not NULL
    for(int i = 0; i < NUM_OF_ALPHABET; i++){
        if(root->children[i])
            return false;
    }
    return true;
}

TrieNode *remove(TrieNode *root, std::string key, int depth = 0){
    
    //if word is not found, then this block is executed
    if(!root || ((depth == key.size()) && (root->isEndOfWord == false))){
        SetConsoleTextAttribute(hConsole, 12);
        animate("No such word found!");
        animate("Please check if you're writing the correct word!\n");
        SetConsoleTextAttribute(hConsole, 15);
        return NULL;
    }
    
    //executed once we reach the depth equal to length of key
    if(depth == key.size()){

        //if word is found, the current node is no longer the end of a word
        if(root->isEndOfWord){
            root->isEndOfWord = false;
            root->meaning = "0";
            root->word = "0";
            
            wordfound = true;

            SetConsoleTextAttribute(hConsole, 10);
            animate("Word found and deleted successfully!\n");
            SetConsoleTextAttribute(hConsole, 15);
        }
        
        //if current node has no children, then delete the current node and set the value to NULL
        if(isEmpty(root)){
            delete(root);
            root = NULL;
        }

        return root;

    }

    //if not last character, recursively call the function for the next character
    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);

    //if root does not have any child (it's only child got deleted),
    //and it is not the end of a word, then delete the node
    if(isEmpty(root) && root->isEndOfWord == false){
        delete root;
        root = NULL;
    }

    return root;
}


//recursive function for both searching the trie structure and for autocompleting words
//autocomplete works regardless of the fact whether the word exists in the trie or not
void search_and_suggest(TrieNode *root, std::string key, int depth = 0){

    //searching operation is done until the depth equal to length of key is reached
    //after that, we begin to find words for autocompletion

    //searching and wordfound flags are set to true until we reach the depth equal to 
    //the length of the key or until we encounter a NULL node
    if(depth <= key.size() && searching){
        
        //if NULL node is found or given key is not a word, searching and wordfound are set to false
        if(!root || (depth == key.size() && root->isEndOfWord == false)){
            SetConsoleTextAttribute(hConsole, 12);
            animate("No such word found!\n");
            wordfound = false;
            SetConsoleTextAttribute(hConsole, 15);
            animate("Did you mean: ");
            searching = false;

            //if length of key is greater than 1, we return the function.
            //The idea behind this is that if key is "ther", the autocomplete/suggestion feature should suggest both 
            //"the" as well as "there", taking into account that it's possible we actually meant to type "the"
            //but ended up accidentally pressing "r".
            //however, if we return for key of length 1 as well, it will go back to depth 0
            //all words begin from depth 0, hence we would get a list of all words in the dictionary
            //as a result, this condition is required
            if(key.size() > 1)
                return;
            
        }
        
        //if the given word is found, wordfound is kept true and searching is set to false 
        else if(depth == key.size() && root->isEndOfWord){
            searching = false;

            SetConsoleTextAttribute(hConsole, 10);
            animate("Word found!!\n");
            SetConsoleTextAttribute(hConsole, 15);
            
            animate((root->word + ": " + root->meaning + "\n"));

            animate("See these other suggestions:");
            
            //if length of key is greater than 1, we return the function.
            //The idea behind this is that if we enter "ther", the autocomplete/suggestion feature should suggest both 
            //"the" as well as "there", taking into account that it's possible we actually meant to type "the"
            //but ended up accidentally pressing "r".
            //however, if we return for key of length 1 as well, it will go back to depth 0
            //all words begin from depth 0, hence we would get a list of all words in the dictionary
            //as a result, this condition is required 
            if(key.size() > 1)
                return;
            
        }

        //if we have not reached the depth of the word,
        //the function is recursively called for the next letter in the key 
        else{
            int index = key[depth] - 'a';
            search_and_suggest(root->children[index], key, depth + 1);
        }
    }

    
    //the suggestion part begins from here:
    int length = key.size();

    
    //we reduce the size of length as we return from the function if length of key is greater than 1
    //we are only interested in words for suggestions if their length is greater than or equal to one less than length of key 
    if(key.size() > 1)
        length -= 1;

    
    //this block will only be executed for depth >= length and if the searching part is completed(that is, when searching == false)
    //additionally, we only show 6 suggestions, as sometimes the suggestions list may get too long
    if((searching == false) && (depth >= length) && (count < 6)){
        
        //if we encounter a NULL node, we return
        if(!root)
            return;
        
        
        
        //if the given node is the end of a word
        //and if this word is not the same as the key word, then we display this word
        if(root->isEndOfWord && root->word != key){
            animate((root->word + "\n"));
            
            //incrementing the counter for number of suggestions displayed
            count++;
        }


        //we iterate for each non-NULL node starting from node at the depth == length to account for maximum suggestions     
        for(int i = 0; i < NUM_OF_ALPHABET; i++){
            if(root->children[i])
                search_and_suggest(root->children[i], key, depth + 1);
        }
    }
    return;
}