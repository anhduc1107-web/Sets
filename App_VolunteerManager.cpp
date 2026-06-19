#include <iostream>
#include <string>
#include <unordered_map>
#include <limits>
#include <cctype>
#include <algorithm>
#include <fstream>  
#include <sstream>  
#include "CustomSet.h" // Integrating the custom generic data structure

using namespace std;

// Structure to hold detailed volunteer information
struct Volunteer {
    string name;
    string cohort;
    string major;
};

class VolunteerManager {
private:
    // Dictionary mapping Student ID -> Details
    unordered_map<string, Volunteer> volunteerDB; 
    
    // UNIVERSAL SET: Stores all Student IDs registered in the system
    CustomSet<string> totalVolunteers; 
    
    // EVENT MAP: Event Name -> CustomSet of participating Student IDs
    unordered_map<string, CustomSet<string>> events; 

    const string DB_FILE = "database.txt";

    // Helper: Standardize strings to uppercase
    string toUpperString(const string& str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(), ::toupper);
        return result;
    }

    // Helper: Validate if the Student ID is exactly 9 digits
    bool validateID(const string& studentID) {
        if (studentID.length() != 9) return false;
        for (char const &c : studentID) {
            if (!isdigit(c)) return false;
        }
        return true;
    }

    // Helper: Automatically calculate Cohort from Student ID
    string calculateCohort(const string& studentID) {
        int year = stoi(studentID.substr(0, 4));
        int cohortNum = year - 1956 + 1;
        return "K" + to_string(cohortNum);
    }

public:
    // Constructor: Auto-loads database when the program starts
    VolunteerManager() {
        loadDatabase();
    }

    //DATA PERSISTENCE METHODS
    // Reads data from the text file into the system memory
    void loadDatabase() {
        ifstream inFile(DB_FILE);
        if (!inFile) {
            cout << "[SYSTEM] No existing database found. Starting fresh.\n";
            return;
        }

        string line;
        int mode = 0; 

        while (getline(inFile, line)) {
            if (line == "===VOLUNTEERS===") { mode = 1; continue; }
            if (line == "===EVENTS===") { mode = 2; continue; }
            if (line.empty()) continue;

            if (mode == 1) {
                stringstream ss(line);
                string studentID, name, cohort, major;
                getline(ss, studentID, '|');
                getline(ss, name, '|');
                getline(ss, cohort, '|');
                getline(ss, major, '|');

                volunteerDB[studentID] = {name, cohort, major};
                totalVolunteers.insert(studentID); // Use Set to store ID
            } 
            else if (mode == 2) {
                stringstream ss(line);
                string eventName, studentID;
                getline(ss, eventName, '|');
                getline(ss, studentID, '|');
                
                events[eventName].insert(studentID); // Insert to Event Set
            }
        }
        inFile.close();
        cout << "[SYSTEM] Loaded data successfully from " << DB_FILE << ".\n";
    }

    // Saves current system memory data securely to the text file
    void saveDatabase() {
        ofstream outFile(DB_FILE);
        if (!outFile) return;

        outFile << "===VOLUNTEERS===\n";
        for (const auto& pair : volunteerDB) {
            const string& mssv = pair.first;
            const Volunteer& v = pair.second;
            outFile << mssv << "|" << v.name << "|" << v.cohort << "|" << v.major << "\n";
        }

        outFile << "===EVENTS===\n";
        for (const auto& eventPair : events) {
            const string& eventName = eventPair.first;
            // Cross-reference to find members of this event's Set safely
            for (const auto& volPair : volunteerDB) {
                const string& mssv = volPair.first;
                if (eventPair.second.search(mssv)) {
                    outFile << eventName << "|" << mssv << "\n";
                }
            }
        }
        outFile.close();
        cout << "\n[SYSTEM] All data securely saved to " << DB_FILE << ".\n";
    }

    // CORE OPERATIONS

    // 1. Add a new volunteer to the system
    bool addVolunteer(const string& studentID, const string& name, const string& major) {
        if (!validateID(studentID)) {
            cout << "\n[ERROR] Invalid format!\n";
            return false;
        }
        
        string autoCohort = calculateCohort(studentID);
        
        if (!totalVolunteers.insert(studentID)) {
            cout << "\n[ERROR] Student ID " << studentID << " already exists in the system!\n";
            return false;
        }
        
        volunteerDB[studentID] = {name, autoCohort, major};
        cout << "\n[SUCCESS] Added Volunteer: " << name << " (" << autoCohort << ").\n";
        return true;
    }

    // 2. Remove a volunteer completely
    bool removeVolunteer(const string& studentID) {
        if (!totalVolunteers.remove(studentID)) { 
            cout << "\n[ERROR] Student ID " << studentID << " not found in the universal set.\n";
            return false;
        }
        
        volunteerDB.erase(studentID); 

        for (auto& pair : events) {
            pair.second.remove(studentID);
        }
        cout << "\n[SUCCESS] Completely removed Volunteer (ID: " << studentID << ").\n";
        return true;
    }

    // 3. Register a volunteer for an event
    bool registerForEvent(string eventName, const string& studentID) {
        eventName = toUpperString(eventName);

        if (!totalVolunteers.search(studentID)) {
            cout << "\n[ERROR] Student ID " << studentID << " is not a registered team member.\n";
            return false;
        }

        if (!events[eventName].insert(studentID)) {
            cout << "\n[WARNING] Volunteer " << studentID << " is ALREADY registered for '" << eventName << "'.\n";
        } else {
            cout << "\n[SUCCESS] Registered ID " << studentID << " for event '" << eventName << "'.\n";
        }
        return true;
    }

    // 4. Withdraw a volunteer from an event
    bool withdrawFromEvent(string eventName, const string& studentID) {
        eventName = toUpperString(eventName);
        
        if (events.find(eventName) == events.end()) {
            cout << "\n[ERROR] Event '" << eventName << "' does not exist.\n";
            return false;
        }

        if (events[eventName].remove(studentID)) {
            cout << "\n[SUCCESS] Withdrew ID " << studentID << " from event '" << eventName << "'.\n";
            return true;
        } else {
            cout << "\n[ERROR] ID " << studentID << " is NOT in the participant list for '" << eventName << "'.\n";
            return false;
        }
    }

    // 5. Generate System Report 
    void generateSystemReport() {
        CustomSet<string> activeVolunteers;
        for (const auto& pair : events) {
            activeVolunteers = activeVolunteers.performUnion(pair.second); 
        }

        CustomSet<string> inactiveVolunteers = totalVolunteers.performDifference(activeVolunteers);

        cout << "\n=======================================================\n";
        cout << " VOLUNTEER TEAM ACTIVITY REPORT \n";
        cout << "=======================================================\n";
        
        cout << "1. Total registered members: " << totalVolunteers.size() << "\n";
        cout << "2. Active members : " << activeVolunteers.size() << "\n";
        cout << "3. Inactive members : " << inactiveVolunteers.size() << "\n";

        // --- ACTIVE SET DETAILS ---
        cout << "\n--- ACTIVE MEMBER DETAILS ---\n";
        if (activeVolunteers.isEmpty()) {
            cout << "No volunteers have attended any events yet.\n";
        } else {
            for (const auto& pair : volunteerDB) {
                const string& studentID = pair.first;
                
                if (activeVolunteers.search(studentID)) {
                    cout << " + " << studentID << " | " << pair.second.name << " (" << pair.second.cohort << ")\n";
                    cout << "   -> Events attended: ";
                    
                    bool firstEvent = true;
                    for (const auto& ev : events) {
                        if (ev.second.search(studentID)) {
                            if (!firstEvent) cout << ", ";
                            cout << ev.first;
                            firstEvent = false;
                        }
                    }
                    cout << "\n";
                }
            }
        }

        // --- INACTIVE SET DETAILS ---
        cout << "\n--- INACTIVE SET DETAILS ---\n";
        if (inactiveVolunteers.isEmpty()) {
            if (totalVolunteers.isEmpty()) cout << "The system is currently empty.\n";
            else cout << "Excellent! 100% of the team is actively participating.\n";
        } else {
            for (const auto& pair : volunteerDB) {
                if (inactiveVolunteers.search(pair.first)) {
                    cout << " - " << pair.first << " | " << pair.second.name << " (" << pair.second.cohort << ")\n";
                }
            }
        }
        cout << "=======================================================\n";
    }
};

void displayMenu() {
    cout << "\n*** VOLUNTEER MANAGEMENT SYSTEM ***\n";
    cout << "1. Add a new Volunteer\n";
    cout << "2. Remove a Volunteer\n";
    cout << "3. Register for an Event\n";
    cout << "4. Withdraw from an Event\n";
    cout << "5. Generate Activity Report\n";
    cout << "6. Save Database & Exit\n"; 
    cout << "Select an option (1-6): ";
}

int main() {
    VolunteerManager manager; 
    int choice;

    while (true) {
        displayMenu();
        if (!(cin >> choice)) { 
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "[ERROR] Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (choice == 6) {
            manager.saveDatabase(); //Save database before exit
            cout << "Exiting system. Goodbye!\n";
            break;
        }

        string studentID, name, major, eventName;

        switch (choice) {
            case 1:
                cout << "Enter Student ID (9 digits, e.g., 2025xxxxx): "; getline(cin, studentID);
                cout << "Enter Full Name: "; getline(cin, name);
                cout << "Enter Major: "; getline(cin, major);
                manager.addVolunteer(studentID, name, major);
                break;
            case 2:
                cout << "Enter Student ID to remove: "; getline(cin, studentID);
                manager.removeVolunteer(studentID);
                break;
            case 3:
                cout << "Enter Event Name: "; getline(cin, eventName);
                cout << "Enter Student ID to register: "; getline(cin, studentID);
                manager.registerForEvent(eventName, studentID);
                break;
            case 4:
                cout << "Enter Event Name: "; getline(cin, eventName);
                cout << "Enter Student ID to withdraw: "; getline(cin, studentID);
                manager.withdrawFromEvent(eventName, studentID);
                break;
            case 5:
                manager.generateSystemReport();
                break;
            default:
                cout << "[ERROR] Please select an option from 1 to 6.\n";
        }
    }

    return 0;
}
