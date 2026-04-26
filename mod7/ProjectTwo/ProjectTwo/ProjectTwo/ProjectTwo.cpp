// ProjectTwo.cpp
// CS 300 Project Two - ABCU Course Planner
//
// Matches key sample output phrases from "Project Two Sample Program Output":
// - "Welcome to the course planner."
// - "X is not a valid option."
// - "Here is a sample schedule:"
// - "Thank you for using the course planner!"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Course {
    string number;
    string title;
    vector<string> prerequisites;
};

// ---------- string helpers ----------
static string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

static string toUpper(string s) {
    transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return static_cast<char>(toupper(c)); });
    return s;
}

// Basic CSV split 
static vector<string> splitCSV(const string& line) {
    vector<string> fields;
    string token;
    stringstream ss(line);

    while (getline(ss, token, ',')) {
        fields.push_back(trim(token));
    }
    return fields;
}

static void printMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}


static bool loadCourses(const string& filename, map<string, Course>& courses) {
    ifstream file(filename);
    if (!file.is_open()) {
        // error simple
        cout << "Error: File could not be opened." << endl;
        return false;
    }

    courses.clear();

    string line;
    unsigned int lineNum = 0;

    while (getline(file, line)) {
        lineNum++;
        line = trim(line);
        if (line.empty()) continue;

        vector<string> fields = splitCSV(line);
        if (fields.size() < 2) {
           
            continue;
        }

        Course c;
        c.number = toUpper(fields[0]);
        c.title = fields[1];

        if (c.number.empty() || c.title.empty()) {
            continue;
        }

        // prerequisites
        for (size_t i = 2; i < fields.size(); ++i) {
            string pre = toUpper(trim(fields[i]));
            if (!pre.empty()) c.prerequisites.push_back(pre);
        }

        courses[c.number] = c;
    }

    return true;
}

static void printCourseList(const map<string, Course>& courses) {
    cout << "Here is a sample schedule:" << endl;
    for (const auto& kv : courses) {
        cout << kv.second.number << ", " << kv.second.title << endl;
    }
}

static void printCourseInfo(const map<string, Course>& courses) {
    cout << "What course do you want to know about? ";
    string input;
    getline(cin, input);

    string key = toUpper(trim(input));
    auto it = courses.find(key);

    if (it == courses.end()) {
        cout << "Course not found." << endl;
        return;
    }

    const Course& c = it->second;
    cout << c.number << ", " << c.title << endl;

    if (c.prerequisites.empty()) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";
    for (size_t i = 0; i < c.prerequisites.size(); ++i) {
        cout << c.prerequisites[i];
        if (i + 1 < c.prerequisites.size()) cout << ", ";
    }
    cout << endl;
}

// ---------- main menu ----------
int main() {
    cout << "Welcome to the course planner." << endl;

    map<string, Course> courses;
    bool loaded = false;

    int choice = 0;

    while (choice != 9) {
        printMenu();

        string input;
        getline(cin, input);
        input = trim(input);

        try {
            choice = stoi(input);
        }
        catch (...) {
            cout << input << " is not a valid option." << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "Enter file name: ";
            string filename;
            getline(cin, filename);

            loaded = loadCourses(trim(filename), courses);
            // If file fails, user can try option 1 again 
            break;
        }
        case 2:
            if (!loaded) {
                cout << "Enter file name: ";
                string filename;
                getline(cin, filename);

                loaded = loadCourses(trim(filename), courses);
                if (!loaded) {
                    // loadCourses already prints an error message
                    break;
                }
            }
            printCourseList(courses);
            break;

        case 3:
            if (!loaded) {
                cout << "Enter file name: ";
                string filename;
                getline(cin, filename);

                loaded = loadCourses(trim(filename), courses);
                if (!loaded) {
                    // loadCourses already prints an error message
                    break;
                }
            }
            printCourseInfo(courses);
            break;

        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

        default:
            cout << choice << " is not a valid option." << endl;
            break;
        }
    }

    return 0;
}