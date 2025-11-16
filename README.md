:

📚 Library Management System (C++ | OOP | File Handling)

A console-based Library Management System built using Object-Oriented Programming (OOP), File Handling, and STL containers in C++.
This project demonstrates practical use of Classes, Objects, Encapsulation, and persistent data storage.

🚀 Features
----------------------
👨‍💼 Admin Features
----------------------
 - Add new books
 - View all books
 - Delete a book by ID
 - Update book details
 - View issued books and due dates
 - Persistent storage using file handling

 ------------------------
👤 User Features
-------------------------
- View available books
- Search books by id
- Issue a book
- Return a book
- See due date of issued books

----------------------------
🧵 Object Oriented Design
---------------------------

The project follows a clean OOP structure:

  Class	                                    Responsibility
  
  Book	                           Represents each book with ID, title, author, and availability
  Library	                         Stores all books (using map<int, Book>) and handles issue/return
   User	                           For normal user operations
  Admin	                           Full access for adding/updating/deleting books

----------------------
🗂 File Handling
----------------------

The project uses 3 main files:
      File	                                               Purpose
  library_data.txt	                    Stores all books permanently (ID, title, author, availability, DueDates (if issued))

--------------------
🔧 Tech Used
--------------------

- C++17
- OOP (Classes, Objects, Encapsulation,Abstraction,Composition)
- File Handling using fstream
- STL (map,string)
- ANSI color formatting for UI

----------------------------------------
- 🖥 Sample Output (Screenshots)
- ---------------------------------------
- <img width="614" height="862" alt="image" src="https://github.com/user-attachments/assets/762293dc-3396-4888-8869-c49c5a83b82d" />

--------------------------
📝 Future Improvements
--------------------------

- Password Encryption
- Sorting and filtering books
- Export report in CSV format

----------------------
⭐ Author
----------------------

- Prachi Kumari Gupta
-- Passionate C++ and Full-Stack learner



