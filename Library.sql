CREATE DATABASE LIBRARY;

USE LIBRARY;

CREATE TABLE Books (
    book_id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL,
    author VARCHAR(255) NOT NULL,
    isbn VARCHAR(255) UNIQUE NOT NULL,
    quantity INT NOT NULL
);

CREATE TABLE Users (
    user_id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    phone VARCHAR(15)
);

CREATE TABLE IssuedBooks (
    issue_id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    book_id INT NOT NULL,
    issue_date DATE NOT NULL,
    return_date DATE,
    FOREIGN KEY (user_id) REFERENCES Users(user_id),
    FOREIGN KEY (book_id) REFERENCES Books(book_id)
);

DESCRIBE Books;
DESCRIBE Users;
DESCRIBE IssuedBooks;

SELECT * FROM Books;
SELECT * FROM Users;
SHOW GRANTS;

INSERT INTO Books (title, author, isbn, quantity)
VALUES ('1984', 'George Orwell', '9780451524935', 10);

DELETE FROM Books
WHERE isbn = '9780451524935';

