//Joshua O. Pagcaliwagan CMSC 21 ST-2L Final Project
//Note: Have passengers.txt and flights.txt in the same folder as this c file to ensure proper running of code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structure definitions
struct date {
    int month;
    int day;
    int year;
};

struct time {
    int hour;
    int minute;
};

struct flight {
    char flight_id[15];
    char destination[40];
    char origin[40];
    struct date departure_date;
    struct date arrival_date;
    struct time departure_time;
    struct time arrival_time;
    int booked_passengers;
    int max_passengers;
    int bonus_miles;
    struct reservation *reservations;
    struct flight *next;
    struct flight *prev;
};

struct reservation {
    struct passenger *passenger;
    struct flight *flight;
    struct reservation *next;
    struct reservation *prev;
};

struct passenger {
    char first_name[40];
    char last_name[40];
    struct date dob;
    char passport_number[15];
    int miles;
    struct reservation *reservations;
    struct passenger *next;
    struct passenger *prev;
};

// Function prototypes
void load_flights();
void load_passengers();
void add_flight();
void insert_flight_in_order(struct flight **flight_list, struct flight *new_flight);
int compare_flights(struct flight *f1, struct flight *f2);
int compare_dates(struct date d1, struct date d2);
int compare_times(struct time t1, struct time t2);
int is_valid_hour(int hour);
int is_valid_minute(int minute);
int is_past_datetime(struct tm *date, int hour, int minute);
int is_realistic_flight_duration(struct tm *departure_date, int departure_hour, int departure_minute, struct tm *arrival_date, int arrival_hour, int arrival_minute);
void edit_flight();
void view_flights();
void view_specific_flight();
void view_available_flights();
void view_fully_booked_flights();
void view_all_flights();
void delete_flight();
void insert_passenger_in_order(struct passenger **head, struct passenger *new_passenger);
int compare_passengers(struct passenger *p1, struct passenger *p2);
void add_passenger();
void edit_passenger();
void book_flight_reservation();
void remove_flight_reservation();
void view_reservations();
void save_data();
void free_memory();
void reset_data();

// Global variables
struct flight *flights_list = NULL;
struct passenger *passengers_list = NULL;

int main() {
    // Load data
    load_flights();
    load_passengers();
    
    // Main menu loop
    int choice;
    do {
        printf("\n");
        printf(" ╔═════════════════════════════════════╗\n");
        printf(" ║   ***Welcome to UPLB Airlines***    ║\n");
        printf(" ╚═════════════════════════════════════╝\n");
        printf("\n");

        printf(" ╔═══════════════════════════════════════╗\n");
        printf(" ║           ***Main Menu***             ║\n");
        printf(" ║═══════════════════════════════════════║\n");
        printf(" ║  (1) Add Flight                       ║\n");
        printf(" ║  (2) Edit Flight                      ║\n");
        printf(" ║  (3) View Flights                     ║\n");
        printf(" ║  (4) Delete Flight                    ║\n");
        printf(" ║  (5) Add Passenger                    ║\n");
        printf(" ║  (6) Edit Passenger                   ║\n");
        printf(" ║  (7) Book Flight Reservation          ║\n");
        printf(" ║  (8) Remove Flight Reservation        ║\n");
        printf(" ║  (9) View Reservations                ║\n");
        printf(" ║  (10) Exit                            ║\n");
        printf(" ║  (11) Reset Data                      ║\n");
        printf(" ╚═══════════════════════════════════════╝\n");

        printf("\n ^_^ Hi! Please select from 1 - 11: ");
        scanf("%d", &choice);
        
        // Calls function based on user's input
        switch(choice) {
            case 1:
                add_flight();
                break;
            case 2:
                edit_flight();
                break;
            case 3:
                view_flights();
                break;
            case 4:
                delete_flight();
                break;
            case 5:
                add_passenger();
                break;
            case 6:
                edit_passenger();
                break;
            case 7:
                book_flight_reservation();
                break;
            case 8:
                remove_flight_reservation();
                break;
            case 9:
                view_reservations();
                break;
            case 10:
                save_data();
                free_memory();
                //Header with border for design
                printf("\n");
                printf(" ╔══════════════════════╗\n");
                printf(" ║Thank you and goodbye!║\n");
                printf(" ╚══════════════════════╝\n");
                printf("\n");
                break;
            case 11:
                reset_data();
                break;
            default:
                printf(" T_T Oh no, please only choose from 1 - 11 \n"); // Error message
        }
    } while(choice != 10); // Loop until user chooses 10 (exit)

    return 0;
}

void load_flights() {
    FILE *file = fopen("flights.txt", "r"); // Open file
    if (file == NULL) {
        printf(" T_T Failed to open flights.txt \n"); // Error message
        return;
    }

    char line[100]; // Read lines
    while (fgets(line, sizeof(line), file)) {
        struct flight *new_flight = (struct flight*)malloc(sizeof(struct flight)); // Allocate memory for new_flight struct
        if (new_flight == NULL) {
            printf(" T_T Failed to allocate memory\n"); // Error message
            return;
        }

        // Parse data and fill new_flight structure
        char *token = strtok(line, "|"); // Split lines into tokens
        // Copy flight id, destination, origin
        strcpy(new_flight->flight_id, token);

        token = strtok(NULL, "|");
        strcpy(new_flight->destination, token);

        token = strtok(NULL, "|");
        strcpy(new_flight->origin, token);

        // Parse depart and arrival date and time 
        token = strtok(NULL, "|");
        sscanf(token, "%d/%d/%d", &new_flight->departure_date.month, &new_flight->departure_date.day, &new_flight->departure_date.year);

        token = strtok(NULL, "|");
        sscanf(token, "%d/%d/%d", &new_flight->arrival_date.month, &new_flight->arrival_date.day, &new_flight->arrival_date.year);

        token = strtok(NULL, "|");
        sscanf(token, "%d:%d", &new_flight->departure_time.hour, &new_flight->departure_time.minute);

        token = strtok(NULL, "|");
        sscanf(token, "%d:%d", &new_flight->arrival_time.hour, &new_flight->arrival_time.minute);

        // Convert and assign details, use atoi to convert strings to integers
        token = strtok(NULL, "|");
        new_flight->booked_passengers = atoi(token);

        token = strtok(NULL, "|");
        new_flight->max_passengers = atoi(token);

        token = strtok(NULL, "|");
        new_flight->bonus_miles = atoi(token);

        new_flight->next = NULL; // Set next pointer to NULL
        new_flight->prev = NULL; // Set prev pointer to NULL

        // Add new_flight to flights_list
        if (flights_list == NULL) {
            flights_list = new_flight; // If empty, become first node
        } else {
            struct flight *current_flight = flights_list;
            while (current_flight->next != NULL) {
                current_flight = current_flight->next; // Go to end of list
            }
            current_flight->next = new_flight; // Set next pointer
            new_flight->prev = current_flight; // Set prev pointer
        }
    }

    fclose(file); // Close file
}

void load_passengers() {
    FILE *file = fopen("passengers.txt", "r"); // Open file
    if (file == NULL) {
        printf(" T_T Failed to open passengers.txt \n"); // Error message
        return;
    }
    
    char line[1000]; // Read lines
    while (fgets(line, sizeof(line), file)) {
        // Allocate memory for new_passenger struct
        struct passenger *new_passenger = (struct passenger*)malloc(sizeof(struct passenger));
        if (new_passenger == NULL) {
            printf(" T_T Failed to allocate memory\n"); // Error message
            fclose(file);
            return;
        }

        // Parse data and fill new_passenger struct
        char *token = strtok(line, "|"); // Split lines into tokens
        // Copy details
        strcpy(new_passenger->first_name, token);
        
        token = strtok(NULL, "|");
        strcpy(new_passenger->last_name, token);
        
        // Parse details
        token = strtok(NULL, "|");
        sscanf(token, "%d/%d/%d", &new_passenger->dob.month, &new_passenger->dob.day, &new_passenger->dob.year);
        
        token = strtok(NULL, "|");
        strcpy(new_passenger->passport_number, token);
        
        // Convert and assign details, use atoi to convert strings to integer
        token = strtok(NULL, "|");
        new_passenger->miles = atoi(token);

        new_passenger->next = NULL; // Set next pointer to NULL
        new_passenger->prev = NULL; // Set prev pointer to NULL

        // Add new_passenger to passengers_list
        if (passengers_list == NULL) {
            passengers_list = new_passenger; // If empty, become first node
        } else {
            struct passenger *current_passenger = passengers_list;
            while (current_passenger->next != NULL) {
                current_passenger = current_passenger->next;  // Go to end of list
            }
            current_passenger->next = new_passenger; // Set next pointer
            new_passenger->prev = current_passenger; // Set prev pointer
        }    
    }
    
    fclose(file); // Close file
}

#define max_hours 19 // Max flight duration

// Get real-world date and time
void get_datetime(struct tm *current) {
    time_t t = time(NULL);
    *current = *localtime(&t); //Get real time
}

// Compare given date and time to real-world
int is_past_datetime(struct tm *flight_date, int flight_hour, int flight_minute) {
    struct tm current;
    get_datetime(&current);//Stores current time

    // Compare year, month, day, hour, and minute
    if (flight_date->tm_year < current.tm_year) return 1; //Year in past
    if (flight_date->tm_year == current.tm_year && flight_date->tm_mon < current.tm_mon) return 1; //Month in past
    if (flight_date->tm_year == current.tm_year && flight_date->tm_mon == current.tm_mon && flight_date->tm_mday < current.tm_mday) return 1; //Day in past
    if (flight_date->tm_year == current.tm_year && flight_date->tm_mon == current.tm_mon && flight_date->tm_mday == current.tm_mday &&
        (flight_hour < current.tm_hour || (flight_hour == current.tm_hour && flight_minute < current.tm_min))) return 1; //Time in past

    return 0; //Flight is not in the past
}

// Check if flight duration is realistic
int is_realistic_flight_duration(struct tm *departure_date, int departure_hour, int departure_minute,
	struct tm *arrival_date, int arrival_hour, int arrival_minute) {
    // Convert to present time                             	
    time_t dep_time = mktime(departure_date);
    time_t arr_time = mktime(arrival_date);
    
    // 3600 seconds in an hour, converts secs to hours
    dep_time += departure_hour * 3600 + departure_minute * 60;
    arr_time += arrival_hour * 3600 + arrival_minute * 60;
    
	//Checks difference of arrive and depart time 
    double duration_hours = difftime(arr_time, dep_time) / 3600.0;
    if (duration_hours <= 0 || duration_hours > max_hours) return 0; // Greater than 19 hours 

    return 1; // Less than 19 hours
}

// Check valid hour
int is_valid_hour(int hour) {
    return hour >= 0 && hour < 24; //Hour can't be 24 and above or negative
}

// Check valid minute
int is_valid_minute(int minute) {
    return minute >= 0 && minute < 60; //Minutes can't be 60 and above or negative
}

void add_flight() {
    // Allocate memory
    struct flight *new_flight = (struct flight*)malloc(sizeof(struct flight));
    if (new_flight == NULL) {
        printf(" T_T Failed to allocate memory\n");
        return;
    }

    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║   Add Flight       ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Ask user for flight ID
    printf(" ^_^ Enter flight ID: ");
    scanf(" %[^\n]", new_flight->flight_id);

    // Check if flight ID already exists
    struct flight *current_flight = flights_list;
    while (current_flight != NULL) {
        if (strcmp(current_flight->flight_id, new_flight->flight_id) == 0) {
            printf(" T_T Oh no, the flight ID already exists\n"); // Error message
            free(new_flight);
            return;
        }
        current_flight = current_flight->next;
    }
    
    // Ask user for details
    printf(" ^_^ Enter destination: ");
    scanf(" %[^\n]", new_flight->destination);

    printf(" ^_^ Enter origin: ");
    scanf(" %[^\n]", new_flight->origin);

    // Departure date validation
    printf(" ^_^ Enter departure date (Month/Day/Year): ");
    struct tm departure_date = {0};
    scanf("%d/%d/%d", &departure_date.tm_mon, &departure_date.tm_mday, &departure_date.tm_year);
    if (departure_date.tm_mon < 1 || departure_date.tm_mon > 12) {
        printf(" T_T Oh no, invalid departure month\n");
        free(new_flight);
        return;
    }
    if (departure_date.tm_mday < 1 || departure_date.tm_mday > 31) {
        printf(" T_T Oh no, invalid departure day\n");
        free(new_flight);
        return;
    }
    departure_date.tm_mon -= 1; // Shift month range from [0-11]
    departure_date.tm_year -= 1900; // Shift year to be years since 1900

    // Validate departure time
    printf(" ^_^ Enter departure time (Hour:Min): ");
    int departure_hour, departure_minute;
    scanf("%d:%d", &departure_hour, &departure_minute);
    if (!is_valid_hour(departure_hour) || !is_valid_minute(departure_minute)) {
        printf("T_T Oh no, invalid departure time\n"); // Validation message
        free(new_flight);
        return;
    }

    // Check if departure date and time is past
    if (is_past_datetime(&departure_date, departure_hour, departure_minute)) {
        printf(" T_T Oh no, the departure is in the past\n"); // Validation message
        free(new_flight);
        return;
    }

    // Arrival date validation
    printf(" ^_^ Enter arrival date (Month/Day/Year): ");
    struct tm arrival_date = {0};
    scanf("%d/%d/%d", &arrival_date.tm_mon, &arrival_date.tm_mday, &arrival_date.tm_year);
    if (arrival_date.tm_mon < 1 || arrival_date.tm_mon > 12) {
        printf(" T_T Oh no, invalid arrival month\n");
        free(new_flight);
        return;
    }
    if (arrival_date.tm_mday < 1 || arrival_date.tm_mday > 31) {
        printf(" T_T Oh no, invalid arrival day\n");
        free(new_flight);
        return;
    }
    arrival_date.tm_mon -= 1; // Shift month range from 0-11
    arrival_date.tm_year -= 1900; // Shift year to be years since 1900

    // Validate arrival time
    printf(" ^_^ Enter arrival time (Hour:Min): ");
    int arrival_hour, arrival_minute;
    scanf("%d:%d", &arrival_hour, &arrival_minute);
    if (!is_valid_hour(arrival_hour) || !is_valid_minute(arrival_minute)) {
        // Validation message
        printf(" T_T Oh no, invalid arrival time\n");
        free(new_flight);
        return;
    }

    // Check if arrival is before departure
    if (arrival_date.tm_year < departure_date.tm_year || // Compare year
        (arrival_date.tm_year == departure_date.tm_year &&
         (arrival_date.tm_mon < departure_date.tm_mon || // Compare month
          (arrival_date.tm_mon == departure_date.tm_mon &&
           (arrival_date.tm_mday < departure_date.tm_mday || // Compare day
            (arrival_date.tm_mday == departure_date.tm_mday &&
             // Compare hour
             (arrival_hour < departure_hour ||
             // Compare minute
              (arrival_hour == departure_hour && arrival_minute < departure_minute)))))))) {
        printf(" T_T Oh no, arrival can't be before departure\n"); // Validation message
        free(new_flight);
        return;
    }

    // Check if flight duration is realistic
    if (!is_realistic_flight_duration(&departure_date, departure_hour, departure_minute,
                                      &arrival_date, arrival_hour, arrival_minute)) {
        printf(" T_T Oh no, the flight duration is too long\n"); // Validation message
        free(new_flight);
        return;
    }
    
    // Store dates and times in new_flight struct
    // +1 since months are 0-11 in C
    // +1900 since years in C start from 1900
    new_flight->departure_date.month = departure_date.tm_mon + 1;
    new_flight->departure_date.day = departure_date.tm_mday;
    new_flight->departure_date.year = departure_date.tm_year + 1900;
    new_flight->departure_time.hour = departure_hour;
    new_flight->departure_time.minute = departure_minute;

    new_flight->arrival_date.month = arrival_date.tm_mon + 1;
    new_flight->arrival_date.day = arrival_date.tm_mday;
    new_flight->arrival_date.year = arrival_date.tm_year + 1900;
    new_flight->arrival_time.hour = arrival_hour;
    new_flight->arrival_time.minute = arrival_minute;

    printf(" ^_^ Enter max count of passengers: ");
    int max_passengers;
    scanf("%d", &max_passengers);
    if (max_passengers < 1) {
        printf(" T_T Oh no, that's too little\n");
        free(new_flight);
        return;
    } else if (max_passengers > 835) {
        printf(" T_T Oh no, that's too much\n");
        free(new_flight);
        return;
    }
    new_flight->max_passengers = max_passengers; 

    printf(" ^_^ Enter bonus miles: ");
    scanf("%d", &new_flight->bonus_miles);

    new_flight->next = NULL; // Set to NULL
    new_flight->prev = NULL; // Set to NULL

    // Insert new_flight into flights_list in correct order
    insert_flight_in_order(&flights_list, new_flight);

    printf(" ^_^ Yey, your flight has been added\n"); // Success message
}

void insert_flight_in_order(struct flight **flight_list, struct flight *new_flight) {
    struct flight *current;

    // Special case for the head end
    // Checks if list is empty, or flight should be before current head
    if (*flight_list == NULL || compare_flights(new_flight, *flight_list) < 0) {
        new_flight->next = *flight_list;
        if (*flight_list != NULL) {
            (*flight_list)->prev = new_flight;
        }
        *flight_list = new_flight;
    } else {
        current = *flight_list;
        // Locate the node before point of insertion
        while (current->next != NULL && compare_flights(new_flight, current->next) > 0) {
            current = current->next;
        }
        new_flight->next = current->next;
        if (current->next != NULL) {
            current->next->prev = new_flight;
        }
        current->next = new_flight;
        new_flight->prev = current;
    }
}

int compare_flights(struct flight *f1, struct flight *f2) {
    // Returns negative, d1 is earlier
    // Returns positive, d2 is earlier
    // Returns zero, date is same
    // Compare dates first
    int date_cmp = compare_dates(f1->departure_date, f2->departure_date);
    if (date_cmp != 0) {
        return date_cmp;
    }

    // Dates are equal, compare times
    int time_cmp = compare_times(f1->departure_time, f2->departure_time);
    if (time_cmp != 0) {
        return time_cmp;
    }

    // Times are equal, compare source
    int source_cmp = strcmp(f1->origin, f2->origin);
    if (source_cmp != 0) {
        return source_cmp;
    }

    // Sources are equal, compare destination
    return strcmp(f1->destination, f2->destination);
}

//Compare which date is earlier
int compare_dates(struct date d1, struct date d2) {
    // Returns negative, d1 is earlier
    // Returns positive, d2 is earlier
    // Returns zero, date is same
    if (d1.year != d2.year) {
        return d1.year - d2.year;
    }
    if (d1.month != d2.month) {
        return d1.month - d2.month;
    }
    return d1.day - d2.day;
}

//Compare which time is earlier
int compare_times(struct time t1, struct time t2) {
    // Returns negative, t1 is earlier
    // Returns positive, t2 is earlier
    // Returns zero, time is same
    if (t1.hour != t2.hour) {
        return t1.hour - t2.hour;
    }
    return t1.minute - t2.minute;
}

void edit_flight() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║    Edit Flight     ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Ask user for flight ID
    char flight_id[15];
    printf(" ^_^ Enter flight ID: ");
    scanf(" %[^\n]", flight_id);

    // Search flight with matching ID
    struct flight *current_flight = flights_list;
    struct flight *prev_flight = NULL;
    while (current_flight != NULL) {
        // If ID matches, print details
        if (strcmp(current_flight->flight_id, flight_id) == 0) {
            printf(" ^_^ Flight %s details:\n", current_flight->flight_id);
            printf(" (1) Max count of passengers: %d\n", current_flight->max_passengers);
            printf(" (2) Departure date: %d/%d/%d\n", current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year);
            printf(" (3) Arrival date: %d/%d/%d\n", current_flight->arrival_date.month, current_flight->arrival_date.day, current_flight->arrival_date.year);
            printf(" (4) Departure time: %d:%d\n", current_flight->departure_time.hour, current_flight->departure_time.minute);
            printf(" (5) Arrival time: %d:%d\n", current_flight->arrival_time.hour, current_flight->arrival_time.minute);

            // Ask user for new details
            printf(" ^_^ Enter new max count of passengers: ");
            int max_passengers;
            scanf("%d", &max_passengers);
            if (max_passengers < 1) { // Less than 1 passenger
                printf(" T_T Oh no, that's too little\n");
                return;
            } else if (max_passengers > 835) { // More than 835 passengers
                printf(" T_T Oh no, that's too much\n");
                return;
            }

            printf(" ^_^ Enter new departure date (Month/Day/Year): ");
            int new_departure_month, new_departure_day, new_departure_year;
            scanf("%d/%d/%d", &new_departure_month, &new_departure_day, &new_departure_year);
            // Validate month and day
            if (new_departure_month < 1 || new_departure_month > 12) {
                printf(" T_T Oh no, invalid departure month\n");
                return;
            }
            if (new_departure_day < 1 || new_departure_day > 31) {
                printf(" T_T Oh no, invalid departure day\n");
                return;
            }

            printf(" ^_^ Enter new departure time (Hour:Min): ");
            int new_departure_hour, new_departure_minute;
            scanf("%d:%d", &new_departure_hour, &new_departure_minute);
            if (!is_valid_hour(new_departure_hour) || !is_valid_minute(new_departure_minute)) {
                printf(" T_T Oh no, invalid departure time\n"); // Error message
                return;
            }

            // Check if departure is in past
            // Structure for comparison
            struct tm departure_tm = {
                .tm_year = new_departure_year - 1900,
                .tm_mon = new_departure_month - 1,
                .tm_mday = new_departure_day,
                .tm_hour = new_departure_hour,
                .tm_min = new_departure_minute
            };
            // Call function to compare
            if (is_past_datetime(&departure_tm, new_departure_hour, new_departure_minute)) {
                printf(" T_T Oh no, the departure is in the past\n"); // Validation message
                return;
            }

            printf(" ^_^ Enter new arrival date (Month/Day/Year): ");
            int new_arrival_month, new_arrival_day, new_arrival_year;
            scanf("%d/%d/%d", &new_arrival_month, &new_arrival_day, &new_arrival_year);
            // Validate month and day
            if (new_arrival_month < 1 || new_arrival_month > 12) {
                printf(" T_T Oh no, invalid arrival month\n");
                return;
            }
            if (new_arrival_day < 1 || new_arrival_day > 31) {
                printf(" T_T Oh no, invalid arrival day\n");
                return;
            }

            printf(" ^_^ Enter new arrival time (Hour:Min): ");
            int new_arrival_hour, new_arrival_minute;
            scanf("%d:%d", &new_arrival_hour, &new_arrival_minute);
            if (!is_valid_hour(new_arrival_hour) || !is_valid_minute(new_arrival_minute)) {
                printf(" T_T Oh no, invalid arrival time\n"); // Validation message
                return;
            }

            // Validate arrival time and date
            if (new_arrival_year < new_departure_year || // Arrival year is before depart
                (new_arrival_year == new_departure_year &&
                 (new_arrival_month < new_departure_month || // Arrival month is before depart
                  (new_arrival_month == new_departure_month &&
                   (new_arrival_day < new_departure_day || // Arrival date is before depart
                    (new_arrival_day == new_departure_day &&
                     (new_arrival_hour < new_departure_hour || // Arrival hour is before depart
                     // Arrival minute is before depart
                      (new_arrival_hour == new_departure_hour &&
                       new_arrival_minute < new_departure_minute)))))))) {
                printf(" T_T Oh no, arrival can't be before departure\n");
                return;
            }

            // Check if flight duration is realistic
            struct tm arrival_tm = {
                .tm_year = new_arrival_year - 1900,
                .tm_mon = new_arrival_month - 1,
                .tm_mday = new_arrival_day,
                .tm_hour = new_arrival_hour,
                .tm_min = new_arrival_minute
            };
            if (!is_realistic_flight_duration(&departure_tm, new_departure_hour, new_departure_minute, &arrival_tm, new_arrival_hour, new_arrival_minute)) {
                printf(" T_T Oh no, the flight duration is too long\n");
                return;
            }

            // Remove flight from list
            if (prev_flight == NULL) {
                flights_list = current_flight->next;
            } else {
                prev_flight->next = current_flight->next;
            }

            // Update flight details
            current_flight->max_passengers = max_passengers;
            current_flight->departure_date.month = new_departure_month;
            current_flight->departure_date.day = new_departure_day;
            current_flight->departure_date.year = new_departure_year;
            current_flight->departure_time.hour = new_departure_hour;
            current_flight->departure_time.minute = new_departure_minute;
            current_flight->arrival_date.month = new_arrival_month;
            current_flight->arrival_date.day = new_arrival_day;
            current_flight->arrival_date.year = new_arrival_year;
            current_flight->arrival_time.hour = new_arrival_hour;
            current_flight->arrival_time.minute = new_arrival_minute;

            // Reinsert flight into list in correct order
            insert_flight_in_order(&flights_list, current_flight);

            printf(" ^_^ Yey, the flight details have been edited\n");
            return;
        }
        prev_flight = current_flight;
        current_flight = current_flight->next; // Move to next flight
    }

    printf(" T_T Oh no, flight ID %s doesn't exist\n", flight_id); // Error message
}

void view_flights() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║   View Flights     ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");
    
    // Ask user for choice
    int choice;
    printf("(1) View Specific Flight\n");
    printf("(2) View All Available Flights\n");
    printf("(3) View All Fully-booked Flights\n");
    printf("(4) View All Flights\n");
    printf(" ^_^ Hey again! Please select from 1 - 4: ");
    scanf("%d", &choice);

    // Call function based on user's choice
    switch (choice) {
        case 1:
            view_specific_flight();
            break;
        case 2:
            view_available_flights();
            break;
        case 3:
            view_fully_booked_flights();
            break;
        case 4:
            view_all_flights();
            break;
        default:
            printf(" T_T Oh no,only choose from 1 - 4\n"); // Error message
    }
}

void view_specific_flight() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║View Specific Flight║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Ask for flight ID
    char flight_id[15];
    printf(" ^_^ Enter flight ID: ");
    scanf("%s", flight_id);

    // Search for matching ID
    struct flight *current_flight = flights_list;
    while (current_flight != NULL) {
        if (strcmp(current_flight->flight_id, flight_id) == 0) {
            // Display details of the specific flight
            printf("Flight details:\n");
            printf("Flight ID: %s\n", current_flight->flight_id);
            printf("Destination: %s\n", current_flight->destination);
            printf("Origin: %s\n", current_flight->origin);
            printf("Departure date: %d/%d/%d\n", current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year);
            printf("Arrival date: %d/%d/%d\n", current_flight->arrival_date.month, current_flight->arrival_date.day, current_flight->arrival_date.year);
            printf("Departure time: %d:%d\n", current_flight->departure_time.hour, current_flight->departure_time.minute);
            printf("Arrival time: %d:%d\n", current_flight->arrival_time.hour, current_flight->arrival_time.minute);
            printf("Booked passengers: %d\n", current_flight->booked_passengers);
            printf("Max count of passengers: %d\n", current_flight->max_passengers);
            printf("Bonus miles: %d\n", current_flight->bonus_miles);
            return;
        }
        current_flight = current_flight->next; // Move to next flight
    }
    printf(" T_T Oh no, flight ID %s doesn't exist\n", flight_id); // Error message
}

void view_available_flights() {
    // Header with border for design
    printf("\n");
    printf(" ╔══════════════════════╗\n");
    printf(" ║View Available Flights║\n");
    printf(" ╚══════════════════════╝\n");
    printf("\n");

    // Pointer to iterate through flights list
    struct flight *current_flight = flights_list;
    int available_flights = 0; // Variable to check available flights

    // Iterate through flights
    while (current_flight != NULL) {
        // Check for available seats
        if (current_flight->booked_passengers < current_flight->max_passengers) {
            // Print flight details
            printf("Flight ID: %s\n", current_flight->flight_id);
            printf("Destination: %s\n", current_flight->destination);
            printf("Origin: %s\n", current_flight->origin);
            printf("Departure date: %d/%d/%d\n", current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year);
            printf("Departure time: %d:%d\n", current_flight->departure_time.hour, current_flight->departure_time.minute);
            printf("Available seats: %d\n", current_flight->max_passengers - current_flight->booked_passengers);
            printf("\n");
            available_flights++;
        }
        current_flight = current_flight->next; // Move to next flight
    }

    if (available_flights == 0) {
        printf(" T_T Oh no, there are no available flights\n"); // Error message
    }
}

void view_fully_booked_flights() {
    // Header with border for design
    printf("\n");
    printf(" ╔═════════════════════════╗\n");
    printf(" ║View Fully Booked Flights║\n");
    printf(" ╚═════════════════════════╝\n");
    printf("\n");

    // Pointer to iterate through flights list
    struct flight *current_flight = flights_list;
    int fully_booked_flights = 0; // Variable to check fully booked flights

    // Iterate through flights
    while (current_flight != NULL) {
        // Check if no more seats
        if (current_flight->booked_passengers == current_flight->max_passengers) {
            // Print flight details
            printf("Flight ID: %s\n", current_flight->flight_id);
            printf("Destination: %s\n", current_flight->destination);
            printf("Origin: %s\n", current_flight->origin);
            printf("Departure date: %d/%d/%d\n", current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year);
            printf("Departure time: %d:%d\n", current_flight->departure_time.hour, current_flight->departure_time.minute);
            printf("\n");
            fully_booked_flights++;
        }
        current_flight = current_flight->next; // Move to next flight
    }

    if (fully_booked_flights == 0) {
        printf(" T_T Oh no, there are no fully booked flights\n"); // Error message
    }
}

void view_all_flights() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║  View All Flights  ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Pointer to iterate through flights list
    struct flight *current_flight = flights_list;
    int total_flights = 0; // Variable to check total flights

    // Iterate through flights
    while (current_flight != NULL) {
        // Print flight details
        printf("Flight ID: %s\n", current_flight->flight_id);
        printf("Destination: %s\n", current_flight->destination);
        printf("Origin: %s\n", current_flight->origin);
        printf("Departure date: %d/%d/%d\n", current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year);
        printf("Departure time: %d:%d\n", current_flight->departure_time.hour, current_flight->departure_time.minute);
        printf("\n");
        total_flights++;
        current_flight = current_flight->next; // Move to next flight
    }

    if (total_flights == 0) {
        printf(" T_T Oh no, there are no flights right now\n"); // Error message
    }
}

void delete_flight() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║    Delete Flight   ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Asks user for flight ID
    char flight_id[15];
    printf(" ^_^ Enter flight ID: ");
    scanf("%s", flight_id);

    // Pointers to track current and previous flights
    struct flight *prev_flight = NULL;
    struct flight *current_flight = flights_list;

    // Loop through flight list
    while (current_flight != NULL) {
        // Check for matching ID
        if (strcmp(current_flight->flight_id, flight_id) == 0) {
            // Confirm deletion
            char confirmation;
            printf("Confirming to delete flight %s? (y/n): ", flight_id);
            scanf(" %c", &confirmation);

            if (confirmation == 'y' || confirmation == 'Y') {
                // If flight to delete is first node
                if (prev_flight == NULL) {
                    flights_list = current_flight->next;
                    if (flights_list != NULL) {
                        flights_list->prev = NULL; // Update prev pointer of new first node
                    }
                } else {
                    prev_flight->next = current_flight->next;
                    if (current_flight->next != NULL) {
                        current_flight->next->prev = prev_flight; // Update prev pointer of next node
                    }
                }
                free(current_flight);
                printf(" ^_^ Yey, flight %s has been deleted :)\n", flight_id); // Success message
            } else {
                printf(" T_T Oh no, deletion has been stopped\n"); // Cancellation message
            }
            return;
        }
        prev_flight = current_flight; // Move to next flight in list
        current_flight = current_flight->next;
    }

    printf(" T_T Oh no, flight ID %s doesn't exist\n", flight_id); // Error message
}

// Compare passengers and arrange alphabetically
    // Returns negative, p1 comes first
    // Returns positive, p2 comes first 
    // Returns zero, same name
int compare_passengers(struct passenger *p1, struct passenger *p2) {
    int last_name_cmp = strcmp(p1->last_name, p2->last_name);
    if (last_name_cmp != 0) {
        return last_name_cmp;
    }
    return strcmp(p1->first_name, p2->first_name);
}

// Removes passenger and insert them in order
void insert_passenger_in_order(struct passenger **head, struct passenger *new_passenger) {
    if (*head == NULL || compare_passengers(*head, new_passenger) > 0) {
        new_passenger->next = *head;
        *head = new_passenger;
    } else {
        struct passenger *current = *head;
        while (current->next != NULL && compare_passengers(current->next, new_passenger) <= 0) {
            current = current->next;
        }
        new_passenger->next = current->next;
        current->next = new_passenger;
    }
}

void add_passenger() {
    // Allocate memory for new passenger
    struct passenger *new_passenger = (struct passenger*)malloc(sizeof(struct passenger));
    if (new_passenger == NULL) {
        printf(" T_T Failed to allocate memory\n"); // Error message
        return;
    }

    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║   Add Passenger    ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Ask user for passenger details
    printf(" ^_^ Enter first name: ");
    scanf(" %[^\n]", new_passenger->first_name);

    printf(" ^_^ Enter last name: ");
    scanf(" %[^\n]", new_passenger->last_name);

    // Validate and ask for date of birth
    struct tm current_time;
    get_datetime(&current_time);
    printf(" ^_^ Enter date of birth (Month/Day/Year): ");
    scanf("%d/%d/%d", &new_passenger->dob.month, &new_passenger->dob.day, &new_passenger->dob.year);

    // Validate month and day
    if (new_passenger->dob.month < 1 || new_passenger->dob.month > 12) {
        printf(" T_T Oh no, invalid birth month\n");
        free(new_passenger);
        return;
    }
    if (new_passenger->dob.day < 1 || new_passenger->dob.day > 31) {
        printf(" T_T Oh no, invalid birth day\n");
        free(new_passenger);
        return;
    }

    // Check if date of birth is in the future
    if (new_passenger->dob.year > current_time.tm_year + 1900 || // Year is in future
        (new_passenger->dob.year == current_time.tm_year + 1900 && new_passenger->dob.month > current_time.tm_mon + 1) || // Month is in future
        (new_passenger->dob.year == current_time.tm_year + 1900 && new_passenger->dob.month == current_time.tm_mon + 1 && new_passenger->dob.day > current_time.tm_mday)) { // Day is in future
        printf(" T_T Oh no, date of birth can't be in future\n"); // Validation message
        free(new_passenger);
        return;
    }

    // Ask for passport number
    printf(" ^_^ Enter passport number: ");
    scanf(" %[^\n]", new_passenger->passport_number); // %[^\n] to read all characters until newline

    new_passenger->miles = 0; // Initial miles
    new_passenger->next = NULL;

    // Insert new_passenger into passengers_list alphabetically 
    if (passengers_list == NULL || compare_passengers(new_passenger, passengers_list) < 0) {
        new_passenger->next = passengers_list;
        passengers_list = new_passenger;
    } else {
        struct passenger *current_passenger = passengers_list;
        while (current_passenger->next != NULL && compare_passengers(new_passenger, current_passenger->next) >= 0) {
            current_passenger = current_passenger->next;
        }
        new_passenger->next = current_passenger->next;
        current_passenger->next = new_passenger;
    }

    printf(" ^_^ Yey, passenger has been added :)\n"); // Success message
}

void edit_passenger() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║   Edit Passenger   ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    // Ask for passport number
    char passport_number[15];
    printf(" ^_^ Enter passport number: ");
    scanf(" %[^\n]", passport_number);

    // Find passenger with the same passport number
    struct passenger *current_passenger = passengers_list;
    struct passenger *prev_passenger = NULL;
    while (current_passenger != NULL) {
        // If passport number matches, enable editing
        if (strcmp(current_passenger->passport_number, passport_number) == 0) {
            // Show current passenger details
            printf("Passenger details of passport number %s:\n", current_passenger->passport_number);
            printf("First name: %s\n", current_passenger->first_name);
            printf("Last name: %s\n", current_passenger->last_name);
            printf("Date of birth: %d/%d/%d\n", current_passenger->dob.month, current_passenger->dob.day, current_passenger->dob.year);

            // Validate new last name
            printf(" ^_^ Enter new last name: ");
            char new_last_name[50];
            scanf(" %[^\n]", new_last_name);

            // Validate new date of birth
            struct tm current_time;
            get_datetime(&current_time);
            int new_dob_month, new_dob_day, new_dob_year;
            printf(" ^_^ Enter new date of birth (Month/Day/Year): ");
            scanf("%d/%d/%d", &new_dob_month, &new_dob_day, &new_dob_year);

            // Validate month and day
            if (new_dob_month < 1 || new_dob_month > 12) {
                printf(" T_T Oh no, invalid birth month\n");
                return;
            }
            if (new_dob_day < 1 || new_dob_day > 31) {
                printf(" T_T Oh no, invalid birth day\n");
                return;
            }

            // Check if date of birth is in the future
            if (new_dob_year > current_time.tm_year + 1900 || // Year is in future
                (new_dob_year == current_time.tm_year + 1900 && new_dob_month > current_time.tm_mon + 1) || // Month is in future
                (new_dob_year == current_time.tm_year + 1900 && new_dob_month == current_time.tm_mon + 1 && new_dob_day > current_time.tm_mday)) { // Day is in future
                printf(" T_T Oh no, date of birth can't be in the future\n"); // Validation message
                return;
            }

            // Remove passenger from list
            if (prev_passenger == NULL) {
                passengers_list = current_passenger->next;
            } else {
                prev_passenger->next = current_passenger->next;
            }

            // Update passenger details
            strcpy(current_passenger->last_name, new_last_name);
            current_passenger->dob.month = new_dob_month;
            current_passenger->dob.day = new_dob_day;
            current_passenger->dob.year = new_dob_year;

            // Reinsert passenger into list in correct order
            insert_passenger_in_order(&passengers_list, current_passenger);

            printf(" ^_^ Yey, passenger details have been edited\n"); // Success message
            return;
        }
        prev_passenger = current_passenger;
        current_passenger = current_passenger->next; // Move to next node
    }

    printf(" T_T Oh no, passport number %s doesn't exist\n", passport_number); // Error message
}

void book_flight_reservation() {
    // Header with border for design
    printf("\n");
    printf(" ╔═══════════════════════╗\n");
    printf(" ║Book Flight Reservation║\n");
    printf(" ╚═══════════════════════╝\n");
    printf("\n");
    
    // Ask passport number
    char passport_number[15];
    printf(" ^_^ Enter passport number: ");
    scanf("%s", passport_number);

    // Find passenger
    struct passenger *current_passenger = passengers_list;
    // Loop through list to find
    while (current_passenger != NULL) {
        // If found, break
        if (strcmp(current_passenger->passport_number, passport_number) == 0) {
            break;
        }
        current_passenger = current_passenger->next;
    }

    if (current_passenger == NULL) {
        printf(" T_T Oh no, passport number %s doesn't exist\n", passport_number); // Error message if not found
        return;
    }

    char flight_id[15];
    printf(" ^_^ Enter flight ID: "); // Ask flight ID
    scanf("%s", flight_id);

    // Find flight
    struct flight *current_flight = flights_list;
    // Loop through list to find
    while (current_flight != NULL) {
        // If found, break
        if (strcmp(current_flight->flight_id, flight_id) == 0) {
            break;
        }
        current_flight = current_flight->next;
    }

    if (current_flight == NULL) {
        printf(" T_T Oh no, flight ID %s doesn't exist\n", flight_id); // Error message if not found
        return;
    }

    // Check overlapping reservations
    struct reservation *current_reservation = current_passenger->reservations;
    // Loop through list
    while (current_reservation != NULL) {
        // Find reservation with same ID
        if (strcmp(current_reservation->flight->flight_id, flight_id) == 0) {
            printf(" T_T Oh no, passenger is already reserved\n"); // Error message
            return;
        }
        current_reservation = current_reservation->next;
    }

    // Check available seats
    if (current_flight->booked_passengers >= current_flight->max_passengers) {
        printf(" T_T Oh no, flight is fully booked \n"); // Error message
        return;
    }

    // Book reservation
    struct reservation *new_reservation = (struct reservation*)malloc(sizeof(struct reservation)); // Allocate memory
    if (new_reservation == NULL) {
        printf(" T_T Failed to allocate memory\n"); // Error message
        return;
    }
    new_reservation->flight = current_flight;
    new_reservation->passenger = current_passenger;
    new_reservation->prev = NULL;

    // Update current passenger's reservation list
    if (current_passenger->reservations == NULL) {
        new_reservation->next = NULL;
        current_passenger->reservations = new_reservation;
    } else {
        new_reservation->next = current_passenger->reservations;
        current_passenger->reservations->prev = new_reservation;
        current_passenger->reservations = new_reservation;
    }

    // Update flight's booked passengers count
    current_flight->booked_passengers++;
    current_passenger->miles += current_flight->bonus_miles;

    printf(" ^_^ Yey, flight %s reservation has been booked\n", flight_id); // Success message
}

void remove_flight_reservation() {
    // Header with border for design
    printf("\n");
    printf(" ╔═════════════════════════╗\n");
    printf(" ║Remove Flight Reservation║\n");
    printf(" ╚═════════════════════════╝\n");
    printf("\n");
    
    char passport_number[15];
    printf(" ^_^ Enter passport number: "); // Asks for passport number
    scanf("%s", passport_number);

    // Find passenger
    struct passenger *current_passenger = passengers_list;
    // Loop through list
    while (current_passenger != NULL) {
        // If matching passport number found, break
        if (strcmp(current_passenger->passport_number, passport_number) == 0) {
            break;
        }
        current_passenger = current_passenger->next;
    }
    
    // If not found, print error message
    if (current_passenger == NULL) {
        printf(" T_T Oh no, passport number %s doesn't exist\n", passport_number);
        return;
    }

    char flight_id[15];
    printf(" ^_^ Enter flight ID: "); // Ask for flight ID
    scanf("%s", flight_id);

    // Find reservation
    struct reservation *current_reservation = current_passenger->reservations;
    // Loop through list
    while (current_reservation != NULL) {
        // If reservation found, break
        if (strcmp(current_reservation->flight->flight_id, flight_id) == 0) {
            break;
        }
        current_reservation = current_reservation->next;
    }
    
    // If not found, print error message
    if (current_reservation == NULL) {
        printf(" T_T Oh no, flight %s reservation doesn't exist\n", flight_id);
        return;
    }

    // Confirm deletion with user
    char confirmation;
    printf("Confirm deletion for flight %s reservation (y/n): ", flight_id);
    scanf(" %c", &confirmation);

    if (confirmation == 'y' || confirmation == 'Y') {
        // Remove reservation
        if (current_reservation->prev == NULL) {
            current_passenger->reservations = current_reservation->next;
            if (current_reservation->next != NULL) {
                current_reservation->next->prev = NULL;
            }
        } else {
            current_reservation->prev->next = current_reservation->next;
            if (current_reservation->next != NULL) {
                current_reservation->next->prev = current_reservation->prev;
            }
        }

        free(current_reservation);
        current_reservation->flight->booked_passengers--;
        current_passenger->miles -= current_reservation->flight->bonus_miles;

        printf(" ^_^ Yey, flight %s reservation has been removed\n", flight_id); // Success message
    } else {
        printf(" T_T Oh no, removal has been stopped\n"); // Cancellation message
    }
}

void view_reservations() {
    // Header with border for design
    printf("\n");
    printf(" ╔════════════════════╗\n");
    printf(" ║ View Reservations  ║\n");
    printf(" ╚════════════════════╝\n");
    printf("\n");

    char passport_number[15];
    printf(" ^_^ Enter passport number: "); // Ask passport number
    scanf("%s", passport_number);

    // Find the passenger
    struct passenger *current_passenger = passengers_list;
    // Loop through list
    while (current_passenger != NULL && strcmp(current_passenger->passport_number, passport_number) != 0) {
        current_passenger = current_passenger->next;
    }

    // If not found, print error message
    if (current_passenger == NULL) {
        printf(" T_T Oh no, passport number %s doesn't exist\n", passport_number);
        return;
    }

    // If the passenger has reservations, print them
    if (current_passenger->reservations != NULL) {
        // Print reservation details
        struct reservation *current_reservation = current_passenger->reservations;
        printf("Reservations for passenger %s %s:\n", current_passenger->first_name, current_passenger->last_name);
        while (current_reservation != NULL) {
            // Print flight details
            printf("Flight ID: %s\n", current_reservation->flight->flight_id);
            printf("Destination: %s\n", current_reservation->flight->destination);
            printf("Origin: %s\n", current_reservation->flight->origin);
            printf("Departure date: %d/%d/%d\n", current_reservation->flight->departure_date.month, current_reservation->flight->departure_date.day, current_reservation->flight->departure_date.year);
            printf("Departure time: %d:%d\n", current_reservation->flight->departure_time.hour, current_reservation->flight->departure_time.minute);
            printf("\n");
            current_reservation = current_reservation->next;
        }
    } else {
        // Open passengers.txt file
        FILE *passengers_file = fopen("passengers.txt", "r");
        if (passengers_file == NULL) {
            printf(" T_T Failed to open passengers.txt\n");
            return;
        }

        // Find the passenger in passengers.txt
        char line[1000];
        int found = 0;
        while (fgets(line, sizeof(line), passengers_file)) {
            char current_passport_number[15];
            sscanf(line, "%*[^|]|%*[^|]|%*d/%*d/%*d|%[^|]|%*d", current_passport_number);
            if (strcmp(current_passport_number, passport_number) == 0) {
                found = 1;
                // Print flight ID's (beyond 5th delimiter)
                char *reservation_token = strtok(line, "|");
                for (int i = 0; i < 5; i++) {
                    reservation_token = strtok(NULL, "|"); // Skip other passenger details
                }
                
                // Check if reservation token is "0"
                if (reservation_token == NULL || strcmp(reservation_token, "0") == 0) {
                    printf(" T_T Oh no, %s %s doesn't have a reservation\n", current_passenger->first_name, current_passenger->last_name);
                } else {
                    printf("Reserved flights for %s %s:\n", current_passenger->first_name, current_passenger->last_name);
                    while (reservation_token != NULL) {
                        printf("Flight ID: %s\n", reservation_token); // Print flight ID
                        reservation_token = strtok(NULL, "|"); // Move to next flight ID
                    }
                }
                break; // Stop once passenger is found
            }
        }
        fclose(passengers_file);

        // Error message
        if (!found) {
            printf(" T_T Oh no, passport number %s doesn't have a reservation\n", passport_number);
        }
    }
}

void save_data() {
    // Save flights data to file
    FILE *flights_file = fopen("flights.txt", "w");
    if (flights_file == NULL) {
        printf(" T_T Failed to open flights.txt\n");
        return;
    }

    // Write flight details to file
    struct flight *current_flight = flights_list;
    while (current_flight != NULL) {
        fprintf(flights_file, "%s|%s|%s|%d/%d/%d|%d/%d/%d|%d:%d|%d:%d|%d|%d|%d\n",
                current_flight->flight_id, current_flight->destination, current_flight->origin,
                current_flight->departure_date.month, current_flight->departure_date.day, current_flight->departure_date.year,
                current_flight->arrival_date.month, current_flight->arrival_date.day, current_flight->arrival_date.year,
                current_flight->departure_time.hour, current_flight->departure_time.minute,
                current_flight->arrival_time.hour, current_flight->arrival_time.minute,
                current_flight->booked_passengers, current_flight->max_passengers,
                current_flight->bonus_miles);
        current_flight = current_flight->next;
    }
    fclose(flights_file);

    // Save passengers data to file
    FILE *passengers_file = fopen("passengers.txt", "w");
    if (passengers_file == NULL) {
        printf(" T_T Failed to open passengers.txt\n");
        return;
    }

    // Write passengers data to file
    struct passenger *current_passenger = passengers_list;
    while (current_passenger != NULL) {
        fprintf(passengers_file, "%s|%s|%d/%d/%d|%s|%d",
                current_passenger->first_name, current_passenger->last_name,
                current_passenger->dob.month, current_passenger->dob.day, current_passenger->dob.year,
                current_passenger->passport_number, current_passenger->miles);
        
        // Write reservations data
        struct reservation *current_reservation = current_passenger->reservations;
        while (current_reservation != NULL) {
            fprintf(passengers_file, "|%s", current_reservation->flight->flight_id);
            current_reservation = current_reservation->next;
        }
        fprintf(passengers_file, "\n");
        
        current_passenger = current_passenger->next;
    }
    fclose(passengers_file);

    printf(" ^_^ Yey, data has been saved\n"); // Success message
}

void free_memory() {
    // Free memory allocated for flights
    struct flight *current_flight = flights_list;
    while (current_flight != NULL) {
        struct flight *temp_flight = current_flight;
        current_flight = current_flight->next;
        free(temp_flight);
    }
    flights_list = NULL;

    // Free memory allocated for passengers
    struct passenger *current_passenger = passengers_list;
    while (current_passenger != NULL) {
        struct passenger *temp_passenger = current_passenger;
        current_passenger = current_passenger->next;
        free(temp_passenger);
    }
    passengers_list = NULL;
}

void reset_data() {
    // Open file to overwrite content
    FILE *passengers_file = fopen("passengers.txt", "w");
    if (passengers_file == NULL) {
        printf(" T_T Failed to open passengers.txt\n"); //Error message
        return;
    }
    fclose(passengers_file);

    // Open file to overwrite content
    FILE *flights_file = fopen("flights.txt", "w");
    if (flights_file == NULL) {
        printf(" T_T Failed to open flights.txt\n"); //Error message
        return;
    }
    fclose(flights_file);

    // Reset lists
    free_memory();
    flights_list = NULL;
    passengers_list = NULL;

    printf(" ^_^ Yey, data has been reset\n"); //Success message
}