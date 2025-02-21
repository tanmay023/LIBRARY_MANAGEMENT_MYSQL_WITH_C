#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <mysqld_error.h>
#include <string.h>

void manage_books();
void manage_users();
void issue_return();
void search_books();

void add_book(char title[], char author[], char isbn[], int quantity);
void remove_book(char isbn[]);
void update_book();
void list_all_books();

void add_user(char name[], char email[], char phone[]);
void remove_user(char name[]);
void update_user();
void list_all_users();

void issue_book();
void return_book();
void view_issued_books();

void by_title();
void by_author();
void by_isbn();

void database_conn(char query[]);

void database_conn(char query[]) {
    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "super";
    const char *database = "LIBRARY";

    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "MySQL initialization failed\n");
        return;
    }

    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        fprintf(stderr, "Database connection failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return;
    }

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    } else {
        if (strstr(query, "SELECT")) {
            MYSQL_RES *res = mysql_store_result(conn);
            if (res) {
                int num_fields = mysql_num_fields(res);
                MYSQL_ROW row;

                while ((row = mysql_fetch_row(res))) {
                	int i;
                    for (i = 0; i < num_fields; i++) {
                        printf("%s\t", row[i] ? row[i] : "NULL");
                    }
                    printf("\n");
                }
                mysql_free_result(res);
            }
        } else {
            printf("\nOperation completed successfully.\n");
        }
    }
    mysql_close(conn);
}

void update_book() {
    char isbn[20];
    printf("\nEnter ISBN of the book to update: ");
    scanf(" %19[^\n]", isbn);

    int choice;
    printf("\n1. Update Title\n2. Update Author\n3. Update Quantity\nChoice: ");
    scanf("%d", &choice);

    char query[300];
    switch(choice) {
        case 1: {
            char title[100];
            printf("Enter new title: ");
            scanf(" %99[^\n]", title);
            sprintf(query, "UPDATE Books SET title='%s' WHERE isbn='%s'", title, isbn);
            break;
        }
        case 2: {
            char author[100];
            printf("Enter new author: ");
            scanf(" %99[^\n]", author);
            sprintf(query, "UPDATE Books SET author='%s' WHERE isbn='%s'", author, isbn);
            break;
        }
        case 3: {
            int quantity;
            printf("Enter new quantity: ");
            scanf("%d", &quantity);
            sprintf(query, "UPDATE Books SET quantity=%d WHERE isbn='%s'", quantity, isbn);
            break;
        }
        default:
            printf("Invalid choice!\n");
            return;
    }
    database_conn(query);
}

void list_all_users() {
    char query[] = "SELECT * FROM Users";
    database_conn(query);
}

void remove_user(char name[]) {
    char query[200];
    sprintf(query, "DELETE FROM Users WHERE name='%s'", name);
    database_conn(query);
}

void add_user(char name[], char email[], char phone[]) {
    char query[300];
    sprintf(query, "INSERT INTO Users (name, email, phone) VALUES ('%s', '%s', '%s')", name, email, phone);
    database_conn(query);
}

void list_all_books() {
    char query[] = "SELECT * FROM Books";
    database_conn(query);
}

void remove_book(char isbn[]) {
    char query[200];
    sprintf(query, "DELETE FROM Books WHERE isbn='%s'", isbn);
    database_conn(query);
}

void add_book(char title[], char author[], char isbn[], int quantity) {
    char query[300];
    sprintf(query, "INSERT INTO Books (title, author, isbn, quantity) VALUES ('%s', '%s', '%s', %d)", title, author, isbn, quantity);
    database_conn(query);
}

void update_user() {
    char name[100];
    printf("\nEnter name of the user to update: ");
    scanf(" %99[^\n]", name);

    int choice;
    printf("\n1. Update Name\n2. Update Email\n3. Update Phone\nChoice: ");
    scanf("%d", &choice);

    char query[300];
    switch(choice) {
        case 1: {
            char new_name[100];
            printf("Enter new name: ");
            scanf(" %99[^\n]", new_name);
            sprintf(query, "UPDATE Users SET name='%s' WHERE name='%s'", new_name, name);
            break;
        }
        case 2: {
            char email[100];
            printf("Enter new email: ");
            scanf(" %99[^\n]", email);
            sprintf(query, "UPDATE Users SET email='%s' WHERE name='%s'", email, name);
            break;
        }
        case 3: {
            char phone[20];
            printf("Enter new phone: ");
            scanf(" %19[^\n]", phone);
            sprintf(query, "UPDATE Users SET phone='%s' WHERE name='%s'", phone, name);
            break;
        }
        default:
            printf("Invalid choice!\n");
            return;
    }
    database_conn(query);
}

void issue_book() {
    char name[100], isbn[20];
    printf("\nEnter user name: ");
    scanf(" %99[^\n]", name);
    printf("Enter book ISBN: ");
    scanf(" %19[^\n]", isbn);

    char issue_query[300];
    sprintf(issue_query, "INSERT INTO IssuedBooks (user_name, book_isbn, issue_date) VALUES ('%s', '%s', CURDATE())", name, isbn);
    database_conn(issue_query);

    char update_query[300];
    sprintf(update_query, "UPDATE Books SET quantity = quantity - 1 WHERE isbn='%s'", isbn);
    database_conn(update_query);
}

void return_book() {
    char name[100], isbn[20];
    printf("\nEnter user name: ");
    scanf(" %99[^\n]", name);
    printf("Enter book ISBN: ");
    scanf(" %19[^\n]", isbn);

    char return_query[300];
    sprintf(return_query, "DELETE FROM IssuedBooks WHERE user_name='%s' AND book_isbn='%s'", name, isbn);
    database_conn(return_query);

    char update_query[300];
    sprintf(update_query, "UPDATE Books SET quantity = quantity + 1 WHERE isbn='%s'", isbn);
    database_conn(update_query);
}

void view_issued_books() {
    char query[] = "SELECT * FROM IssuedBooks";
    database_conn(query);
}

void by_title() {
    char title[100];
    printf("\nEnter title to search: ");
    scanf(" %99[^\n]", title);
    char query[300];
    sprintf(query, "SELECT * FROM Books WHERE title LIKE '%%%s%%'", title);
    database_conn(query);
}

void by_author() {
    char author[100];
    printf("\nEnter author to search: ");
    scanf(" %99[^\n]", author);
    char query[300];
    sprintf(query, "SELECT * FROM Books WHERE author LIKE '%%%s%%'", author);
    database_conn(query);
}

void by_isbn() {
    char isbn[20];
    printf("\nEnter ISBN to search: ");
    scanf(" %19[^\n]", isbn);
    char query[300];
    sprintf(query, "SELECT * FROM Books WHERE isbn='%s'", isbn);
    database_conn(query);
}

void manage_books()
{
	int n;
	printf("\n---BOOKS MANAGEMENT MENU---\n");
	do{
		printf("\nWHAT OPERATION DO YOU WANT TO PERFORM?");
		printf("\n\n1.ADD BOOK.");
		printf("\n2.REMOVE BOOK.");
		printf("\n3.UPDATE BOOK.");
		printf("\n4.LIST ALL BOOKS.");
		printf("\n5.EXIT TO MAIN MENU.");
	
		printf("\n\nCHOOSE THE OPTION:");
		scanf("%d",&n);
	
	
		switch(n)
		{
			case 1:
				{
					char title[100], author[100], isbn[20];
					int quantity;
				
					printf("\nENTER TITLE OF BOOK YOU WANT TO ADD:");
					scanf(" %99[^\n]", title);
				
					printf("\nENTER THE AUTHOR OF BOOK:");
					scanf(" %99[^\n]", author);	
				
					printf("\nENTER THE ISBN NUMBER:");
					scanf(" %9[^\n]", isbn);
				
					printf("\nENTER THE QUANTITY OF BOOKS:");
					scanf("%d", &quantity);
				
					add_book(title,author,isbn,quantity);
				}
				
				break;
			
			case 2: 
				{
					char isbn[20];
					
					printf("\nENTER THE ISBN NUMBER OF BOOK YOU WANT TO DELETE:");
					scanf(" %10[^\n]", isbn);
					remove_book(isbn);	
				}
			
				break;
			
			case 3:update_book();
				break;
			
			case 4:list_all_books();
				break;
			
			case 5:
				break;
			
			default:
				printf("\nENTER VALID CHOICE!!!");	
		}
	}
	while(n!=5);
}

void manage_users()
{
	int n;
	printf("\n---USER MANAGEMENT MENU---\n");
	do{
		printf("\nWHAT OPERATION DO YOU WANT TO PERFORM?");
		printf("\n\n1.ADD USER.");
		printf("\n2.REMOVE USER.");
		printf("\n3.UPDATE USER.");
		printf("\n4.LIST ALL USERS.");
		printf("\n5.EXIT TO MAIN MENU.");
	
		printf("\n\nCHOOSE THE OPTION:");
		scanf("%d",&n);
	
	
		switch(n)
		{
			case 1:
				{
					char name[100];
					char email[100];
					char phone[20];
				
					printf("\nENTER NAME OF USER:");
					scanf(" %99[^\n]", name);
				
					printf("\nENTER EMAIL OF USER:");
					scanf(" %99[^\n]", email);
					
					printf("\nENTER PHONE NUMBER OF USER:");
					scanf(" %10[^\n]", phone); 
				
					add_user(name,email,phone);	
				}
				
				break;
			
			case 2: 
				{
					char name[100];
					
					printf("\nENTER NAME OF USER YOU WANT TO DELETE:");
					scanf(" %99[^\n]", name);
					remove_user(name);
				}			
					
				break;
			
			case 3:
				{

					update_user();	
				} 
				break;
			
			case 4: 
				{
					list_all_users();	
				}			
				break;
			
			case 5:
				break;
			
			default:
				printf("\nENTER VALID CHOICE!!!");	
		}
	}
	while(n!=5);
}

void issue_return()
{
	int n;
	printf("\n---ISSUE BOOK/RETURN BOOK MENU---\n");
	do{
		printf("\nWHAT OPERATION DO YOU WANT TO PERFORM?");
		printf("\n\n1.ISSUE BOOK.");
		printf("\n2.RETURN BOOK.");
		printf("\n3.VIEW ISSUED BOOKS.");
		printf("\n4.EXIT TO MAIN MENU.");
	
		printf("\n\nCHOOSE THE OPTION:");
		scanf("%d",&n);
	
	
		switch(n)
		{
			case 1: issue_book();
				break;
			
			case 2: return_book();
				break;
			
			case 3: view_issued_books();
				break;
			
			case 4: 
				break;
			
			default:
				printf("\nENTER VALID CHOICE!!!");	
		}
	}
	while(n!=4);
}

void search_books()
{
	int n;
	printf("\n---SEARCH MENU---\n");
	do{
		printf("\nHOW YOU WANT TO SEARCH THE BOOK?");
		printf("\n\n1.SEARCH BY TITLE.");
		printf("\n2.SEARCH BY AUTHOR.");
		printf("\n3.SEARCH BY ISBN NUMBER.");
		printf("\n4.EXIT TO MAIN MENU.");
		
		printf("\n\nCHOOSE THE OPTION:");
		scanf("%d",&n);
	
	
		switch(n)
		{
			case 1: by_title();
				break;
			
			case 2: by_author();
				break;
			
			case 3: by_isbn();
				break;
			
			case 4:
				break;

			default:
				printf("\nENTER VALID CHOICE!!!");	
		}
	}
	while(n!=4);
}

int main()
{  
    printf("---WELCOME TO THE LIBRARY MANAGEMENT SYSTEM---\n");
	int n;
	
	printf("\n---MAIN MENU---\n");
	do{	
		printf("\n1.MANAGE BOOKS.");
		printf("\n2.MANAGE USERS.");
		printf("\n3.ISSUE/RETURN BOOKS.");
		printf("\n4.SEARCH FOR BOOKS.");
		printf("\n5.EXIT SYSTEM.");
		
		printf("\n\nCHOOSE THE OPTION:");
			scanf("%d",&n);
		switch(n)
		{
			case 1 : manage_books();
				break;
				
			case 2: manage_users();
				break;
				
			case 3: issue_return();
				break;
				
			case 4:	search_books();
				break;
				
			case 5: printf("\nEXIT SYSTEM.");
				break;
				
			default:
				printf("\nENTER VALID CHOICE!!!");	
		}	
	}while(n!=5);
}
