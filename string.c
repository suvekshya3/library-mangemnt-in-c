#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_STUDENTS 50
#define MAX_ISSUED_BOOKS 500

struct Date {
    int day;
    int month;
    int year;
};

struct Book {
    int id;
    char title[100];
    char author[100];
    int available;
};

struct Student {
    int id;
    char name[100];
    int booksIssued;
};

struct IssuedBook {
    int bookId;
    int studentId;
    struct Date issueDate;
    struct Date returnDate;
};

void displayBooks(struct Book *library, int size) {
    printf("%-5s %-30s %-20s %-10s\n", "ID", "Title", "Author", "Available");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < size; ++i) {
        printf("%-5d %-30s %-20s %-10d\n", library[i].id, library[i].title, library[i].author, library[i].available);
    }
}

void displayStudents(struct Student *students, int size) {
    printf("%-5s %-30s %-10s\n", "ID", "Name", "Books Issued");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < size; ++i) {
        printf("%-5d %-30s %-10d\n", students[i].id, students[i].name, students[i].booksIssued);
    }
}

void displayIssuedBooks(struct IssuedBook *issuedBooks, int size) {
    printf("%-5s %-5s %-30s %-15s %-15s %-15s %-15s\n", "BookID", "StdID", "Book Title", "IssueDay", "IssueMonth", "IssueYear", "ReturnDate");
    printf("-----------------------------------------------------------------------------\n");
    for (int i = 0; i < size; ++i) {
        printf("%-5d %-5d %-30s %-15d %-15d %-15d %-15d\n",
               issuedBooks[i].bookId,
               issuedBooks[i].studentId,
               "TODO: Get Title",  // Book title information needs to be added
               issuedBooks[i].issueDate.day,
               issuedBooks[i].issueDate.month,
               issuedBooks[i].issueDate.year,
               issuedBooks[i].returnDate.year);
    }
}

void issueBook(struct Book *library, int bookCount, struct Student *students, int studentCount, struct IssuedBook *issuedBooks, int *issuedCount) {
    int bookId, studentId;

    printf("Enter Book ID to issue: ");
    scanf("%d", &bookId);

    // Find the book in the library
    int bookIndex = -1;
    for (int i = 0; i < bookCount; ++i) {
        if (library[i].id == bookId) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found!\n");
        return;
    }

    printf("Enter Student ID: ");
    scanf("%d", &studentId);

    // Find the student
    int studentIndex = -1;
    for (int i = 0; i < studentCount; ++i) {
        if (students[i].id == studentId) {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1) {
        printf("Student not found!\n");
        return;
    }

    // Check if the book is available
    if (library[bookIndex].available > 0) {
        // Issue the book
        library[bookIndex].available--;

        // Record the issued book
        issuedBooks[*issuedCount].bookId = bookId;
        issuedBooks[*issuedCount].studentId = studentId;

        // Get the current date
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);

        issuedBooks[*issuedCount].issueDate.day = tm.tm_mday;
        issuedBooks[*issuedCount].issueDate.month = tm.tm_mon + 1;
        issuedBooks[*issuedCount].issueDate.year = tm.tm_year + 1900;

        printf("Book issued successfully!\n");
        printf("Book Title: %s\n", library[bookIndex].title);
        printf("Student Name: %s\n", students[studentIndex].name);
        printf("Issue Date: %d-%02d-%02d\n", issuedBooks[*issuedCount].issueDate.year, issuedBooks[*issuedCount].issueDate.month, issuedBooks[*issuedCount].issueDate.day);

        (*issuedCount)++;
    } else {
        printf("Book not available for issuance.\n");
    }
}

void returnBook(struct Book *library, int bookCount, struct IssuedBook *issuedBooks, int issuedCount) {
    int bookId;

    printf("Enter Book ID to return: ");
    scanf("%d", &bookId);

    // Find the book in the library
    int bookIndex = -1;
    for (int i = 0; i < bookCount; ++i) {
        if (library[i].id == bookId) {
            bookIndex = i;
            break;
        }
    }

    if (bookIndex == -1) {
        printf("Book not found!\n");
        return;
    }

    // Find the issued book
    int issuedIndex = -1;
    for (int i = 0; i < issuedCount; ++i) {
        if (issuedBooks[i].bookId == bookId) {
            issuedIndex = i;
            break;
        }
    }

    if (issuedIndex == -1) {
        printf("Book is not currently issued.\n");
        return;
    }

    // Get the current date for return
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
issuedBooks[issuedIndex].returnDate.day = tm.tm_mday;
    issuedBooks[issuedIndex].returnDate.month = tm.tm_mon + 1;
    issuedBooks[issuedIndex].returnDate.year = tm.tm_year + 1900;

    // Increase the available count of the book in the library
    library[bookIndex].available++;

    printf("Book returned successfully!\n");
    printf("Book Title: %s\n", library[bookIndex].title);
    printf("Return Date: %d-%02d-%02d\n", issuedBooks[issuedIndex].returnDate.year, issuedBooks[issuedIndex].returnDate.month, issuedBooks[issuedIndex].returnDate.day);
}

int main() {
    struct Book library[MAX_BOOKS];
    struct Student students[MAX_STUDENTS];
    struct IssuedBook issuedBooks[MAX_ISSUED_BOOKS];

    // Initializing some sample data
    int bookCount = 3;
    library[0] = (struct Book){1, "The C Programming Language", "K&R", 5};
    library[1] = (struct Book){2, "Ikigai", "Héctor García ", 10};
    library[2] = (struct Book){3, "Rich dad poor dad ", "Robert", 3};

    int studentCount = 2;
    students[0] = (struct Student){101, "Ram karki", 0};
    students[1] = (struct Student){102, "Sita Thapa", 0};

    int issuedCount = 0;

    int choice;

    do {
        printf("\n<== Library Management System ==>\n");
        printf("1. Display Books\n");
        printf("2. Display Students\n");
        printf("3. Issue Book\n");
        printf("4. Return Book\n");
        printf("5. Display Issued Books\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayBooks(library, bookCount);
                break;
            case 2:
                displayStudents(students, studentCount);
                break;
            case 3:
                issueBook(library, bookCount, students, studentCount, issuedBooks, &issuedCount);
                break;
            case 4:
                returnBook(library, bookCount, issuedBooks, issuedCount);
                break;
            case 5:
                displayIssuedBooks(issuedBooks, issuedCount);
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);

    return 0;
} 
