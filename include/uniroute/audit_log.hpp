#ifndef UNIROUTE_AUDIT_LOG_HPP
#define UNIROUTE_AUDIT_LOG_HPP

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

    LogStack(const LogStack&) = delete;
    LogStack& operator=(const LogStack&) = delete;

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

            addLog(std::atoi(p[0].c_str()), p[1], p[2], p[3] == "1", p[4]);
            loaded++;
        }

        return loaded;
    }

    void clear()
    {
        logs.clear();
    }
};

} // namespace uniroute

#endif
