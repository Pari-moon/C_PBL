#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FLIGHTS 10
#define PNR_LEN 7

typedef struct {
    char pnr[PNR_LEN];
    char name[50];
    int age;
    char flight_no[10];
    int seat_no;
} Passenger;

typedef struct {
    char flight_no[10];
    char destination[50];
    int max_seats;
    int available_seats;
} Flight;

Flight flights[MAX_FLIGHTS];
int flight_count = 0;

void loadFlights();
void saveFlights();
void generatePNR(char *pnr);
void bookFlight();
void cancelBooking();
void modifyBooking();
void checkAvailability();
void displayMenu();
void clearInputBuffer();

int main() {
    srand(time(NULL));
    loadFlights();

    char choice_str[10];
    int choice;
    while (1) {
        displayMenu();
        printf("\nEnter your choice: ");
        if (fgets(choice_str, sizeof(choice_str), stdin) == NULL) break;
        if (sscanf(choice_str, "%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1: bookFlight(); break;
            case 2: cancelBooking(); break;
            case 3: modifyBooking(); break;
            case 4: checkAvailability(); break;
            case 5: 
                printf("Exiting system. Have a nice day!\n");
                exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); 
    }
    return 0;
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadFlights() {
    FILE *fp = fopen("flights.txt", "r");
    if (!fp) {
        printf("Error: flights.txt not found! Creating default data...\n");
        fp = fopen("flights.txt", "w");
        fprintf(fp, "AI101,Mumbai,50,50\nAI202,Delhi,40,40\nAI303,Bangalore,30,30\n");
        fclose(fp);
        loadFlights();
        return;
    }
    flight_count = 0;
    while (fscanf(fp, "%[^,],%[^,],%d,%d\n", 
           flights[flight_count].flight_no, 
           flights[flight_count].destination, 
           &flights[flight_count].max_seats, 
           &flights[flight_count].available_seats) == 4) {
        flight_count++;
        if (flight_count >= MAX_FLIGHTS) break;
    }
    fclose(fp);
}

void saveFlights() {
    FILE *fp = fopen("flights.txt", "w");
    for (int i = 0; i < flight_count; i++) {
        fprintf(fp, "%s,%s,%d,%d\n", 
                flights[i].flight_no, 
                flights[i].destination, 
                flights[i].max_seats, 
                flights[i].available_seats);
    }
    fclose(fp);
}

void generatePNR(char *pnr) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 6; i++) {
        pnr[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    pnr[6] = '\0';
}

void bookFlight() {
    checkAvailability();
    char fno[10];
    printf("\nEnter Flight Number to book: ");
    scanf("%s", fno);
    clearInputBuffer();

    int found = -1;
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_no, fno) == 0) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("Flight not found!\n");
        return;
    }

    if (flights[found].available_seats <= 0) {
        printf("No seats available on this flight!\n");
        return;
    }

    Passenger p;
    printf("Enter Passenger Name: ");
    fgets(p.name, 50, stdin);
    p.name[strcspn(p.name, "\n")] = 0;
    
    printf("Enter Age: ");
    scanf("%d", &p.age);
    clearInputBuffer();
    
    strcpy(p.flight_no, fno);
    p.seat_no = flights[found].max_seats - flights[found].available_seats + 1;
    generatePNR(p.pnr);

    FILE *fp = fopen("reservations.txt", "a+");
    fprintf(fp, "%s,%s,%d,%s,%d\n", p.pnr, p.name, p.age, p.flight_no, p.seat_no);
    fclose(fp);

    flights[found].available_seats--;
    saveFlights();

    printf("\nBooking Successful!\n");
    printf("PNR: %s | Seat No: %d\n", p.pnr, p.seat_no);
}

void cancelBooking() {
    char pnr[7];
    printf("Enter PNR to cancel: ");
    scanf("%s", pnr);
    clearInputBuffer();

    FILE *fp = fopen("reservations.txt", "r");
    if (!fp) {
        printf("No reservations found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    char line[150];
    int cancelled = 0;
    Passenger p;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^,],%d", p.pnr, p.name, &p.age, p.flight_no, &p.seat_no) == 5) {
            if (strcmp(p.pnr, pnr) == 0) {
                cancelled = 1;
                for (int i = 0; i < flight_count; i++) {
                    if (strcmp(flights[i].flight_no, p.flight_no) == 0) {
                        flights[i].available_seats++;
                        break;
                    }
                }
                continue; 
            }
        }
        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);

    remove("reservations.txt");
    rename("temp.txt", "reservations.txt");

    if (cancelled) {
        saveFlights();
        printf("Reservation with PNR %s cancelled successfully!\n", pnr);
    } else {
        printf("PNR not found!\n");
    }
}

void modifyBooking() {
    char pnr[7];
    printf("Enter PNR to modify: ");
    scanf("%s", pnr);
    clearInputBuffer();

    FILE *fp = fopen("reservations.txt", "r");
    if (!fp) {
        printf("No reservations found!\n");
        return;
    }

    FILE *temp = fopen("temp.txt", "w");
    char line[150];
    int modified = 0;
    Passenger p;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%[^,],%[^,],%d,%[^,],%d", p.pnr, p.name, &p.age, p.flight_no, &p.seat_no) == 5) {
            if (strcmp(p.pnr, pnr) == 0) {
                modified = 1;
                printf("Current Details: Name: %s, Age: %d\n", p.name, p.age);
                printf("Enter New Name: ");
                fgets(p.name, 50, stdin);
                p.name[strcspn(p.name, "\n")] = 0;
                printf("Enter New Age: ");
                scanf("%d", &p.age);
                clearInputBuffer();
                
                fprintf(temp, "%s,%s,%d,%s,%d\n", p.pnr, p.name, p.age, p.flight_no, p.seat_no);
                continue;
            }
        }
        fprintf(temp, "%s", line);
    }

    fclose(fp);
    fclose(temp);

    remove("reservations.txt");
    rename("temp.txt", "reservations.txt");

    if (modified) {
        printf("Details updated successfully!\n");
    } else {
        printf("PNR not found!\n");
    }
}

void checkAvailability() {
    printf("\n--- Available Flights ---\n");
    printf("%-10s %-20s %-12s %-15s\n", "Flight No", "Destination", "Total Seats", "Available");
    for (int i = 0; i < flight_count; i++) {
        printf("%-10s %-20s %-12d %-15d\n", 
               flights[i].flight_no, flights[i].destination, 
               flights[i].max_seats, flights[i].available_seats);
    }
}

void displayMenu() {
    printf("\n=========================================\n");
    printf("   AIRLINE RESERVATION MANAGEMENT SYSTEM   \n");
    printf("=========================================\n");
    printf("1. Book Flight\n");
    printf("2. Cancel Booking\n");
    printf("3. Modify Booking\n");
    printf("4. Check Seat Availability\n");
    printf("5. Exit\n");
}
