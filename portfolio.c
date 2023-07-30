#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ANSI color escape codes
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"

void display_menu() {
    printf("\n------ MENU ------\n");
    printf("1. Add Allocation\n");
    printf("2. Display Allocations\n");
    printf("3. Money Left\n");
    printf("4. Exit\n");
}

struct Portfolio {
    float allocation, percentage_of_income;
    char name_allocation[50];
};

float calc_percent_inc(float income, float allocation) {
    float percentage_inc;
    percentage_inc = ((100*allocation)/income);
    return percentage_inc;
}

float money_reserve(float sum_of_all_allocations, float income) {
    float money_in_reserve;

    if (sum_of_all_allocations < income) {
            money_in_reserve = income - sum_of_all_allocations;
        }
    
    return money_in_reserve;
}

int main() {
    float income, money_left;
    float sum_of_all_allocations = 0;
    int count = 0;
    int money_reserve_flag = 1;
    struct Portfolio *portfolio = NULL;
    int choice;

    printf("Welcome to your Portfolio!\n");

    printf("Enter the initial amount: ");
    scanf("%f", &income);

    do {
        display_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                count++;
                
                // creating a temp pointer to allocate memory
                struct Portfolio *temp = (struct Portfolio*)realloc(portfolio, count * sizeof(struct Portfolio));

                if (temp != NULL) {
                    portfolio = temp; // if its sucessfully allocated, assign it to portfolio
                } else {
                    free(portfolio); // in case of failure. Free the previous memory that was sucessfully allocated.
                    printf("Memory allocation failed.");
                    exit(EXIT_FAILURE); // note that temp doesnt need to be freed bc it failed to have its memory allocated.
                }

                memset(&portfolio[count-1], 0, sizeof(struct Portfolio)); // initializing with zeros

                do {
                    float temp_portfolio_allocation; // temp allocation bc we dont know if the user will have enough money to sucessfully allocate it.
                    printf("How much money are you going to allocate: \n");
                    scanf("%f", &temp_portfolio_allocation);

                    money_left = money_reserve(sum_of_all_allocations, income);
                    
                    if (temp_portfolio_allocation > money_left) {
                        printf(ANSI_COLOR_RED "You don't have enough money for that allocation!\n" ANSI_COLOR_RESET);
                        printf("You have left: " ANSI_COLOR_GREEN"%.2f" ANSI_COLOR_RESET "\n", money_left);
                        money_reserve_flag = 1; // set flag to 1 (keep asking for a sucessful allocation)
                    } else {
                        portfolio[count-1].allocation = temp_portfolio_allocation;
                        sum_of_all_allocations += portfolio[count-1].allocation;
                        money_reserve_flag = 0; // set flag to 0 (get out of the loop)
                    }
                    } while (money_reserve_flag == 1);

                money_left = money_reserve(sum_of_all_allocations, income);
        
                printf("Name of allocation: \n");
                scanf(" %[^\n]", &portfolio[count-1].name_allocation); //count 

                portfolio[count-1].percentage_of_income = calc_percent_inc(income, portfolio[count-1].allocation); // note that count - 1 bc we start with count++, so to get every index we do count - 1

                break;
            case 2: // loop through every allocation 
                for (int i = 0; i < count; i++) { 
                   printf("You allocated %.2f " ANSI_COLOR_GREEN "(%.2f%%)" ANSI_COLOR_RESET " of %.2f to %s\n", portfolio[i].allocation, portfolio[i].percentage_of_income, income, portfolio[i].name_allocation);
                    }   
                break;
            case 3: // show money that is left
                money_left = money_reserve(sum_of_all_allocations, income);
                printf("You still have " ANSI_COLOR_GREEN "%.2f" ANSI_COLOR_RESET " to spend", money_left);
                break;
            case 4:
                printf("Exiting the program\n");
                break;
            default:
                printf("Invalid Entry, please try again\n");
                break;
                } } while (choice != 4);

    free(portfolio);

    return 0;
}