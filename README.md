# Restaurant Table Reservation System

## Overview

This program implements a **Restaurant Table Reservation System** using multithreading in C with the `pthread` library and synchronization mechanisms such as semaphores and mutexes. It simulates customers reserving tables in a restaurant, managing reservations, and handling customer interactions such as modifying or canceling reservations.

## Features

1. **Customer Table Reservation:**
   - Customers arrive and reserve tables if available.
   - Customers release the table after they finish dining.

2. **Cancellation of Reservation:**
   - A customer can cancel their table reservation, making the table available for others.

3. **Modify Table Reservation:**
   - Customers can change their reserved table to another table if available.

4. **Dynamic Reservation System:**
   - The system dynamically displays available tables and updates reservations in real-time.

5. **Thread-Safe Operations:**
   - Semaphores (`sem_t`) ensure controlled access to tables.
   - Mutex (`pthread_mutex_t`) protects critical sections, preventing data corruption.

6. **Customizable Inputs:**
   - The user specifies the number of customers and tables at the start.

7. **Multithreading Simulation:**
   - Each customer is handled by a separate thread, ensuring concurrent execution.

---

## Prerequisites

- A C compiler (e.g., GCC).
- POSIX threads (`pthread`) library.
- Basic knowledge of multithreading and synchronization in C.

---

## How It Works

### Key Components:

1. **Tables Management:**
   - Tables are managed using an array (`tableStatus`), where:
     - `-1` indicates the table is available.
     - Any other value represents the `ID` of the customer occupying the table.
   - A semaphore (`tableSem`) ensures only the specified number of tables are occupied.

2. **Thread Functions:**
   - **Customer Arrival (`customer`):**
     - Reserves a table if available.
     - Sleeps for 2 seconds to simulate dining time.
     - Releases the table after dining.
   - **Reservation and Cancellation (`reserveTable`, `leaveTable`):**
     - Customers can cancel or modify their reservations.
     - Available tables are dynamically updated.

3. **Synchronization Mechanisms:**
   - **Semaphores:**
     - Used to track the number of available tables.
   - **Mutex:**
     - Protects critical sections like updating `tablesAvailable` and `tableStatus`.

4. **Input-Driven Menu:**
   - **1:** Cancel reservation.
   - **2:** Modify reservation.
   - **0:** Proceed without further changes.

---

## How to Run the Program

1. **Compile the Code:**
   ```bash
   gcc -pthread -o reservation_system reservation_system.c
   ```

2. **Run the Program:**
   ```bash
   ./reservation_system
   ```

3. **Enter Inputs:**
   - Specify the number of customers and tables.
   - Interact with the menu-driven system.

---

## Sample Input and Output

### Input:
```
Enter number of customers: 5
Enter number of tables: 3
```

### Output:
```
Restaurant Table Reservation System
Customers: 5, Tables: 3
---------------------------------------------
Customer[1]    Arrives and wants to reserve a table.
Customer[1]    Reserved a table. Tables Available: 2.
Customer[2]    Arrives and wants to reserve a table.
Customer[2]    Reserved a table. Tables Available: 1.
Customer[3]    Arrives and wants to reserve a table.
Customer[3]    Reserved a table. Tables Available: 0.
...
Available Tables: 1 3
1 to cancel a reservation
2 to modify table no
0 to proceed
Enter your choice: 
```

---

## Limitations

1. Customers must wait if all tables are occupied.
2. Thread synchronization and data sharing can introduce race conditions if not properly handled.

---

## Improvements and Future Enhancements

1. **Graphical User Interface (GUI):**
   - Add a GUI for better visualization of table reservations.
2. **Real-Time Reservation Updates:**
   - Display available and reserved tables dynamically in a live dashboard.
3. **Additional Features:**
   - Include reservation time limits or priority reservations.
   - Add customer preferences for table selection.
4. **Error Handling:**
   - Improve input validation and error messages for invalid actions.

---

## Compilation Flags

- Use `-pthread` to link the POSIX threads library:
  ```bash
  gcc -pthread -o reservation_system reservation_system.c
  ```

---

## Conclusion

The **Restaurant Table Reservation System** demonstrates the practical use of multithreading, semaphores, and mutexes to manage concurrent processes in a resource-limited system. It is a simple yet effective simulation for learning synchronization and multithreaded programming concepts.
