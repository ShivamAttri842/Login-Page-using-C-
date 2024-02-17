#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h> // for isalnum and ispunct

#define ENTER 13
#define TAB 9
#define BCKSPC 8

// Define a structure to store user data
struct User {
    char fullName[50];
    char email[50];
    char password[50];
    char username[50];
    char phone[12]; // Increased size to accommodate Indian phone numbers
};

// Function prototypes
void getUserInput(char field[], char value[], int maxLength);
void getPassword(char password[], int maxLength);
int isStrongPassword(const char password[]); // Check if password is strong
void displayWelcomeMessage();
void signup();
void login();

int main() {
    system("color 0b");
    int opt;

    displayWelcomeMessage();

    printf("\n\nYour choice:\t");
    scanf("%d", &opt);
    fflush(stdin); // Clear input buffer

    switch (opt) {
        case 1:
            signup();
            break;
        case 2:
            login();
            break;
        case 3:
            printf("\t\t\tBye Bye :)");
            break;
        default:
            printf("Invalid choice");
    }

    return 0;
}

// Function to display welcome message and menu options
void displayWelcomeMessage() {
    printf("\n\t\t\t\t----------Welcome to the Authentication System----------");
    printf("\nPlease choose your operation");
    printf("\n1. Signup");
    printf("\n2. Login");
    printf("\n3. Exit");
}

// Function to prompt user for input
void getUserInput(char field[], char value[], int maxLength) {
    printf("\nEnter your %s:\t", field);
    fgets(value, maxLength, stdin);
    value[strcspn(value, "\n")] = 0; // Remove newline character
}

// Function to take password input without showing characters on screen
void getPassword(char password[], int maxLength) {
    int i = 0;
    char ch;
    int hasNumeric = 0; // Flag to track if password contains at least one numeric value
    int hasSymbol = 0; // Flag to track if password contains at least one symbol

    while (1) {
        ch = getch();
        if (ch == ENTER || ch == TAB) {
            password[i] = '\0'; // Terminate string
            break;
        } else if (ch == BCKSPC) {
            if (i > 0) {
                i--; // Move cursor back
                printf("\b \b"); // Clear character from screen
            }
        } else if (i < maxLength - 1) { // Ensure it doesn't exceed maxLength
            if (isdigit(ch)) { // Check if character is a digit
                password[i++] = ch; // Store character in password
                hasNumeric = 1; // Set numeric flag
            } else if (ispunct(ch)) { // Check if character is a symbol
                password[i++] = ch; // Store character in password
                hasSymbol = 1; // Set symbol flag
            }
            printf("*"); // Display '*' on screen (to hide password)
        }
    }

    // Check if password meets complexity requirements
    if (!hasNumeric || !hasSymbol) {
        printf("\n\nPassword must contain at least one numeric value and at least one symbol.\n");
        printf("Please try again.\n");
        getPassword(password, maxLength); // Prompt user for password again
    }
}

// Function to check if password is strong
int isStrongPassword(const char password[]) {
    int hasNumeric = 0;
    int hasSymbol = 0;

    for (int i = 0; password[i] != '\0'; i++) {
        if (isdigit(password[i])) {
            hasNumeric = 1;
        } else if (ispunct(password[i])) {
            hasSymbol = 1;
        }
    }

    return hasNumeric && hasSymbol;
}

// Function for signup operation
void signup() {
    system("cls");
    struct User user;

    getUserInput("full name", user.fullName, sizeof(user.fullName));
    getUserInput("email", user.email, sizeof(user.email));
    getUserInput("phone number", user.phone, sizeof(user.phone));
    printf("Enter your password (at least 6 characters, including at least one symbol and one numeric value):\t");
    getPassword(user.password, sizeof(user.password));

    // Prompt for username
    printf("\nEnter your desired username (or leave blank to use email as username):\t");
    fgets(user.username, sizeof(user.username), stdin);
    user.username[strcspn(user.username, "\n")] = 0; // Remove newline character

    if (strlen(user.username) == 0) {
        // If username is not provided, use email as username
        strcpy(user.username, user.email);
    }

    // Write user data to file
    FILE *fp = fopen("Users.dat", "a+");
    if (fp != NULL) {
        fwrite(&user, sizeof(struct User), 1, fp);
        printf("\n\nUser registration success, Your username is %s", user.username);
    } else {
        printf("\n\nSorry! Something went wrong :(\nPlease sign up again");
    }
    fclose(fp);
}

// Function for login operation
void login() {
    system("cls");
    char username[50], password[50];
    struct User user;

    getUserInput("username", username, sizeof(username));
    printf("Enter your password:\t");
    getPassword(password, sizeof(password));

    FILE *fp = fopen("Users.dat", "r");
    if (fp != NULL) {
        while (fread(&user, sizeof(struct User), 1, fp)) {
            if (!strcmp(user.username, username) && !strcmp(user.password, password)) {
                printf("\n\t\t\t\t\t\tWelcome %s", user.fullName);
                printf("\n\n|Full Name:\t%s", user.fullName);
                printf("\n|Email:\t\t%s", user.email);
                printf("\n|Username:\t%s", user.username);
                printf("\n|Phone No.:\t%s", user.phone);
                break;
            }
        }
        fclose(fp);
    } else {
        printf("\n\nError accessing user database. Please try again later.");
    }

    if (feof(fp)) {
        printf("\n\nInvalid username or password.");
    }
}
