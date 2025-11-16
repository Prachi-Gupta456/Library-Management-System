#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <ctime>
#include <iomanip>
#include <unordered_map>
using namespace std;

// ANSI CODES FOR COLORED UI ================

const string red  = "\033[31m";
const string orange = "\033[38;2;255;165;0m";
const string green  = "\033[32m";
const string blue  = "\033[34m";
const string yellow  = "\033[33m";
const string pink = "\033[35m";
const string bold = "\033[1m";
const string cyan = "\033[96m";
const string reset  = "\033[0m";

// ===========================================


string getCurrTime(int daysToAdd){
                  
                time_t now = time(0);

                now += daysToAdd * 24 * 60 * 60;

                tm* ltm = localtime(&now);

                stringstream ss;

                ss << 1900 + ltm->tm_year << "-"
                   << setw(2) << setfill('0') << 1 + ltm->tm_mon << "-"
                   << setw(2) << setfill('0') << ltm->tm_mday;

                return ss.str();
}

// Book Details--------
class Book{

    public:
             string title, author;
             int id, quantity;
            
};
// --------------------

// Library ------------
class Library{
  
    unordered_map<int,Book>booksById;
    unordered_map<int,string>dueDates;       // id -> dueDates
    public:
           
           void loadFromFile(){
                    ifstream fin("library_data.txt");

                    if(!fin)return;
                    
                    string line;

                    while(getline(fin,line)){

                        stringstream ss(line);
                        string id,title,author,qty,due;

                        getline(ss,id,',');
                        getline(ss,title,',');
                        getline(ss,author,',');
                        getline(ss,qty,',');
                        getline(ss,due,',');

                        Book bk;
                    
                    // exception handling----------------
                      try{           
                         
                        bk.id = stoi(id);
                        bk.quantity = stoi(qty); 

                      }
                      catch(const exception e) {
                          continue;
                      }
                    // ----------------------------------

                        bk.title = title;
                        bk.author = author;

                        // adding book to memory
                        if(due != ""){
                            dueDates[bk.id] = due;
                        }

                        booksById[bk.id] = bk;
    
                    }

                    fin.close();
           }


           Library(){
                 loadFromFile();   
           }

           bool isAvailable(int id){
                return booksById.find(id) != booksById.end();
           }
            int getBookCopiesInfo(int id){
                return booksById[id].quantity;
              }
           
           int dateToInt(string date , int st_idx , int len){
               return  stoi(date.substr(st_idx,len));
           }

           int calculateFine(string dueDate){

                   string today_date = getCurrTime(0);

                   int yr_diff = dateToInt(dueDate,0,4) - dateToInt(today_date,0,4);
                   int month_diff = dateToInt(dueDate,5,2) - dateToInt(today_date,5,2);
                   int day_diff = dateToInt(dueDate,8,2) - dateToInt(today_date,8,2);

                   int total_days = (yr_diff*365) + (month_diff*30) + (day_diff);

                   if(total_days <= 7)return 0;

                   int fine  = total_days*5;
                   return fine;

           }

            void  getBookDetails(int id){

              if(isAvailable(id)){

                    Book bk = booksById[id];
                    cout << "\n------------------------------\n";
                    cout << orange << "\nBook id : " << yellow << bk.id << reset;
                    cout <<  orange << "\nAuthor  : " << yellow << bk.author << reset;
                    cout <<  orange << "\nTitle   : " <<  yellow << bk.title << reset;
                    cout <<  orange << "\nQuantity: " <<  yellow << bk.quantity << reset;
                   
              }

              else{
                cout << red << "\nBook is not available!\n" << reset;
                cout << "\n---------------------\n";
              }
           }


           void addBook(Book& newBook){
                 
                 booksById[newBook.id] = newBook;

                 saveToFile();

                 cout << "\n---------------------";
                 
           }

           void deleteBook(int id){
                
               if(isAvailable(id)){

                    booksById.erase(id);
                    dueDates.erase(id);
    
                    cout << green <<"\nBook removed from Library!\n" << reset;
                    cout << "\n---------------------\n";

                    saveToFile();

               }

               else{
                   cout << red << "\nBook Id not Found!\n" << reset;
                   cout << "\n---------------------\n";
               }

           }

           void updateQuantity(int id,int new_qty){
                 
                    if(isAvailable(id)){
                            booksById[id].quantity = new_qty;
                            cout << green << "Quantity Updated successfully!" << reset;
                            cout << "\n---------------------\n";

                            saveToFile();
                    }
                    else{
                        cout << red << "\nBook Id not Found!\n" << reset;
                        cout << "\n---------------------\n";
                    }

           }

           bool issueBook(int id){

                     if(isAvailable(id)){
                        
                        int curr_qty =  booksById[id].quantity;

                        string dueDate = getCurrTime(7);

                        if(curr_qty > 0){
                            
                            dueDates[id] = dueDate;
                            booksById[id].quantity--;
                        
                            saveToFile();
                            return true;
                        }
                    }
                        return false;
           }

           void returnBook(int id){
                  
                   int fine = calculateFine(dueDates[id]);

                   if(fine > 0){
                     cout << yellow << "\n Book Returned Late!\n" << reset;
                     cout << yellow << "Late Fine Charge is 5 rupees per Day!\n" << reset;
                     cout << red << "Fine Charged : " << fine <<"\n" << reset ;
                   }

                   else{
                      cout << "--------------------------";
                      cout << green << "\n Book Returned On Time!" << reset;
                   }

                  dueDates.erase(id);

                  booksById[id].quantity++;
                  cout << "\n----------------------------\n";

                  saveToFile();

           }

           void view_all_issued_books(){
                   cout << pink << "\nAll Issued Books\n" << reset;


                   for(auto& it : dueDates){
                       
                    int id = it.first;

                    Book bk = booksById[id];
                    
                    cout << "\n---------------------\n";
                    cout << orange << "\nBook id : " << yellow << bk.id << reset;
                    cout << orange << "\nAuthor  : " << yellow << bk.author << reset;
                    cout << orange << "\nTitle   : " << yellow << bk.title << reset;
                    cout << orange << "\nQuantity: " << yellow << bk.quantity << reset;
                    
                    cout << "\n---------------------\n";
                   }

                   cout << "\n---------------------\n";
           }
           

           void viewAllRecords(){
                   cout << green << "\nAll Books Available in Library\n" << reset;


                   for(auto& it : booksById){
                       
                    Book bk = it.second;
                    
                    cout << "\n---------------------\n";
                    cout << orange << "\nBook id : " << yellow << bk.id << reset;
                    cout << orange << "\nAuthor  : " << yellow << bk.author << reset;
                    cout << orange << "\nTitle   : " << yellow << bk.title << reset;
                    cout << orange << "\nQuantity: " << yellow << bk.quantity << reset;
                    
                    cout << "\n---------------------\n";
                   }
                   cout << "\n---------------------\n";
           }
           
           void saveToFile(){
                 // id,title,author,quantity

                 ofstream fout("library_data.txt");

                 for(const auto& it : booksById){
                       
                       const Book bk = it.second;
                        string due = "";

                        if(dueDates.find(bk.id) != dueDates.end()){
                             due = dueDates[bk.id];
                        }

                        fout << bk.id << "," 
                             << bk.title << "," 
                             << bk.author << ","
                             << bk.quantity <<","
                             << due << "\n" ;    

                   }
               
                   fout.close();
           }

           ~Library(){

                  saveToFile();
              
           }
          
};
// --------------------

// Admin---------------
class Admin{
    
    public:

          void addBook(Library& lib,Book& newBook){
               
                 lib.addBook(newBook);
                 cout << green << "\nBook Added!" << reset;
                 cout << "\n---------------------\n";
          }

           void deleteBook(Library& lib,int id){
                 lib.deleteBook(id);
           }

           void updateQuantity(Library& lib,int id,int new_qty){
                 lib.updateQuantity(id,new_qty);
           }

           void viewAllRecords(Library& lib){
                 lib.viewAllRecords();
           } 

           void view_all_issued_books(Library& lib){
                 lib.view_all_issued_books();
           } 

};
// ----------------------

// User------------------
class User{

       public:

             void searchBook(Library& lib,int id){
                     lib.getBookDetails(id);
                     cout << "\n-------------------------------\n\n";
             }

             void viewAvailableBooks(Library& lib){
                     lib.viewAllRecords();
             }

             void requestIssue(Library& lib,int id){

                 if(lib.issueBook(id)){

                    cout << pink << "\nBook Details\n" << reset;
                    lib.getBookDetails(id);

                    // print issue date and due date
                     cout << blue << "\nIssue Date : " << yellow << getCurrTime(0) << reset;
                     cout << blue << "\nDue   Date : " << yellow << getCurrTime(7) << reset;
                      cout << yellow << "\n\nOnly " << lib.getBookCopiesInfo(id) << " copies left !\n" << reset;
                     
                     cout << green << "\nBook Issued Successfully!" << reset;
                     cout << "\n------------------------------------\n";

                 }

                 else{

                    cout << red << "\nBook is not Available!\n" << reset;
                    cout << "\n-------------------------------\n";

                 }

             }

             void requestReturn(Library& lib,int id){
                   lib.returnBook(id);
                   cout << "\n-------------------------------\n";
             }            
};
// ----------------------

void heading(){
       
   cout<<pink<<"==================================================\n"<<reset;
   cout<< yellow << bold << "\t  Library MANAGEMENT SYSTEM\t\n"<<reset;
   cout<<pink<<"==================================================\n"<<reset;
   
}

bool verify(string password){
              
        return password == "libadmin@%$578";
}
          
bool verification(){

       for(int attempts = 1; attempts <= 3; attempts++){

                string password;
                cout << blue << "\nEnter Password : " << reset;
                cin >> password;
          
                 if(verify(password)){
                        cout << green << "Logged in successfully\n" << reset;
                        cout << "--------------------------";
                        return true;
                    }
    
                else{
       
                        cout << red << bold << "Incorrect Password!" << reset;
                        cout << yellow << "\nTry Again!\n" << reset;
                        cout << "\n--------------------------";
                }
              
        }
       
           
            return false;
    }




    void showAdminFeatures(){

    cout << blue << "\n1 . Add Book\n" << reset;
    cout << blue << "2 . Delete Book\n" << reset;
    cout << blue << "3 . Update Book Quantity\n" << reset;
    cout << blue << "4 . Display All Books\n" << reset;
    cout << blue << "5 . Exit\n" << reset;
    cout << "\n--------------------------";

}

void showUserFeatures(){
    
    cout << blue << "\n1 . Search Book\n" << reset;
    cout << blue << "2 . Issue Book\n" << reset;
    cout << blue << "3 . Return Book\n" << reset;
    cout << blue << "4 . Display All Books\n" << reset;
    cout << blue << "5 . Exit\n"<< reset;

}

void loginAdmin(Library& lib){
    
    if(verification()){

        Admin adm;
       
    while(1){

        showAdminFeatures();

        int choice;
        cout << cyan << "\nEnter choice : " << reset;
        cin >> choice;

        
        if (cin.fail()) {
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 cout << yellow << "Invalid input! Try again.\n" << reset;
                 continue;
        }

        cout << "\n---------------------\n";

        switch(choice){
            
            case 1 : {

                 cout << orange << "\nEnter Book details\n" << reset;

                 Book newBook;

                 cout << cyan << "\nBook Id : " << reset;
                 cin >> newBook.id;

                 cin.ignore(numeric_limits<streamsize>::max(), '\n');


                 cout << cyan << "Book Title : "<< reset;
                 getline(cin,newBook.title);

                 cout << cyan << "Book Author : "<< reset;
                 getline(cin,newBook.author);

                 cout << cyan << "Book Quantity : "<< reset;
                 cin >> newBook.quantity;

                 adm.addBook(lib,newBook);
                 
                 break;

            }

            case 2 : {

                   int id;
                   cout << cyan << "\nEnter Book Id : "<< reset;
                   cin >> id;
                   
                   adm.deleteBook(lib,id);
                   
                   break;
            }

            case 3 : {
                   int id,new_qty;
                   cout << cyan << "\nEnter Book Id : " << reset;
                   cin >> id;

                   cout <<  cyan << "\nEnter new quantity : " << reset;
                   cin >> new_qty;

                   
                   adm.updateQuantity(lib,id,new_qty);
                   break;
            }
            case 4 : {
                  adm.viewAllRecords(lib);
                  break;
            }
            default : {
                  cout << "\n-----------------------------------------------------\n";
                  cout << green << "\nYou have been logged out. All operations saved.\n" << reset;
                  cout << "\n-----------------------------------------------------\n";
                  return;
            }

        }
    }
}
         else{
            cout << "\n--------------------------------------------\n";
            cout << red << "Login failed! Maximum attempts reached.\n" <<reset;
            cout << yellow << "Please try again later or contact the admin.\n" << reset;
            cout << "--------------------------------------------\n";
         }
}

void loginUser(Library& lib){

    User usr;

    while(1){

        showUserFeatures();
        int choice;
        cout << cyan << "\nEnter choice : " << reset;
        cin >> choice;

         if (cin.fail()) {
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 cout << "Invalid input. Try again !\n";
                 continue;
        }
        
        cout << "\n";

        switch(choice){
             case 1 : {

                int id;
                cout << cyan << "Enter Book Id : " << reset;
                cin >> id;

                usr.searchBook(lib,id);
                break;
             }

             case 2 : {

                int id;
                cout << cyan << "Enter Book Id : " << reset;
                cin >> id;

                usr.requestIssue(lib,id);
                break;
             }

             case 3 : {

                int id;
                cout << cyan << "Enter Book Id : "  << reset;
                cin >> id;
                
                usr.requestReturn(lib,id);
                break;
             } 

             case 4 : {
                usr.viewAvailableBooks(lib);
                break;
             }

             default : {
                cout << "\n-----------------------------------------------------\n";
                cout << green << "Thank you for using the Library System. Happy reading!" << reset;
                cout << "\n-----------------------------------------------------\n";
                return;
             }
        }
    }

}

int main(){
    
    heading();

    Library lib;

    int role;
    
    cout << "\n---------\n";
    cout << blue << "  Roles \n" << reset;
    cout << "---------\n";

    cout << yellow << "\n1 . Admin\n" << yellow << "2 . User\n" << reset;

    cout << "--------------------------";
    cout << cyan << "\nEnter Your Role : " << reset;
    cin >> role;
    cout << "--------------------------";

    switch(role){

        case 1 : {
            loginAdmin(lib);
            break;
        }
        case 2 : {
            cout << green << "\n    Welcome User !\n" << reset;
             cout << "--------------------------\n";
            loginUser(lib);
        }
    }

}
