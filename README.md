Car Rental System
This is a simple car rental system implemented in C++. It allows users to manage customers, employees, and cars. The system is designed to be modular, with separate classes for managing customers, employees, and cars.

Getting Started
To get started, extract the code into a seperate folde and compile the code using a C++ compiler. Once compiled, run the executable to launch the system.

Prerequisites
This system requires a C++ compiler to build and run.

Built With
C++11

Authors
Tanishq Maheshwari


Acknowledgments
Thanks to Prof. Indranil Saha for providing inspiration for this project.
Thanks to all the wonderful teaching assistants for reviewing this project.

Initial Database:

CUSTOMERS:
ID, NAME, PASSWORD, DUES, RECORD, CURRENTLY RENTED CARS(number), OTHER DETAILS 
1,C1,P1,0,40,0,NA
2,C2,P2,0,40,0,NA
3,C3,P3,0,40,0,NA
4,C4,P4,0,40,0,NA
5,C5,P5,0,40,0,NA
 
EMPLOYEES:
ID, NAME, PASSWORD, DUES, RECORD, CURRENTLY RENTED CARS(number), OTHER DETAILS
1,E1,P1,0,40,0,NA
2,E2,P2,0,40,0,NA
3,E3,P3,0,40,0,NA
4,E4,P4,0,40,0,NA
5,E5,P5,0,40,0,NA

CARS: 
ID, CONDITION, OTHER DETAILS, AVAILABILITY, CURRENTLY RENTED BY(Like "E1" for employee with id1), BOOK DATE, DUE DATE

1,CAR1,NA,AVAILABLE,NA,NA,NA
2,CAR2,NA,AVAILABLE,NA,NA,NA
3,CAR3,NA,AVAILABLE,NA,NA,NA
4,CAR4,NA,AVAILABLE,NA,NA,NA
5,CAR5,NA,AVAILABLE,NA,NA,NA

MANAGER ACCESS: ID= 1  , PASSWORD = manager
ASSUMPTIONS:
1. User won't tamper with given text files.
2. Some  checks like whether the corresponding model of the car has been entered during car return is assumed to be user's reponibility.
3. Most edge cases have been dealt with.
4. Apologies for any missed cases. Could have been left due to time constraint.
5. also IDs to objects are awarded automatically using counters stored in file "globals.txt". They are saved at end of main function. So properly exit program by following the instructions that are output otherwise data may become asynchronous.