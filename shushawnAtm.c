#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// STANDARD DEFINITIONS TO PROVIDE CROSS PLATFORM clear screen and pause commands
#if defined(_WIN32) || defined(_WIN64)
    #define clrscr() system("cls")
    #define pause() system("pause")
#else
    #define clrscr() printf("\x1B[1;1H\x1B[2J")
    #define pause() fflush(stdout); system("read -r -p \"Press any key to continue...\" key")
#endif

#define NAME_LEN 20

//stores user data
typedef struct UserData {

    char cardName [ NAME_LEN ] ;
    int   cardNumber ,
            cardPin ,
            chequingBalance ,
            savingsBalance ,
            otherBalance ;

} tUserData ;

//node connects user data
typedef struct UserNode {

    tUserData data ;
    struct Node *next ;

} tUserNode ;

//stores transaction
typedef struct HistoryData {

    char    operations [ NAME_LEN ] ,
                accountAffected [ NAME_LEN ] ;
    int     changeAmount ,
              timeStamp ;

} tHistoryData ;

//node connects transaction data
typedef struct HistoryNode {

    tHistoryData data ;
    struct Node *next ;

} tHistoryNode ;

//forward declaration of functions
void readFile ( tUserNode **userHead ) ;
tUserNode * addUserNode (  tUserNode **userHead, tUserNode *userCurrent ) ;
tUserNode * loginValidation ( tUserNode *userHead ) ;
int transaction ( tUserNode *userCurrent, tHistoryNode **historyHead ) ;
int menuChoice ( void ) ;
tHistoryNode * addHistoryNode ( tHistoryNode **historyHead, tHistoryNode *historyCurrent ) ;
tHistoryNode * depositFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead,  tHistoryNode *historyCurrent ) ;
tHistoryNode * withdrawalFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead, tHistoryNode *historyCurrent ) ;
tHistoryNode * payBillFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead, tHistoryNode *historyCurrent ) ;
void transactionHistory ( tHistoryNode *historyHead, tHistoryNode *historyCurrent ) ;
void cleanHistoryList ( tHistoryNode *historyHead ) ;
void cleanUserList ( tUserNode *userHead ) ;

void displayData ( tUserNode *userHead ) {
    tUserNode *userCurrent = userHead;

   while ( userCurrent != NULL ) {
        printf ( "%s %i %i %i %i %i \n",
                userCurrent->data.cardName, userCurrent->data.cardNumber, userCurrent->data.cardPin, userCurrent->data.chequingBalance,userCurrent->data.savingsBalance, userCurrent->data.otherBalance ) ;
        userCurrent = userCurrent -> next ;
   }

}

//after verifying data from login validation, allows user to make transactions
int main (  ) {

   int choice = 1 ;
    tUserNode     *userHead = NULL ,
                            *userCurrent = NULL ;
    tHistoryNode *historyHead = NULL ;



    readFile ( &userHead ) ;

    //displayData ( userHead ) ;
    do {
        userCurrent = loginValidation ( userHead) ;
        choice = transaction ( userCurrent, &historyHead ) ;
        cleanHistoryList ( historyHead ) ;
        historyHead = NULL ;
        system ( "cls" ) ;
    } while ( choice == 1 ) ;

    cleanUserList ( userHead ) ;
    userHead = NULL ;
}

//reads file and scans user data into linked list
void  readFile ( tUserNode **userHead ) {

    FILE *fp ;
    char fileName [ NAME_LEN ] = "atmUsers.txt" ;
    int userNum = 0 ;
    tUserNode  *userCurrent  ;

    fp = fopen ( fileName, "r" ) ;      //opens file

    if ( fp == NULL ) printf ( "File opened unsuccessfully. " ) ;

    userCurrent = *userHead ;
    while ( !feof ( fp ) ) {
        userCurrent = addUserNode ( userHead, userCurrent ) ;
        fscanf ( fp, " %[^\n] %i %i %i %i %i " , userCurrent->data.cardName, &userCurrent->data.cardNumber, &userCurrent->data.cardPin,
                                                                        &userCurrent->data.chequingBalance, &userCurrent->data.savingsBalance, &userCurrent->data.otherBalance ) ;
    }
    fclose ( fp ) ;
}

//adds new node to save user data. if head is null, new node is created.
tUserNode * addUserNode (  tUserNode **userHead,  tUserNode *userCurrent ) {

    tUserNode *userNew = malloc ( sizeof ( tUserNode ) ) ;
    userNew->next = NULL ;

    //add to the back of the list
    if ( (*userHead == NULL) || (userCurrent == NULL) ) {
        *userHead = userNew ;
        return *userHead ;
    }
    else {
        userCurrent -> next =  userNew ;
        userCurrent = userCurrent -> next ;
        userCurrent -> next = NULL ;

        return userCurrent ;
    }
    return userNew ;
}

//asks user for card number and card pin. if validated, allows user to account information. if not valid, tells user input invalid
tUserNode * loginValidation ( tUserNode *userHead ) {

    int i = 0 ,
           userNum = 0 ,
           userPin = 0 ;
    tUserNode *userCurrent = NULL;

    while ( i == 0) {
        printf ( "\t\t\t   Welcome to Shushawn's Bank!\n\nPlease enter the following data: " ) ;        //welcomes user and gets data

        printf ( "\nCard Number: " ) ;
        scanf ( "%i", &userNum ) ;

        printf ( "Pin: " ) ;
        scanf ( "%i", &userPin ) ;

        userCurrent = userHead ;
        while ( userCurrent != NULL ) {
            if ( userCurrent->data.cardNumber == userNum && userCurrent->data.cardPin == userPin ) {
                i = 2 ;
                break ;
            }
            else userCurrent = userCurrent -> next ;
        }

       if ( i == 0 ) {
            printf ( "\nAccount information incorrect. Please try again!\n" ) ;       //alerts user if the data entered is correct
            system ( "pause" ) ;
        }
        system ( "cls" ) ;
    }

    return userCurrent ;
}

//menu opens and user can choose to deposit, withdraw, pay a bill or see transaction history. when they are finished they can see final balances
int transaction ( tUserNode *userCurrent, tHistoryNode **historyHead ) {

    int i = 0 ,
          a = 0 ,
          choice = 0 ;
    char account ;
    tHistoryNode *historyCurrent = NULL ;

    do {
        printf ( "\nHi, %s!\n\t\t\t         Chequing        Savings        Other        \n================================================================================\n",
                    userCurrent->data.cardName ) ;
        printf ( "Your account balances are:\t%-15.2f\t%-15.2f\t%-15.2f\nOverdraft Limit: $1000\n",
                    ( ( float ) userCurrent->data.chequingBalance ) / 100, ( ( float ) userCurrent->data.savingsBalance / 100 ) , ( ( float ) userCurrent->data.otherBalance ) / 100 )  ;

        choice = menuChoice ( ) ;

        switch ( choice ) {

            case 0 :
                system ( "pause" ) ;
                system ( "cls" ) ;

                printf ( "Thank you for using Shushawn's Bank ATM!\n\nYour account balances are:\n\tChequing: %0.2f\n\tSavings: %0.2f\n\tOther: %0.2f\n\n",
                        (  ( float ) userCurrent->data.chequingBalance ) / 100, ( ( float ) userCurrent->data.savingsBalance / 100 ) , ( ( float ) userCurrent->data.otherBalance ) / 100 ) ;
                printf ( "\nWould you like to use another account? (1 for yes, 2 for no) : " ) ;
                scanf ( "%i", &i ) ;
                return i ;
                break ;

            case 1 :
                while ( 1 ) {
                    printf ( "\nPlease enter the account you would like to deposit to (c,s,o): " ) ;
                    scanf ( "%c", &account ) ;
                    getchar () ;

                    if ( ( account == 'c' ) || ( account == 's' )|| ( account == 'o' ) )  break ;
                    else printf ( "Invalid input. Only type c, s, or o.") ;
                }

                historyCurrent = depositFunction ( account, userCurrent, historyHead, historyCurrent ) ;
                break ;

            case 2:
                 while ( 1 ) {
                    printf ( "\nPlease enter the account you would like to withdraw from (c,s,o): " ) ;
                    scanf ( "%c", &account ) ;
                    getchar () ;

                    if ( ( account == 'c' ) || ( account == 's' )|| ( account == 'o' ) )  break ;
                    else printf ( "Invalid input. Only type c, s, or o.") ;
                }

                historyCurrent = withdrawalFunction ( account, userCurrent, historyHead, historyCurrent ) ;
                break ;

            case 3:
                 while ( 1 ) {
                    printf ( "\nPlease enter the account you would like to pay your bill from (c,s,o): " ) ;
                    scanf ( "%c", &account ) ;
                    getchar () ;

                    if ( ( account == 'c' ) || ( account == 's' )|| ( account == 'o' ) )  break ;
                    else printf ( "Invalid input. Only type c, s, or o.") ;
                }

                historyCurrent = payBillFunction ( account, userCurrent, historyHead, historyCurrent ) ;
                break ;

            case 4:
                system ( "cls" ) ;
                transactionHistory ( *historyHead, historyCurrent ) ;
                break ;

            default :
                break ;
        }
        system ( "pause" ) ;
        system ( "cls" ) ;
    } while ( i == 0 ) ;
}

//displays menu and returns user choice
int menuChoice ( ) {

    int choice = 0 ;

    printf ( "\n=======================\nWhat can we do for you?\n=======================\n\t1 - Deposit\n\t2 - Withdrawal\n\t3 - Pay a bill\n\t4 - See Transaction history\n\n\t0 - Quit" ) ;
    printf ( "\n\nCHOICE: " );
    scanf ( "%i", &choice ) ;
    getchar();

    return choice ;
}

//adds history node to store transaction. returns current position
tHistoryNode * addHistoryNode ( tHistoryNode **historyHead, tHistoryNode *historyCurrent ) {

    tHistoryNode *historyNew = malloc ( sizeof ( tHistoryNode ) ) ;

    // Add to the Front of the List
    historyNew->next = *historyHead;
    *historyHead = historyNew ;
     return *historyHead ;

    /*
    historyNew->next = NULL ;

    if ( (*historyHead == NULL) || (historyCurrent == NULL) ) {
        *historyHead = historyNew ;
        return *historyHead ;
    }
    else {
        historyCurrent -> next =  historyNew ;
        historyCurrent = historyCurrent -> next ;
        historyCurrent -> next = NULL ;

        return historyCurrent ;
    }
    */
}

//allows user to make multiple deposits. add the end updates final total
tHistoryNode * depositFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead, tHistoryNode *historyCurrent ) {

    int i = 0 ;
    float depositNum = 0 ;

    system ( "cls" ) ;

    while ( depositNum != -1 ) {
        printf ( "\nPlease enter deposit amount(-1 to end): " ) ;
        scanf ( "%f", &depositNum ) ;
        getchar () ;

        if ( depositNum == -1 ) break;
        if ( depositNum < -1 ) {
            printf ( "\nDeposit has to be greater than 0!\n" ) ;
            break ;
        }
        historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
        strcpy ( historyCurrent->data.operations, "Deposit" ) ;
        historyCurrent->data.changeAmount = ( int ) ( depositNum *100 ) ;
        historyCurrent->data.timeStamp = time(NULL) ;

        switch ( account ) {
            case 'c' :
                userCurrent->data.chequingBalance +=  ( int ) ( depositNum *100 ) ;
                strcpy ( historyCurrent->data.accountAffected, "Chequing"  ) ;
                break ;

            case 's' :
                userCurrent->data.savingsBalance +=  ( int ) ( depositNum *100 ) ;
                strcpy ( historyCurrent->data.accountAffected, "Savings"  ) ;
                break ;

            case 'o' :
                userCurrent->data.otherBalance +=  ( int ) ( depositNum *100 ) ;
                strcpy ( historyCurrent->data.accountAffected, "Other"  ) ;
                break ;

            default :
                break ;
        }
     printf ( "\nAccount updated. You have debited $%0.2f\n", depositNum ) ;
    }
    return historyCurrent ;
}

//allows user to withdraw money that is a multiple of 20/50 and that they have enough money for, including overdraft.
tHistoryNode * withdrawalFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead,  tHistoryNode *historyCurrent ) {

   float withdrawalNum = 0 ;
   int     maxWithdrawal = 0 ,
              numTwenty = 0,
              numFifty = 0,
              temp = 0;

    printf ( "\nPlease enter withdrawal amount: " ) ;
    scanf ( "%f", &withdrawalNum ) ;
    getchar () ;

   temp = (int)( withdrawalNum * 100 ) % 5000 ;
    if (  temp % 2000 != 0  ) {
        account = 'w' ;
    }
    else {
        numFifty  =  (int)( withdrawalNum * 100 )  / 5000  ;
        numTwenty = temp / 2000 ;
    }

    switch ( account ) {

        case 'c' :
            maxWithdrawal = 100000 + userCurrent->data.chequingBalance ;
            if ( maxWithdrawal >  ( int ) ( withdrawalNum *100 ) ) {
                userCurrent->data.chequingBalance =  userCurrent->data.chequingBalance - ( int ) ( withdrawalNum *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n\t%i x 50\n\t%i x 20\n", withdrawalNum, numFifty, numTwenty ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Withdrawal" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Chequing"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( withdrawalNum *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot withdraw that money.") ;
            break ;

        case 's' :
            maxWithdrawal = 100000 + userCurrent->data.savingsBalance ;
            if ( maxWithdrawal >  ( int ) ( withdrawalNum *100 ) ) {
                userCurrent->data.savingsBalance =  userCurrent->data.savingsBalance - ( int ) ( withdrawalNum *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n\t%i x 50\n\t%i x 20\n", withdrawalNum, numFifty, numTwenty ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Withdrawal" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Savings"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( withdrawalNum *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot withdraw that money.") ;
            break ;

        case 'o' :
            maxWithdrawal = 100000 + userCurrent->data.otherBalance ;
            if ( maxWithdrawal >  ( int ) ( withdrawalNum *100 ) ) {
                userCurrent->data.otherBalance =  userCurrent->data.otherBalance - ( int ) ( withdrawalNum *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n\t%i x 50\n\t%i x 20\n", withdrawalNum, numFifty, numTwenty ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Withdrawal" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Other"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( withdrawalNum *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot withdraw that money.") ;
            break ;

        default :
            printf ( "\nWithdrawal amount cannot be split into $20 and/or $50.\n" ) ;
            break ;
    }

    return historyCurrent ;
}

//allows user to pay a bill if they have the means to do so
tHistoryNode * payBillFunction ( char account, tUserNode *userCurrent, tHistoryNode **historyHead, tHistoryNode *historyCurrent ) {
    char billName [ NAME_LEN ] ;
    float billCost = 0 ;
    int maxBillCost = 0 ;

    printf ( "\nPlease enter the name of the bill you would like to pay for: " ) ;
    scanf ( "%s", billName ) ;
    printf ( "Please enter the cost of the bill: $ " ) ;
    scanf ( "%f", &billCost ) ;

    switch ( account ) {

        case 'c' :
            maxBillCost = 100000 + userCurrent->data.chequingBalance ;
            if ( maxBillCost >  ( int ) ( billCost *100 ) ) {
                userCurrent->data.chequingBalance =  userCurrent->data.chequingBalance - ( int ) ( billCost *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n", billCost ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Bill" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Chequing"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( billCost *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot pay for the bill.") ;
            break ;

        case 's' :
            maxBillCost = 100000 + userCurrent->data.savingsBalance ;
            if ( maxBillCost >  ( int ) ( billCost *100 ) ) {
                userCurrent->data.savingsBalance =  userCurrent->data.savingsBalance - ( int ) ( billCost *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n", billCost ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Bill" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Savings"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( billCost *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot pay for the bill.") ;
            break ;

        case 'o' :
            maxBillCost = 100000 + userCurrent->data.otherBalance ;
            if ( maxBillCost >  ( int ) ( billCost *100 ) ) {
                userCurrent->data.otherBalance =  userCurrent->data.otherBalance - ( int ) ( billCost *100 ) ;
                printf ( "\nAccount updated. You withdrew $%0.2f\n", billCost ) ;

                historyCurrent = addHistoryNode ( historyHead, historyCurrent ) ;
                strcpy ( historyCurrent->data.operations, "Bill" ) ;
                strcpy ( historyCurrent->data.accountAffected, "Other"  ) ;
                historyCurrent->data.changeAmount = ( int ) ( billCost *100 ) ;
                historyCurrent->data.timeStamp = time(NULL) ;
            }
            else printf ( "\nYou cannot pay for the bill.") ;
            break ;

        default :
            break ;
    }
    return historyCurrent ;
}

void transactionHistory ( tHistoryNode *historyHead, tHistoryNode *historyCurrent ) {

    historyCurrent = historyHead ;
    printf ( "Operation\tAccount Affected\tChange Amount\t\tTime\n================================================================================\n" ) ;
   while ( historyCurrent != NULL ) {
        printf ( "%-10s\t %-15s\t%-15.2f\t%s\n",
                historyCurrent->data.operations, historyCurrent->data.accountAffected, ( float )historyCurrent->data.changeAmount / 100, ctime(&historyCurrent->data.timeStamp) ) ;
        historyCurrent = historyCurrent -> next ;
   }
}

void cleanHistoryList ( tHistoryNode *historyHead ) {

    tHistoryNode *historyCurrent = historyHead ,
                             *historyTemp ;

    while ( historyCurrent != NULL ) {
        historyTemp = historyCurrent ;
        historyCurrent = historyCurrent->next ;
        free ( historyTemp ) ;
    }

}

void cleanUserList ( tUserNode *userHead ) {

     tUserNode *userCurrent = userHead ,
                        *userTemp ;

    while ( userCurrent != NULL ) {
        userTemp = userCurrent ;
        userCurrent = userCurrent->next ;
        free ( userTemp ) ;
    }
}
