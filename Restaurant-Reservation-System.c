#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


int tablesAvailable;
sem_t tableSem; //control access the tables
pthread_mutex_t mutex; //protect CS
int *tableStatus;
int tableNum;


typedef struct {
    long id;
    int tableNum;
} Customer;


void *customer(void* arg) {
    long id = (long) arg + 1;
    printf("Customer[%ld]\tArrives and wants to reserve a table.\n", id);
    sem_wait(&tableSem);
    pthread_mutex_lock(&mutex);
    tablesAvailable--;
    printf("Customer[%ld]\tReserved a table. Tables Available: %d\n", id, tablesAvailable);
    pthread_mutex_unlock(&mutex);
    sleep(2);
    pthread_mutex_lock(&mutex);
    tablesAvailable++;
    printf("Customer[%ld]\tLeft the table. Tables Available: %d\n", id, tablesAvailable);
    pthread_mutex_unlock(&mutex);
    sem_post(&tableSem);
    pthread_exit(NULL);
}


void *leaveTable(void *arg) {
    Customer *customer = (Customer *) arg;
    long id = customer->id;
    int tableNum = customer->tableNum;
    sleep(2);
    pthread_mutex_lock(&mutex);
    tableStatus[tableNum - 1] = -1;
    tablesAvailable++;
    printf("Customer[%ld]\tLeft the table. Tables Available: %d\n", id, tablesAvailable);
    pthread_mutex_unlock(&mutex);
    sem_post(&tableSem);
    free(customer);
    pthread_exit(NULL);
}


Customer *reserveTable(void *arg) {
    long id = (long) arg + 1;
    printf("Customer[%ld]\tArrives and wants to reserve a table.\n", id);
    sem_wait(&tableSem);
    pthread_mutex_lock(&mutex);
    int reservedTable = -1;
    for (int i = 0; i < tableNum; i++) {
        if (tableStatus[i] == -1) {
            tableStatus[i] = id;
            tablesAvailable--;
            reservedTable = i + 1;
            printf("Customer[%ld]\tReserved table %d.\nTables Available: %d.\nTable NO: ", id, reservedTable, tablesAvailable);
            for (int j = 0; j < tableNum; j++) {
                if (tableStatus[j] == -1) {
                    printf("%d ", j + 1);
                }
            }
            printf("\n");
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    Customer *customer = (Customer *) malloc(sizeof(Customer));
    customer->id = id;
    customer->tableNum = reservedTable;
    return customer;
}


void displayAvailableTables() {
    pthread_mutex_lock(&mutex);
    printf("Available Tables: ");
    int available = 0;
    for (int i = 0; i < tableNum; i++) {
        if (tableStatus[i] == -1) {
            printf("%d ", i + 1);
            available++;
        }
    }
    if (available == 0) {
        printf("No tables available");
    }
    printf("\n");
    pthread_mutex_unlock(&mutex);
}

int main() {
    int customerNum;
    printf("Enter number of customers: ");
    scanf("%d", &customerNum);
    printf("Enter number of tables: ");
    scanf("%d", &tableNum);

    if (customerNum > tableNum) {
    
        printf("Restaurant Table Reservation System\n");
        printf("Customers: %d, Tables: %d\n", customerNum, tableNum);
        printf("---------------------------------------------\n");

        sem_init(&tableSem, 0, tableNum);
        pthread_mutex_init(&mutex, NULL);
        pthread_t customerThread[customerNum];

        tablesAvailable = tableNum;

        for(long id = 0; id < customerNum; ++id) {
            pthread_create(&customerThread[id], NULL, customer, (void*)id);
            usleep(500);
        }

        for(int id = 0; id < customerNum; ++id) {
            pthread_join(customerThread[id], NULL);
        }

        sem_destroy(&tableSem);
        pthread_mutex_destroy(&mutex);

        printf("---------------------------------------------\n");
        printf("Restaurant Table Reservation System End!\n");
        return 0;
    }

    printf("Restaurant Table Reservation System\n");
    printf("Customers: %d, Tables: %d\n", customerNum, tableNum);
    printf("---------------------------------------------\n");

    sem_init(&tableSem, 0, tableNum);
    pthread_mutex_init(&mutex, NULL);
    pthread_t customerThread[customerNum];
    tableStatus = (int*) malloc(tableNum * sizeof(int));
    tablesAvailable = tableNum;

    for (int i = 0; i < tableNum; i++) {
        tableStatus[i] = -1;
    }

    Customer *customers[customerNum];
    for(long id = 0; id < customerNum; ++id) {
        customers[id] = reserveTable((void*)id);
        usleep(500);
    }

    while (1) {
        int choice, customerId, newTableNum;
        printf("1 to cancel a reservation\n2 to modify table no\n0 to proceed\nEnter your choice: ");
        scanf("%d", &choice);
        if (choice == 0) break;
        switch (choice) {
            case 1:
                printf("Enter customer ID to cancel reservation: ");
                scanf("%d", &customerId);
                pthread_mutex_lock(&mutex);
                int cancelled = 0;
                for (int i = 0; i < tableNum; i++) {
                    if (tableStatus[i] == customerId) {
                        tableStatus[i] = -1;
                        tablesAvailable++;
                        sem_post(&tableSem);
                        printf("Reservation for customer[%d] cancelled.\nTable %d is now available.\n", customerId, i + 1);
                        cancelled = 1;
                        break;
                    }
                }
                if (!cancelled) {
                    printf("No reservation found for customer[%d].\n", customerId);
                }
                pthread_mutex_unlock(&mutex);
                break;
            case 2:
                printf("Enter customer ID to modify table reservation: ");
                scanf("%d", &customerId);
                displayAvailableTables();
                printf("Enter new table number: ");
                scanf("%d", &newTableNum);
                pthread_mutex_lock(&mutex);

                int modified = 0;
                for (int i = 0; i < tableNum; i++) {
                    if (tableStatus[i] == customerId) {
                        if (tableStatus[newTableNum - 1] == -1) {
                            tableStatus[newTableNum - 1] = customerId;
                            tableStatus[i] = -1;
                            printf("Customer[%d] moved from table %d to table %d.\n", customerId, i + 1, newTableNum);
                            modified = 1;
                        } else {
                            printf("Table %d is not available.\n", newTableNum);
                        }
                        break;
                    }
                }
                if (!modified && tableStatus[newTableNum - 1] != -1) {
                    printf("No reservation found for customer[%d].\n", customerId);
                }
                pthread_mutex_unlock(&mutex);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
        displayAvailableTables();
    }

    for(int id = 0; id < customerNum; ++id) {
        if (customers[id] != NULL && customers[id]->tableNum != -1 && customers[id]->id != 1) {
            pthread_create(&customerThread[id], NULL, leaveTable, (void*)customers[id]);
        }
    }

    for(int id = 0; id < customerNum; ++id) {
        if (customers[id] != NULL && customers[id]->tableNum != -1 && customers[id]->id != 1) {
            pthread_join(customerThread[id], NULL);
        }
    }

    sem_destroy(&tableSem);
    pthread_mutex_destroy(&mutex);
    free(tableStatus);
    printf("---------------------------------------------\n");
    printf("Restaurant Table Reservation System End!\n");
    return 0;
}