    #include<stdio.h>
    #include<string.h>
    #include<time.h>
    #include<conio.h>
    #include<math.h>
    #include<limits.h>
    #include<ctype.h>
    #define acc_length 5


    #define ACCOUNT_FILE "account.dat"
    #define TRANSACTION_FILE "transaction.dat"

    void clear_input_buffer() {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
            // Discard characters until newline or EOF
        }
    }

    void create_account();
    void deposit_money(int logged_in_acc);
    void withdraw_money(int logged_in_acc);
    void check_balance(int logged_in_acc);
    void view_all_accounts();
    void admin_login();
    void admin_menu();
    void customer_menu(int logged_in_acc);
    void transaction_history(int logged_in_acc);
    int save_transaction(int acc_no, char *type, float amount, float balance);
    void delete_account();
    void search_account();
    void update_account();
    int customer_login();
    void transfer_money(int logged_in_acc);
    int get_pin(char pin[] , int size);
    int auto_generate_account_number();
    int save_transfer_transactions(
        int sender_acc, int receiver_acc,
        float amount, float sender_balance,
        float receiver_balance
    );




        typedef struct{
            char name[40];
            int acc_no;
            char pin[5];
            float balance;
            



    }account;

        typedef struct{
            int acc_no;
            char type[15];
            float amount;
            float balance;
            char date[20];
            char time[20];

    }transaction;

        int main(){

        

            while(1){
                int choice;

                printf("================================");
                printf("\nWELCOME TO DHAN LAKSHMI BANK\n");
                printf("================================");

                printf("\n1: CUSTOMER LOGIN");
                printf("\n2. CREATE ACCOUNT");
                printf("\n3: ADMIN LOGIN");
                printf("\n4: EXIT\n");

                printf("\nSELECT YOUR CHOICE\n");
                if(scanf("%d", &choice) != 1) {
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer();
                    continue;
                }
                clear_input_buffer(); // Clear the input buffer after reading the choice


        
        
            switch(choice){

                case 1:
                    { int logged_in_acc = customer_login();
                    if(logged_in_acc != -1){
                        customer_menu(logged_in_acc);
                    }
                }
                
                break;
                case 2:
                create_account();
                break;
                case 3:
                admin_login();
                break;
                case 4:
                printf("\nThank you for using DHAN LAKSHMI BANK.\n");
                return 0;
                default:
                printf("invalid choice\n,try again :-) ");
                break;



            }
        }
    }

        void create_account(){
            account acc;//object name account 

            account acc_r;//structure variaable to read the existing accounts

            FILE *file = fopen("account.dat","ab+");// generally extensi0n of binary number,append mode to used

                if(file == NULL){ //    wether this fopen has given us a file pointer or not 

                    printf("\nUnable to open a file\n");

                return;

            }
        
            printf("\n Enter the Name of account holder:");

            fgets(acc.name,sizeof(acc.name),stdin);// name will have space between them thats why fgets    
        // remove the new line character added by fgets()

            int index = strcspn(acc.name, "\n");// search for \n;

            acc.name[index] = '\0';//single character uses'' 
            int has_non_spaces = 0;
            for(int i = 0;  acc.name[i] != '\0';i++){
                if(!isspace((unsigned char)acc.name[i])){
                    has_non_spaces = 1;
                    break;
                }
            }

            if(!has_non_spaces){
                printf("Error: Account holder name cannot be emopty or consist only of spaces.\n");
                fclose(file);
                return;
            }

            acc.acc_no = auto_generate_account_number();
            if(acc.acc_no == -1){
                printf("Account creation failed.\n");
                return;
                
            }
            printf("Your ACCOUNT NUMBER IS: %d\n",acc.acc_no);


            printf("Enter the 4 digit PIN\n");

            if(!get_pin(acc.pin,sizeof(acc.pin))){
                printf("Warning:pin must be of 4 digits long\n");
                fclose(file);
                return;

            }
            

            if(strlen(acc.pin) != 4){
                printf("WARNING: PIN must be 4 digits long\n");
                fclose(file);
                return;

            }
            for(int i = 0;i<4;i++){

                if(acc.pin[i] < '0' || acc.pin[i] > '9')
                {
                    printf("WARNING: PIN must contain only digits\n");
                    fclose(file);
                    return;

                }
            }

        // move to the beginning of the file
            rewind(file);

            while(fread(&acc_r, sizeof(account),1,file) == 1)
        {
                if(acc_r.acc_no == acc.acc_no){//check wether this account is existing or not


                printf("\n=============================\n");
                printf("account number already exists\n");
                printf("enter other account number\n");
                printf("================================\n");

                fclose(file);// close the file
                
                return;//do not create the account

            }

        }
        // If account number is unique,
        // initialize the balance to zero

            acc.balance = 0;

            if(fwrite(&acc, sizeof(acc), 1, file) != 1 ) {
                printf("ERROR: Unable to save the new account record\n");
                fclose(file);
                return;
            }

            fclose(file);

            printf("\nACCOUNT CREATED SUCCESSFULLY\n");
                
        }

        
    void deposit_money(int logged_in_acc)
    {
        FILE *file = fopen(ACCOUNT_FILE, "rb+");

        if (file == NULL) {
            printf("Unable to open account file!\n");
            return;
        }

        float amount;
        account acc_r;
        int found = 0;

        printf("Enter amount to be credited: ");

        if (scanf("%f", &amount) != 1) {
            printf("Invalid input!\n");
            clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
            fclose(file);
            return;
        }
        clear_input_buffer(); // Clear the input buffer after reading the amount

        if (!isfinite(amount) || amount <= 0.0f){
            printf("Error: Invalid amount. Please enter a positive number.\n");
            fclose(file);
            return;
        }


        /* Read account records until the logged-in account is found */
        while (fread(&acc_r, sizeof(account), 1, file) == 1) {
            if (acc_r.acc_no == logged_in_acc) {
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Account not found!\n");
            fclose(file);
            return;
        }
    // Check whether the new balance is finite
        if (!isfinite(acc_r.balance + amount)) {
            printf("ERROR: Balance would become invalid.\n");
            fclose(file);
            return;
        }



        float old_balance = acc_r.balance;
        acc_r.balance += amount;

        /* Move back to the start of the matching account record */
        if (fseek(file, -(long)sizeof(account), SEEK_CUR) != 0) {
            printf("Unable to locate account record!\n");
            fclose(file);
            return;
        }

        /* Update the account balance */
        if (fwrite(&acc_r, sizeof(account), 1, file) != 1 ||
            fflush(file) != 0) {
            printf("Unable to update account balance!\n");
            fclose(file);
            return;
        }

        /* Save transaction history */
        if (!save_transaction(logged_in_acc, "deposit",
                            amount, acc_r.balance)) {

            /* Attempt to restore the previous balance */
            acc_r.balance = old_balance;

            if (fseek(file, -(long)sizeof(account), SEEK_CUR) == 0 &&
                fwrite(&acc_r, sizeof(account), 1, file) == 1 &&
                fflush(file) == 0) {
                printf("Deposit cancelled. Previous balance restored.\n");
            } else {
                printf("CRITICAL: Balance restoration failed!\n");
            }

            fclose(file);
            return;
        }

        printf("\nDeposit successful!\n");
        printf("Amount credited: Rs %.2f\n", amount);
        printf("New balance: Rs %.2f\n", acc_r.balance);

        fclose(file);
    }

        

        void withdraw_money(int logged_in_acc){
            FILE *file = fopen(ACCOUNT_FILE,"rb+");
                if(file == NULL){
                    printf("unable to open the account file\n");
        return;

        }

        
        float amount;
        account acc_r;
        int found = 0;
    
        printf("enter amount to be withdrawn:\n");
        if(scanf("%f", &amount ) != 1)
        {
            printf("invalid input\n");
            clear_input_buffer();
            fclose(file);
            return;

            }
            clear_input_buffer(); // Clear the input buffer after reading the amount

            if(!isfinite(amount) || amount <= 0.0f){
                printf("Error: Invalid amount. Please enter a positive number.\n");
                fclose(file);
                return;
            }

        while(fread(&acc_r,sizeof(acc_r),1,file) == 1){
            if(acc_r.acc_no == logged_in_acc){
                found = 1;
                break;
            }
        }
            if(!found) {
                printf("account not found\n");
                fclose(file);
                return;
            }
            if (!isfinite(acc_r.balance) || acc_r.balance < 0.0f ||
            !isfinite(acc_r.balance - amount)) {
            printf("ERROR: Invalid account balance or withdrawal result.\n");
            fclose(file);
            return;
        }       

        
            if(acc_r.balance < amount){
                printf("insufficient balance\n");
                fclose(file);
                return;

        }
        float old_balance = acc_r.balance;
        acc_r.balance  -= amount;

        if(fseek(file,-(long)sizeof(account),SEEK_CUR) != 0){
            printf("ERROR: Unable to locate account record\n");

        fclose(file);
        return;
            }
            if (fwrite(&acc_r, sizeof(account), 1, file) != 1 ||
                fflush(file) != 0) {
                printf("ERROR: Unable to update account balance!\n");
                fclose(file);
                return;
            }
            if(!save_transaction(logged_in_acc,"withdrawal",amount,acc_r.balance)){
                acc_r.balance = old_balance;
                if(fseek(file,-(long)sizeof(account),SEEK_CUR) == 0 &&
                fwrite(&acc_r,sizeof(account),1,file) == 1 && fflush(file) == 0){
                    printf("ERROR: Wthdrawal cancelled. Previous balanced restored.\n");
                }
                else{
                    printf("ERROR: Balance restoration failed!\n");

                }
                fclose(file);
                return;

                
                }



                printf("\nWithdrawal successful!\n");
                printf("Amount debited: Rs %.2f\n", amount);
                printf("Remaining balance: Rs %.2f\n", acc_r.balance);

                fclose(file);


                


        
    }
        void check_balance(int logged_in_acc){
            FILE *file = fopen(ACCOUNT_FILE,"rb");
                if(file == NULL){
                    printf("Unable to open the account file\n");

        return;
        
        }
        account acc_read;


            while(fread(&acc_read,sizeof(acc_read),1,file)){   
                if(acc_read.acc_no == logged_in_acc){
                    printf("=============================\n");
                    printf("    ACCOUNT BALANCE\n");
                    printf("=============================\n");
                    printf("Account No : %d\n", acc_read.acc_no);
                    printf("balance checked = RS %.2f\n " , acc_read.balance);
                    printf("=============================\n");

                    fclose(file);

        return;
            }
        }
        fclose(file);
        printf(" Account not found\n");

    }
        void admin_login(){
            char password[20];
            printf("enter the admin password:\n");
            scanf("%19s", password);

                if(strcmp(password, "admin123") == 0){
                    printf("\n login successful\n");
                    admin_menu();

        }
            else
        { 
                printf("\n login failed\n");

        }

    }
        void customer_menu(int logged_in_acc){


            while(1){
                int choice;

                printf("===============================\n");
                printf("       CUSTOMER MENU\n");
                printf("===============================\n");

                printf("Logged in account : %d\n", logged_in_acc);


                printf("\n1: Deposit Money");
                printf("\n2: Withdraw Money");
                printf("\n3: Check Balance");
                printf("\n4: Transaction History");
                printf("\n5: Transfer Money");
                printf("\n6: EXITING SYSTEM");
                printf("\nchoose one option\n");
                if(scanf("%d", &choice) != 1){
                    printf("Invalid input. Please enter a number.\n");
                    clear_input_buffer(); // Clear the input buffer
                    continue;
                }
                clear_input_buffer(); // Clear the input buffer after reading the choice

        
            switch(choice){
                
                
                case 1:
                deposit_money(logged_in_acc);
                break;
                case 2:
                withdraw_money(logged_in_acc);
                break;
                case 3:
                check_balance(logged_in_acc);
                break;
                case 4:
                transaction_history(logged_in_acc);
                break;

                case 5:
                transfer_money(logged_in_acc);
                break;

                case 6:
                printf("\nthank you for choosing dhan lakshmi bank, sorry for any inconvnience ");
            
                return ;

                default:
                printf("invalid choice\n,try again :-) ");
                break;



            }
        }
        }
        void admin_menu(){
            while(1){
            int choice;

                printf("===============================");
                printf(" ADMIN MENU");
                printf("===============================");
                
                printf("\n1: View All Accounts");
                printf("\n2: Delete Account");
                printf("\n3: Search Account");
                printf("\n4: update account");
                printf("\n5: EXIT");
                printf("\nchoose one option\n");
                if(scanf("%d", &choice) != 1){
                    printf("Invalid input. Please enter a number.\n");
                    while(getchar() != '\n'); // Clear the input buffer
                    continue;
                }
                getchar();

        
            switch(choice){

                case 1:
                view_all_accounts();
                break;
                case 2:
                delete_account();
                break;

                case 3:
                search_account();
                break;

                case 4:
                update_account();
                break;

                case 5:
                printf("\nthank you for choosing dhan lakshmi bank, sorry for any inconvnience ");
            
                return;

                default:
                printf("invalid choice\n,try again :-) ");
                break;
            }
        }
    }
        void view_all_accounts(){
        // we only want to read 
            FILE *file = fopen(ACCOUNT_FILE, "rb");// opening the account file in read only mode
            // check if the file is opened successfully
            if(file == NULL){
                printf("unabel to open the account file\n");
            return;

        }
            account acc_r;

            int found = 0;

            printf("================================");
            printf("\nALL ACCOUNTS\n");
            printf("================================");

            printf("%-30s %-15s %-10s\n", "NAME", "ACCOUNT NUMBER", "BALANCE");//herer balance is a word so thats why we usE String here

            printf("--------------------------------");

                while(fread(&acc_r, sizeof(acc_r), 1,file) == 1){
                    found = 1;

                    printf("%-30s %-15d %-10.2f\n", acc_r.name, acc_r.acc_no, acc_r.balance);// herer we actually meant with Money

            }
                if(!found){
                    printf("no accounts found\n");

            }
            fclose(file);// close the file after reading


    }
        int save_transaction(int acc_no, char *type, float amount, float balance){

            transaction trans;
            // storing trasnsaction information in the transaction struct

            trans.acc_no = acc_no;
            
            // Copy transaction type (Deposit / Withdraw)
            
            strncpy(trans.type,type, sizeof(trans.type) - 1);// trans.type is a character array of size 15, so
            
            trans.type[sizeof(trans.type) - 1] = '\0';  //ensure null termination
            
            trans.amount = amount;
            
            trans.balance = balance;
            // gets current date and time
            time_t now = time(NULL);
            struct tm *t = localtime(&now);

            if ( t == NULL) {
                printf("ERROR: Unable to get time and date for transaction\n");
                return 0;
            }
            // Store date
        strftime(trans.date, sizeof(trans.date), "%d-%m-%Y", t);

            // Store time
            strftime(trans.time, sizeof(trans.time), "%H:%M:%S", t);
            
            // opening the trasnaction file in append mode to add new transactions
            
            FILE *file = fopen(TRANSACTION_FILE, "ab+");

                if(file == NULL)
                {
                    printf("Unable to open the transaction file\n");
                    return 0;
                }
            // write one transaction record to the transaction file
            int success = (fwrite(&trans, sizeof(transaction), 1, file) == 1);
            
            if(success && fflush (file)!= 0) {
                success = 0;
                printf("ERROR: Unable to flush the data to the transaction file\n");
            }
            if (fclose(file) != 0 ) {
                success = 0;
                
            }
            if(!success) {
                printf("ERROR: Unable to save the transaction record\n");
            return 0;

            }
            return 1;// Transaction saved successfully

            
    }

    int save_transfer_transactions(
        int sender_acc, int receiver_acc,
        float amount, float sender_balance,
        float receiver_balance
    ) {
        transaction trans[2];

        memset(trans, 0, sizeof(trans));

        trans[0].acc_no = sender_acc;
        strcpy(trans[0].type, "transfer out");
        trans[0].amount = amount;
        trans[0].balance = sender_balance;

        trans[1].acc_no = receiver_acc;
        strcpy(trans[1].type, "transfer in");
        trans[1].amount = amount;
        trans[1].balance = receiver_balance;

        time_t now = time(NULL);
        struct tm *t = localtime(&now);

        if (t == NULL) {
            printf("ERROR: Unable to get date and time.\n");
            return 0;
        }

        strftime(trans[0].date, sizeof(trans[0].date),
                "%d-%m-%Y", t);
        strftime(trans[0].time, sizeof(trans[0].time),
                "%H:%M:%S", t);

        strcpy(trans[1].date, trans[0].date);
        strcpy(trans[1].time, trans[0].time);

        FILE *file = fopen(TRANSACTION_FILE, "ab");

        if (file == NULL) {
            printf("ERROR: Unable to open transaction file.\n");
            return 0;
        }

        int success =
            (fwrite(trans, sizeof(transaction), 2, file) == 2);

        if (success && fflush(file) != 0) {
            success = 0;
        }

        if (fclose(file) != 0) {
            success = 0;
        }

        if (!success) {
            printf("ERROR: Unable to save both transfer records.\n");
            return 0;
        }

        return 1;
    }

        void transaction_history(int logged_in_acc){
        
            FILE *file = fopen(TRANSACTION_FILE, "rb");
                if(file == NULL){  
                    printf("unable to open the transaction file\n");
                    return;
            }
            transaction trans_r;

            int found = 0;
        

            printf("================================");
            printf("\n    TRANSACTION HISTORY\n");
            printf("================================\n");

            printf("%-30s %-15s %-10s %-10s %-10s\n" , "TYPE", "AMOUNT","BALANCE", "DATE","TIME");

            // reads one item 1 at a time of size of sizeof(transaction) from the file and store in variable trans_r 
                while(fread(&trans_r , sizeof(transaction), 1, file) == 1){
                    if(trans_r.acc_no == logged_in_acc){
                        found = 1;
                        printf("%-30s %-15.2f %-10.2f %-10s %-10s\n", trans_r.type, trans_r.amount, trans_r.balance, trans_r.date,trans_r.time);
                    }
            }
                if(!found){
                    printf("no transactions found for the given account number\n");
                }
            fclose(file);

    }
        void delete_account(){
            FILE *file = fopen(ACCOUNT_FILE,"rb");
            if(file==NULL){
                printf("unable to open the account file.\n");
                return;
                
            }
            FILE *temp = fopen("temp.dat","wb");

            if(temp == NULL){
                printf(" unable to open the temporary file.\n");
                fclose(file);
                return;

            }
            int acc_no;// stores the account number to be deleted
            int found = 0;

            account acc;

            printf("Enter the account number to be deleted:\n");
            if(scanf("%d",&acc_no) != 1){
                printf("Error: Invalid input. Please enter a valid account number.\n");
                fclose(file);
                fclose(temp);
                remove("temp.dat");
                return;
            }

            int copy_ok = 1;

                while(fread(&acc,sizeof(acc),1,file) == 1){
                    if(acc.acc_no == acc_no){// if the account number matches
                    found = 1;// seting the found to 1 
                    // skip the writing this record to temp.dat
                    continue;
                }
                if(fwrite(&acc,sizeof(acc),1,temp) != 1){
                    copy_ok = 0;
                    break;
                }
                // write all the accounts to the temop files 
                
            }
            if (ferror(file)){
                    copy_ok = 0;
                }
            if (fclose(file) !=0){
                copy_ok = 0;

            }
            if(fclose(temp) !=0){
                copy_ok = 0;
            }
            if(!copy_ok ){
                printf("ERROR: Unable to copy records to temporary file.\n");
                remove("temp.dat");
                return;


            }
            
    if (found) {
        if (rename(ACCOUNT_FILE, "account_backup.dat") != 0) {
            printf("ERROR: Could not back up the original account file.\n");
            remove("temp.dat");
            return;
        }

        if (rename("temp.dat", ACCOUNT_FILE) != 0) {
            printf("ERROR: Could not install updated account file.\n");

            if (rename("account_backup.dat", ACCOUNT_FILE) != 0) {
                printf("CRITICAL: Restore account_backup.dat manually.\n");
            } else {
                printf("Original account file restored.\n");
            }

            remove("temp.dat");
            return;
        }

        printf("Account has been successfully deleted.\n");

        if (remove("account_backup.dat") != 0) {
            printf("Warning: Backup file could not be removed.\n");
        }
    } else {
        remove("temp.dat");
        printf("Account does not exist with the given account number.\n");
    }

    }
        void search_account(){
            FILE *file = fopen(ACCOUNT_FILE, "rb");

                if(file == NULL){
                    printf("unable to open the account file.\n");
                return;

            }
            int acc_no;// stores the account number to be searched 
            int found = 0;//used to check wether the account is found or not

            account acc_r;

            printf("enter the account number to be searched:\n");
            if(scanf("%d", &acc_no) != 1){
                printf("Error: Invalid input. Please enter a valid account number.\n");
                clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
                fclose(file);
                return;
            }

                while(fread(&acc_r, sizeof(account), 1, file) == 1)//read one complete account record from the file and put its data inside the acc_r struct one by one
            {
                    if(acc_r.acc_no == acc_no){
                        found = 1;
                        
                        printf("===============================");
                        printf("      ACCOUNT DETAILS:\n");
                        printf("===============================");

                        printf("Name     : %s\n", acc_r.name);
                        printf("Account No: %d\n", acc_r.acc_no);
                        printf("Balance  : %.2f\n", acc_r.balance);

                        printf("===============================");
                        // account found so no need to continue searching further
                        break;

                    }
                }

                        if(!found){
                        printf("Account with account number %d does not found.\n", acc_no);
                    
                }
                fclose(file);


            
        }
    void update_account(){
        FILE *file = fopen(ACCOUNT_FILE,"rb+");

        if(file == NULL){
            printf("unable to open the file.\n");
            return;
        }

        int acc_no;
        int found = 0;
        account acc;

        printf("\n enter the account number to update\n");
        if(scanf("%d" , &acc_no) != 1){
            printf("Warning: Invalid input for account number.\n");
            clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
            fclose(file);
            return;
        }
        clear_input_buffer(); // Clear the input buffer after reading the account number

        while(fread(&acc, sizeof(account),1,file) == 1){
            if(acc.acc_no == acc_no){
                found = 1;
                
                printf("==========================\n");
                printf("     CURRENT DETAILS      \n");
                printf("==========================\n");

                printf("Name      : %s\n", acc.name);
                printf("Account no: %d\n", acc_no);
                printf("balance   : %.2f\n", acc.balance);

                printf("enter new account holder name:\n");
                fgets(acc.name , (sizeof(acc.name)), stdin);


                // remove the newline added by the fgets
                acc.name[strcspn(acc.name,"\n")] = '\0';//it remove the \n
                // fread moves the file pointer to the next record
                //move back to one record in file so we can overwrite the old account


                // now program will not give succesfull message even if the write fails
                //because we are checking the return values of fseek, frwite and flush 
                if(fseek(file, -(long)sizeof(account), SEEK_CUR) != 0 ||
                    fwrite(&acc, sizeof(account), 1, file) != 1 || 
                    fflush(file) != 0){
                        printf("ERROR: Unable to update the account record\n");
                        fclose(file);
                        return;
                }
                printf("===================================\n");
                printf("    ACCOUNT UPDATED SUCCESSFULLY\n   ");
                printf("===================================\n");

                printf("Name      : %s\n", acc.name);
                printf("Account No: %d\n", acc.acc_no);
                printf("Balance   : %.2f\n", acc.balance);

                break;
        }
    }
            if(!found){
                printf("Account with account number = %d does not found, TRY again\n", acc_no);

            }
            fclose(file);
    }
    int customer_login(){
        FILE *file = fopen(ACCOUNT_FILE,"rb");

        if (file == NULL){
            printf("unable to open the file:");
            return -1;

        }
        int acc_no;
        char pin[5];
        account acc;
        printf("\n================================\n");
        printf("        CUSTOMER LOGIN\n");
        printf("================================\n");
        printf("Enter the account number");
        if(scanf("%d", &acc_no) != 1){
            printf("Invalid input for account number.\n");
            clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
            fclose(file);
            return -1;
        }
        getchar();
        // will check the acc number is correct or not , We do this before asking the pin

        int account_found = 0;

        while(fread(&acc,sizeof(account),1,file) == 1){
            if(acc.acc_no == acc_no){
                account_found = 1;
                break;

            }
            
        }
        if(!account_found){
            printf("\nWARNING: Wrong account number, pleaase try again\n");
            fclose(file);
            return -1;
            
        }
        // now give 3 chance to write the PIN correctly

        for(int attempts = 0; attempts < 3; attempts++){

        
            printf("Enter PIN:");

            int valid_pin = get_pin(pin,sizeof(pin));
            

            

                if(valid_pin && strcmp(acc.pin,pin) == 0){
                    printf("LOGIN SUCCESSFULLY!!\n");
                    printf("WELCOME , %s!\n" , acc.name);

                    fclose(file);

                    return acc.acc_no;

                }
                // pin is wrong
                printf("WARNING: Wrong PIN, please try again\n");

                if(attempts < 2){
                    printf(" Remaining attempts: %d\n", 2 - attempts);

                }
            }
            // if loop finishes without returning, it means customer had used all 3 attempts
            printf("===============================\n");
            printf(" LOGIN FAILED, TRY AGAIN LATER\n");
            printf("===============================\n");
            fclose(file);
            return -1;

        }
    void transfer_money(int logged_in_acc){

        FILE *file = fopen(ACCOUNT_FILE,"rb+");// going with read write mode because we will read and write the file

        if(file == NULL){
            printf("Unable to open the file");
            return;


        }

        int receiver_acc_no;
        float amount;
        
        // sturcture variable to store the account details of sender and reciever
        account sender;
        account receiver;

        // these files will store the position of sender and receiver in record inside the file
    

        // use to check wether the sender and receiver exist

        int sender_found = 0;
        int receiver_found = 0;
        long sender_position = -1;
        long receiver_position = -1;

        printf("\n===============================\n");
        printf("        TRANSFER MONEY\n");
        printf("\n===============================\n");

        //Ask the customer for the recivers account number

        printf("Enter receiver's account number: ");
        if(scanf("%d", &receiver_acc_no )!= 1){
            printf("Invalid input for account number.\n");
            clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
            fclose(file);
            return;
        }
        clear_input_buffer(); // Clear the input buffer after reading the account number
        if(receiver_acc_no == logged_in_acc){
            printf("You cannot transfer money to your own account.\n");
            fclose(file);
            return;
        }   

        printf("\nEnter the amount to be transferred: ");
        if(scanf("%f", &amount) != 1){
            printf("Invalid input for amount.\n");
            clear_input_buffer(); // Clear the input buffer to avoid issues with subsequent inputs
            fclose(file);
            return;
        }
        clear_input_buffer(); // Clear the input buffer after reading the amount

        if(!isfinite(amount) || amount <= 0.0f){
            printf("Error: Invalid amount. Please enter a positive number.\n");
            fclose(file);
            return;
        }

        // search the senders's account details in the file
        rewind(file);

        while(1)
        { 
            long pos = ftell(file);// get he current postion of the file pointer
            if(pos == -1 ||
                fread(&sender, sizeof(account) , 1, file) != 1){
                    break;

                }
            if(sender.acc_no == logged_in_acc){
                sender_position = pos;
                break;

            }
        
        }
        if(sender_position == -1){
            printf("Sender account not found\n");
            fclose(file);
            return;
        }

        // search for the reveiver's account details in the file
        rewind(file);

        while(1)
        { 
            long pos = ftell(file);// get he current postion of the file pointer
            if(pos == -1 ||
                fread(&receiver, sizeof(account) , 1, file) != 1){
                    break;

                }
            if(receiver.acc_no == receiver_acc_no){
                receiver_position = pos;
                break;
            }
        }
        if(receiver_position == -1){
            printf("Receiver account not found\n");
            fclose(file);
            return;
        }
        if(sender.balance < amount){
            printf("ERROR: Insufficient balance in sender's account\n");
            fclose(file);
            return;
        }   

        if (!isfinite(sender.balance) ||
        !isfinite(receiver.balance) ||
        !isfinite(sender.balance - amount) ||
        !isfinite(receiver.balance + amount)) {

        printf("ERROR: Transfer would result in an invalid balance.\n");
        fclose(file);
        return;
    }



        // save original account details for rollback in case of failure    
        account old_sender = sender;
        account old_receiver = receiver;

        // calculate the new balances for both 
        sender.balance -= amount;
        receiver.balance += amount;

        // update the sender's account record in the file
        if(fseek(file, sender_position, SEEK_SET) != 0
            || fwrite(&sender,sizeof(account),1,file) != 1 || 
            fflush(file) != 0){
                printf("ERROR: Unable to update sender's account\n");

                //attempt to restore the original sender's record
                if (fseek(file, sender_position, SEEK_SET) == 0 &&
                    fwrite(&old_sender, sizeof(account),1,file) == 1 &&
                    fflush(file) == 0){
                        printf("Original sender's balance restored.\n");
                    }    else{
                            printf("CRITICAL: Sender's balance restoration failed!\n");
                }
                    
                fclose(file);
                return;
            }

        // update the receiver's account record in the file
        if(fseek(file, receiver_position, SEEK_SET) != 0
            || fwrite(&receiver,sizeof(account),1,file) != 1 ||
            fflush(file) != 0){
                printf("ERROR: Unable to update receiver's account\n");
                int restored = 1;

        
        // Restore the sender's original record
            if (fseek(file, sender_position, SEEK_SET) != 0 ||//poitioning failed checking
                fwrite(&old_sender, sizeof(account), 1, file) != 1 ||//means writing the complete record failed
                fflush(file) != 0) {//means flushing the file buffer to disk failed
                restored = 0;
            }

            // Restore the receiver's original record
            if (fseek(file, receiver_position, SEEK_SET) != 0 ||
                fwrite(&old_receiver, sizeof(account), 1, file) != 1 ||
                fflush(file) != 0) {
                restored = 0;
            }
            // Report whether restoration succeeded
            if (restored){
                printf("Original balances restored.\n");

            }
            else
            {
                printf("CRITICAL: Balance restoration failed!\n");
            }
            fclose(file);
            return;
        }

                // Save transfer-out and transfer-in history records
        if (!save_transfer_transactions(
                sender.acc_no,
                receiver.acc_no,
                amount,
                sender.balance,
                receiver.balance)) {

            printf("Unable to save transfer history.\n");

            int restored = 1;

            // Restore sender's original balance
            if (fseek(file, sender_position, SEEK_SET) != 0 ||
                fwrite(&old_sender, sizeof(account), 1, file) != 1 ||
                fflush(file) != 0) {
                restored = 0;
            }

            // Restore receiver's original balance
            if (fseek(file, receiver_position, SEEK_SET) != 0 ||
                fwrite(&old_receiver, sizeof(account), 1, file) != 1 ||
                fflush(file) != 0) {
                restored = 0;
            }

            if (restored){
                printf("Original balances restored.\n");
            }
            else
            {
                printf("CRITICAL: Balance restoration failed!\n");
            }

            fclose(file);
            return;
        }

        
        



        printf("=================================\n");
        printf("     MONEY SENT SUCCESSFULLY\n");
        printf("=================================\n");

        printf("From Account: %d\n", sender.acc_no);
        printf("To Account: %d\n", receiver.acc_no);
        printf("Amount sent: %.2f\n", amount);
        printf("Balance remaining: %.2f\n", sender.balance);
        
        printf("===================================\n");


        //closing the file
        fclose(file);
        
    }

    int auto_generate_account_number(){

        FILE *file = fopen(ACCOUNT_FILE,"rb");
        account acc;
        int max_acc_no = 10000;

        if(file == NULL){
            return 10001;

        }
        while(fread(&acc,sizeof(account),1,file) == 1){
            if(acc.acc_no > max_acc_no){
                max_acc_no = acc.acc_no;
            }

        }
        fclose(file);
        if(max_acc_no == INT_MAX){
            printf("Error: Cannot generate another account number.\n");
            return -1;
        }
        return max_acc_no + 1;
        
    }
        int get_pin(char pin[] , int size){
            int i = 0;  
            int too_long = 0; // Flag to indicate if the input exceeds the maximum size
            char ch;

            //read keys until the user preses enter or the maximum size reached

            while((ch = _getch()) != '\r' && ch != '\n')// '\r' is carriage return, '\n' is newline
            
            {
                //handle backspace
                if(ch == 8)
                { 
                    if(too_long){
                        too_long = 0;
                        
                    }
                    else if((i > 0))
                    {
                        i--;// remove the last digit from the pin
                        printf("\b \b");// erase the displayed asterisk from the console
                    }
                }
                // accept only digits and check size limit
                else if( ch >= '0' && ch <= '9')
                {
                    if(i < size - 1)
                    {
                        pin[i++] = (char)ch;
                        printf("*");// display asterisk for each digit entered
                    }
                    else
                    {
                        too_long = 1; // Set the flag if the input exceeds the maximum size
                    printf("\nWARNING: PIN is too long. Maximum 4 digits allowed.\n");// Display a warning message
                    }

    }        }
            // end the pin string with null character
            pin[i] = '\0';
            printf("\n");

        return !too_long && i == 4;// Return 1 if the input was too long and exactly 4 digits were entered, otherwise return 0
        }

