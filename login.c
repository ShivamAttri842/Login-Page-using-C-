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
void takepassword(char pwd[50]);
void printStars(const char str[]); // Function to print stars
int authenticateUser(char email[], char password[]);
void getPasswordAndShowOption(char password[]); // Function to get password and show option during signup

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
    } while (!(strstr(newUser.email, "@") && strstr(newUser.email, ".com")));

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
    getPasswordAndShowOption(newUser.password);

    // Set username equal to email
    strcpy(newUser.username, newUser.email);

    // Write user data to file
    FILE *fp = fopen("Users.dat", "a+");
    if (fp != NULL) {
        fwrite(&newUser, sizeof(struct User), 1, fp);
        fclose(fp);
        printf("\nRegistration successful!\n");
    } else {
        printf("\nError: Unable to open file for writing.\n");
    }
}

void login() {
    char email[50];
    char password[50];

    printf("\nLogin\n");
    printf("Enter your email: ");
    fgets(email, sizeof(email), stdin);
    printf("Enter your password: ");
    takepassword(password);

    // Authenticate user
    int authResult = authenticateUser(email, password);
    if (authResult == 1) {
        printf("\nLogged in successfully!\n");
    } else {
        printf("\nInvalid email or password.\n");
        printf("Invalid password entered: %s\n", password);
    }
}

// Function to clear input buffer
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function to take password securely without showing characters
void takepassword(char pwd[50]) {
    int i = 0;
    char ch;

    while (1) {
        ch = getch();
        if (ch == '\n' || ch == '\r') {
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
        }
    }
}

// Function to print characters as stars
void printStars(const char str[]) {
    for (int i = 0; i < strlen(str); i++) {
        printf("*");
    }
}

// Function to get password and show option during signup
void getPasswordAndShowOption(char password[]) {
    int i = 0;
    char ch;
    int hasNumeric = 0; // Flag to track if password contains at least one numeric value
    int hasSymbol = 0; // Flag to track if password contains at least one symbol

    printf("Enter your password (at least one symbol and one numeric value required): ");
    while (1) {
        ch = getch();
        if (ch == '\n' || ch == '\r') {
            password[i] = '\0';
            break;
        } else if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
            if (!hasSymbol && ispunct(ch)) {
                hasSymbol = 1;
            }
            if (!hasNumeric && isdigit(ch)) {
                hasNumeric = 1;
            }
        }
    }

    // Show option to display password
    printf("\nDo you want to show your password? (1 for Yes, 0 for No): ");
    int showPassword;
    scanf("%d", &showPassword);
    clearInputBuffer(); // Clear input buffer

    if (showPassword) {
        printf("Your password is: %s\n", password);
    }

    // Validate password complexity
    if (!hasNumeric || !hasSymbol) {
        printf("Password must contain at least one symbol and one numeric value. Please try again.\n");
        getPasswordAndShowOption(password);
    }
}

// Function to authenticate user
int authenticateUser(char email[], char password[]) {
    struct User currentUser;

    FILE *fp = fopen("Users.dat", "r");
    if (fp != NULL) {
        while (fread(&currentUser, sizeof(struct User), 1, fp)) {
            if (strcmp(currentUser.email, email) == 0 && strcmp(currentUser.password, password) == 0) {
                fclose(fp);
                return 1; // Authentication successful
            }
        }
        fclose(fp);
    } else {
        printf("\nError: Unable to open file for reading.\n");
    }

    return 0; // Authentication failed
}
