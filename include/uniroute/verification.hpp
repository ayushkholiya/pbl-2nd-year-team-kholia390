#ifndef UNIROUTE_VERIFICATION_HPP
#define UNIROUTE_VERIFICATION_HPP

#include "student_registry.hpp"
#include <iostream>
#include <string>

namespace uniroute {

using std::cout;
using std::endl;
using std::string;

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

} // namespace uniroute

#endif
