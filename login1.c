#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>

// Define structure to store user data
struct User {
    char fullName[50];
    char email[50];
    char password[50];
    char phone[15]; // Increased size to accommodate phone numbers
    char username[50]; // Added username member
};

// Function prototypes
void signup();
void login();
void clearInputBuffer();
void printStars(const char str[]);
void takepassword(char pwd[50]);

int main() {
    int choice;

    printf("\n\t\t\t\t----------Welcome to the Authentication System----------\n");

    do {
        printf("\nPlease choose your operation:");
        printf("\n1. Signup");
        printf("\n2. Login");
        printf("\n3. Exit\n");
        printf("\nYour choice: ");
        scanf("%d", &choice);
        clearInputBuffer(); // Clear input buffer

        switch (choice) {
            case 1:
                signup();
                break;
            case 2:
                login();
                break;
            case 3:
                printf("Goodbye!");
                break;
            default:
                printf("Invalid choice! Please enter a number between 1 and 3.\n");
        }

        if (choice == 1 || choice == 2)
            break; // Exit the loop after signup or login

    } while (choice != 3);

    return 0;
}

void signup() {
    struct User newUser;

    printf("\nSign Up\n");
    printf("Enter your full name: ");
    fgets(newUser.fullName, sizeof(newUser.fullName), stdin);

    // Validate email (contains "@" and ends with ".com")
    do {
        printf("Enter your email: ");
        fgets(newUser.email, sizeof(newUser.email), stdin);
    } while (!(strstr(newUser.email, "@") && strstr(newUser.email, "gmail.com")));

    // Validate phone number (numeric only)
    int validPhoneNumber = 0;
    do {
        printf("Enter your phone number: ");
        fgets(newUser.phone, sizeof(newUser.phone), stdin);
        // Check if phone number contains only numeric characters
        validPhoneNumber = 1;
        for (int i = 0; i < strlen(newUser.phone) - 1; i++) {
            if (!isdigit(newUser.phone[i])) {
                validPhoneNumber = 0;
                printf("Invalid phone number! Please enter numeric characters only.\n");
                break;
            }
        }
    } while (!validPhoneNumber);

    // Take password securely
    takepassword(newUser.password);

    // Set username equal to email
    strcpy(newUser.username, newUser.email);

    printf("\nRegistration successful!\n");

    // Ask user if they want to show the password
    printf("Do you want to show your password? (1 for Yes, 0 for No): ");
    int showPassword;
    scanf("%d", &showPassword);
    clearInputBuffer(); // Clear input buffer

    if (showPassword) {
        printf("Your password is: %s\n", newUser.password);
    }
}

void login() {
    char email[50];
    char password[50];

    printf("\nLogin\n");
    printf("Enter your email: ");
    fgets(email, sizeof(email), stdin);
    printf("Enter your password: ");
    fgets(password, sizeof(password), stdin);

    printf("\nLogged in successfully!\n");
}

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to print characters as stars
void printStars(const char str[]) {
    for (int i = 0; i < strlen(str); i++) {
        printf("*");
    }
}

// Function to take password securely without showing characters
void takepassword(char pwd[50]) {
    int i = 0;
    char ch;
    int hasSymbol = 0;
    int hasDigit = 0;

    printf("Enter your password (at least one symbol and one numeric value required): ");

    while (1) {
        ch = getch();
        if (ch == '\n' || ch == '\r') {
            if (!hasSymbol || !hasDigit) {
                printf("\nPassword must contain at least one symbol and one numeric value. Please try again.\n");
                printf("Enter your password (at least one symbol and one numeric value required): ");
                i = 0;
                hasSymbol = 0;
                hasDigit = 0;
                continue;
            }
            pwd[i] = '\0';
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            pwd[i++] = ch;
            printf("*");
            if (!hasSymbol && ispunct(ch)) {
                hasSymbol = 1;
            }
            if (!hasDigit && isdigit(ch)) {
                hasDigit = 1;
            }
        }
    }
}
