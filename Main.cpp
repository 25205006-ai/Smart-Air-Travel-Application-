#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept> 

using namespace std;

// ==========================================
// === TEMPLATES ===
// ==========================================
template <typename T>
void printHeader(T title) {
    cout << "\n========================================\n";
    cout << "  " << title << "  \n";
    cout << "========================================\n";
}

template <class T>
class Formatter {
private:
    T value;
public:
    Formatter(T val) : value(val) {}
    void displayFormatted() { cout << "[" << value << "]"; }
};

// ==========================================
// === FLIGHT CLASSES (Inheritance/Polymorphism) ===
// ==========================================
class Flight {
protected: 
    string flightID;
    string origin;
    string destination;
    int availableSeats;
    double price;

public:
    Flight(string id, string orig, string dest, int seats, double pr)
        : flightID(id), origin(orig), destination(dest), availableSeats(seats), price(pr) {}
    virtual ~Flight() {}

    virtual void displayDetails() const {
        cout << left << setw(10) << flightID 
             << setw(15) << origin 
             << setw(15) << destination 
             << setw(10) << availableSeats 
             << "$" << setw(9) << price;
    }

    virtual string getFlightType() const = 0; 
    virtual string getSaveData() const {
        return flightID + "," + origin + "," + destination + "," + to_string(availableSeats) + "," + to_string(price);
    }

    string getID() const { return flightID; }
    string getOrigin() const { return origin; }
    string getDestination() const { return destination; }
    int getSeats() const { return availableSeats; }
    double getPrice() const { return price; }
    void updateSeats(int change) { availableSeats += change; }
};

class DomesticFlight : public Flight {
private:
    double domesticTax;
public:
    DomesticFlight(string id, string orig, string dest, int seats, double pr, double tax = 15.0)
        : Flight(id, orig, dest, seats, pr), domesticTax(tax) {}

    void displayDetails() const override {
        Flight::displayDetails();
        cout << " [Domestic - Tax: $" << domesticTax << "]" << endl;
    }

    string getFlightType() const override { return "Domestic"; }
    string getSaveData() const override {
        return "D," + Flight::getSaveData() + "," + to_string(domesticTax);
    }
};

class InternationalFlight : public Flight {
private:
    string passportRequired;
public:
    InternationalFlight(string id, string orig, string dest, int seats, double pr, string passport = "Yes")
        : Flight(id, orig, dest, seats, pr), passportRequired(passport) {}

    void displayDetails() const override {
        Flight::displayDetails();
        cout << " [International - Passport: " << passportRequired << "]" << endl;
    }

    string getFlightType() const override { return "International"; }
    string getSaveData() const override {
        return "I," + Flight::getSaveData() + "," + passportRequired;
    }
};

// ==========================================
// === USER CLASSES ===
// ==========================================
class User {
protected:
    string username;
    string password;
public:
    User(string u, string p) : username(u), password(p) {}
    virtual ~User() {}
    string getUsername() const { return username; }
    bool validateCredentials(string u, string p) { return (username == u && password == p); }
};

class Passenger : public User {
private:
    string name;
    int age;
    string dob;
public:
    Passenger(string u, string p, string n, int a, string d)
        : User(u, p), name(n), age(a), dob(d) {}

    string getSaveData() const {
        return username + "," + password + "," + name + "," + to_string(age) + "," + dob;
    }
    void displayPassenger() const {
        cout << "Name: " << name << " | Age: " << age << " | DOB: " << dob << endl;
    }
};

class Admin : public User {
public:
    Admin(string u, string p) : User(u, p) {}
};

// ==========================================
// === BOOKING CLASS ===
// ==========================================
class Booking {
private:
    string bookingID, passengerUser, flightID, status; 
public:
    Booking(string bID, string pUser, string fID, string stat)
        : bookingID(bID), passengerUser(pUser), flightID(fID), status(stat) {}

    string getSaveData() const { return bookingID + "," + passengerUser + "," + flightID + "," + status; }
    void cancelBooking() { status = "Cancelled"; }
    string getStatus() const { return status; }
    string getFlightID() const { return flightID; }
    string getPassengerUser() const { return passengerUser; }
    
    void displayBooking() const {
        cout << "Booking ID: " << bookingID << " | User: " << passengerUser 
             << " | Flight ID: " << flightID << " | Status: " << status << endl;
    }

    friend void printOfficialInvoice(const Booking& b);
};

void printOfficialInvoice(const Booking& b) {
    cout << "\n--- OFFICIAL AIRLINE INVOICE ---\n";
    cout << "TICKET REF: #" << b.bookingID << " | ISSUED TO: " << b.passengerUser << " | FLIGHT: " << b.flightID << endl;
    cout << "--------------------------------\n";
}

// ==========================================
// === SYSTEM LOGIC ===
// ==========================================
class SmartBookingSystem {
private:
    vector<Flight*> flights;
    vector<Passenger> passengers;
    vector<Booking> bookings;
    Admin* admin;
    Passenger* loggedInPassenger;
    int nextBookingID;

public:
    SmartBookingSystem() {
        admin = new Admin("admin", "admin123"); 
        loggedInPassenger = nullptr;
        nextBookingID = 1000;
        
        loadFiles();

        // -------------------------------------------------------------------
        // AUTO-GENERATE 50 REALISTIC FLIGHTS IF FILE IS EMPTY
        // Airlines: AI (Air India), 6E (IndiGo), UK (Vistara), SG (SpiceJet)
        // -------------------------------------------------------------------
        if (flights.empty()) {
            // --- 25 DOMESTIC FLIGHTS ---
            flights.push_back(new DomesticFlight("AI-101", "Delhi", "Mumbai", 180, 120.50));
            flights.push_back(new DomesticFlight("AI-102", "Mumbai", "Delhi", 180, 115.00));
            flights.push_back(new DomesticFlight("AI-205", "Delhi", "Chennai", 150, 135.00));
            flights.push_back(new DomesticFlight("AI-310", "Bangalore", "Kolkata", 160, 110.00));
            flights.push_back(new DomesticFlight("AI-404", "Hyderabad", "Pune", 120, 85.00));
            flights.push_back(new DomesticFlight("6E-501", "Chennai", "Mumbai", 180, 105.00));
            flights.push_back(new DomesticFlight("6E-502", "Mumbai", "Bangalore", 180, 95.00));
            flights.push_back(new DomesticFlight("6E-603", "Delhi", "Goa", 200, 150.00));
            flights.push_back(new DomesticFlight("6E-604", "Goa", "Delhi", 200, 145.00));
            flights.push_back(new DomesticFlight("6E-711", "Kolkata", "Delhi", 180, 130.00));
            flights.push_back(new DomesticFlight("UK-801", "Delhi", "Pune", 150, 140.00));
            flights.push_back(new DomesticFlight("UK-802", "Pune", "Delhi", 150, 135.00));
            flights.push_back(new DomesticFlight("UK-855", "Bangalore", "Mumbai", 160, 115.00));
            flights.push_back(new DomesticFlight("UK-877", "Chennai", "Hyderabad", 140, 80.00));
            flights.push_back(new DomesticFlight("UK-912", "Mumbai", "Ahmedabad", 120, 65.00));
            flights.push_back(new DomesticFlight("SG-111", "Delhi", "Jaipur", 90, 50.00));
            flights.push_back(new DomesticFlight("SG-112", "Jaipur", "Delhi", 90, 45.00));
            flights.push_back(new DomesticFlight("SG-222", "Hyderabad", "Chennai", 180, 75.00));
            flights.push_back(new DomesticFlight("SG-333", "Ahmedabad", "Bangalore", 180, 125.00));
            flights.push_back(new DomesticFlight("SG-444", "Kolkata", "Guwahati", 120, 85.00));
            flights.push_back(new DomesticFlight("AI-150", "Delhi", "Lucknow", 140, 60.00));
            flights.push_back(new DomesticFlight("6E-250", "Mumbai", "Kochi", 180, 130.00));
            flights.push_back(new DomesticFlight("UK-350", "Bangalore", "Goa", 150, 95.00));
            flights.push_back(new DomesticFlight("SG-450", "Chennai", "Madurai", 90, 55.00));
            flights.push_back(new DomesticFlight("AI-550", "Delhi", "Srinagar", 160, 145.00));

            // --- 25 INTERNATIONAL FLIGHTS ---
            flights.push_back(new InternationalFlight("AI-901", "Delhi", "NewYork", 350, 1200.00));
            flights.push_back(new InternationalFlight("AI-902", "NewYork", "Delhi", 350, 1150.00));
            flights.push_back(new InternationalFlight("AI-915", "Mumbai", "London", 300, 800.00));
            flights.push_back(new InternationalFlight("AI-920", "Delhi", "Paris", 280, 750.00));
            flights.push_back(new InternationalFlight("AI-933", "Chennai", "Singapore", 250, 350.00));
            flights.push_back(new InternationalFlight("6E-1001", "Delhi", "Dubai", 220, 250.00));
            flights.push_back(new InternationalFlight("6E-1002", "Mumbai", "Dubai", 220, 240.00));
            flights.push_back(new InternationalFlight("6E-1050", "Bangalore", "Bangkok", 180, 280.00));
            flights.push_back(new InternationalFlight("6E-1075", "Kolkata", "Bangkok", 180, 260.00));
            flights.push_back(new InternationalFlight("6E-1100", "Chennai", "Colombo", 150, 150.00));
            flights.push_back(new InternationalFlight("UK-2001", "Delhi", "Frankfurt", 280, 850.00));
            flights.push_back(new InternationalFlight("UK-2002", "Mumbai", "Singapore", 250, 400.00));
            flights.push_back(new InternationalFlight("UK-2050", "Delhi", "London", 300, 820.00));
            flights.push_back(new InternationalFlight("UK-2088", "Bangalore", "London", 300, 840.00));
            flights.push_back(new InternationalFlight("UK-2100", "Delhi", "Tokyo", 250, 900.00));
            flights.push_back(new InternationalFlight("EK-501", "Mumbai", "Dubai", 400, 350.00));
            flights.push_back(new InternationalFlight("EK-502", "Delhi", "Dubai", 400, 360.00));
            flights.push_back(new InternationalFlight("EK-503", "Bangalore", "Dubai", 350, 380.00));
            flights.push_back(new InternationalFlight("SG-5001", "Delhi", "Kabul", 150, 300.00));
            flights.push_back(new InternationalFlight("SG-5002", "Chennai", "Male", 180, 320.00));
            flights.push_back(new InternationalFlight("AI-800", "Hyderabad", "Chicago", 300, 1300.00));
            flights.push_back(new InternationalFlight("AI-850", "Delhi", "Toronto", 320, 1250.00));
            flights.push_back(new InternationalFlight("6E-1200", "Mumbai", "Muscat", 180, 220.00));
            flights.push_back(new InternationalFlight("UK-3000", "Delhi", "Sydney", 300, 1100.00));
            flights.push_back(new InternationalFlight("AI-999", "Chennai", "KualaLumpur", 200, 290.00));

            saveFiles(); 
        }
    }

    ~SmartBookingSystem() {
        saveFiles();
        for (auto f : flights) delete f;
        delete admin;
    }

    void loadFiles() {
        ifstream fFile("flights.txt");
        if (fFile.is_open()) {
            string line;
            while (getline(fFile, line)) {
                stringstream ss(line);
                string type, id, orig, dest, seatsStr, priceStr, extra;
                getline(ss, type, ','); getline(ss, id, ','); getline(ss, orig, ',');
                getline(ss, dest, ','); getline(ss, seatsStr, ','); getline(ss, priceStr, ','); getline(ss, extra, ',');

                if (type == "D") flights.push_back(new DomesticFlight(id, orig, dest, stoi(seatsStr), stod(priceStr), stod(extra)));
                else if (type == "I") flights.push_back(new InternationalFlight(id, orig, dest, stoi(seatsStr), stod(priceStr), extra));
            }
            fFile.close();
        }

        ifstream pFile("passengers.txt");
        if (pFile.is_open()) {
            string line;
            while (getline(pFile, line)) {
                stringstream ss(line);
                string u, p, n, aStr, d;
                getline(ss, u, ','); getline(ss, p, ','); getline(ss, n, ','); getline(ss, aStr, ','); getline(ss, d, ',');
                passengers.push_back(Passenger(u, p, n, stoi(aStr), d));
            }
            pFile.close();
        }

        ifstream bFile("bookings.txt");
        if (bFile.is_open()) {
            string line;
            while (getline(bFile, line)) {
                stringstream ss(line);
                string bID, pUser, fID, stat;
                getline(ss, bID, ','); getline(ss, pUser, ','); getline(ss, fID, ','); getline(ss, stat, ',');
                bookings.push_back(Booking(bID, pUser, fID, stat));
                nextBookingID = stoi(bID) + 1; 
            }
            bFile.close();
        }
    }

    void saveFiles() {
        ofstream fFile("flights.txt");
        for (auto f : flights) fFile << f->getSaveData() << endl;
        
        ofstream pFile("passengers.txt");
        for (auto p : passengers) pFile << p.getSaveData() << endl;

        ofstream bFile("bookings.txt");
        for (auto b : bookings) bFile << b.getSaveData() << endl;
    }

    void registerPassenger() {
        try {
            string u, p, n, d; int a;
            cout << "\n--- Register --- \nUsername: "; cin >> u;
            cout << "Password: "; cin >> p;
            cout << "Full Name: "; cin.ignore(); getline(cin, n);
            cout << "Age: "; cin >> a;
            
            if (a < 0 || a > 120) throw invalid_argument("Error: Invalid Age entered! Registration Failed.");

            cout << "Date of Birth (DD/MM/YYYY): "; cin >> d;
            passengers.push_back(Passenger(u, p, n, a, d));
            cout << "Registration Successful!\n";
            saveFiles();
        } 
        catch (const invalid_argument& e) { cout << e.what() << endl; }
    }

    bool loginPassenger() {
        string u, p;
        cout << "\n--- Passenger Login ---\nUsername: "; cin >> u;
        cout << "Password: "; cin >> p;
        for (auto& pass : passengers) {
            if (pass.validateCredentials(u, p)) {
                loggedInPassenger = &pass;
                cout << "Login Successful. Welcome " << u << "!\n";
                return true;
            }
        }
        cout << "Invalid Credentials!\n";
        return false;
    }

    bool loginAdmin() {
        string u, p;
        cout << "\n--- Admin Login ---\nUsername: "; cin >> u;
        cout << "Password: "; cin >> p;
        if (admin->validateCredentials(u, p)) {
            cout << "Admin Login Successful!\n";
            return true;
        }
        cout << "Invalid Admin Credentials!\n";
        return false;
    }

    void addFlight() {
        int choice, seats; string id, orig, dest; double price;
        cout << "\n1. Domestic Flight\n2. International Flight\nChoice: "; cin >> choice;
        cout << "Flight ID: "; cin >> id;
        cout << "Origin: "; cin >> orig;
        cout << "Destination: "; cin >> dest;
        cout << "Seats: "; cin >> seats;
        cout << "Price: $"; cin >> price;

        if (choice == 1) flights.push_back(new DomesticFlight(id, orig, dest, seats, price));
        else flights.push_back(new InternationalFlight(id, orig, dest, seats, price));
        
        cout << "Flight Added Successfully!\n";
        saveFiles();
    }

    void viewAllFlights() {
        cout << "\n--- Available Flights ---\n";
        cout << left << setw(10) << "ID" << setw(15) << "Origin" << setw(15) << "Destination" << setw(10) << "Seats" << "Price" << endl;
        cout << "------------------------------------------------------------\n";
        for (auto f : flights) f->displayDetails(); 
    }

    void searchFlight(string dest) {
        bool found = false;
        cout << "\n--- Search Results for Destination: " << dest << " ---\n";
        for (auto f : flights) {
            if (f->getDestination() == dest && f->getSeats() > 0) {
                f->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "No available flights found.\n";
    }

    void searchFlight(double maxPrice) {
        bool found = false;
        cout << "\n--- Search Results for Flights under $" << maxPrice << " ---\n";
        for (auto f : flights) {
            if (f->getPrice() <= maxPrice && f->getSeats() > 0) {
                f->displayDetails();
                found = true;
            }
        }
        if (!found) cout << "No flights found under this budget.\n";
    }

    void searchMenu() {
        int choice;
        cout << "\nSearch by:\n1. Destination\n2. Maximum Budget\nChoice: ";
        cin >> choice;
        if(choice == 1){
            string d; cout << "Enter Destination: "; cin >> d;
            searchFlight(d); 
        } else {
            double p; cout << "Enter Max Budget: $"; cin >> p;
            searchFlight(p); 
        }
    }

    void reserveSeat() {
        string fID;
        viewAllFlights();
        cout << "\nEnter Flight ID to book (e.g. AI-101): "; cin >> fID;

        for (auto f : flights) {
            if (f->getID() == fID) {
                if (f->getSeats() > 0) {
                    f->updateSeats(-1); 
                    bookings.push_back(Booking(to_string(nextBookingID++), loggedInPassenger->getUsername(), fID, "Active"));
                    cout << "Booking Confirmed! Seat Reserved.\n";
                    
                    Formatter<string> fmt(fID);
                    cout << "Saved to itinerary for flight ";
                    fmt.displayFormatted(); cout << endl;
                    
                    saveFiles();
                } else {
                    cout << "Flight is Fully Booked!\n";
                }
                return;
            }
        }
        cout << "Flight ID not found.\n";
    }

    void viewMyBookings() {
        cout << "\n--- My Bookings ---\n";
        bool found = false;
        for (const auto& b : bookings) {
            if (b.getPassengerUser() == loggedInPassenger->getUsername()) {
                printOfficialInvoice(b);
                found = true;
            }
        }
        if (!found) cout << "No bookings found.\n";
    }

    void logout() { loggedInPassenger = nullptr; cout << "Logged out.\n"; }
};

// ==========================================
// === MAIN ENTRY ===
// ==========================================
int main() {
    SmartBookingSystem sys;
    int mainChoice;

    while (true) {
        printHeader("SMART AIR TRAVEL BOOKING APPLICATION");
        cout << "1. Passenger Login\n2. Passenger Registration\n3. Admin Login\n0. Exit Application\nSelect Role: ";
        cin >> mainChoice;

        if (mainChoice == 0) break;
        else if (mainChoice == 2) { sys.registerPassenger(); }
        else if (mainChoice == 1) {
            if (sys.loginPassenger()) {
                int pChoice;
                while (true) {
                    printHeader("Passenger Menu");
                    cout << "1. View All Flights\n2. Search Flights\n3. Reserve Seat\n4. View My Bookings\n0. Logout\nChoice: ";
                    cin >> pChoice;
                    if (pChoice == 0) { sys.logout(); break; }
                    switch (pChoice) {
                        case 1: sys.viewAllFlights(); break;
                        case 2: sys.searchMenu(); break; 
                        case 3: sys.reserveSeat(); break;
                        case 4: sys.viewMyBookings(); break;
                    }
                }
            }
        }
        else if (mainChoice == 3) {
            if (sys.loginAdmin()) {
                int aChoice;
                while (true) {
                    printHeader("Admin Menu");
                    cout << "1. Add Flight\n2. View All Flights\n0. Logout\nChoice: ";
                    cin >> aChoice;
                    if (aChoice == 0) break;
                    switch (aChoice) {
                        case 1: sys.addFlight(); break;
                        case 2: sys.viewAllFlights(); break;
                    }
                }
            }
        }
    }
    cout << "Thank you for using Smart Air Travel Booking System!\n";
    return 0;
}
