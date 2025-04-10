#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

// ANSI escape codes for text formatting and color
const string RESET = "\033[0m";   // Reset color and formatting
const string CLEAR = "\033[2J";  // To clear the screen
const string BOLD = "\033[1m";   // Bold text
const string UNDERLINE = "\033[4m";  // Underlined text
const string ITALIC = "\033[3m";     // Italic text
const string RED = "\033[31m";    // Red text color
const string GREEN = "\033[32m"; // Green text color
const string BLUE = "\033[34m"; // Blue text color
const string CYAN = "\033[48;5;6m";    //Cyan background colour  
const string LIGHT_YELLOW = "\033[38;5;11m";  // Light yellow text color
const string LIGHT_BLUE = "\033[1;34m";    // Light Blue text color
const string LIGHT_GREEN = "\033[1;32m";     // Light Green text color



// Functions
void setColor(string color) {
    cout << color;
}

void resetText() {
    cout << RESET;
}
 
void clearScreen() {
    cout << CLEAR;
}

void setTextStyle(bool bold, bool underline, bool italic) {
    if (bold)
        cout << BOLD;
    if (underline) 
        cout << UNDERLINE;
    if (italic) 
        cout << ITALIC;
}

void moveTo(int x, int y) {
    cout << "\033[" << y << ";" << x << "H";  // Move cursor to (x, y)
}

void centerText(const string& text, int width = 100, int height = 24) {
    int x = (width - text.length()) / 2;    // Calculate the X position for center
    int y = height / 2;                     // Calculate the Y position for center
    moveTo(x, y);
    cout << text;
}

// Main function
int main() {
    // Set text style
    setColor(LIGHT_BLUE);
    setTextStyle(true, true, false);
    string text = "Welcome to Roadrunner";
    centerText(text);
   
    resetText();
    setColor(LIGHT_YELLOW);
    cout << "\nThis Program Helps you to find your needed rosources ";
    cout << "\nEnter to continue ";
    char ch = _getch(); 
    cout<< "You pressed:"<< ch <<endl;
    clearScreen();

    moveTo(2, 2);
    cout << "Enter the topic You want to study : ";
    string topic;
    cin >> topic;
    // string matching 

    cout << "Is the tpoic you intend to study is"; //topic name 
    cout << "1) Yes";
    cout << "2) No";
    int topic_selection;
    cin >> topic_selection;

    if (topic_selection == 1)
    { // function call 
    }
    else
    {//cout most matched topics 
    }
    
    // REQUIRED proficiency
   //  required durtion
    //


     resetText();
     return 0;
}