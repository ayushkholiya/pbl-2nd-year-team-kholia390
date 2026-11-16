#ifndef UNIROUTE_STUDENT_REGISTRY_HPP
#define UNIROUTE_STUDENT_REGISTRY_HPP

#include "common.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

namespace uniroute {

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

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

    StudentRegistry(const StudentRegistry&) = delete;
    StudentRegistry& operator=(const StudentRegistry&) = delete;

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
            s.id = std::atoi(p[0].c_str());
            s.name = p[1];
            s.registeredRoute = p[2];
            s.authorizedShift = p[3];
            s.passID = std::atoi(p[4].c_str());
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
inline void loadSampleData(StudentRegistry& registry)
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

} // namespace uniroute

#endif
