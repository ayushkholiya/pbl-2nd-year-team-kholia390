// =====================================================================
//  UNIROUTE - SMART BUS VERIFICATION SYSTEM (Team CodeVerse, PBL Phase 3)
//  Language : C++
//  Concepts : Linked List, Arrays, Stack, OOP and File Handling
// =====================================================================
#include <iostream>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cctype>
using namespace std;

const string STUDENT_FILE = "students.txt";
const string LOG_FILE = "logs.txt";

bool inputClosed = false;                      // becomes true if the input ends

// ---------------------------------------------------------------------
// Small helper functions
// ---------------------------------------------------------------------
string toLower(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

// splits "a|b|c" into parts[]; true only if exactly n parts were found
bool splitLine(const string& line, string parts[], int n)
{
    stringstream ss(line);
    string item;
    int count = 0;

    while (getline(ss, item, '|'))
    {
        if (count < n)
            parts[count] = item;
        count++;
    }

    return count == n;
}

bool readInt(int& value)                        // reads a whole number safely
{
    if (!(cin >> value))
    {
        if (cin.eof())
            inputClosed = true;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return true;
}

bool readLine(string& text)                     // reads a full line (spaces allowed)
{
    if (!getline(cin, text))
    {
        inputClosed = true;
        return false;
    }

    return true;
}



struct StudentRecord
{
    int id;
    string name;
    string registeredRoute;
    string authorizedShift;
    int passID;
    bool feePaid;
    string passType;                           // "One-Way" or "Two-Way"
};

struct StudentNode
{
    StudentRecord data;
    StudentNode* next;

    StudentNode(StudentRecord s)
    {
        data = s;
        next = nullptr;
    }
};

class StudentRegistry
{
private:
    StudentNode* head;

    void swapData(StudentNode* a, StudentNode* b)
    {
        StudentRecord temp = a->data;
        a->data = b->data;
        b->data = temp;
    }

public:
    StudentRegistry()
    {
        head = nullptr;
    }

    bool addStudent(StudentRecord s)
    {
        if (findStudentByid(s.id) != nullptr ||
            findStudentByPassID(s.passID) != nullptr)
            return false;

        StudentNode* node = new StudentNode(s);

        if (head == nullptr)
        {
            head = node;
        }
        else
        {
            StudentNode* temp = head;

            while (temp->next != nullptr)
                temp = temp->next;

            temp->next = node;
        }

        return true;
    }

    StudentRecord* findStudentByid(int ID)
    {
        StudentNode* temp = head;

        while (temp != nullptr)
        {
            if (temp->data.id == ID)
                return &temp->data;

            temp = temp->next;
        }

        return nullptr;
    }

    StudentRecord* findStudentByPassID(int pid)
    {
        StudentNode* temp = head;

        while (temp != nullptr)
        {
            if (temp->data.passID == pid)
                return &temp->data;

            temp = temp->next;
        }

        return nullptr;
    }

    // DELETE: removes a node from the head, middle or end of the list
    bool deleteStudent(int id)
    {
        StudentNode* current = head;
        StudentNode* previous = nullptr;

        while (current != nullptr && current->data.id != id)
        {
            previous = current;
            current = current->next;
        }

        if (current == nullptr)
            return false;

        if (previous == nullptr)
            head = current->next;              // deleting the first node
        else
            previous->next = current->next;

        delete current;
        return true;
    }

    // UPDATE: changes route, shift, fee status and pass type of a student
    bool updateStudent(int id, const string& route, const string& shift,
                       bool fee, const string& type)
    {
        StudentRecord* s = findStudentByid(id);

        if (s == nullptr)
            return false;

        s->registeredRoute = route;
        s->authorizedShift = shift;
        s->feePaid = fee;
        s->passType = type;
        return true;
    }

    // SORT: bubble sort on the linked list (swaps the data of neighbours)
    void sortById()
    {
        if (head == nullptr)
            return;

        bool swapped;

        do
        {
            swapped = false;

            for (StudentNode* t = head; t->next != nullptr; t = t->next)
            {
                if (t->data.id > t->next->data.id)
                {
                    swapData(t, t->next);
                    swapped = true;
                }
            }
        } while (swapped);
    }

    void sortByName()
    {
        if (head == nullptr)
            return;

        bool swapped;

        do
        {
            swapped = false;

            for (StudentNode* t = head; t->next != nullptr; t = t->next)
            {
                if (toLower(t->data.name) > toLower(t->next->data.name))
                {
                    swapData(t, t->next);
                    swapped = true;
                }
            }
        } while (swapped);
    }

    // SEARCH BY NAME: part of the name is enough, capital/small letters ignored
    int searchByName(const string& part)
    {
        string key = toLower(part);
        int found = 0;

        for (StudentNode* t = head; t != nullptr; t = t->next)
        {
            if (toLower(t->data.name).find(key) != string::npos)
            {
                cout << "ID " << t->data.id
                     << " | " << t->data.name
                     << " | " << t->data.registeredRoute
                     << " | " << t->data.authorizedShift
                     << " | Pass " << t->data.passID
                     << " (" << t->data.passType << ")"
                     << " | " << (t->data.feePaid ? "Fee paid" : "Fee unpaid")
                     << endl;
                found++;
            }
        }

        return found;
    }

    int count() const
    {
        int c = 0;

        for (StudentNode* t = head; t != nullptr; t = t->next)
            c++;

        return c;
    }

    void displayStudents()
    {
        if (head == nullptr)
        {
            cout << "No student records available.\n";
            return;
        }

        StudentNode* temp = head;

        while (temp != nullptr)
        {
            cout << "\nStudent ID: " << temp->data.id
                 << "\nName: " << temp->data.name
                 << "\nRegistered Route: " << temp->data.registeredRoute
                 << "\nAuthorized Shift: " << temp->data.authorizedShift
                 << "\nPass ID: " << temp->data.passID
                 << "\nPass Type: " << temp->data.passType
                 << "\nFee: " << (temp->data.feePaid ? "Paid" : "Unpaid")
                 << endl;

            temp = temp->next;
        }
    }

    // FILE HANDLING: one student per line, fields separated by '|'
    bool saveToFile(const string& filename)
    {
        ofstream out(filename.c_str());

        if (!out)
            return false;

        for (StudentNode* t = head; t != nullptr; t = t->next)
        {
            out << t->data.id << '|' << t->data.name << '|'
                << t->data.registeredRoute << '|' << t->data.authorizedShift << '|'
                << t->data.passID << '|' << (t->data.feePaid ? 1 : 0) << '|'
                << t->data.passType << '\n';
        }

        return true;
    }

    int loadFromFile(const string& filename)
    {
        ifstream in(filename.c_str());

        if (!in)
            return 0;

        string line;
        int loaded = 0;

        while (getline(in, line))
        {
            string p[7];

            if (!splitLine(line, p, 7))
                continue;

            StudentRecord s;
            s.id = atoi(p[0].c_str());
            s.name = p[1];
            s.registeredRoute = p[2];
            s.authorizedShift = p[3];
            s.passID = atoi(p[4].c_str());
            s.feePaid = (p[5] == "1");
            s.passType = p[6];

            if (s.id > 0 && s.passID > 0 && addStudent(s))
                loaded++;
        }

        return loaded;
    }

    void clear()
    {
        while (head != nullptr)
        {
            StudentNode* temp = head;
            head = head->next;
            delete temp;
        }
    }

    ~StudentRegistry()
    {
        clear();
    }
};

// pre-loaded sample database (used when no saved file exists)
void loadSampleData(StudentRegistry& registry)
{
    StudentRecord sample[10] = {
        {101, "Amit Sharma",  "Prem Nagar",    "Morning", 5001, true,  "One-Way"},
        {102, "Rahul Verma",  "ISBT",          "Evening", 5002, false, "Two-Way"},
        {103, "Priya Negi",   "Prem Nagar",    "Morning", 5003, true,  "Two-Way"},
        {104, "Karan Singh",  "Airport",       "Morning", 5004, true,  "One-Way"},
        {105, "Sneha Rawat",  "Clock Tower",   "Evening", 5005, true,  "Two-Way"},
        {106, "Vikas Joshi",  "Hillen",        "Morning", 5006, false, "One-Way"},
        {107, "Anjali Bisht", "Rajpur Road",   "Evening", 5007, true,  "Two-Way"},
        {108, "Mohit Thapa",  "Dalanwala",     "Morning", 5008, true,  "One-Way"},
        {109, "Neha Panwar",  "Raipur",        "Evening", 5009, true,  "Two-Way"},
        {110, "Aman Gupta",   "Sahastradhara", "Morning", 5010, true,  "One-Way"}
    };

    for (int i = 0; i < 10; i++)
        registry.addStudent(sample[i]);
}



class RouteShift
{
private:
    string routes[12] = {
        "Hillen", "Airport", "Prem Nagar", "ISBT",
        "Rajpur Road", "Clock Tower", "Dalanwala",
        "Raipur", "Sahastradhara", "Jakhan",
        "Clement Town", "Mussoorie Road"
    };

    string shifts[2] = {"Morning", "Evening"};

    int routeIndex;                            // current bus (-1 = not selected)
    int shiftIndex;

public:
    RouteShift()
    {
        routeIndex = -1;
        shiftIndex = -1;

    }

    int routeCount() const { return 12; }

    string getRouteName(int i) const { return routes[i]; }

    string getShiftName(int i) const { return shifts[i]; }

    void displayRoutes()
    {
        cout << "\n--- AVAILABLE ROUTES ---\n";

        for (int i = 0; i < 12; i++)
            cout << i + 1 << ". " << routes[i] << endl;
    }

    void setCurrentRoute(int choice)
    {
        if (choice >= 1 && choice <= 12)
        {
            routeIndex = choice - 1;
            cout << "Route selected successfully.\n";
        }
        else
        {
            routeIndex = -1;
            cout << "Invalid route choice!\n";
        }
    }

    void setCurrentShift(int choice)
    {
        if (choice >= 1 && choice <= 2)
        {
            shiftIndex = choice - 1;
            cout << "Shift selected successfully.\n";
        }
        else
        {
            shiftIndex = -1;
            cout << "Invalid shift choice!\n";
        }
    }

    string getCurrentRoute() const
    {
        return routeIndex >= 0 ? routes[routeIndex] : "";
    }

    string getCurrentShift() const
    {
        return shiftIndex >= 0 ? shifts[shiftIndex] : "";
    }

    bool hasValidSelection() const
    {
        return routeIndex >= 0 && shiftIndex >= 0;
    }

};

struct TripEntry                               // one approved trip of today
{
    int id;
    string shift;
};

class Verification
{
private:
    static const int MAX_TRIPS = 100;

    string lastReason;

    TripEntry trips[MAX_TRIPS];                // ARRAY of today's approved trips
    int tripCount;

    int totalAttempts;
    int grantedCount;
    int deniedCount;

    int tripsToday(int id) const
    {
        int count = 0;

        for (int i = 0; i < tripCount; i++)
        {
            if (trips[i].id == id)
                count++;
        }

        return count;
    }

    bool deny(const string& reason)
    {
        lastReason = reason;
        cout << reason << "\nACCESS DENIED\n";
        deniedCount++;
        return false;
    }

public:
    Verification()
    {
        lastReason = "";
        tripCount = 0;
        totalAttempts = 0;
        grantedCount = 0;
        deniedCount = 0;
    }

    // Version 1: the student record is already found (by Student ID or Pass ID)
    bool verifyStudent(
        const StudentRecord* student,
        bool routeShiftSelected,
        const string& currentRoute,
        const string& currentShift)
    {
        totalAttempts++;

        // STEP 0: find the student (the fee is stored in the record)
        if (student == nullptr)
            return deny("Student record not found.");

        // STEP 1: FEE CHECK
        if (!student->feePaid)
            return deny("Fee not paid. Pass is not valid.");

        // STEP 2: are live route and shift available?
        if (!routeShiftSelected)
            return deny("Route or shift not selected.");

        // STEP 3: route check
        if (student->registeredRoute != currentRoute)
            return deny("Route mismatch. Registered route is "
                        + student->registeredRoute + ".");

        // STEP 4: shift check
        if (student->authorizedShift != currentShift)
            return deny("Shift mismatch. Authorized shift is "
                        + student->authorizedShift + ".");

        // STEP 5: pass type  ->  One-Way = 1 trip per day, Two-Way = 2 trips
        int allowedTrips;

        if (student->passType == "One-Way")
            allowedTrips = 1;
        else if (student->passType == "Two-Way")
            allowedTrips = 2;
        else
            return deny("Invalid pass type.");

        // STEP 6: daily trip limit
        if (tripsToday(student->id) >= allowedTrips)
            return deny(student->passType + " pass: daily trip limit reached.");

        if (tripCount >= MAX_TRIPS)
            return deny("Daily trip record is full.");

        // all checks passed: record the trip
        trips[tripCount].id = student->id;
        trips[tripCount].shift = currentShift;
        tripCount++;

        lastReason = "Identity, fee, route, shift and pass rules matched.";

        cout << "Student record found: " << student->name << endl;
        cout << "Fee paid. Route matched. Shift matched.\n";
        cout << "Pass type: " << student->passType
             << " (trip " << tripsToday(student->id)
             << " of " << allowedTrips << " today)\n";
        cout << "ACCESS GRANTED\n";

        grantedCount++;
        return true;
    }

    // Version 2 (function overloading): the typed ID must match the record
    bool verifyStudent(
        int requestedID,
        const StudentRecord* student,
        bool routeShiftSelected,
        const string& currentRoute,
        const string& currentShift)
    {
        if (student != nullptr && student->id != requestedID)
            student = nullptr;

        return verifyStudent(student, routeShiftSelected,
                             currentRoute, currentShift);
    }

    string getLastReason() const
    {
        return lastReason;
    }

    void showStatistics() const
    {
        cout << "\n--- VERIFICATION STATISTICS ---\n";
        cout << "Total attempts : " << totalAttempts << endl;
        cout << "Granted        : " << grantedCount << endl;
        cout << "Denied         : " << deniedCount << endl;
        cout << "Trips today    : " << tripCount << endl;

        if (totalAttempts > 0)
            cout << "Success rate   : "
                 << (grantedCount * 100.0 / totalAttempts) << "%\n";
    }

    void resetDailyTrips()
    {
        tripCount = 0;
        cout << "New day started. Daily trip records cleared.\n";
    }
};



struct VerificationLog
{
    int id;
    string route;
    string shift;
    bool granted;
    string reason;
};

struct LogNode
{
    VerificationLog data;
    LogNode* next;

    LogNode(VerificationLog l)
    {
        data = l;
        next = nullptr;
    }
};

// STACK (Last In First Out): push, pop, peek, isEmpty, size
class LogStack
{
private:
    LogNode* top;
    int itemCount;

    // writes the oldest record first, so loading it back keeps the order
    void writeBottomUp(LogNode* node, ofstream& out) const
    {
        if (node == nullptr)
            return;

        writeBottomUp(node->next, out);

        out << node->data.id << '|' << node->data.route << '|'
            << node->data.shift << '|' << (node->data.granted ? 1 : 0) << '|'
            << node->data.reason << '\n';
    }

public:
    LogStack()
    {
        top = nullptr;
        itemCount = 0;
    }

    void push(VerificationLog log)
    {
        LogNode* node = new LogNode(log);
        node->next = top;                      // new node goes on top
        top = node;
        itemCount++;
    }

    bool pop(VerificationLog& out)
    {
        if (top == nullptr)
            return false;

        LogNode* temp = top;
        out = temp->data;
        top = top->next;
        delete temp;
        itemCount--;
        return true;
    }

    bool peek(VerificationLog& out) const
    {
        if (top == nullptr)
            return false;

        out = top->data;
        return true;
    }

    bool isEmpty() const { return top == nullptr; }

    int size() const { return itemCount; }

    const LogNode* getTop() const { return top; }

    bool saveToFile(const string& filename) const
    {
        ofstream out(filename.c_str());

        if (!out)
            return false;

        writeBottomUp(top, out);
        return true;
    }

    void clear()
    {
        VerificationLog temp;

        while (pop(temp))
        {
        }
    }

    ~LogStack()
    {
        clear();
    }
};

class AuditLog
{
private:
    LogStack logs;

    static void printLog(const VerificationLog& log)
    {
        cout << "\nStudent id: " << log.id
             << "\nRoute: " << log.route
             << "\nShift: " << log.shift
             << "\nResult: " << (log.granted ? "GRANTED" : "DENIED")
             << "\nReason: " << log.reason
             << "\n-------------------------\n";
    }

public:
    void addLog(int ID, string route, string shift, bool granted, string reason)
    {
        VerificationLog log;

        log.id = ID;
        log.route = route;
        log.shift = shift;
        log.granted = granted;
        log.reason = reason;

        logs.push(log);
    }

    // latest first, because the top of the stack is the newest record
    void displayLatestLogs()
    {
        if (logs.isEmpty())
        {
            cout << "No verification logs available.\n";
            return;
        }

        cout << "\n--- VERIFICATION HISTORY (latest first) ---\n";

        for (const LogNode* n = logs.getTop(); n != nullptr; n = n->next)
            printLog(n->data);
    }

    // history of one student only
    void displayLogsForStudent(int id)
    {
        int found = 0;

        for (const LogNode* n = logs.getTop(); n != nullptr; n = n->next)
        {
            if (n->data.id == id)
            {
                if (found == 0)
                    cout << "\n--- HISTORY OF STUDENT " << id << " ---\n";

                printLog(n->data);
                found++;
            }
        }

        if (found == 0)
            cout << "No history found for this student.\n";
    }

    int count() const { return logs.size(); }

    bool saveToFile(const string& filename) const
    {
        return logs.saveToFile(filename);
    }

    int loadFromFile(const string& filename)
    {
        ifstream in(filename.c_str());

        if (!in)
            return 0;

        string line;
        int loaded = 0;

        while (getline(in, line))
        {
            string p[5];

            if (!splitLine(line, p, 5))
                continue;

            addLog(atoi(p[0].c_str()), p[1], p[2], p[3] == "1", p[4]);
            loaded++;
        }

        return loaded;
    }

    void clear()
    {
        logs.clear();
    }
};


// =====================================================================
// MAIN PROGRAM
// =====================================================================

// shared flow used by verification by Student ID and Pass ID
void processVerification(int requestedID, StudentRecord* student, bool byID,
                         RouteShift& routeManager, Verification& verifier,
                         AuditLog& audit)
{
    bool selected = routeManager.hasValidSelection();
    string route = routeManager.getCurrentRoute();
    string shift = routeManager.getCurrentShift();
    bool result;

    if (byID)
        result = verifier.verifyStudent(requestedID, student, selected,
                                        route, shift);
    else
        result = verifier.verifyStudent(student, selected, route, shift);

    int logID = (student != nullptr) ? student->id : requestedID;

    audit.addLog(logID, route, shift, result, verifier.getLastReason());
}

void saveAllData(StudentRegistry& registry, AuditLog& audit)
{
    bool a = registry.saveToFile(STUDENT_FILE);
    bool b = audit.saveToFile(LOG_FILE);

    if (a && b)
        cout << "Data saved to " << STUDENT_FILE << " and " << LOG_FILE << ".\n";
    else
        cout << "Could not save data to file.\n";
}

void printMenu()
{
    cout << "\n===== UNIROUTE SMART BUS VERIFICATION SYSTEM =====\n";
    cout << " 1. Register Student\n";
    cout << " 2. Display Students\n";
    cout << " 3. Display Routes\n";
    cout << " 4. Select Route\n";
    cout << " 5. Select Shift\n";
    cout << " 6. Verify Student by Student ID\n";
    cout << " 7. View Verification Logs\n";
    cout << " 8. Verify Student by Pass ID\n";
    cout << " 9. Verification Statistics\n";
    cout << "10. Start New Day (reset daily trips)\n";
    cout << "11. Delete Student\n";
    cout << "12. Update Student\n";
    cout << "13. Sort Students (by ID or Name)\n";
    cout << "14. Search Student by Name\n";
    cout << "15. View History of One Student\n";
    cout << "16. Save Data to Files\n";
    cout << "17. Reset to Sample Database\n";
    cout << " 0. Exit (data is saved automatically)\n";
    cout << "Enter choice: ";
}

int main()
{
    StudentRegistry registry;
    RouteShift routeManager;
    Verification verifier;
    AuditLog audit;

    // ---- start-up: load saved files, or the sample database ----
    int loadedStudents = registry.loadFromFile(STUDENT_FILE);
    int loadedLogs = audit.loadFromFile(LOG_FILE);

    if (loadedStudents == 0)
    {
        loadSampleData(registry);
        cout << "10 sample students loaded into the database.\n";
    }
    else
    {
        cout << loadedStudents << " students and " << loadedLogs
             << " history records loaded from file.\n";
    }

    int choice = -1;

    do
    {
        printMenu();

        if (!readInt(choice))
        {
            if (inputClosed)
                break;

            cout << "Invalid input. Enter a number.\n";
            choice = -1;
            continue;
        }

        switch (choice)
        {
            case 1:                                   // register student
            {
                StudentRecord s;
                int routeNo, shiftNo, feeChoice, typeChoice;

                cout << "Enter Student ID: ";
                if (!readInt(s.id) || s.id <= 0)
                {
                    cout << "Invalid Student ID.\n";
                    break;
                }

                cout << "Enter Student Name: ";
                readLine(s.name);

                if (s.name.empty() || s.name.find('|') != string::npos)
                {
                    cout << "Invalid name.\n";
                    break;
                }

                routeManager.displayRoutes();
                cout << "Enter route number: ";
                if (!readInt(routeNo) || routeNo < 1 || routeNo > routeManager.routeCount())
                {
                    cout << "Invalid route number.\n";
                    break;
                }

                cout << "Shift (1 = Morning, 2 = Evening): ";
                if (!readInt(shiftNo) || shiftNo < 1 || shiftNo > 2)
                {
                    cout << "Invalid shift.\n";
                    break;
                }

                cout << "Enter Pass ID: ";
                if (!readInt(s.passID) || s.passID <= 0)
                {
                    cout << "Invalid Pass ID.\n";
                    break;
                }

                cout << "Fee paid? (1 = Yes, 0 = No): ";
                if (!readInt(feeChoice) || (feeChoice != 0 && feeChoice != 1))
                {
                    cout << "Invalid fee choice.\n";
                    break;
                }

                cout << "Pass type (1 = One-Way, 2 = Two-Way): ";
                if (!readInt(typeChoice) || (typeChoice != 1 && typeChoice != 2))
                {
                    cout << "Invalid pass type.\n";
                    break;
                }

                s.registeredRoute = routeManager.getRouteName(routeNo - 1);
                s.authorizedShift = routeManager.getShiftName(shiftNo - 1);
                s.feePaid = (feeChoice == 1);
                s.passType = (typeChoice == 1) ? "One-Way" : "Two-Way";

                if (registry.addStudent(s))
                    cout << "Student registered successfully.\n";
                else
                    cout << "Student ID or Pass ID already exists.\n";

                break;
            }

            case 2:
                registry.displayStudents();
                break;

            case 3:
                routeManager.displayRoutes();
                break;

            case 4:
            {
                routeManager.displayRoutes();

                int routeChoice;
                cout << "Enter route number: ";

                if (readInt(routeChoice))
                    routeManager.setCurrentRoute(routeChoice);
                else
                    cout << "Invalid input.\n";

                break;
            }

            case 5:
            {
                int shiftChoice;

                cout << "1. Morning\n2. Evening\n";
                cout << "Enter shift number: ";

                if (readInt(shiftChoice))
                    routeManager.setCurrentShift(shiftChoice);
                else
                    cout << "Invalid input.\n";

                break;
            }

            case 6:                                   // verify by Student ID
            {
                int id;

                cout << "Enter Student ID to verify: ";

                if (!readInt(id))
                {
                    cout << "Invalid Student ID.\n";
                    break;
                }

                StudentRecord* student = registry.findStudentByid(id);
                processVerification(id, student, true, routeManager, verifier, audit);
                break;
            }

            case 7:
                audit.displayLatestLogs();
                break;

            case 8:                                   // verify by Pass ID
            {
                int pid;

                cout << "Enter Pass ID to verify: ";

                if (!readInt(pid))
                {
                    cout << "Invalid Pass ID.\n";
                    break;
                }

                StudentRecord* student = registry.findStudentByPassID(pid);
                processVerification(-1, student, false, routeManager, verifier, audit);
                break;
            }

            case 9:
                verifier.showStatistics();
                break;

            case 10:
                verifier.resetDailyTrips();
                break;

            case 11:                                  // delete student
            {
                int id;

                cout << "Enter Student ID to delete: ";

                if (!readInt(id))
                {
                    cout << "Invalid Student ID.\n";
                    break;
                }

                if (registry.deleteStudent(id))
                    cout << "Student deleted successfully.\n";
                else
                    cout << "Student not found.\n";

                break;
            }

            case 12:                                  // update student
            {
                int id, routeNo, shiftNo, feeChoice, typeChoice;

                cout << "Enter Student ID to update: ";

                if (!readInt(id))
                {
                    cout << "Invalid Student ID.\n";
                    break;
                }

                if (registry.findStudentByid(id) == nullptr)
                {
                    cout << "Student not found.\n";
                    break;
                }

                routeManager.displayRoutes();
                cout << "New route number: ";
                if (!readInt(routeNo) || routeNo < 1 || routeNo > routeManager.routeCount())
                {
                    cout << "Invalid route number.\n";
                    break;
                }

                cout << "New shift (1 = Morning, 2 = Evening): ";
                if (!readInt(shiftNo) || shiftNo < 1 || shiftNo > 2)
                {
                    cout << "Invalid shift.\n";
                    break;
                }

                cout << "Fee paid? (1 = Yes, 0 = No): ";
                if (!readInt(feeChoice) || (feeChoice != 0 && feeChoice != 1))
                {
                    cout << "Invalid fee choice.\n";
                    break;
                }

                cout << "Pass type (1 = One-Way, 2 = Two-Way): ";
                if (!readInt(typeChoice) || (typeChoice != 1 && typeChoice != 2))
                {
                    cout << "Invalid pass type.\n";
                    break;
                }

                registry.updateStudent(id,
                                       routeManager.getRouteName(routeNo - 1),
                                       routeManager.getShiftName(shiftNo - 1),
                                       feeChoice == 1,
                                       typeChoice == 1 ? "One-Way" : "Two-Way");
                cout << "Student updated successfully.\n";
                break;
            }

            case 13:                                  // sort students
            {
                int how;

                cout << "Sort by: 1 = Student ID, 2 = Name: ";

                if (!readInt(how) || (how != 1 && how != 2))
                {
                    cout << "Invalid choice.\n";
                    break;
                }

                if (how == 1)
                    registry.sortById();
                else
                    registry.sortByName();

                cout << "Students sorted.\n";
                registry.displayStudents();
                break;
            }

            case 14:                                  // search by name
            {
                string part;

                cout << "Enter name (or part of it): ";
                readLine(part);

                if (part.empty())
                {
                    cout << "Please type something to search.\n";
                    break;
                }

                if (registry.searchByName(part) == 0)
                    cout << "No student found with this name.\n";

                break;
            }

            case 15:
            {
                int id;

                cout << "Enter Student ID: ";

                if (readInt(id))
                    audit.displayLogsForStudent(id);
                else
                    cout << "Invalid Student ID.\n";

                break;
            }

            case 16:
                saveAllData(registry, audit);
                break;

            case 17:                                  // back to the sample data
                registry.clear();
                audit.clear();
                verifier.resetDailyTrips();
                loadSampleData(registry);
                cout << "Sample database restored (10 students, empty history).\n";
                break;

            case 0:
                saveAllData(registry, audit);
                cout << "Exiting UNIROUTE.\n";
                break;

            default:
                cout << "Invalid menu choice!\n";
        }

    } while (choice != 0 && !inputClosed);

    return 0;
}
