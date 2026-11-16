#ifndef UNIROUTE_ROUTE_SHIFT_HPP
#define UNIROUTE_ROUTE_SHIFT_HPP

#include <iostream>
#include <string>

namespace uniroute {

using std::cout;
using std::endl;
using std::string;

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

} // namespace uniroute

#endif
