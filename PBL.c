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
int flight_count = 3; // Hardcoded for this stage

void generatePNR(char *pnr) {
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < 6; i++) {
        pnr[i] = charset[rand() % 36];
    }
    pnr[6] = '\0';
}

void checkAvailability() {
    printf("\n%-10s %-20s %-15s\n", "Flight No", "Destination", "Available");
    for (int i = 0; i < flight_count; i++) {
        printf("%-10s %-20s %-15d\n", flights[i].flight_no, flights[i].destination, flights[i].available_seats);
    }
}

void bookFlight() {
    char fno[10];
    printf("\nEnter Flight Number: ");
    scanf("%s", fno);

    int found = -1;
    for (int i = 0; i < flight_count; i++) {
        if (strcmp(flights[i].flight_no, fno) == 0) { found = i; break; }
    }

    if (found == -1 || flights[found].available_seats <= 0) {
        printf("Flight not available!\n");
        return;
    }

    Passenger p;
    printf("Enter Name: "); scanf("%s", p.name);
    printf("Enter Age: "); scanf("%d", &p.age);
    
    generatePNR(p.pnr);
    p.seat_no = flights[found].max_seats - flights[found].available_seats + 1;
    flights[found].available_seats--;

    printf("\nBooking Success! PNR: %s | Seat: %d\n", p.pnr, p.seat_no);
}

// ... main() and displayMenu() from Stage 1 ...
// (Add mock data in main for testing)
int main() {
    srand(time(NULL));
    strcpy(flights[0].flight_no, "AI101"); strcpy(flights[0].destination, "Mumbai"); flights[0].max_seats = 50; flights[0].available_seats = 50;
    strcpy(flights[1].flight_no, "AI202"); strcpy(flights[1].destination, "Delhi"); flights[1].max_seats = 40; flights[1].available_seats = 40;
    // ... continue as per Stage 1 ...
}
