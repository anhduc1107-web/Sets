#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// Create a Class to encapsulate the entire program
class SetManager {
private:
    vector<int> my_set;
    string filename;

    // Helper function: Check if a number already exists in the vector
    bool contains(int num) {
        for (size_t i = 0; i < my_set.size(); ++i) {
            if (my_set[i] == num) {
                return true; // Already exists
            }
        }
        return false; // Does not exist
    }

public:
    // Constructor
    SetManager(string f) {
        filename = f;
    }

    // Function to add an element (Only adds if it doesn't exist)
    bool addElement(int num) {
        if (!contains(num)) {
            my_set.push_back(num); // Add to the end of the vector
            return true;
        }
        return false;
    }

    // Function to remove an element
    bool removeElement(int num) {
        for (vector<int>::iterator it = my_set.begin(); it != my_set.end(); ++it) {
            if (*it == num) {
                my_set.erase(it); // Erase the element at the iterator's position
                return true;
            }
        }
        return false;
    }

    // Function to display the set
    void display() {
        if (my_set.empty()) {
            cout << "=> The Set is currently empty!" << endl;
            return;
        }
        cout << "=> Current numbers in the Set: { ";
        for (size_t i = 0; i < my_set.size(); ++i) {
            cout << my_set[i] << " ";
        }
        cout << "}" << endl;
    }

    // Function to save to file
    void saveToFile() {
        // C++98 requires .c_str() to convert std::string to const char*
        ofstream file(filename.c_str()); 
        if (file.is_open()) {
            for (size_t i = 0; i < my_set.size(); ++i) {
                file << my_set[i] << "\n";
            }
            file.close();
            cout << "=> Successfully saved all data to '" << filename << "'." << endl;
        } else {
            cout << "=> Error: Unable to create or open the file for writing." << endl;
        }
    }

    // Function containing the main menu loop, can be called from any main function
    void run() {
        int choice;
        int num;

        cout << "========================================\n";
        cout << "    SET MANAGEMENT PROGRAM (C++ OOP)\n";
        cout << "========================================\n";

        while (true) {
            cout << "\n--- MAIN MENU ---\n";
            cout << "1. Add a number to the Set\n";
            cout << "2. Remove a number from the Set\n";
            cout << "3. Display the Set\n";
            cout << "4. Save to file & Exit\n";
            cout << "Enter your choice (1-4): ";
            
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter the number to add: ";
                    cin >> num;
                    if (addElement(num)) {
                        cout << "=> Successfully added " << num << "." << endl;
                    } else {
                        cout << "=> Number " << num << " already exists. Not added." << endl;
                    }
                    break;
                case 2:
                    cout << "Enter the number to remove: ";
                    cin >> num;
                    if (removeElement(num)) {
                        cout << "=> Successfully removed " << num << "." << endl;
                    } else {
                        cout << "=> Number " << num << " is not in the Set." << endl;
                    }
                    break;
                case 3:
                    display();
                    break;
                case 4:
                    saveToFile();
                    cout << "=> Exiting the program. Goodbye!" << endl;
                    return; // Exit the run() function
                default:
                    cout << "=> Invalid choice! Please enter a number from 1 to 4." << endl;
                    break;
            }
        }
    }
};

// The main function is now completely clean and isolated
int main() {
    // Just instantiate the object and call the run() function
    SetManager myProgram("set_data.txt");
    myProgram.run();

    return 0;
}
