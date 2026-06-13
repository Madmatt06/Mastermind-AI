/*
    Dr. Mark E. Lehr
    CSC 7 Template for Mastermind AI
    May 11th, 2021
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
using namespace std;

//Function Prototypes
string AI(char,char);
bool eval(string,string,char &,char &);
string set();

int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare variables
    string code,guess;  //code to break, and current guess
    char rr,rw;         //right digit in right place vs. wrong place
    int nGuess;         //number of guesses
    
    //Initialize Values
    nGuess=0;
    code=set();
    rr=rw=0;
    
    //Loop until solved and count to find solution
    do{
       nGuess++;
       guess=AI(rr,rw);
    }while(eval(code,guess,rr,rw));
    
    //Output the solution
    cout<<"Number of guesses using Linear Search = "<<nGuess<<endl;
    cout<<code<<"="<<guess<<endl;

    //Exit the program
    return 0;
}

string AI(char rr,char rw){
    // This is a state machine. The AI will have multiple states it can enter.
    // The first (0) is color searching mode where it will try to find how many of each color is in the code.
    // The second (1) is the positioning guessing mode where it will try to determine where the color is located in the code.
    // The third (2) is final guess mode where it will submit what has to be the answer based off what it learned.
    // (This is my first time making a state machine in C++)
    static int state = 0;
    // This keeps track of what color it is working with currently if the state needs it
    static int color = -1;
    // Used to test the position of a color
    static int position = -1;
    // Used to keep track of how many of each color there is
    static int colorCount[10] = {};
    // Keeps track of what is known
    static int answer[4] = {-1, -1, -1, -1};
    // Keeps track of what number is not in the code to use for testing positions
    static int wrong = -1;
    // Keep track of how many were last in the correct position
    static int lastrr = 0;
    // Used to see if it found all the colors in the code
    static int found = 0;

    string sGuess="0000";
    // This first switch is largley used to handle information about the previous guess
    switch(state) {
        case 0:
        // The first time the function runs the color will be out of bounds
        if(color != -1) {
            colorCount[color] = rr + rw;
            found += rr + rw;
        }

        if(rr+rw == 0) wrong = color;
        
        // This runs if we are done with colors
        if(color >= 8 || found == 4) {
            // Shortcut to reduce guesses (last color will be remaining not found)
            if(found != 4) colorCount[color+1] = 4 - found;
            found = 4;

            // If a unused color was not found the next one is assumed to not be used
            if(wrong == -1) wrong = color + 1;
            
            // Resets and moves on to the next state
            state++;
            color = 0;

            // Skips colors that don't have any found
            while(color <= 9 && colorCount[color] == 0) {
                color++;
            }
        }
        break;

        case 1:
        {
            if(position != -1) {
                // if rr increased, a correct position was found
                if(rr > lastrr) {
                    answer[position] = color;
                    lastrr = rr;
                    colorCount[color]--;
                }
            }

            int remains = 0;
            for(int i = position + 1; i < 4; i++) {
                if(answer[i] == -1) remains++;
            }
            
            // If the color count matches the remaining, fill all of them
            if(remains == colorCount[color]) {
                for(int i = position + 1; i < 4; i++) {
                    if(answer[i] == -1) {
                        answer[i] = color;
                        lastrr++;
                    }
                }
                colorCount[color] = 0;
            }

            // Resets the position and moves to next color if done with current color
            if(position >= 3 || colorCount[color] <= 0) {
                position = -1;
                color++;

                // Searches for next color that exists in code
                while(color <= 9 && colorCount[color] == 0) {
                    color++;
                }

                // Moves to next state when done
                if(color > 9) {
                    state++;
                }
            }
        }
        break; 
        default:
        break;

    }

    // This switch statement makes the next guesses
    switch(state) {
        // Checks how many of each color exists
        case 0:
        color++;
        for(int i = 0; i < 4; i++) sGuess[i] = color + '0';
        break;

        case 1:
        // Searches for a position which hasn't been figured out yet
        do {
            position++;
        } while(answer[position] != -1);

        // Fills in the knowns, guess, and unknown
        for(int i = 0; i < 4; i++) {
            if(i == position) {
                sGuess[i] = color + '0';
            } else{
                if (answer[i] == -1) {
                    sGuess[i] = wrong + '0';
                } else {
                    sGuess[i] = answer[i] + '0';
                }
            }
        }

        break;

        case 2:
        // Returns final guess
        for(int i = 0; i < 4; i++) {
            sGuess[i] = answer[i] + '0';
        }
    }
    return sGuess;
}

bool eval(string code,string guess,char &rr,char &rw){
    string check="    ";
    rr=0,rw=0;
    //Check how many are right place
    for(int i=0;i<code.length();i++){
        if(code[i]==guess[i]){
            rr++;
            check[i]='x';
            guess[i]='x';
        }
    }
    //Check how many are wrong place
    for(int j=0;j<code.length();j++){
        for(int i=0;i<code.length();i++){
            if((i!=j)&&(code[i]==guess[j])&&(check[i]==' ')){
                rw++;
                check[i]='x';
                break;
            }
        }
    }
    
    //Found or not
    if(rr==4)return false;
    return true;
}

string set(){
    string code="0000";
    for(int i=0;i<code.length();i++){
        code[i]=rand()%10+'0';
    }
    return code;
}