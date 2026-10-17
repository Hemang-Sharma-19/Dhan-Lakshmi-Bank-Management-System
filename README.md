
# Dhan Lakshmi Bank Management System

A console-based banking application developed in C using structures, functions, and file handling.

## Features

- Create a new bank account
- Customer login using account number and PIN
- Admin login
- Deposit money
- Withdraw money
- Transfer funds between accounts
- View account details
- Maintain transaction records
- Store account and transaction data using files

## Technologies Used

- **Language:** C
- **Concepts:** Structures, Functions, File Handling, Pointers
- **Compiler:** GCC (MinGW on Windows)
- **Editor:** Visual Studio Code

## Project Structure

```text
Dhan-Lakshmi-Bank-Management-System/
├── bank_management.c
├── README.md
└── .gitignore
```

## How to Run

1. Install a compatible C compiler on Windows.
2. Open the project folder in VS Code.
3. Open the terminal in the project directory.
4. Compile the program:

   ```bash
   gcc bank_management.c -o bank_management
   ```

5. Run the program in the terminal:

   ```powershell
   .\bank_management.exe
   ```

**Note:** This program uses `conio.h` and `_getch()`, so it requires a compatible Windows C compiler.

## Data Storage

The program uses local files to store account information and transaction records.

- `account.dat` — account data
- `transaction.dat` — transaction history

These files are generated or used locally by the application and should not be uploaded to GitHub if they contain account information.

## Disclaimer

This project was developed for educational purposes as part of learning C programming and file handling. It is not intended for real banking operations. Do not use real banking credentials or sensitive financial information.
