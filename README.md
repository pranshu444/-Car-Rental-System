# Car_rental_system

This is a Car rental system implemented purely using C++. Here, we dive into object oriented programming. Various concepts such as data encapsulation, polymorphism and inheritance have been implemented through the use of access specifiers, friend functions, virtual functions, static variables and multi-level inheritance of classes. CSV files have used to maintain databases. File handling has been used to handle databases.

## Structure

All the database files can be found (with some initial entries) in the 'Database files' directory. The Car_rental_sysytem.cpp file constitutes the main controller of our system.

## How to run the system?

### Prerequisites
C++ compiler

### Steps
1) Download the Car_rental_system.cpp file and database files from this repository.
2) Ensure all of them reside in the same directory.
3) Run the Car_rental_system.cpp file and interact with the system on your terminal.

## About the system

The system is used to handle data of cars, customers, employees and managers. Both customers and employees can rent cars. Employees, however, are provided with extra benefits like discounts, lesser fines and a higher cap on the number of cars that can be rented. Managers handle information of cars, customers and employees. At least one manager should always exist.

The system interacts with three kinds of users. These users are listed below:

### Manager:

A manager can add new cars. He can update and delete the cars as well. He can view rented cars along with the renter and due date. He cannot delete a rented car.

A manager can add new customers and employees. He can update their profile, clear their dues and get a rented car returned. He cannot delete a customer/employee until they return all cars they have rented.
At any time, he can view their details including the number of cars rented and the dues.

A manager can also add and delete other managers. However, he can update his own profile only. Note that the system does not allow deleting a manager when only one manager is left.

### Customer and Employee:

A customer/employee can view his profile, the details of the cars he is currently renting and the cars available for rent. He can rent a car provided his record score satisfies the necessary conditon. However, he has to contact a manager to return a car, clear his dues or to update his profile.

They are imposed a fine (in addition to the rent) for returning a rented car after the due date or damaging a rented car. These actions have a negative impact on their record score as well.

The record score is calculated as follows:

### Record score calculation for a given customer/employee:

   Let t be the total number of cars rented so far.
   Let d be the number of cars returned with damage.
   Let x be the number of days any rented car is used beyond the due date.

   Then, record score = (t-(d+x/10))/t

   Let m be the minimum record score required to rent a car. 

   An employee/ a customer can rent atmost n cars where n is the ceil of [(record_score-m)*10]. 
   For a new employee/customer, the record score used to rent his first car will be m+0.1.

### Assumptions:

1) The manager ensures the ID of no two users match. The ID of no two cars match.
2) The user gives the correct type of input. For example, when the user is expected to enter an integer, he enters an integer.
3) The customer/employee enters a valid and correct due date.

   The system handles inputs efficiently as long as their type is correct. It takes care of most of the edge cases. The system is quite efficient to be used as long as there are no deliberate intrusions from the user side. 
