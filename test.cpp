#include <vector>
#include <sstream>
#include <string.h>
#include <fstream>
#include <iostream>

#define MAX_CARS_AN_EMPLOYEE_CAN_RENT 50
#define EMPLOYEE_FINE_RATE_PER_DAY_IN_Rs 150
#define EMPLOYEE_DISCOUNT_PERCENT 15
#define MIN_RECORD_SCORE_FOR_AN_EMPLOYEE_TO_RENT_A_CAR 0.5

#define MAX_CARS_A_CUSTOMER_CAN_RENT 30
#define CUSTOMER_FINE_RATE_PER_DAY_IN_Rs 200
#define CUSTOMER_DISCOUNT_PERCENT 0
#define MIN_RECORD_SCORE_FOR_A_CUSTOMER_TO_RENT_A_CAR 0.7

/* RECORD SCORE CALCULATION

   Let t be the total number of cars rented so far.
   Let d be the number of cars returned with damage.
   Let x be the number of days a rented car is used beyond the due date.

   Then, record score = (t-(d+x/10))/t

   Let m be the minimum record score required to rent a car.

   An employee/ a customer can rent atmost n cars where n is the ceil of [(record_score-m)*10]. 
   For a new employee/customer, the record score used to rent his first car will be m+0.1.
*/

// In this file various check points are used to transfer control from one part to another part of the program. All of them use a label that begins with 'CP'.

using namespace std;

// PART 1 - CLASSES ................................................................................
class Car
{
        bool condition;        //private attribute, so that it is not inherited by RentedCar class

    public:
        string Car_ID;
        string model;
        float rent;    //Rent (in Rs) per day
        
        void new_car();  
        int get_obj(string);
        bool get_condition(string);
        void set_condition(string, bool);
};

class RentedCar : public Car
{
    public:
        string type_of_renter;
        string renter_user_id;
        int DueDate;
        int DueMonth;
        int DueYear;
      
    int get_obj(string);
};

class User
{
  public:
    string name;
    string user_id;
    string password;

    int user_select();
    string login(int);

};

class Renter : public User
{
  public:
    int NumCurrRentedCars;
    int TotalRentedCars;
    int TotalDamagedCars;
    int TotalDelayDays;
    float dues;
};

class Manager: public User
{
   public:
     void manager_portal(); 
     void new_manager();
     int get_obj(string);
};

class Employee: public Renter
{
  public:

    static int discount_percent;
    static float minRecordScore;
    static float fine_rate;

    int get_obj(string);
    void new_employee();
    void clear_dues(float);
    void employee_portal();
};

int Employee::discount_percent = EMPLOYEE_DISCOUNT_PERCENT;
float Employee::minRecordScore = MIN_RECORD_SCORE_FOR_AN_EMPLOYEE_TO_RENT_A_CAR;
float Employee::fine_rate = EMPLOYEE_FINE_RATE_PER_DAY_IN_Rs;

class Customer: public Renter
{
  public:

    static int discount_percent;
    static float minRecordScore;
    static float fine_rate;

    void new_customer();
    int get_obj(string);
    void clear_dues(float);
    void customer_portal();
};

int Customer::discount_percent = CUSTOMER_DISCOUNT_PERCENT;
float Customer::minRecordScore = MIN_RECORD_SCORE_FOR_A_CUSTOMER_TO_RENT_A_CAR;
float Customer::fine_rate = CUSTOMER_FINE_RATE_PER_DAY_IN_Rs;
// END OF PART-1...................................................................................

// PART-2 DATABASE CLASSES.........................................................................
class Database
{
  public:
    int Delete_row(string);
    int Display_row(string);
    void Display_all();
    int count();

    static string file;
    
    virtual string get_file()=0;
};

string Database:: file = "";

class Renter_database : public Database
{
  public:
    int DisplayCurrRentedCarsOfRenter(string);

    static string file;

    virtual string get_file()=0;
};

string Renter_database:: file = "";

class Car_database : public Database
{
    string get_file();

    public:
      static string file;

      void insert_car(string, string, float, bool);
};

string Car_database:: file = "Unrented_Cars.csv";

class RentedCar_database : public Database
{
    string get_file();

    public:
      static string file;

    int Display_DueDate(string);
    int DisplayCarsOfRenter(string, string);
    void insert_rented_car(string, string, float, string, string, int, int, int);
};

string RentedCar_database:: file = "Rented_Cars.csv";

class Manager_database : public Database
{
   string get_file();

   public:
      static string file;
      
    void insert_manager(string,string,string);
};

string Manager_database:: file = "Managers.csv";

class Employee_database : public Renter_database
{
    string get_file();

    public:
      static string file;

    void insert_employee(string, string, string, int, int, int, int, float);
};

string Employee_database:: file = "Employees.csv";

class Customer_database : public Renter_database
{
    string get_file();

    public:
      static string file;

    void insert_customer(string, string, string, int, int, int, int, float);
};

string Customer_database:: file = "Customers.csv";
// END OF PART-2..................................................................................

// PART-3 MEMBER FUNCTION DEFINITIONS OF CLASS CAR................................................
int Car:: get_obj(string Car_ID)
{
    ifstream fin;
    fin.open("Unrented_Cars.csv");
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == Car_ID)
      {
        found = 1;
        this->Car_ID = word;
        getline(ss,word,',');
        model = word;
        getline(ss,word,',');
        rent = stof(word);
        getline(ss,word,',');
        istringstream(word) >> condition;
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}

void Car:: new_car()
{
  string Car_ID;
  string model;
  float rent;
  cout<<"Please enter new car details"<<endl;
  cout<<"Enter new car ID (A single word):"<<endl;               
  cin>>Car_ID;
  cout<<"Enter new car model (A single word):"<<endl;
  cin>>model;
  cout<<"Enter rent (in Rs. per day):"<<endl;
  cin>>rent;
  Car_database carDB;
  carDB.insert_car(Car_ID, model, rent, 1);
}

bool Car::get_condition(string car_id)
{
    ifstream fin;
    fin.open("Unrented_Cars.csv");
    fin.seekg(0);
    string word, line;
    bool Condition;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == car_id)
      {
        getline(ss,word,',');
        getline(ss,word,',');
        getline(ss,word,',');
        istringstream(word) >> Condition;
        fin.close();
        return Condition;
      }
    }
}

void Car::set_condition(string car_id, bool condition)
{
    Car_database carDB;
    Car car;
    car.get_obj(car_id);
    carDB.Delete_row(car_id);
    carDB.insert_car(car.Car_ID,car.model,car.rent,condition);
}
// END OF PART-3..................................................................................

// PART-4 MEMBER FUNCTION DEFINITIONS OF CLASS RENTED_CAR.........................................
int RentedCar:: get_obj(string Rcar_id)
{
    ifstream fin;
    fin.open("Rented_Cars.csv");
    fin.seekg(0);
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == Rcar_id)
      {
        found = 1;
        Car_ID = word;
        getline(ss,word,',');
        model = word;
        getline(ss,word,',');
        rent = stof(word);
        getline(ss,word,',');
        type_of_renter = word;
        getline(ss,word,',');
        renter_user_id = word;
        getline(ss,word,',');
        DueDate = stoi(word);
        getline(ss,word,',');
        DueMonth = stoi(word);
        getline(ss,word,',');
        DueYear = stoi(word);
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}
// END OF PART-4.....................................................................................

// PART-5 MEMBER FUNCTION DEFINITIONS OF CLASS MANAGER...............................................
int Manager::get_obj(string Manager_id)
{
    ifstream fin;
    fin.open("Managers.csv");
    fin.seekg(0);
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == Manager_id)
      {
        found = 1;
        user_id = word;
        getline(ss,word,',');
        name = word;
        getline(ss,word,',');
        password = word;
        
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}

void Manager::new_manager()
{
  cout<<"Please enter details of new manager"<<endl;
  cout<<"Enter user-id (a singe word):"<<endl;
  cin>>user_id;
  cout<<"Enter name (a single word):"<<endl;
  cin>>name;
  cout<<"Enter password (cannot contain spaces):"<<endl;
  cin>>password;
  Manager_database mangDB;
  mangDB.insert_manager(user_id,name,password);
}

void Manager:: manager_portal()
{
   ifstream fin;

   bool Condition; 
   int choice,operation,field, found, car_found, NumOfManagers; 
   int condition = 1;                       
   char c,damaged,late;
   float fine1=0, fine2=0, fine=0, amount_paid=0;
   string user_ID,Car_ID, car_returned;

   Car Car_obj;
   RentedCar Rcar;
   Employee employee;
   Customer customer;
   Car_database CarDB;
   RentedCar_database RcarDB;
   Employee_database employeeDB;
   Customer_database customerDB;
   Manager_database managerDB;
   
   CP1:cout<<"Please provide your choice:"<<endl;
   cout<<"1- Operations on Unrented Cars"<<endl;
   cout<<"2- Operations on Rented Cars"<<endl;
   cout<<"3- Operations on Employees"<<endl;
   cout<<"4- Operations on Customers"<<endl;
   cout<<"5- Operations on Managers"<<endl;
   cout<<"6- Log Out"<<endl;
   cin>>choice;                                   
   
   fine1=0, fine2=0, fine=0, amount_paid=0;

   switch(choice)
   {
    case 1:
            fin.open("Unrented_Cars.csv");
            if(!(fin.is_open()))
            {
              cout<<"Can't perform the desired operations"<<endl;
              cout<<"File \"Unrented_Cars.csv\" not found!!"<<endl;
              goto CP1;
            }
            else
             fin.close();
            
            CP2: cout<<"Please select operation:"<<endl;
            cout<<"1- Add New Car"<<endl;
            cout<<"2- Update Existing Car"<<endl;
            cout<<"3- Delete Existing Car"<<endl;
            cout<<"4- Display All Unrented Cars"<<endl;
            cout<<"5- Display Specific Car"<<endl;
            cout<<"6- Back to main menu"<<endl;
            cin>>operation;                             

            switch(operation)               
            {
              case 1: 
                    Car_obj.new_car();
                    cout<<"Successfully created new car!!\n"<<endl;
                    break;
              case 2:
                    cout<<"Enter Car ID:"<<endl;
                    cin>>Car_ID;
                    found = Car_obj.get_obj(Car_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP2;
                    }
                    else
                    {
                      CarDB.Display_row(Car_ID); 
                      cout<<"Are you sure you want to update? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        CP02: cout<<"Which field do you want to update?"<<endl;
                        cout<<"1- Car_ID"<<endl;
                        cout<<"2- Model"<<endl;
                        cout<<"3- Rent"<<endl;
                        cout<<"4- Condition"<<endl;
                        cin>>field;
                        Condition = Car_obj.get_condition(Car_ID);

                        switch(field)
                        {
                          case 1:
                              cout<<"Enter new Car_ID (single word)"<<endl;
                              cin>>Car_obj.Car_ID;
                              break;
                          case 2:
                              cout<<"Enter new model (single word)"<<endl;
                              cin>>Car_obj.model;
                              break;
                          case 3:
                              cout<<"Enter new rent (In Rs per day)"<<endl;
                              cin>>Car_obj.rent;
                              break;
                          case 4:
                              Condition = !Condition;
                              cout<<"Car condition has been inverted"<<endl;
                              break;
                          default:
                              cout<<"Invalid choice"<<endl;
                              goto CP02;
                        }
                        CarDB.Delete_row(Car_ID); 
                        Car_ID = Car_obj.Car_ID; 
                        CarDB.insert_car(Car_obj.Car_ID, Car_obj.model, Car_obj.rent, Condition);
                        cout<<"Successfully updated!\n"<<endl;
                        cout<<"Do you want to continue updating? [y=yes/ n=any other character]"<<endl;
                        cin>>c;
                        if(c=='y' || c=='Y')
                          goto CP02; 
                      }
                      else
                        goto CP2;
                    }
                    break;
              case 3:
                    cout<<"Enter Car ID:"<<endl;
                    cin>>Car_ID;
                    found = CarDB.Display_row(Car_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP2;
                    }
                    else
                    {
                      cout<<"Are you sure you want to delete? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        CarDB.Delete_row(Car_ID);  //Ignoring return value since file will always be found
                        cout<<"Successfully deleted Car!\n"<<endl;
                      }
                      else
                        goto CP2;
                    }
                    break;
              case 4:
                    CarDB.Display_all();
                    break;
              case 5:
                    cout<<"Enter Car ID:"<<endl;
                    cin>>Car_ID;
                    found = CarDB.Display_row(Car_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP2;
                    }
                    break;
              case 6:
                    break;
              default:
                    cout<<"Invalid choice\n"<<endl;
                    goto CP2;
            }
            break;
  
    case 2:
            fin.open("Rented_Cars.csv");
            if(!(fin.is_open()))
            {
              cout<<"Can't perform the desired operations"<<endl;
              cout<<"File \"Rented_Cars.csv\" not found!!"<<endl;
              goto CP1;
            }
            else
             fin.close();
            
            CP3: cout<<"Please select operation:"<<endl;
            cout<<"1- Display All Rented Cars"<<endl;
            cout<<"2- Display Specific Rented Car"<<endl;
            cout<<"3- View Due Date of a Rented car"<<endl;
            cout<<"4- Back to main menu"<<endl;
            cin>>operation;                    

            switch(operation)
            {
              case 1:
                    RcarDB.Display_all();
                    break;
              case 2:
                    cout<<"Enter Rented Car ID"<<endl;
                    cin>>Car_ID;
                    found = RcarDB.Display_row(Car_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP3;
                    }
                    break;
              case 3:
                    cout<<"Enter Rented Car ID"<<endl;
                    cin>>Car_ID;
                    found = RcarDB.Display_DueDate(Car_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP3;
                    }
                    break;
              case 4:
                    break;
              default:
                    cout<<"Invalid choice"<<endl;
                    goto CP3;
            }
            break;
    case 3:
            fin.open("Employees.csv");
            if(!fin.is_open())
            {
              cout<<"Can't perform the desired operations"<<endl;
              cout<<"File \"Employees.csv\" not found!!"<<endl;
              goto CP1;
            }
            else
               fin.close();

            CP4: cout<<"Please select the operation:"<<endl;
            cout<<"1- Add New Employee"<<endl;
            cout<<"2- Update Existing Employee"<<endl;
            cout<<"3- Delete Existing Employee"<<endl;
            cout<<"4- Display All Employees"<<endl;
            cout<<"5- Display Specific Employee"<<endl;
            cout<<"6- Display Cars Currently Rented by a Specific Employee"<<endl;
            cout<<"7- Clear Dues"<<endl;
            cout<<"8- Return Car"<<endl;
            cout<<"9- Back to main menu"<<endl;
            cin>>operation;

            switch(operation)               
            {
              case 1:
                    employee.new_employee();
                    cout<<"Successfully added new employee!\n"<<endl;
                    break;
              case 2: 
                    cout<<"Enter Employee User-ID:"<<endl;
                    cin>>user_ID;
                    found = employee.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP4;
                    }
                    else
                    {
                      employeeDB.Display_row(user_ID); 
                      cout<<"Are you sure you want to update? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        CP5: cout<<"Which field do you want to update?"<<endl;
                        cout<<"1- Employee User ID"<<endl;
                        cout<<"2- Employee Name"<<endl;
                        cout<<"3- Employee password"<<endl;
                        cin>>field;
                        switch(field)
                        {
                          case 1:
                            cout<<"Enter new user ID [single word]"<<endl;
                            cin>>employee.user_id;
                            break;
                          case 2:
                            cout<<"Enter new name [single word]"<<endl;
                            cin>>employee.name;
                            break;
                          case 3:
                            cout<<"Enter new password [cannot contain spaces]"<<endl;
                            cin>>employee.password;
                            break;
                          default:
                            cout<<"Invalid choice!"<<endl;
                        }
                        employeeDB.Delete_row(user_ID); 
                        user_ID = employee.user_id; 
                        employeeDB.insert_employee(employee.user_id, employee.name, employee.password, employee.NumCurrRentedCars, employee.TotalRentedCars, employee.TotalDamagedCars, employee.TotalDelayDays, employee.dues);
                        cout<<"Successfully updated!\n"<<endl;
                        cout<<"Do you want to continue updating? [y=yes/ n=any other character]"<<endl;
                        cin>>c;
                        if(c=='y' || c=='Y')
                          goto CP5;
                      }
                      else
                        goto CP4;
                    }
                    break;
              case 3:
                    cout<<"Enter Employee ID:"<<endl;
                    cin>>user_ID;
                    found = employeeDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP4;
                    }
                    else
                    {
                      found = RcarDB.DisplayCarsOfRenter("Employee",user_ID);
                      if(found)
                      {
                        cout<<"The employee currently rents these car(s)"<<endl;
                        cout<<"The employee cannot be deleted unless these car(s) are returned\n"<<endl;
                        goto CP4;
                      }
                      cout<<"Are you sure you want to delete? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        employeeDB.Delete_row(user_ID);  
                        cout<<"Successfully deleted Employee!\n"<<endl;
                      }
                      else
                        goto CP4;
                    }
                    break;
              case 4:
                    employeeDB.Display_all();
                    break;
              case 5:
                    cout<<"Enter Employee ID:"<<endl;
                    cin>>user_ID;
                    found = employeeDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP4;
                    }
                    break;
              case 6:
                    cout<<"Enter Employee ID:"<<endl;
                    cin>>user_ID;
                    found = employee.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP4;
                    }
                    found = RcarDB.DisplayCarsOfRenter("Employee",user_ID);
                    if(!found)
                      cout<<"The employee has not rented any car currently"<<endl;
                    break;
              case 7:
                    cout<<"Enter Employee ID:"<<endl;
                    cin>>user_ID;
                    found = employee.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP4;
                    }
                    cout<<"Pending amount to be paid:"<<employee.dues<<endl;
                    CP6: cout<<"Enter amount paid now:"<<endl;
                    cin>>amount_paid;
                    if(employee.dues>=amount_paid)
                    {
                      employee.clear_dues(amount_paid);
                      cout<<"Successfully cleared dues!!"<<endl;
                    }
                    else
                    {
                      cout<<"Amount paid exceeds fine due"<<endl;
                      goto CP6;
                    }
                    break;
              case 8:
                    cout<<"Enter Employee ID:"<<endl;
                    cin>>user_ID;
                    found = employee.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP4;
                    }
                    else
                    {
                      cout<<"Which car was returned?"<<endl;
                      cin>>car_returned;
                      car_found = Rcar.get_obj(car_returned);
                      if(!car_found)
                      {
                      cout<<"Car not found\n"<<endl;
                      goto CP4;
                      }
                      else
                      {
                      RcarDB.Display_row(car_returned);
                      cout<<"Was the car returned after the due date? [y=yes/ any other character = no]"<<endl;
                      cin>>late;
                      if(late == 'y' || late == 'Y')
                      {
                        cout<<"How many extra days was the car used for?"<<endl;
                        cin>>fine1;
                        fine = fine1*employee.fine_rate;
                      }
                      cout<<"Was the car damaged when returned? [y = damaged/ any other character = not damaged]"<<endl;
                      cin>>damaged;
                      if(damaged == 'y' || damaged == 'Y')
                      {
                          cout<<"How much fine is imposed for the damage? (in Rs.)"<<endl;
                          cin>>fine2;
                          fine+=fine2;
                          condition = 0;
                      }
                      employeeDB.Delete_row(user_ID);
                      employeeDB.insert_employee(employee.user_id,employee.name,employee.password,employee.NumCurrRentedCars -1,employee.TotalRentedCars,employee.TotalDamagedCars + (int)(fine2>0),employee.TotalDelayDays + (int)(fine1>0),employee.dues+fine);
                      RcarDB.Delete_row(car_returned);
                      CarDB.insert_car(Rcar.Car_ID, Rcar.model, Rcar.rent, condition);
                      cout<<"Successfully returned car!\n"<<endl;
                      }
                    }
                    break;
              case 9:
                    break;
              default:
                    cout<<"Invalid choice\n"<<endl;
                    goto CP4;
            }
            break;
    case 4:
            fin.open("Customers.csv");
            if(!fin.is_open())
            {
              cout<<"Can't perform the desired operations"<<endl;
              cout<<"File \"Customers.csv\" not found!!"<<endl;
              goto CP1;
            }
            else
               fin.close();

            CP14: cout<<"Please select the operation:"<<endl;
            cout<<"1- Add New Customer"<<endl;
            cout<<"2- Update Existing Customer"<<endl;
            cout<<"3- Delete Existing Customer"<<endl;
            cout<<"4- Display All Customers"<<endl;
            cout<<"5- Display Specific Customer"<<endl;
            cout<<"6- Display Cars Rented by a Specific Customer"<<endl;
            cout<<"7- Clear Dues"<<endl;
            cout<<"8- Return Car"<<endl;
            cout<<"9- Back to main menu"<<endl;
            cin>>operation;

            switch(operation)               
            {
              case 1:
                    customer.new_customer();
                    cout<<"Successfully added new customer!\n"<<endl;
                    break;
              case 2: 
                    cout<<"Enter customer User-ID:"<<endl;
                    cin>>user_ID;
                    found = customer.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP14;
                    }
                    else
                    {
                      customerDB.Display_row(user_ID); 
                      cout<<"Are you sure you want to update? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        CP15: cout<<"Which field do you want to update?"<<endl;
                        cout<<"1- Customer User ID"<<endl;
                        cout<<"2- Customer Name"<<endl;
                        cout<<"3- Customer password"<<endl;
                        cin>>field;
                        switch(field)
                        {
                          case 1:
                            cout<<"Enter new user ID [single word]"<<endl;
                            cin>>customer.user_id;
                            break;
                          case 2:
                            cout<<"Enter new name [single word]"<<endl;
                            cin>>customer.name;
                            break;
                          case 3:
                            cout<<"Enter new password [cannot contain spaces]"<<endl;
                            cin>>customer.password;
                            break;
                          default:
                            cout<<"Invalid choice!"<<endl;
                        }
                        customerDB.Delete_row(user_ID); 
                        user_ID = customer.user_id; 
                        customerDB.insert_customer(customer.user_id, customer.name, customer.password, customer.NumCurrRentedCars, customer.TotalRentedCars, customer.TotalDamagedCars, customer.TotalDelayDays, customer.dues);
                        cout<<"Successfully updated!\n"<<endl;
                        cout<<"Do you want to continue updating? [y=yes/ n=any other character]"<<endl;
                        cin>>c;
                        if(c=='y' || c=='Y')
                          goto CP15;
                      }
                      else
                        goto CP14;
                    }
                    break;
              case 3:
                    cout<<"Enter customer ID:"<<endl;
                    cin>>user_ID;
                    found = customerDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP14;
                    }
                    else
                    {
                      found = RcarDB.DisplayCarsOfRenter("Customer",user_ID);
                      if(found)
                      {
                        cout<<"The customer currently rents these cars"<<endl;
                        cout<<"The customer cannot be deleted unless these cars are returned\n"<<endl;
                        goto CP4;
                      }
                      cout<<"Are you sure you want to delete? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        customerDB.Delete_row(user_ID);  
                        cout<<"Successfully deleted customer!\n"<<endl;
                      }
                      else
                        goto CP14;
                    }
                    break;
              case 4:
                    customerDB.Display_all();
                    break;
              case 5:
                    cout<<"Enter customer ID:"<<endl;
                    cin>>user_ID;
                    found = customerDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP14;
                    }
                    break;
              case 6:
                    cout<<"Enter customer ID:"<<endl;
                    cin>>user_ID;
                    found = customer.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP14;
                    }
                    found = RcarDB.DisplayCarsOfRenter("Customer",user_ID);
                    if(!found)
                        cout<<"The customer has not rented any car currently"<<endl;
                    break;
              case 7:
                    cout<<"Enter customer ID:"<<endl;
                    cin>>user_ID;
                    found = customer.get_obj(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP14;
                    }
                    cout<<"Pending amount to be paid:"<<customer.dues<<endl;
                    CP16: cout<<"Enter amount paid now:"<<endl;
                    cin>>amount_paid;
                    if(customer.dues>=amount_paid)
                    {
                      customer.clear_dues(amount_paid);
                      cout<<"Successfully cleared dues!\n"<<endl;
                    }
                    else
                    {
                      cout<<"Amount paid exceeds fine due"<<endl;
                      goto CP16;
                    }
                    break;
              case 8:
                    cout<<"Enter customer ID:"<<endl;
                    cin>>user_ID;
                    found = customerDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found!\n"<<endl;
                      goto CP14;
                    }
                    else
                    {
                      cout<<"Which car was returned?"<<endl;
                      cin>>car_returned;
                      car_found = Rcar.get_obj(car_returned);
                      if(!car_found)
                      {
                      cout<<"Car not found\n"<<endl;
                      goto CP14;
                      }
                      else
                      {
                      RcarDB.Display_row(car_returned);
                      cout<<"Was the car returned after the due date? [y=yes/ any other character = no]"<<endl;
                      cin>>late;
                      if(late == 'y' || late == 'Y')
                      {
                        cout<<"How many extra days was the car used for?"<<endl;
                        cin>>fine1;
                        fine = fine1*customer.fine_rate;
                      }
                      cout<<"Was the car damaged when returned? [y = damaged/ any other character = not damaged]"<<endl;
                      cin>>damaged;
                      if(damaged == 'y' || damaged == 'Y')
                      {
                          cout<<"How much fine is imposed for the damage?(in Rs.)"<<endl;
                          cin>>fine2;
                          fine+=fine2;
                          condition = 0;
                      }
                      customerDB.Delete_row(user_ID);
                      customerDB.insert_customer(customer.user_id,customer.name,customer.password,customer.NumCurrRentedCars -1,customer.TotalRentedCars,customer.TotalDamagedCars + (int)(fine2>0),customer.TotalDelayDays + (int)(fine1>0),customer.dues+fine);
                      RcarDB.Delete_row(car_returned);
                      CarDB.insert_car(Rcar.Car_ID, Rcar.model, Rcar.rent, condition);
                      cout<<"Successfully returned car!"<<endl;
                      }
                    }
                    break;
              case 9:
                    break;
              default:
                    cout<<"Invalid choice\n"<<endl;
                    goto CP14;
            }
            break;
    case 5:
            fin.open("Managers.csv");
            if(!fin.is_open())
            {
              cout<<"Can't perform the desired operations"<<endl;
              cout<<"File \"Managers.csv\" not found!!"<<endl;
              goto CP1;
            }
            else
               fin.close();

            CP9: cout<<"Please select the operation:"<<endl;
            cout<<"1- Add New Manager"<<endl;
            cout<<"2- Update details of self"<<endl;
            cout<<"3- Delete Manager"<<endl;
            cout<<"4- Display All Managers"<<endl;
            cout<<"5- Back to main menu"<<endl;
            cin>>operation;

            switch(operation)
            {
                case 1:
                  new_manager();
                  cout<<"Successfully added new manager!\n"<<endl;
                  break;
                case 2:
                  CP10: cout<<"Which field do you wish to update?"<<endl;
                  cout<<"1- User-ID"<<endl;
                  cout<<"2- Name"<<endl;
                  cout<<"3- Password"<<endl;
                  cin>>field;
                  managerDB.Delete_row(user_id); 

                  switch(field)
                  {
                          case 1:
                            cout<<"Enter new user ID [single word]"<<endl;
                            cin>>user_id;
                            break;
                          case 2:
                            cout<<"Enter new name [single word]"<<endl;
                            cin>>name;
                            break;
                          case 3:
                            cout<<"Enter new password [cannot contain spaces]"<<endl;
                            cin>>password;
                            break;
                          default:
                            cout<<"Invalid choice!"<<endl;
                  } 
                  managerDB.insert_manager(user_id, name, password);
                  cout<<"Successfully updated!\n"<<endl;
                  cout<<"Do you want to continue updating? [y=yes/ n=any other character]"<<endl;
                  cin>>c;
                  if(c=='y' || c=='Y')
                      goto CP10;
                  break;

                case 3:
                    NumOfManagers = managerDB.count();
                    if(NumOfManagers == 1)
                    {
                       cout<<"Can't delete manager, since only 1 manager is present\n"<<endl;
                       goto CP9;
                    }
                    cout<<"Enter Manager ID:"<<endl;
                    cin>>user_ID;
                    found = managerDB.Display_row(user_ID);
                    if(!found)
                    {
                      cout<<"ID not found\n"<<endl;
                      goto CP9;
                    }
                    else
                    {
                      cout<<"Are you sure you want to delete? [y=yes/ any other character= no]"<<endl;
                      cin>>c;
                      if(c=='y'|| c=='Y')
                      {
                        managerDB.Delete_row(user_ID);  
                        cout<<"Successfully deleted Manager!\n"<<endl;
                      }
                      else
                        goto CP9;
                    }
                    break;
                case 4:
                      managerDB.Display_all();
                      break;
                case 5:
                      break;
                default:
                      cout<<"Invalid choice!\n"<<endl;
                      goto CP9;
            }
            break;
    case 6:
            cout<<"Are you sure you want to log out? [y=yes/ any other character= no]"<<endl;
            cin>>c;
            if(c=='y'|| c=='Y')
            {
              cout<<"Successfully Logged out\n"<<endl;
              return;
            }
            break;
    default:
            cout<<"Invalid choice"<<endl;   
   }
   goto CP1;
}
// END OF PART-5..................................................................................

// PART-6 MEMBER FUNCTION DEFINITIONS OF CLASS EMPLOYEE...........................................
int Employee:: get_obj(string Employee_ID)
{
    ifstream fin;
    fin.open("Employees.csv");
    fin.seekg(0);
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == Employee_ID)
      {
        found = 1;
        int i=0;
        user_id = word;
        getline(ss,word,',');
        name = word;
        getline(ss,word,',');
        password = word;
        getline(ss,word,',');
        NumCurrRentedCars = stoi(word);
        getline(ss,word,',');
        TotalRentedCars = stoi(word);
        getline(ss,word,',');
        TotalDamagedCars = stoi(word);
        getline(ss,word,',');
        TotalDelayDays = stoi(word);
        getline(ss,word,',');
        dues = stof(word);
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}

void Employee::new_employee()
{
  cout<<"Please enter new employee details"<<endl;
  cout<<"Enter Name [single word]"<<endl;
  cin>>name;
  cout<<"Enter User ID [single word]"<<endl;
  cin>>user_id;
  cout<<"Enter password [cannot contain spaces]"<<endl;
  cin>>password;
  Employee_database employeeDB;
  employeeDB.insert_employee(user_id, name, password, 0, 0, 0, 0, 0.0);
}

void Employee:: clear_dues(float amt_paid)
{
  Employee_database employeeDB;
  employeeDB.Delete_row(user_id);  
  employeeDB.insert_employee(user_id, name, password, NumCurrRentedCars, TotalRentedCars, TotalDamagedCars, TotalDelayDays, dues-amt_paid);
}

void Employee:: employee_portal()
{
  char c;
  int choice;
  int found =0;
  int NumDays;
  int DueDate, DueMonth, DueYear;
  float rent;
  float record_score;
  string Car_ID;
  Employee_database employeeDB;
  RentedCar_database RcarDB;
  Car_database CarDB;
  Car car;

  CP21:cout<<"Please select your choice"<<endl;
  cout<<"1- View Profile"<<endl;
  cout<<"2- View Currently Rented Cars"<<endl;
  cout<<"3- View Available Cars"<<endl;
  cout<<"4- Rent a Car"<<endl;
  cout<<"5- Logout"<<endl;
  cout<<"Please contact manager to update profile,clear dues or return a car"<<endl;
  cin>>choice;

  switch (choice)
  {
    case 1:
          employeeDB.Display_row(user_id);
          break;
    case 2:
          found = RcarDB.DisplayCarsOfRenter("Employee",user_id);
          if(!found)
          {
            cout<<"You have not rented any car currently\n"<<endl;
            goto CP21;
          }
          found=0;
          break;
    case 3:
          CarDB.Display_all();                   
          break;
    case 4:
          cout<<"Congratulations! Employees now have "<<EMPLOYEE_DISCOUNT_PERCENT<<"% discount on all cars!!"<<endl;
          if(TotalRentedCars>0)
              record_score = (TotalRentedCars - TotalDamagedCars - (TotalDelayDays/10.0))/TotalRentedCars;
          else
              record_score = MIN_RECORD_SCORE_FOR_AN_EMPLOYEE_TO_RENT_A_CAR + 0.1;  //Refer documentation at the beginning of this file
          if(NumCurrRentedCars < (record_score - minRecordScore)*10)
          {
              cout<<"Enter Car ID"<<endl;
              cin>>Car_ID;
              found = car.get_obj(Car_ID);
              if(!(found))
              {
                cout<<"ID not found"<<endl;
                goto CP21;
              }
              else
              {
                if(!(car.get_condition(Car_ID)))
                {
                  cout<<"The car is currently in a damaged condition. You can't rent the car until its repaired."<<endl;
                  goto CP21;
                }
                cout<<"Please see the details of the car:"<<endl;
                CarDB.Display_row(Car_ID);
                cout<<"Are you sure you want to rent? [y=yes/ any other character=no]"<<endl;
                cin>>c;
                if(c=='y'|c=='Y')
                {
                    CP22:cout<<"Enter number of days the car would be rented"<<endl;
                    cin>>NumDays;
                    if(NumDays<0)
                    {
                      cout<<"Invalid value"<<endl;
                      goto CP22;
                    }
                    cout<<"Enter Due Date [1-31]"<<endl;
                    CP31:cin>>DueDate;
                    if(!(DueDate>=1 && DueDate<=31))
                    {
                      cout<<"Invalid due date, please enter correct due date [1-31]"<<endl;
                      goto CP31;
                    }
                    cout<<"Enter Due Month[1-12]"<<endl;
                     CP32: cin>>DueMonth;
                    if(!(DueMonth>=1 && DueMonth<=12))
                    {
                      cout<<"Invalid due month, please enter correct due month [1-12]"<<endl;
                      goto CP32;
                    }
                    cout<<"Enter Due Year"<<endl;
                    cin>>DueYear;
                    rent = (car.rent - (car.rent*(discount_percent/100.0)))*NumDays;
                    RcarDB.insert_rented_car(car.Car_ID,car.model,car.rent,"Employee",user_id,DueDate,DueMonth,DueYear);
                    employeeDB.Delete_row(user_id);
                    employeeDB.insert_employee(user_id,name,password,NumCurrRentedCars+1,TotalRentedCars+1,TotalDamagedCars,TotalDelayDays,dues+rent);
                    CarDB.Delete_row(Car_ID);
                    cout<<"Successfully rented!\n"<<endl;
                }
                else
                  goto CP21;
              }
          }
          else
          {
            cout<<"You have already rented "<<NumCurrRentedCars<<" cars"<<endl;
            cout<<"Sorry! You can't rent more than"<< NumCurrRentedCars<<"based on your record\n"<<endl;
          }
          break;
    case 5:
          cout<<"Logged out successfully!\n"<<endl;
          return;
    default:
          cout<<"Invalid choice"<<endl; 
  }
  goto CP21;
}
// END OF PART-6..................................................................................

// PART-7 MEMBER FUNCTION DEFINITIONS OF CLASS CUSTOMER...........................................
int Customer:: get_obj(string Customer_ID)
{
    ifstream fin;
    fin.open("Customers.csv");
    fin.seekg(0);
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      if(word == Customer_ID)
      {
        found = 1;
        int i=0;
        user_id = word;
        getline(ss,word,',');
        name = word;
        getline(ss,word,',');
        password = word;
        getline(ss,word,',');
        NumCurrRentedCars = stoi(word);
        getline(ss,word,',');
        TotalRentedCars = stoi(word);
        getline(ss,word,',');
        TotalDamagedCars = stoi(word);
        getline(ss,word,',');
        TotalDelayDays = stoi(word);
        getline(ss,word,',');
        dues = stof(word);
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}

void Customer::new_customer()
{
  cout<<"Please enter new customer details"<<endl;
  cout<<"Enter Name [single word]"<<endl;
  cin>>name;
  cout<<"Enter User ID [single word]"<<endl;
  cin>>user_id;
  cout<<"Enter password [cannot contain spaces]"<<endl;
  cin>>password;
  Customer_database custDB;
  custDB.insert_customer(user_id, name, password, 0, 0, 0, 0, 0.0);
}

void Customer:: clear_dues(float amt_paid)
{
  Customer_database customerDB;
  customerDB.Delete_row(user_id);  
  customerDB.insert_customer(user_id, name, password, NumCurrRentedCars, TotalRentedCars, TotalDamagedCars, TotalDelayDays, dues-amt_paid);
}

void Customer:: customer_portal()
{
  char c;
  int choice;
  int found =0;
  int NumDays;
  int DueDate, DueMonth, DueYear;
  float rent;
  float record_score;
  string Car_ID;
  Customer_database customerDB;
  RentedCar_database RcarDB;
  Car_database CarDB;
  Car car;

  CP23:cout<<"Please select your choice"<<endl;
  cout<<"1- View Profile"<<endl;
  cout<<"2- View Currently Rented Cars"<<endl;
  cout<<"3- View Available Cars"<<endl;
  cout<<"4- Rent a Car"<<endl;
  cout<<"5- Logout"<<endl;
  cout<<"Please contact manager to update profile,clear dues or return a car"<<endl;
  cin>>choice;

  switch (choice)
  {
    case 1:
          customerDB.Display_row(user_id);
          break;
    case 2:
          found = RcarDB.DisplayCarsOfRenter("Customer",user_id);
          if(!found)
          {
            cout<<"You have not rented any car currently\n"<<endl;
            goto CP23;
          }
          found=0;
          break;
    case 3:
          CarDB.Display_all();                   
          break;
    case 4:
          if(TotalRentedCars>0)
              record_score = (TotalRentedCars - TotalDamagedCars - (TotalDelayDays/10.0))/TotalRentedCars;
          else
              record_score = MIN_RECORD_SCORE_FOR_A_CUSTOMER_TO_RENT_A_CAR + 0.1;  //Refer documentation at the beginning of this file
          if(NumCurrRentedCars < (record_score - minRecordScore)*10)
          {
              cout<<"Enter Car ID"<<endl;
              cin>>Car_ID;
              found = car.get_obj(Car_ID);
              if(!(found))
              {
                cout<<"ID not found"<<endl;
                goto CP23;
              }
              else
              {
                cout<<"Please see the details of the car:"<<endl;
                CarDB.Display_row(Car_ID);
                cout<<"Are you sure you want to rent? [y=yes/ any other character=no]"<<endl;
                cin>>c;
                if(c=='y'|c=='Y')
                {
                    CP24:cout<<"Enter number of days the car would be rented"<<endl;
                    cin>>NumDays;
                    if(NumDays<0)
                    {
                      cout<<"Invalid value"<<endl;
                      goto CP24;
                    }
                    cout<<"Enter Due Date [1-31]"<<endl;
                    cin>>DueDate;
                    cout<<"Enter Due Month[1-12]"<<endl;
                    cin>>DueMonth;
                    cout<<"Enter Due Year"<<endl;
                    cin>>DueYear;
                    rent = (car.rent - (car.rent*(discount_percent/100.0)))*NumDays;
                    RcarDB.insert_rented_car(car.Car_ID,car.model,car.rent,"Customer",user_id,DueDate,DueMonth,DueYear);
                    customerDB.Delete_row(user_id);
                    customerDB.insert_customer(user_id,name,password,NumCurrRentedCars+1,TotalRentedCars+1,TotalDamagedCars,TotalDelayDays,dues+rent);
                    CarDB.Delete_row(Car_ID);
                    cout<<"Successfully rented!\n"<<endl;
                }
                else
                  goto CP23;
              }
          }
          else
          {
            cout<<"You have already rented "<<NumCurrRentedCars<<" cars"<<endl;
            cout<<"Sorry! You can't rent more than"<< NumCurrRentedCars<<"based on your record\n"<<endl;
          }
          break;
    case 5:
          cout<<"Logged out successfully!\n"<<endl;
          return;
    default:
          cout<<"Invalid choice"<<endl; 
  }
  goto CP23;
}
// END OF PART-7..................................................................................

// PART-8 MEMBER FUNCTION DEFINITIONS OF CLASS DATABASE...........................................
int Database:: Display_row(string ID)
{
  string file = get_file();
  ifstream fin;
  fin.open(file);
  fin.seekg(0);
  vector<string> headings;                 
  string line,word;

  getline(fin,line);
  stringstream ss(line);
  while(getline(ss,word,','))
    headings.push_back(word);
  int i=0, found =0;
  while(getline(fin,line))
  {
    stringstream ss(line);
    getline(ss,word,',');
    if(word == ID)
    {
      cout<<"\nDetails:\n"<<endl;
      cout<<headings[i++]<<": "<<word<<endl;
      while(getline(ss,word,','))
        cout<<headings[i++]<<": "<<word<<endl;
      found = 1;
      cout<<endl;
      break;
    }
  }

  fin.close();

  if(!found)
    return 0;
  else
    return 1;
}

void Database:: Display_all()
{
  string file = get_file();
  ifstream fin;
  fin.open(file);
  fin.seekg(0);
  vector<string> headings;
  string word;
  string line;
  int found=0;

  getline(fin,line);
  stringstream ss(line);
  while(getline(ss,word,','))
    headings.push_back(word);
  int num_col = headings.size();
  int i;
  while(getline(fin,line))
  {
    if(line=="")
        continue;
    found = 1;
    i=0;
    stringstream ss1(line);
    cout<<"....................................................................................."<<endl;
    while(i<num_col)
    {
      getline(ss1,word,',');
      cout<<headings[i++]<<": "<<word<<endl;
    }
    cout<<".....................................................................................\n"<<endl;
  }

  fin.close();

  if(!found)
    cout<<"No entries present"<<endl;
}

int Database:: Delete_row(string ID)
{
  string file = get_file();
  ifstream fin;
  ofstream fout;
  fin.open(file);   
  fin.seekg(0);                  
  fout.open("temp.csv");
  fout.seekp(0);           
  int found=0;
  string line,word;

  while(getline(fin,line))
  {
    stringstream ss(line);
    getline(ss,word,',');
    if(word != ID)
      fout<<line<<"\n";
    else
      found = 1;
  }

  fin.close();
  fout.close();

  if(!found)
  {
    cout<<"No such ID found\n"<<endl;
    return 0;
  }
  else
  {
    char* array1 = new char[file.length()+1];
    strcpy(array1, file.c_str());
    remove(array1);
    rename("temp.csv", array1);
    return 1;
  }
}

int Database:: count()
{
  string file = get_file();
  ifstream fin;
  fin.open(file);
  fin.seekg(0);
  string line;
  int count = 0;

  getline(fin,line);
  while(getline(fin,line))
  {
    if(line!="")
        count++;
  }
  
  fin.close();
  return count;
}
// END OF PART-8 .................................................................................

// PART-9 MEMBER FUNCTIONS OF CLASS CAR_DATABASE.................................................
void Car_database:: insert_car(string Car_ID, string model, float rent, bool condition)
{
    ofstream file_write;
    file_write.open("Unrented_Cars.csv",ios::app);
    file_write<<Car_ID<<","<<model<<","<<rent<<","<<condition<<"\n";
    file_write.close();
}

string Car_database::get_file()
{
  return Car_database::file;
}
// END OF PART-9.................................................................................

// PART-10 MEMBER FUNCTIONS OF CLASS RENTED_CAR_DATABASE..........................................
int RentedCar_database:: Display_DueDate(string Rcar_ID)
{
    ifstream fin;
    fin.open("Rented_Cars.csv");
    fin.seekg(0);
    string word, line;
    int found = 0;

    while(getline(fin,line))
    {
      stringstream ss(line);
      getline(ss,word,',');
      int i=0;
      if(word == Rcar_ID)
      {
        found = 1;
        while(i<4)
        {
          getline(ss,word,',');
          i++;
        }
        getline(ss,word,',');
        cout<<"Due Date:"<<word<<endl;
        getline(ss,word,',');
        cout<<"Due Month:"<<word<<endl;
        getline(ss, word, ',');
        cout<<"Due Year:"<<word<<"\n"<<endl;
        fin.close();
        return 1;
      }
    }

    fin.close();
    if(!found)
       return 0;
}

int RentedCar_database:: DisplayCarsOfRenter(string renter_type, string user_ID)
{
  ifstream fin;
  fin.open("Rented_Cars.csv");
  fin.seekg(0);
  string line,line1,word,word1,word2;
  int found = 0;
  getline(fin,line);

  while(getline(fin,line))
  {
    line1 = line;
    stringstream ss(line);
    stringstream ss1(line1);
    getline(ss,word1,',');
    getline(ss,word,',');
    getline(ss,word,',');
    getline(ss,word,',');
    getline(ss,word2,',');
    if(word2 == user_ID && word == renter_type)
    {
      found = 1;
      RentedCar_database RcarDB;
      RcarDB.Display_row(word1);
    }
  }

  fin.close();
  return found;
}

void RentedCar_database:: insert_rented_car(string car_id, string model, float rent, string renter_type, string renter_id, int DueDate, int DueMonth, int DueYear)
{
  ofstream fwrite;
  fwrite.open("Rented_Cars.csv",ios::app);
  fwrite<<car_id<<","<<model<<","<<rent<<","<<renter_type<<","<<renter_id<<","<<DueDate<<","<<DueMonth<<","<<DueYear<<"\n";
  fwrite.close();
}

string RentedCar_database:: get_file()
{
  return RentedCar_database:: file;
}
// END OF PART-10.................................................................................

// PART-11 MEMBER FUNCTIONS OF CLASS MANAGER_DATABASE.............................................
void Manager_database:: insert_manager(string user_ID, string name, string password)
{
    ofstream file_write;
    file_write.open("Managers.csv",ios::app);
    file_write<<user_ID<<","<<name<<","<<password<<"\n";
    file_write.close();
}

string Manager_database::get_file()
{
  return Manager_database:: file;
}
// END OF PART-11.................................................................................

// PART-12 MEMBER FUNCTIONS OF CLASS EMPLOYEE_DATABASE............................................
void Employee_database:: insert_employee(string user_id, string name, string password, int NumCurrRentedCars, int TotalRentedCars, int TotalDamagedCars, int TotalDelayDays, float dues)
{
  ofstream file_write;
  file_write.open("Employees.csv",ios::app);
  file_write<<user_id<<","<<name<<","<<password<<","<<NumCurrRentedCars<<","<<TotalRentedCars<<","<<TotalDamagedCars<<","<<TotalDelayDays<<","<<dues<<"\n";
  file_write.close();
}

string Employee_database:: get_file()
{
  return Employee_database::file;
}
// END OF PART-12.................................................................................

// PART-13 MEMBER FUNCTIONS OF CLASS CUSTOMER_DATABASE............................................
void Customer_database:: insert_customer(string user_id, string name, string password, int NumCurrRentedCars, int TotalRentedCars, int TotalDamagedCars, int TotalDelayDays, float dues)
{
  ofstream file_write;
  file_write.open("Customers.csv",ios::app);
  file_write<<user_id<<","<<name<<","<<password<<","<<NumCurrRentedCars<<","<<TotalRentedCars<<","<<TotalDamagedCars<<","<<TotalDelayDays<<","<<dues<<"\n";
  file_write.close();
}

string Customer_database::get_file()
{
  return Customer_database:: file;
}
// END OF PART-13.................................................................................

// PART-14 MEMBER FUNCTIONS OF CLASS USER.........................................................
int User:: user_select()
{
    int user;
    cout<<"Enter your choice:"<<endl;
    cout<<"1-Manager"<<endl;
    cout<<"2-Employee"<<endl;
    cout<<"3-Customer"<<endl;
    cin>>user;
    return user;
}

string User:: login(int user)
{
    string user_id;
    string password;
    ifstream fin;
    int file_found=0;
    int found=0;
    int valid=0;
    string line,word;

    USER_ID: cout<<"Enter User ID"<<endl;
    cin>>user_id;

    PASSWORD:cout<<"Enter password"<<endl;
    cin>>password;

    if(user == 1)
      fin.open("Managers.csv");
    else if(user == 2)
      fin.open("Employees.csv");
    else
      fin.open("Customers.csv");

    if(!fin.is_open()) 
    {                                  
      cout<<"Sorry, file does not exist!\n";
      return "";
    }

    fin.seekg(0);
    while(getline(fin,line))
    {
        stringstream ss(line);
        getline(ss,word,',');
        if(word==user_id)
        {
            found=1;
            getline(ss,word,',');
            getline(ss,word,',');
            if(word==password)
              valid=1;
            break;
        }
    }

    if(found==0)
    {
      cout<<"User not found!\n"<<endl;
      fin.close();
      goto USER_ID;
    }
    else
    {
        if(valid==0)
        {
          cout<<"Invalid password!\n"<<endl;
          fin.close();
          goto PASSWORD;
        }
        else
          cout<<"Logged in successfully!!\n"<<endl;
    }
    fin.close();
    return user_id;
}
// END OF PART-14.................................................................................

// PART-15 THE MAIN() FUNCTION ...................................................................
int main()
{
    cout<<"Welcome to Car Rental System :)"<<endl;
    int user,choice;
    User User_obj;
    string User_ID;
    Manager manager;
    Customer customer;
    Employee employee;

    CP0: user = User_obj.user_select();

    while(!(user>=1 && user <=3))
    {
        cout<<"Invalid choice!! \n"<<endl;
        user = User_obj.user_select();
    }
    
    User_ID=User_obj.login(user);

    if(User_ID=="")
        goto CP0;

    switch(user)
    {
        case 1:
             manager.get_obj(User_ID);
             manager.manager_portal();
             break;
        case 2:
             employee.get_obj(User_ID);
             employee.employee_portal();
             break;
        case 3:
             customer.get_obj(User_ID);
             customer.customer_portal();
             break;
    }
    goto CP0;
    return 0;
}
// END OF PART-15 ................................................................................
