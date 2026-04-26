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

// Function Prototypes (Stubs)
void bookFlight() { printf("Booking feature coming soon!\n"); }
void cancelBooking() { printf("Cancellation feature coming soon!\n"); }
void modifyBooking() { printf("Modification feature coming soon!\n"); }
void checkAvailability() { printf("Availability check coming soon!\n"); }
void displayMenu();

int main() {
    int choice;
    while (1) {
        displayMenu();
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }

        switch (choice) {
            case 1: bookFlight(); break;
            case 2: cancelBooking(); break;
            case 3: modifyBooking(); break;
            case 4: checkAvailability(); break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
        printf("\nPress Enter to continue...");
        getchar(); getchar(); 
    }
    return 0;
}

void displayMenu() {
    printf("\n=== AIRLINE RESERVATION SYSTEM ===\n");
    printf("1. Book Flight\n");
    printf("2. Cancel Booking\n");
    printf("3. Modify Booking\n");
    printf("4. Check Seat Availability\n");
    printf("5. Exit\n");
}
