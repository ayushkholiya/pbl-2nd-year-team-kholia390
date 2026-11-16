#include "uniroute/audit_log.hpp"
#include "uniroute/route_shift.hpp"
#include "uniroute/student_registry.hpp"
#include "uniroute/verification.hpp"

#include <iostream>
#include <string>

using namespace std;
using namespace uniroute;

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
