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
    static int answer[4] = {-1, -1, -1, -1};
    static int wrong = -1;
    static int lastrr = 0;
    static int found = 0;
    string sGuess="0000";
    switch(state) {
        case 0:
        if(color != -1) {
            colorCount[color] = rr + rw;
            found += rr + rw;
        }
        if(rr+rw == 0) {
            wrong = color;
        }
        if(color >= 8 || found == 4) {
            if(found != 4) {
                colorCount[color+1] = 4 - found;
            }
            if(wrong == -1) {
                wrong = color + 1;
            }
            found = 4;
            state++;
            color = 0;
            while(color <= 9 && colorCount[color] == 0) {
                color++;
            }
        }
        break;

        case 1:
        if(position != -1) {
            if(rr > lastrr) {
                answer[position] = color;
                lastrr = rr;
                colorCount[color]--;
            }
        }
        if(position >= 3 || colorCount[color] <= 0) {
            position = -1;
            color++;
            while(color <= 9 && colorCount[color] == 0) {
                color++;
            }
            if(color > 9) {
                state++;
            }
        }
        break; 
        default:
        break;

    }
    switch(state) {
        case 0:
        color++;
        for(int i = 0; i < 4; i++) sGuess[i] = color + '0';
        break;

        case 1:
        do {
            position++;
        } while(answer[position] != -1);
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