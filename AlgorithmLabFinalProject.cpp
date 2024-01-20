#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

//KUTUPHANE OTOMASYONU


//Kitap Islem Yapilari
typedef struct Book {
    int barcode;
    char bookTitle[80];
    char authorName[80];
} book;

//Kitap Islem Yapilari
typedef struct Member {
    char id[30];
    char fullName[80];
    char birthDate[20];
    int status;
} member;

//Kitap Islem Yapilari
typedef struct Loan {
    char memberId[20];
    int bookBarcode;
    int daysToUse;
    int daysUsed;
    int status;
} loan;


//Kullanici Islemleri

//Kullaniciya ID veren fonksiyon
char* generateUniqueID() {
    srand((unsigned int)time(NULL));
    int randomID = rand() % 9000 + 1000;

    char idString[5];
    sprintf(idString, "%d", randomID);

    char* uniqueID = (char*)malloc(10);   //malloc kisminda yardim aldim ,  dinamik bellek tahsis edilirmis ve ID saklarmis.
    strcpy(uniqueID, idString);

    return uniqueID;
}

//Kullanici Ekleme Fonksiyonu
void addMember() {
    system("cls");
    printf("Member Addition Screen \n\n");

    member m1;
    char* generatedID = generateUniqueID();
    strcpy(m1.id, generatedID);
    free(generatedID); //bellegi bosaltir , oylesine ekledim

    printf("Full Name    : ");
    scanf(" %[^\n]s", m1.fullName);
    printf("Birth Date   : ");
    scanf(" %[^\n]s", m1.birthDate);
    m1.status = 0; //bos kalacak !

    FILE *filePtr = fopen("members.dat", "a+b");
    fwrite(&m1, sizeof(member), 1, filePtr);
    fclose(filePtr);

    printf("Member record complete.\n");
}

//Kullanici Silme Fonksiyonu
void removeMember() {
    system("cls");
    printf("Member removal screen \n\n");

    member m1;
    int result = 0; // silme isleminin sonucunu saklamak için kullandim (backup)
    char memberId[20];

    printf("ID     : ");
    scanf(" %[^\n]s", memberId);

    FILE *filePtr = fopen("members.dat", "r+b");
    FILE *backupPtr = fopen("backup.dat", "w+b");

    while (fread(&m1, sizeof(member), 1, filePtr) != NULL) {
        if (strcmp(memberId, m1.id) != 0)
            fwrite(&m1, sizeof(member), 1, backupPtr);
        else
            result = 1;
    }

    fclose(filePtr);
    fclose(backupPtr);

    if (result == 0)
        printf("Member record with ID %s not found.\n", memberId);
    else {
        remove("members.dat");
        rename("backup.dat", "members.dat");
        printf("Member record with ID %s deleted.\n", memberId);
    }
}

//Kullanici Listeleme Fonksiyonu
void listMembers() {
    system("cls");
    printf("Member listing screen \n\n");

    member m1;
    int memberCount = 0;

    FILE *filePtr = fopen("members.dat", "r+b");

    printf("%-30s%-40s%-30s%-10s\n", "-ID-", "-FULL NAME-", "-BIRTH DATE-", "-STATUS-");

    while (fread(&m1, sizeof(member), 1, filePtr) != NULL) {
        printf("%-30s%-40s%-30s%-10d-\n", m1.id, m1.fullName, m1.birthDate);
        memberCount++;
    }

    fclose(filePtr);
    printf("\nTotal member count: %d\n", memberCount);
}
 
//Kullanici Aratan Fonksiyon
void searchMembers() {
    system("cls");
    printf("Member search screen \n\n");

    char searchID[30];
    printf("Enter the member ID to search: ");
    scanf(" %[^\n]s", searchID);

    member m1;
    int found = 0;

    FILE *filePtr = fopen("members.dat", "r+b");

    printf("%-20s%-30s%-20s%-8s\n", "ID", "FULL NAME", "BIRTH DATE", "STATUS");

    while (fread(&m1, sizeof(member), 1, filePtr) == 1) {
        if (strcmp(m1.id, searchID) == 0) {
            printf("%-20s%-30s%-20s%-8d-\n", m1.id, m1.fullName, m1.birthDate, m1.status);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Member with ID %s not found.\n", searchID);
    }

    fclose(filePtr);
}

//Kullanicilari Guncelleyen Fonksiyon
void updateMember(char *idPtr) {
    member m1;
    int result = 0;

    FILE *filePtr = fopen("members.dat", "r+b");

    while (fread(&m1, sizeof(member), 1, filePtr) == 1) {
        if (strcmp(idPtr, m1.id) == 0) {
            result = 1;
            
            printf("Enter new full name   : ");
            scanf(" %[^\n]s", m1.fullName);
            printf("Enter new birth date  : ");
            scanf(" %[^\n]s", m1.birthDate);

            fseek(filePtr, -sizeof(member), SEEK_CUR);
            fwrite(&m1, sizeof(member), 1, filePtr);
            
            printf("Member information updated.\n");
            break;
        }
    }

    fclose(filePtr);

    if (result == 0) {
        printf("Member with ID %s not found.\n", idPtr);
    }
}

//Kullanici Islemleri Menusu Fonksiyonu
void memberOperations() {
    system("cls");
    printf("Member Operations Screen\n\n");

    int choice;
    printf("1- Add Member\n");
    printf("2- Remove Member\n");
    printf("3- List Members\n");
    printf("4- Search for Members\n");
    printf("5- Update Member\n");
    printf("0- Back to main menu\n");
    printf("Your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addMember();
            break;
        case 2:
            removeMember();
            break;
        case 3:
            listMembers();
            break;
        case 4:
            searchMembers();
            break;
        case 5:
    {
        char memberId[30];
        printf("Enter the member ID to update: ");
        scanf(" %[^\n]s", memberId);
        updateMember(memberId);
    }
    break;
            break; 
        case 0:
            break;
        default:
            printf("Invalid choice, redirecting to the main menu !\n");
            break;
    }
}

//Kitap Islemleri

//Kitaplar Icin Barkod Ureten Fonksiyon
int generateAutomaticBarcode() {
    int highestBarcode = 0;
    FILE* ptr = fopen("books.dat", "rb");

    if (ptr != NULL) {
        book tempBook;
        while (fread(&tempBook, sizeof(book), 1, ptr) == 1) {
            if (tempBook.barcode > highestBarcode) {
                highestBarcode = tempBook.barcode;
            }
        }
        fclose(ptr);
    }

    return highestBarcode + 1;
}

//Kitap Guncelleyen Fonksiyon
void updateBook() {
    system("cls");
    printf("Book update screen \n\n");
    book b1;
    
    int barcode;
    printf("Enter the book barcode to update: ");
    scanf("%d", &barcode);

    FILE* filePtr = fopen("books.dat", "r+b");
    int found = 0;

    while (fread(&b1, sizeof(book), 1, filePtr) == 1) {
        if (barcode == b1.barcode) {
            found = 1;

            printf("Enter new book title   : ");
            scanf(" %[^\n]s", b1.bookTitle);
            printf("Enter new author name  : ");
            scanf(" %[^\n]s", b1.authorName);

            fseek(filePtr, -sizeof(book), SEEK_CUR);
            fwrite(&b1, sizeof(book), 1, filePtr);

            printf("Book with barcode %d updated.\n", barcode);
            break;
        }
    }

    fclose(filePtr);

    if (!found) {
        printf("Book with barcode %d not found.\n", barcode);
    }
}

//Kitap Ekleyen Fonsiyon
void addBook() {
    system("cls");
    printf("Book addition screen \n\n");

    book b1;

    b1.barcode = generateAutomaticBarcode();
    printf("Automatically generated barcode: %d\n", b1.barcode);

    printf("Book title   : ");
    scanf(" %[^\n]s", b1.bookTitle);
    printf("Author name  : ");
    scanf(" %[^\n]s", b1.authorName);

    FILE* ptr = fopen("books.dat", "a+b");
    fwrite(&b1, sizeof(book), 1, ptr);
    fclose(ptr);

    printf("Book record complete\n");
}

//Kitap Silen Fonksiyon
void deleteBook() {
    system("cls");
    printf("Book deletion screen.... \n\n");

    book b1;
    int barcode, result = 0;

    printf("Barcode      : ");
    scanf("%d", &barcode);

    FILE* ptr = fopen("books.dat", "rb");
    FILE* yptr = fopen("backup.dat", "wb");

    while (fread(&b1, sizeof(book), 1, ptr) == 1) {
        if (barcode != b1.barcode) {
            fwrite(&b1, sizeof(book), 1, yptr);
        } else {
            result = 1;
        }
    }

    fclose(ptr);
    fclose(yptr);

    if (result == 0) {
        printf("%d barcode book not found \n", barcode);
    } else {
        remove("books.dat");
        rename("backup.dat", "books.dat");
        printf("%d barcode book deleted \n", barcode);
    }
}

//Kitaplari Listeleyen Fonksiyon
void listBooks() {
    system("cls");
    printf("Book listing screen \n\n");

    book b1;
    int bookCount = 0;

    FILE* ptr = fopen("books.dat", "r+b");

    printf("%-20s%-30s%-30s\n", "BARCODE", "BOOK-TITLE", "AUTHOR-NAME");

    while (fread(&b1, sizeof(book), 1, ptr) == 1) {
        printf("%-20d%-30s%-30s\n", b1.barcode, b1.bookTitle, b1.authorName);
    }

    fclose(ptr);
}

//Kitap Aratan Fonsiyon
void searchforBook() {
    system("cls");
    printf("Book search screen \n\n");

    char searchKeyword[80];
    printf("Enter the keyword to search for in book titles: ");
    scanf(" %[^\n]s", searchKeyword);

    FILE* filePtr = fopen("books.dat", "rb");
    book b1;
    int found = 0;

    printf("%-20s%-30s%-30s\n", "BARCODE", "BOOK-TITLE", "AUTHOR-NAME");

    while (fread(&b1, sizeof(book), 1, filePtr) == 1) {
    	
        if (strstr(b1.bookTitle, searchKeyword) != NULL) {
            printf("%-20d%-30s%-30s\n", b1.barcode, b1.bookTitle, b1.authorName);
            found = 1;
        }
    }

    fclose(filePtr);

    if (!found) {
        printf("No books found with the keyword '%s' in the title.\n", searchKeyword);
    }
}

//Kitap Islemleri Menusu Fonksiyonu
void bookOperations() {
    system("cls");
    printf("Book operations screen.... \n\n");

    int choice;
    printf("1- Add Book \n");
    printf("2- Delete Book \n");
    printf("3- List Books \n");
    printf("4- Update Book \n");
    printf("5- Search for Book \n");
    printf("0- Return to main menu \n");
    printf("Your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            deleteBook();
            break;
        case 3:
            listBooks();
            break;
        case 4:
            updateBook();
            break;
        case 5:
            searchforBook();
            break;
        case 0:
            break;
        default:
            printf("Invalid choice, redirecting to the main menu... \n");
            break;
    }
}

//Odunc Kitap Islemleri

//Kitap Odunc Alan Fonksiyon
void borrow() {
	system("cls");
    printf("Borrow Screen \n\n");
	
}

//Kitap Iade Eden Fonksiyon
void returnBook() {
	system("cls");
    printf("Return Book Screen \n\n");
	
}

//Tum Odunc Islemlerini Listeleyen Fonksiyon
void borrowOperations() {
	system("cls");
    printf("Borrow Operations Screen \n\n");
	
}

//Odunc Islemler Menusu
void loanOperations() {
    system("cls");
    printf("Loan Operations Screen \n\n");

    int choice;
    printf("1- Borrow a Book \n");
    printf("2- Return a Book \n");
    printf("3- All Loan Operations by User\n");
    printf("0- Return to main menu \n");
    printf("Your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            borrow();
            break;
        case 2:
            returnBook();
            break;
        case 3:
            borrowOperations();
            break;
        case 0:
            break;
        default:
            printf("Invalid choice, redirecting to the main menu... \n");
            break;
    }
}

//Menu
int main() {
    int option;

    do {
        printf("Library Management System\n");
        printf("1- Book Operations \n");
        printf("2- Member Operations \n");
        printf("3- Loan Operations \n");
        printf("0- Exit \n");
        printf("Your choice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                bookOperations();
                break;
            case 2:
            	memberOperations();
                break;
            case 3:
            	loanOperations();
                break;
            case 0:
                printf("Exiting the program... \n");
                break;
            default:
                printf("Invalid choice, please try again... \n");
                break;
        }

    } while (option != 0);

    return 0;
}


