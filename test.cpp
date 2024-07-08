#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <limits>

using namespace std;
int C_counter;
int E_counter;
int CAR_counter;

template <typename T>
T getValidatedInput(const std::string& prompt) {
    T value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            // Input was successful, break out of the loop
            break;
        } else {
            // Clear the input buffer to handle invalid input
            std::cin.clear();
            // Discard the invalid input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            // Provide an error message
            std::cerr << "Invalid input. Please enter a valid value." << std::endl;
        }
    }
    return value;
}

class Customer{
    protected:
    int id;
    string name;
    string password;
    int dues;
    int record;
    string other_details;
    int current;
    public:
    Customer(int id,  string& name,  string& password,int dues, int record,int current, string& other_details);

    int get_dues()  { return dues; }
    int get_record()  { return record; }

void disp_cars() {
        ifstream file("Cars.txt");
        int model, tid;string condn, other_details,line,bdate, ddate,rentedby;
        string avail; istringstream s; char comma;

        while (getline(file,line)) { 
            s.str(line); s>>model >> comma; std::getline(s,condn,',');  std::getline(s,other_details,','); getline(s,avail,','); std::getline(s,rentedby,',');std::getline(s,bdate,',');std::getline(s,ddate);
            istringstream s1(rentedby); s1>>comma >> tid;
            if (avail=="AVAILABLE" || (tid==this->id && comma =='C')) {
                std::cout << model << " " << condn << " " << other_details <<" "<<avail<<" "<<rentedby<<" "<<bdate<<" "<<ddate<<endl;
            } s.clear(); s1.clear();
        }
        file.close();
    }
    void set_current(int x){this->current=x;}


    friend class DatabaseC;
    friend void run(const string& role, int id);
};

Customer::Customer(int id,  string& name,  string& password,int dues, int record,int current,  string& other_details){
    this->id=id; this->name=name; this->password=password; this->other_details=other_details; this->dues=dues; this->record=record;
this->current=current;
}


class Employee{
     protected:
    int id;
    string name;
    string password;
    string other_details;
    int dues;
    int record;
    int current;
    public:
    Employee(int id,  string& name,  string& password,int dues, int record ,int current,  string& other_details);

    int get_dues()  { return dues; }
    int get_record()  { return record; }
    

   void disp_cars() {
        ifstream file("Cars.txt");
        int model, tid;string condn, other_details,line,bdate, ddate,rentedby;
        string avail; istringstream s; char comma;

        while (getline(file,line)) { 
            s.str(line); s>>model >> comma; std::getline(s,condn,',');  std::getline(s,other_details,','); getline(s,avail,','); std::getline(s,rentedby,',');std::getline(s,bdate,',');std::getline(s,ddate);
            istringstream s1(rentedby); s1>>comma >> tid;
            if (avail=="AVAILABLE" || (tid==this->id && comma =='C')) {
                std::cout << model << " " << condn << " " << other_details <<" "<<avail<<" "<<rentedby<<" "<<bdate<<" "<<ddate<<endl;
            } s.clear(); s1.clear();
        }
        file.close();
    }
    void set_current(int x){this->current=x;}
    friend class DatabaseE;
    friend void run(const string& role, int id);
};
Employee::Employee(int id,  string& name,  string& password,int dues, int record,int current,  string& other_details){
    this->id=id; this->name=name; this->password=password; this->other_details=other_details; this->dues=dues; this->record=record;
this->current=current;
}

class Car {
protected:
    int model;
    string condn;
    string other_details;
    string avail;
    string rentedby;
    string book_date;
    string due_date;

public:
    Car(int model,string condn,string other_details, string avail, string rentedby,string book_date, string due_date);

    void show_due_date() {
        std::cout << due_date;
    }

    friend class CarDatabase;
    friend class Manager;
};
Car::Car(int model,string condn,string other_details, string avail, string rentedby,string book_date, string due_date){
    this->model=model; this->condn=condn; this->other_details=other_details; this->avail=avail; this->rentedby=rentedby;this->book_date=book_date; this->due_date=due_date;
}
class DatabaseC {
private:
    string filename;
    friend class Manager;

public:
    DatabaseC( string filename);

    void addCustomer(Customer& customer) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << customer.id << "," << customer.name << "," << customer.password << ","
                 << customer.dues << "," << customer.record << ","<< customer.current<< ",";


            file << customer.other_details << "\n";

            file.close();
            std::cout << "Customer added successfully.\n Assigned Employee id: "<< customer.id<< endl;
        } else {
            cerr << "Error opening file for adding customer.\n";
        }
    }

    void updateCustomer(Customer& customer) {
        fstream file(filename, ios::in | ios::out);
        if (file.is_open()) {
            vector<string> lines;
            string line;

            while (std::getline(file, line)) {
                size_t pos = line.find(to_string(customer.id)); 
                if (pos ==0) {
                    line = to_string(customer.id) + "," + customer.name + "," + customer.password + ","
                           + to_string(customer.dues) + "," + to_string(customer.record) + ","+ to_string(customer.current) + ",";

                    line += customer.other_details;
                }
                lines.push_back(line);
            }

            file.close();

            ofstream outFile(filename);
            for ( auto& l : lines) {
                outFile << l << "\n";
            }

            std::cout << "Customer updated successfully.\n";
        } else {
            cerr << "Error opening file for updating customer.\n";
        }
    }

    void deleteCustomer(int personId) {
    ifstream fileIn(filename);
    if (!fileIn.is_open()) {
        cerr << "Error opening file for deleting customer.\n";
        return;
    }

    ofstream fileOut("temp.txt");
    string line;
    bool deleted = false;

    while (std::getline(fileIn, line)) {
        std::cout << line << " ";
        std::cout << line.find(to_string(personId)) << endl;
        if (line.find(to_string(personId)) !=0) {
            fileOut << line << "\n";
        } else {
            deleted = true;
        }
    }

    fileIn.close();
    fileOut.close();

    if (deleted) {
        remove(filename.c_str());
        rename("temp.txt", filename.c_str());
        std::cout << "Customer deleted successfully.\n";
    } else {
        std::cout << "Customer not found.\n";
        remove("temp.txt");
    }
}

    string searchCustomer(int customerId) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error opening file for searching customer: " << filename << endl;
        return "";
    }

    string line;
    bool found = false;

    while (std::getline(file, line)) {
        size_t pos = line.find(to_string(customerId));
        if (pos ==0) {
            std::cout << "Customer found:\n" << line << "\n";
            found = true;
            file.close(); // Close the file before returning
            return line;
        }
    }

    file.close();

    if (!found) {
        std::cout << "Customer not found.\n";
    }

    return "\0";
}


    void viewCustomer() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (std::getline(file, line)) {
                std::cout << line << "\n";
                line.clear();
            }
            file.close();
        } else {
            cerr << "Error opening file for viewing customers.\n";
        }
    }
};
DatabaseC::DatabaseC( string filename){
    this->filename=filename;
}
class DatabaseE {
private:
    string filename;
    friend class Manager;

public:
    DatabaseE(string filename);

    void addEmployee(Employee& employee) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << employee.id << "," << employee.name << "," << employee.password << ","
                 << employee.dues << "," << employee.record << ","<< employee.current << ",";


            file << employee.other_details << "\n";

            file.close();
            std::cout << "Employee added successfully.\n Assigned Employee id : "<< employee.id<<endl;
        } else {
            cerr << "Error opening file for adding Employee.\n";
        }
    }

    void updateEmployee(Employee& employee) {
        fstream file(filename, ios::in | ios::out);
        if (file.is_open()) {
            vector<string> lines;
            string line;

            while (std::getline(file, line)) {
                size_t pos = line.find(to_string(employee.id));
                if (pos ==0) {
                    line = to_string(employee.id) + "," + employee.name + "," + employee.password + ","
                           + to_string(employee.dues) + "," + to_string(employee.record) + "," + to_string(employee.current) + ",";

                    line += employee.other_details;
                }
                lines.push_back(line);
            }

            file.close();

            ofstream outFile(filename);
            for (auto& l : lines) {
                outFile << l << "\n";
            }

            std::cout << "Employee updated successfully.\n";
        } else {
            cerr << "Error opening file for updating Employee.\n";
        }
    }

    void deleteEmployee(int employeeId) {
        ifstream fileIn(filename);
        if (!fileIn.is_open()) {
            cerr << "Error opening file for deleting Employee.\n";
            return;
        }

        ofstream fileOut("temp.txt");
        string line;
        bool deleted = false;

        while (std::getline(fileIn, line)) {
            if (line.find(to_string(employeeId)) !=0) {
                fileOut << line << "\n";
            } else {
                deleted = true;
            }
        }

        fileIn.close();
        fileOut.close();

        if (deleted) {
            remove(filename.c_str());
            rename("temp.txt", filename.c_str());
            std::cout << "Employee deleted successfully.\n";
        } else {
            std::cout << "Employee not found.\n";
            remove("temp.txt");
        }
    }

    string searchEmployee(int employeeId) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            bool found = false;

            while (std::getline(file, line)) {
                size_t pos = line.find(to_string(employeeId));
                if (pos ==0) {
                    std::cout << "Employee found:\n" << line << "\n";
                    found = true;
                    return line;
                    break;
                }
            }

            file.close();

            if (!found) {
                std::cout << "Employee not found.\n";
            }
        } else {
            cerr << "Error opening file for searching Employees.\n";
        }
        return "\0";
    }

    void viewEmployee() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (std::getline(file, line)) {
                std::cout << line << "\n";
            }
            file.close();
        } else {
            cerr << "Error opening file for viewing Employees.\n";
        }
    }
};

DatabaseE::DatabaseE( string filename){
    this->filename=filename;
}

class CarDatabase {
private:
    string filename;
    friend class Manager;

public:
    CarDatabase(string filename);

    void addCar(Car& car) {
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << car.model << "," << car.condn << "," << car.other_details << ","
                 << car.avail<<"," <<car.rentedby<< "," << car.book_date << "," << car.due_date << "\n";

            file.close();
            std::cout << "Car added successfully.\nAssigned Car model:"<<car.model<< endl;
        } else {
            cerr << "Error opening file for adding car.\n";
        }
    }

    void updateCar(Car& newCar) {
        fstream file(filename, ios::in | ios::out);
        if (file.is_open()) {
            vector<string> lines; istringstream s; char comma; int id;
            string line;

            while (std::getline(file, line)) {
                s.str(line); s>>id>>comma; s.clear();
                if (id ==newCar.model){
                    line = to_string(newCar.model) + "," + newCar.condn + "," + newCar.other_details + ","
                           + newCar.avail + "," + newCar.rentedby+","+ newCar.book_date + "," 
                           + newCar.due_date;
                }
                lines.push_back(line);
            }

            file.close();

            ofstream outFile(filename);
            for (auto& l : lines) {
                outFile << l << "\n";
            }

            std::cout << "Car updated successfully.\n";
        } else {
            cerr << "Error opening file for updating car.\n";
        }
    }

    void deleteCar(int carModel) {
        ifstream fileIn(filename);
        if (!fileIn.is_open()) {
            cerr << "Error opening file for deleting car.\n";
            return;
        }

        ofstream fileOut("temp.txt");
        string line;
        bool deleted = false;

        while (std::getline(fileIn, line)) {
            if (line.find(to_string(carModel)) !=0) {
                fileOut << line << "\n";
            } else {
                deleted = true;
            }
        }

        fileIn.close();
        fileOut.close();

        if (deleted) {
            remove(filename.c_str());
            rename("temp.txt",filename.c_str());
            std::cout << "Car deleted successfully.\n";
        } else {
            std::cout << "Car not found.\n";
            remove("temp.txt");
        }
    }

    string searchCar(int carModel) {
        ifstream file(filename);
        if (file.is_open()) {
            string line; istringstream s; char comma; int id;
            bool found = false;

            while (std::getline(file, line)) {
                 s.str(line); s>>id>>comma; s.clear();
                if (id==carModel) {
                    std::cout << "Car found:\n" << line << "\n";
                    found = true;
                    return line;
                    break;
                }
            }

            file.close();

            if (!found) {
                std::cout << "Car not found.\n";
            }
        } else {
            cerr << "Error opening file for searching cars.\n";
        }
        return "\0";
    }

    void viewCars() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (std::getline(file, line)) {
                std::cout << line << "\n";
            }
            file.close();
        } else {
            cerr << "Error opening file for viewing cars.\n";
        }
    }
};
CarDatabase::CarDatabase(string filename){this->filename=filename;}

class Manager{
    protected:
    int id;
    string name;
    string password;
public:
    void addCustomer(DatabaseC& db, Customer& customer) {
        db.addCustomer(customer);
    }

    void updateCustomer(DatabaseC& db,  Customer& person) {
        db.updateCustomer(person);
    }

    void deleteCustomer(DatabaseC& db, int personId) {
        db.deleteCustomer(personId);
    }

    void viewCustomer(DatabaseC& db) {
        db.viewCustomer();
    }

    void searchCustomer(DatabaseC& db, int personId) {
        db.searchCustomer(personId);
    }

     void addEmployee(DatabaseE& db, Employee& employee) {
        db.addEmployee(employee);
    }

    void updateEmployee(DatabaseE& db,  Employee& employee) {
        db.updateEmployee(employee);
    }

    void deleteEmployee(DatabaseE& db, int personId) {
        db.deleteEmployee(personId);
    }

    void viewEmployee(DatabaseE& db) {
        db.viewEmployee();
    }

    void searchEmployee(DatabaseE& db, int personId){
        db.searchEmployee(personId);
    }


    void addCar(CarDatabase& carDb,  Car& car) {
        carDb.addCar(car);
    }

    void updateCar(CarDatabase& carDb,Car& car) {
        carDb.updateCar(car);
    }

    void deleteCar(CarDatabase& carDb,  int carModel) {
        carDb.deleteCar(carModel);
    }

    void viewCars(CarDatabase& carDb) {
        carDb.viewCars();
    }
    void searchCar(CarDatabase& carDb,  int carModel){
        carDb.searchCar(carModel);
    }

    void displayCarInfo(Car& car, CarDatabase& carDb) {
        std::cout << "Car Model: " << car.model << endl;
        std::cout << "Condition: " << car.condn << endl;
        std::cout << "Other Details: " << car.other_details << endl;
        std::cout << "Availability: " << car.avail  << endl;
        std::cout << "Book Date: " << car.book_date << endl;
        std::cout << "Due Date: " << car.due_date;
        std::cout << "\n---\n";
    }

    Manager(void);
};
Manager::Manager(void){
    this->id=1; this->name="manager";this->password="123"; 
};

int* login_signup( string role) {
    
    string file = role + "s.txt";
    
    int t = 0;    
    int* x = new int[2];

    while (1) {
       
        t=getValidatedInput<int>("Choose: \n 1.Enter '1' to login: \n2.Enter '2' to sign up(option only available for CUSTOMERS): \n3. Enter any other number to exit: \n");
        

         if (t == 1&&role!="Manager"&&(role=="Customer"||role=="Employee")) {
            ifstream inFile1(file, ios::app);
            int id, tid, a, ufound = 0;
            string pwd, tpwd, line,dummy;
            id= getValidatedInput<int>("Enter your id:");
            pwd= getValidatedInput<string>("Enter password:");

            while (std::getline(inFile1, line)) {
                stringstream ss(line);
                ss>>tid;  // Read the 1st attribute
                std::getline(ss, dummy,',');  std::getline(ss, dummy, ',');  // Skip the 2nd attribute
                std::getline(ss, tpwd, ','); // Read the 3rd attribute
                if (tid == id) {
                    ufound = 1;
                    if (tpwd == pwd) { // Use == for password comparison
                        std::cout << "Login Successful\n";
                        inFile1.close();
                        x[0] = 1;
                        x[1] = id;
                        return x;
                    }
                }
            }

            if (ufound == 0) {
                std::cout << "User not registered\n";
            } else {
                std::cout << "Incorrect PASSWORD\n";
            }
            inFile1.close(); // Close the file on error
            continue;
        } 
        else if(t == 1&&role=="Manager"){
            int id; string pwd;
            id= getValidatedInput<int>("Enter your id:");
            pwd= getValidatedInput<string>("Enter password:");
            if(id==1&&pwd=="manager"){
                std::cout << "Login Successful\n";
                x[0]=1; x[1]=1;
                return x;
            }
            else{std::cout<<"Incorrect ID or PASSWORD"; continue;}
        }
        else if (t == 2) {
            ofstream inFile1(file, ios::app);
            if (role == "Customer") {
                string pwd, name, other_details; int record = 40;
                std::cout << "Enter your name:\n";
                std::cin.ignore();
                std::getline(std::cin, name);
                
                pwd= getValidatedInput<string>("Create a Password:\n");

                std::cout << "Enter other details(if none, enter 'NA')\n";
                std::cin.ignore();
                std::getline(std::cin, other_details);

                C_counter++;
                inFile1 << C_counter << "," << name << "," << pwd << ","<< 0 << "," << record<< "," << 0 << "," << other_details<<"\n";
                std::cout << "Successfully Registered.\n Assigned Customer id:" << C_counter<<endl;
            } else {
                std::cout << "Sign up not available for the roles: 'Employee' and 'Manager'\n";
            }
            inFile1.close();
            continue;
        } 
        else {
            x[0] = 0;
            return x;
        }
    }
}

void run(const std::string& role, int id) {

    DatabaseC customerDb("Customers.txt");
    DatabaseE employeeDb("Employees.txt");
    CarDatabase carDb("Cars.txt");
    std::string name, pwd, othdetail, line;
    int dues, record, x,n, maxCars;

    if (role == "Customer") {
        std::cout.setstate(std::ios_base::failbit); // set error flag, disabling output
        line = customerDb.searchCustomer(id);
        std::cout.clear(); // clear flags, reenabling output
        std::istringstream iss(line), s1;
        int model, current;
        std::string extra, car, condn, other_details, rentedby, bdate, ddate;
        char comma;
        string avail;

        // Extract values using std::getline
        iss >> x >> comma;
        std::getline(iss, name, ',');
        std::getline(iss, pwd, ',');
        iss >> dues >> comma >> record >> comma >> current >> comma;
        std::getline(iss, othdetail);
        Customer customer(id, name, pwd, dues, record, current, othdetail);

        while (true) {

            int choice;
            choice= getValidatedInput<int>("\nCustomer Menu:\n1. Enter '1' to Display available/rented cars\n 2. Enter '2' to Rent a car\n 3. Enter '3' to  View personal information\n 4. Enter any other integer to Exit\n Enter your choice: \n");

            switch (choice) {
                case 1:
                    customer.disp_cars();
                    break;
                case 2:
                    maxCars = customer.record / 40;
                    if (maxCars < 1) maxCars = 1;
                    if (maxCars <= customer.current) {
                        std::cout << "Customer not eligible to rent more cars\n";
                        break;
                    }
                    model = getValidatedInput<int>("Enter the model of car you want to rent:");
                   
                    car = carDb.searchCar(model);
                    if (car == "\0") { break; }
                    s1.str(car);
                    s1 >> model >> comma;
                    std::getline(s1, condn, ',');
                    std::getline(s1, other_details, ',');
                    std::getline(s1, avail,',');

                    if (avail=="AVAILABLE") {

                        avail="NOT AVAILABLE";
                        rentedby = "C" + std::to_string(customer.id);

                        n=getValidatedInput<int>("Enter the number of days of lease:\n");

                        ddate=getValidatedInput<string>("Enter lease date(format: DD-MM-YYYY): \n");

                        std::cout << "The lease will amount to: Rs." << n * 100 << std::endl;

                        extra= getValidatedInput<string>("To continue, enter 'YES', else enter anything else: \n");
                        if (extra != "YES") { break; }


                        // Get the current system time
                        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        // Convert the time to a local time structure
                        std::tm* localTime = std::localtime(&currentTime);
                        // Format the date
                        std::ostringstream oss;
                        oss << std::put_time(localTime, "%d-%m-%Y");
                        // Store the formatted date in a string
                        bdate = oss.str();

                        Car newcar(model, condn, other_details, avail, rentedby, bdate, ddate);
                        carDb.updateCar(newcar);
                        current++;
                        customer.set_current(current);
                        customerDb.updateCustomer(customer);
                    } else {
                        std::cout << "Chosen Car is currently unavailable";
                    }
                    break;
                case 3:
                    std::cout << "Personal Information:\n";
                    std::cout << "Customer ID: " << customer.id << "\n";
                    std::cout << "Name: " << customer.name << "\n";
                    std::cout << "Password:" << customer.password;
                    std::cout << "Other Details: " << customer.other_details << "\n";
                    std::cout << "Dues: " << customer.get_dues() << "\n";
                    std::cout << "Record: " << customer.get_record() << "\n";
                    std::cout << "Number of Cars Rented: " << customer.current << "\n";
                    break;
                default:
                    std::cout << "Exiting customer menu.\n";
                    return;
            }
        }
    }
    else if (role == "Employee") {
        std::cout.setstate(std::ios_base::failbit); // set error flag, disabling output
        line= employeeDb.searchEmployee(id);
        std::cout.clear(); // clear flags, reenabling output
        std::istringstream iss(line); istringstream s1;
        int model, current; string car,extra, condn, other_details,rentedby, bdate,ddate; char comma; string avail;

        // Extract values using std::getline
        iss>>x; std::getline(iss, name, ','); std::getline(iss, pwd, ',');iss >> dues >> comma >> record >> comma>> current>> comma; std::getline(iss,othdetail);
        Employee employee(id, name ,pwd, dues, record, current, othdetail);

        while (true) {

            int choice, model; 
            choice= getValidatedInput<int>("\nCustomer Menu:\n1. Enter '1' to Display available/rented cars\n 2. Enter '2' to Rent a car\n 3. Enter '3' to  View personal information\n 4. Enter any other integer to Exit\n Enter your choice: \n");


            switch (choice) {
                // Similar functionality as in Customer's case
                case 1:
                    employee.disp_cars();
                    break;
                case 2:
                    maxCars = employee.record / 40;
                    if (maxCars < 1) maxCars = 1;
                    if(maxCars<=employee.current){std::cout<< "Customer not eligible to rent more cars\n"; break;}
                    model = getValidatedInput<int>("Enter the model of car you want to rent:");

                    int n;
                    car = carDb.searchCar(model);
                    if(car=="\0"){break;}

                    s1.str(car);
                    s1 >> model >> comma; std::getline(s1,condn,','); std::getline(s1,other_details,','); s1>> avail >> comma;
                    if(avail=="AVAILABLE"){
                        avail="NOT AVAILABLE";
                        rentedby="E"+ to_string(employee.id); 

                        n=getValidatedInput<int>("Enter the number of days of lease:\n");
                        
                        ddate=getValidatedInput<string>("Enter lease date(format: DD-MM-YYYY): \n");

                        std::cout<< "The lease will amount to: Rs." << n*100;
                        extra= getValidatedInput<string>("To continue, enter 'YES', else enter anything else: \n");

                        if(extra!="YES"){break;}

                        // Get the current system time
                        auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                        // Convert the time to a local time structure
                        std::tm* localTime = std::localtime(&currentTime);
                        // Format the date
                        std::ostringstream oss;
                        oss << std::put_time(localTime, "%d-%m-%Y");
                        // Store the formatted date in a string
                        bdate = oss.str();

                        Car newcar(model,condn,other_details,avail,rentedby,bdate,ddate);
                        carDb.updateCar(newcar); 
                        std::cout<<"Car leased successfully \n"; current++; employee.set_current(current); employeeDb.updateEmployee(employee);

                    } 
                    else{std::cout<<"Chosen Car is currently unavailable";}
                    break;
                case 3:
                    std::cout << "Personal Information:\n";
                    std::cout << "Customer ID: " << employee.id << "\n";
                    std::cout << "Name: " << employee.name << "\n";
                    std::cout << "Password:" << employee.password;
                    std::cout << "Other Details: " << employee.other_details << "\n";
                    std::cout << "Dues: " << employee.get_dues() << "\n";
                    std::cout << "Record: " << employee.get_record() << "\n";
                    std::cout << "Number of Cars Rented: " << employee.current << "\n";
                    break;
                default:
                    std::cout << "Exiting employee menu.\n";
                    return;
                
            }
        }
    }  
    else if (role == "Manager") {
        Manager manager;

        while (true) {
            std::cout << "Manager Menu:\n";
            std::cout << "1. View all customers\n";
            std::cout << "2. View all employees\n";
            std::cout << "3. View all cars\n";
            std::cout << "4. Add new customer\n";
            std::cout << "5. Add new employee\n";
            std::cout << "6. Add new car\n";
            std::cout << "7. Update customer\n";
            std::cout << "8. Update employee\n";
            std::cout << "9. Update car\n";
            std::cout <<"10. Delete customer\n";
            std::cout <<"11. Delete employee\n";
            std::cout <<"12. Delete car\n";
            std::cout <<"13. Search customer\n";
            std::cout <<"14. Search employee\n";
            std::cout <<"15. Search car\n";
            std::cout <<"16. Return car\n";
            std::cout <<"17. Enter any other integer to exit\n";
            std::cout <<"Enter your choice: ";
            int choice;
            std::cin >> choice;
            int model; string condn; string other_details; string rentedby, bdate, ddate, avail;
            int id1,current; int j=0;

            switch (choice){
                case 1:
                    std::cout << "All Customers:\n";
                    manager.viewCustomer(customerDb);
                    break;
                case 2:
                    std::cout << "All Employees:\n";
                    manager.viewEmployee(employeeDb);
                    break;
                case 3:
                    std::cout << "All Cars:\n";
                    manager.viewCars(carDb);
                    break;
                case 4: {
                    
                    std::cout<<"Enter the following details of the customer: name, password, other details(if none, enter\"NA\" \n) ";
                    std::cin.ignore(); std::getline(std::cin,name);  pwd= getValidatedInput<string>("") ;std::getline(std::cin, othdetail);
                    C_counter++;
                    Customer newCustomer(C_counter,name,pwd,0, 40,0,othdetail);
                    manager.addCustomer(customerDb, newCustomer);
                    break;
                }
                case 5: {
                    
                    std::cout<<"Enter the following details of the employee: name, password, other details(if none, enter\"NA\" \n) ";
                    std::cin.ignore(); std::getline(std::cin,name);   
                    pwd= getValidatedInput<string>(""); 
                    std::getline(std::cin, othdetail);

                    E_counter++;
                    Employee newEmployee(E_counter, name, pwd,0,40,0,othdetail);
                    manager.addEmployee(employeeDb, newEmployee);
                    break;
                }
                case 6: {
                    std::cout<<"Enter the following Car details: condition,other details(if none, enter\"NA\":\n)";cin.clear();
                    cin.ignore(); getline(cin,condn); 
                    getline(cin,other_details);
        
                    CAR_counter++; 
                    cout<<other_details;
                    Car newCar(CAR_counter,condn,other_details,"AVAILABLE","NA","NA","NA");
                    manager.addCar(carDb, newCar);

                    break;
                }
                case 7: {
                    std::cout<<"Enter the following updated Customer details: \n";
                    id1=getValidatedInput<int>("Enter original id of customer you want to update:\n");

                    cout<<"Enter name:\n"; std::cin.ignore(); std::getline(std::cin,name);

                    pwd=getValidatedInput<string>("Enter password:\n");

                    cout<<"Enter other details(if none enter'NA'):\n"; std::getline(std::cin,othdetail);

                    dues = getValidatedInput<int>("Enter dues:\n") ;

                    record = getValidatedInput<int>("Enter record:\n") ;

                    current = getValidatedInput<int>("Enter number of currently rented cars:\n") ;
                    
                    Customer newCustomer(id1, name, pwd,dues,record,current , othdetail);
                    manager.updateCustomer(customerDb, newCustomer);
                    break;
                }
                case 8: {
                    std::cout<<"Enter the following updated Employee details: \n";
                    id1=getValidatedInput<int>("Enter original id of customer you want to update:\n");

                    cout<<"Enter name:\n"; std::cin.ignore(); std::getline(std::cin,name);

                    pwd=getValidatedInput<string>("Enter password:\n");

                    cout<<"Enter other details(if none enter'NA'):\n"; std::getline(std::cin,othdetail);

                    dues = getValidatedInput<int>("Enter dues:\n") ;

                    record = getValidatedInput<int>("Enter record:\n") ;

                    current = getValidatedInput<int>("Enter number of currently rented cars:\n") ;                    
                    Employee newEmployee(id1, name, pwd,dues,record,current, othdetail);
                    manager.updateEmployee(employeeDb, newEmployee);
                    break;
                }
                case 9: {cin.clear();
                    std::cout<<"Enter the following updated Car details:\n";
                    model= getValidatedInput<int>("Enter original model of car you want to update:\n");
                    cout<<"Enter condition:\n"; std::cin.ignore(); std::getline(std::cin,condn);
                    cout<<"Enter other details:\n";std::getline(std::cin,other_details); 
                    cout<<"Enter availabilitty:\n"; std::getline(std::cin,avail);
                    cin.clear();
                    rentedby= getValidatedInput<string>("Enter concatenation of first letter of role and id of rentee:\n example: C2 or E2\n"); 
                    cout<<"Enter Booking Date(format: dd-mm-yyyy , enter NA if none):\n";cin.ignore();getline(std::cin,bdate); 
                    cout<<"Enter Due Date(format: dd-mm-yyyy , enter NA if none):\n";std::getline(std::cin,ddate);
                    Car newCar(model,condn,other_details,avail,rentedby,bdate,ddate);
                    
                    manager.updateCar(carDb, newCar);
                    break;
                }
                case 10: {
                    id1 = getValidatedInput<int>("Enter id(integer) of customer you want to delete:\n");
                    if(id1==C_counter){C_counter--;}
                    manager.deleteCustomer(customerDb, id1);
                    
                    break;
                }
                case 11: {
                    id1 = getValidatedInput<int>("Enter id(integer) of employee you want to delete:\n");
                    if(id1==E_counter){E_counter--;}
                    manager.deleteEmployee(employeeDb, id1);
                    break;
                }
                case 12: {
                    model = getValidatedInput<int>("Enter model(integer) of car you want to delete: \n");
                    if(id1==CAR_counter){CAR_counter--;}
                    manager.deleteCar(carDb, model);
                    break;
                }
                case 13: {
                    id1 = getValidatedInput<int>("Enter id(integer) of customer you want to search:\n");
                    manager.searchCustomer(customerDb, id1);
                    break;
                }
                case 14: {
                    id1 = getValidatedInput<int>("Enter id(integer) of employee you want to search:\n");

                    manager.searchEmployee(employeeDb, id1);
                    break;
                }
                case 15: {
                    model = getValidatedInput<int>("Enter model(integer) of car you want to search: \n");
                    std::cin >> model;
                    manager.searchCar(carDb, model);
                    break;
                }
                case 16: {
                    string role1, p,damage; int delay, comma , pay; int x[6],amount;
                    
                    model = getValidatedInput<int>("Enter model(integer) of car you want to return: \n");
                        
                        std::cout.setstate(std::ios_base::failbit); // set error flag, disabling output
                        line=carDb.searchCar(model);
                        std::cout.clear(); // clear flags, reenabling output
                        
                        if(line=="\0"){std::cout<<"Invalid Model, not found \n"; break;}                    
                    
                    delay = getValidatedInput<int>("Enter no. of days of delay in submission(if none enter 0): \n");
                    
                    damage= getValidatedInput<string>("Is the car damaged('YES' or 'NO'): \n");
                    
                    int fine= delay*100; if(damage=="YES"){fine+=500;}
                    
                    std::cout<<"Fine = Rs."<< fine<<endl;
                    
                    id1 = getValidatedInput<int>("Enter Rentee's id :\n");

                    
                    role1 = getValidatedInput<string>("Enter role of renter('Customer'or'Employee'): \n");
                    
                    pay = getValidatedInput<int>("1. Enter '1' to Pay Later \n 2.Enter any other integer to Pay Now \nChoose:\n");
                    
                    if(role1=="Employee"){
                        std::cout.setstate(std::ios_base::failbit); // set error flag, disabling output
                        p=employeeDb.searchEmployee(id1);
                        std::cout.clear(); // clear flags, reenabling output
                       if (p.empty()) {
    std::cout << "ID not found \n";
    break;
}

std::vector<int> x(6, 0); 

for (int i = 0, j = 0; i < p.length() && j < 6; i++) {
    if (p[i] == ',') {
        x[j] = i;
        j++;
    }
}


id1 = stoi(p.substr(0, x[0]));
name = p.substr(x[0] + 1, x[1] - x[0] - 1);
pwd = p.substr(x[1] + 1, x[2] - x[1] - 1);
dues = stoi(p.substr(x[2] + 1, x[3] - x[2] - 1));
record = stoi(p.substr(x[3] + 1, x[4] - x[3] - 1));
current = stoi(p.substr(x[4] + 1, x[5] - x[4] - 1));
othdetail = p.substr(x[5] + 1, p.length() -x[5] - 1);


dues+=fine;
if (pay != 1) { 
    amount= getValidatedInput<int>("Enter the amount you want to pay:\n Rs."); 
    if(dues>=amount){dues-=amount;}else{dues=0;}
}

if (delay == 0) {
    if (record + 40 > 100) {
        record = 100;
    } else {
        record += 40;
    }
} else {
    if (record - 40 < 40) {
        record = 40;
    } else {
        record -= 40;
    }
}

if (current !=0) {
    current--;
}

                        Employee newEmployee(id1, name, pwd,dues,record,current, othdetail);

                        employeeDb.updateEmployee(newEmployee);
                    std::cout<<"Enter the following updated Car details: \n condition, other details(if none, enter\"NA\"):\n";
                    std::cin.ignore(); std::getline(std::cin,condn);  
                    std::getline(std::cin,other_details);

                    Car newCar(model,condn,other_details,"AVAILABLE","NA","NA","NA");
                    
                    manager.updateCar(carDb, newCar);

                    }
                    if(role1=="Customer"){
                        std::cout.setstate(std::ios_base::failbit); // set error flag, disabling output
p = customerDb.searchCustomer(id1);
std::cout.clear(); // clear flags, reenabling output

if (p.empty()) {
    std::cout << "ID not found \n";
    break;
}

std::vector<int> x(6, 0); 

for (int i = 0, j = 0; i < p.length() && j < 6; i++) {
    if (p[i] == ',') {
        x[j] = i; 
        j++;
    }
}


id1 = stoi(p.substr(0, x[0]));
name = p.substr(x[0] + 1, x[1] - x[0] - 1);
pwd = p.substr(x[1] + 1, x[2] - x[1] - 1);
dues = stoi(p.substr(x[2] + 1, x[3] - x[2] - 1));
record = stoi(p.substr(x[3] + 1, x[4] - x[3] - 1));
current = stoi(p.substr(x[4] + 1, x[5] - x[4] - 1));
othdetail = p.substr(x[5] + 1, p.length() -x[5] - 1);

dues+=fine;
if (pay != 1) { 
    amount= getValidatedInput<int>("Enter the amount you want to pay:\n Rs."); 
    if(dues>=amount){dues-=amount;}else{dues=0;}
}

if (delay == 0) {
    if (record + 40 > 100) {
        record = 100;
    } else {
        record += 40;
    }
} else {
    if (record - 40 < 40) {
        record = 40;
    } else {
        record -= 40;
    }
}

if (current !=0) {
    current--;
}



Customer newCustomer(id1, name, pwd, dues, record, current, othdetail);
customerDb.updateCustomer(newCustomer);

std::cout << "Enter the following updated Car details: \n condition, other details(if none, enter\"NA\"):\n";
std::cin.ignore();
std::getline(std::cin, condn);
std::getline(std::cin, other_details);

Car newCar(model, condn, other_details, "AVAILABLE", "NA", "NA", "NA");

manager.updateCar(carDb, newCar);

                    break;
                    }
                }
                default:
                    std::cout << "Invalid Role.\n"; return;
            }
        } 
    } 
    else {
        std::cout << "Invalid role.\n";
    }
}
void saveGlobalVariables() {
    std::ofstream outFile("globals.txt");

    if (outFile.is_open()) {
        outFile << C_counter << std::endl;
        outFile << E_counter << std::endl;
        outFile << CAR_counter << std::endl;

        outFile.close();
    } else {
        std::cerr << "Unable to open Counter file for writing." << std::endl;
    }
}

// Function to load global variables from a file
void loadGlobalVariables() {
    std::ifstream inFile("globals.txt");

    if (inFile.is_open()) {
        inFile >> C_counter;
        inFile >> E_counter;
        inFile >> CAR_counter;

        inFile.close();
    } else {
        std::cerr << "Unable to Counter open file for reading." << std::endl;
    }
}


int main(){
    loadGlobalVariables();
    string role; int exit=0;
    int* x;
    int terminate=0;
    while(terminate==0){
    role=getValidatedInput<string>("Welcome to ABC Car Rental Services\n Enter your Role: 'Customer' or 'Employee' or 'Manager':\n");
    
    if(role!="Manager"&&role!="Customer"&&role!="Employee"){
        cout<<"Invalid Role!"<<endl;
        continue;
    }
    x = login_signup(role);

    if (x[0] == 1) {
        run(role, x[1]);
    } else {
        std::cout << "Exiting login/signup.";
    }
    terminate= getValidatedInput<int>("To exit , enter 1, else enter 0:");
    }

    delete[] x;
    saveGlobalVariables();
    
    return 0;
}
