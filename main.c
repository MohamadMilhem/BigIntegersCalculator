/*
 * Author: Mohammad Milhem
 * ID : 1211053
 * Sec: 3
 */

#include <stdio.h>
#include <stdlib.h>


typedef struct node node;
typedef struct expression expression;

#define MAX_LIMIT 1024


char numbers[MAX_LIMIT][MAX_LIMIT];
FILE *in, *out;
int file_size;

struct node {
    int value;
    struct node* next;
    struct node* pre;
};

struct expression {
    int index1, index2;
    char operation;
    struct node* result;
    struct expression* next;
};


expression *results, *last_result;

/* functions prototypes */

node* create_node(int);
expression* create_expression(int, int, char, node*);
void insert_node_after(node*, node*);
void insert_expression_after(expression*, expression*);
node* sum(node*, node*);
node* subtract(node*, node*);
node* multiply(node*, node*);
node* divide(node*, node*, node**);
int is_greater_or_equal(node*, node*);
node* convert_to_linked_list(char*);
void delete_node_list(node*);
void delete_expression_list(expression*);
void print_number(node*);
void Display_menu();
void read_file();
int driver();


/*
 * Allocates memory for a new node, assign values to the node fields.
 *
 * Parameters:
 *  - value: The data value we want to store in the new node.
 *
 * Returns:
 *  - A node pointer to the new node just created, with the values of the specified fields.
 */

node* create_node(int value){
    node* new_node = (node*)malloc(sizeof(node));

    new_node->value = value;
    new_node->pre = NULL;
    new_node->next = NULL;

    return new_node;
}

/*
 * Allocates memory for a new expression, assign values to the expression fields.
 *
 * Parameters:
 *  - idx1: the index of the first number in the expression.
 *  - idx2: the index of the second number in the expression.
 *  - operation: the operation of the expression.
 *  - result: the result of applying the operation on the numbers.
 *
 * Returns:
 *  - A expression pointer to the new expression just created, with the values of the specified fields.
 */


expression* create_expression(int idx1, int idx2, char operation ,node* result){
    expression* new_expression = (expression*)malloc(sizeof(expression));
    new_expression->index1 = idx1;
    new_expression->index2 = idx2;
    new_expression->operation = operation;
    new_expression->result = result;
    new_expression->next = NULL;
}


/*
 * Checks if a list represents a negative number by checking data field in the head node.
 *
 * Parameters:
 *  - list: node pointer to the head of the linked list we want to check.
 *
 * Returns:
 *  - if the list represents negative number returns 1. Otherwise, return 0.
 */

int isNegative(node* list){
    return list->value == -1;
}



/*
 * Deletes a linked list of nodes.
 *
 * Parameters:
 *  - list: node pointer to the head of the linked list we want to delete.
 *
 */

void delete_node_list(node* list) {
    while (list != NULL) {
        node* temp = list;
        list = list->next;
        free(temp);
    }
}


/*
 * Deletes a linked list of expressions.
 *
 * Parameters:
 *  - list: expression pointer to the head of the linked list we want to delete.
 *
 */

void delete_expression_list(expression* list){
    while(list != NULL){
        expression* temp = list;
        list = list->next;
        delete_node_list(temp->result);
        free(temp);
    }

}

/*
 * Insert a node in a linked list after a given node.
 *
 * Parameters:
 *  - pos: node pointer to the node we want to insert the new node after it.
 *  - new_node: node pointer to the node we want to insert.
 *
 */

void insert_after(node* pos, node* new_node){

    if (pos->next != NULL){
        new_node->next = pos->next;
        pos->next->pre = new_node;
    }

    pos->next = new_node;
    new_node->pre = pos;

}

/*
 * Insert a expression in a linked list after a given node.
 *
 * Parameters:
 *  - pos: node pointer to the expression we want to insert the new expression after it.
 *  - new_expression: expression pointer to the expression we want to insert.
 *
 */

void insert_expression_after(expression* pos, expression* new_expression){
    new_expression->next = pos->next;
    pos->next = new_expression;
}


/*
 * Calculate the sum of two numbers represented as two linked lists.
 *
 * Parameters:
 *  - l1: node pointer to the head of the first linked list represents the first number.
 *  - l2: node pointer to the head of the second linked list represents the second number.
 *
 * Returns:
 *  - A node pointer to head of the result linked list represents the sum answer of the two numbers.
 */

node* sum(node* l1, node* l2){

    node* result_head = create_node(0);
    node* result = result_head;

    // sign manipulation

    if (isNegative(l1) && isNegative(l2)){ // -A + -B = -(A + B)
        result_head->value = -1;
    }
    else if (isNegative(l1)){              // -A + B = B - A
        l1->value = 0;
        return subtract(l2, l1);
    }
    else if (isNegative(l2)){              // A + -B = A - B
        l2->value = 0;
        return subtract(l1, l2);
    }


    int carry = 0;
    node* p1 = l1->next;
    node* p2 = l2->next;

    while(p1 != NULL || p2 != NULL || carry != 0){
        int digit_l1 = 0, digit_l2 = 0;

        if (p1 != NULL){
            digit_l1 = p1->value;
            p1 = p1->next;
        }

        if (p2 != NULL){
            digit_l2 = p2->value;
            p2 = p2->next;
        }

        int result_value = digit_l1 + digit_l2 + carry;
        carry = result_value/10;
        int result_digit = result_value%10;

        node* new_digit = create_node(result_digit);
        insert_after(result, new_digit);

        result = result->next;

    }

    return result_head;

}


/*
 * Calculate the difference between two numbers represented as two linked lists. (i.e. L1 - L2)
 *
 * Parameters:
 *  - l1: node pointer to the head of the first linked list represents the first number.
 *  - l2: node pointer to the head of the second linked list represents the second number.
 *
 * Returns:
 *  - A node pointer to head of the result linked list represents the difference between the two numbers.
 */

node* subtract(node* l1, node* l2){
    node* result_head = create_node(0);
    node* result = result_head;


    // sign manipulation

    if (isNegative(l1) && isNegative(l2)){  // -A - -B = B - A
        l1->value = 0;
        l2->value = 0;
        return subtract(l2, l1);
    }

    else if (isNegative(l1) && !isNegative(l2)){ // -A - B = -(A + B)
        l2->value = -1;
        return sum(l1, l2);
    }

    else if (!isNegative(l1) && isNegative(l2)){ // A - -B = A + B
        l2->value = 0;
        return sum(l1, l2);
    }



    int borrow = 0;
    node* p1 = l1->next;
    node* p2 = l2->next;

    if (is_greater_or_equal(l1, l2)){
        p1 = l1->next;
        p2 = l2->next;
    }
    else {
        p1 = l2->next;
        p2 = l1->next;
        result_head->value = -1;
    }


    while(p1 != NULL){
        int digit_l1 = 0, digit_l2 = 0, borrow = 0;

        digit_l1 = p1->value;
        p1 = p1->next;

        if (p2 != NULL){
            digit_l2 = p2->value;
            p2 = p2->next;
        }


        if (digit_l1 < digit_l2){
            (p1->value)--;
            borrow++;
        }

        int result_value = digit_l1 + borrow*10 - digit_l2;


        node* new_digit = create_node(result_value);

        insert_after(result, new_digit);

        result = result->next;

    }


    // removing trailing zeros.
    remove_trailing_zeros(result_head);



    return result_head;

}

/*
 * Checks if the first number is greater of equal to the second number represented as two linked lists. (i.e. L1 >= L2)
 *
 * Parameters:
 *  - l1: node pointer to the head of the first linked list represents the first number.
 *  - l2: node pointer to the head of the second linked list represents the second number.
 *
 * Returns:
 *  - value of 1 if the first number is greater of equal to the second one. Otherwise, returns 0.
 */

int is_greater_or_equal(node* l1, node* l2){

    node* p1 = l1->next;
    node* p2 = l2->next;

    int result = 1;

    while(p1 != NULL || p2 != NULL){
        int digit_l1 = 0, digit_l2 = 0;

        if (p1 != NULL){
            digit_l1 = p1->value;
            p1 = p1->next;
        }

        if (p2 != NULL){
            digit_l2 = p2->value;
            p2 = p2->next;
        }

        if (digit_l2 > digit_l1)
            result = 0;

        else if (digit_l2 < digit_l1)
            result = 1;

    }


    return result;



}

/*
 * Calculate the multiplication result of two numbers represented as two linked lists. (i.e. L1 * L2)
 *
 * Parameters:
 *  - l1: node pointer to the head of the first linked list represents the first number.
 *  - l2: node pointer to the head of the second linked list represents the second number.
 *
 * Returns:
 *  - A node pointer to head of the result linked list represents the multiplication result of the two numbers.
 */

node* multiply(node* l1 , node* l2){

    node* result_head = create_node(0);
    node* result = result_head;

    node* p1 = l1->next;

    int weight = 0;

    while(p1 != NULL){

        int digit_l1 = p1->value;

        node* p2 = l2->next;

        node* presult_head = create_node(0);
        node* presult = presult_head;

        for (int i = 0 ; i < weight ; i++){
            node* leading_zero = create_node(0);
            insert_after(presult, leading_zero);
            presult = presult->next;
        }


        int carry = 0;

        while(p2 != NULL || carry != 0){
            int digit_l2 = 0;

            if (p2 != NULL){
                digit_l2 = p2->value;
                p2 = p2->next;
            }

            int result_value = digit_l1 * digit_l2 + carry;

            int result_digit = result_value%10;
            carry = result_value/10;


            node* new_digit = create_node(result_digit);
            insert_after(presult, new_digit);

            presult = presult->next;

        }


        result_head = sum(result_head, presult_head);
        p1 = p1->next;
        weight++;
    }

    if ((isNegative(l1) && !isNegative(l2)) || (!isNegative(l1) && isNegative(l2))){
        result_head->value = -1;
    }


    remove_trailing_zeros(result_head);

    return result_head;

}

/*
 * Calculate the division result of two numbers represented as two linked lists with remainder. (i.e. L1 / L2, L1 % L2)
 *
 * Parameters:
 *  - l1: node pointer to the head of the first linked list represents the first number.
 *  - l2: node pointer to the head of the second linked list represents the second number.
 *  - remainder: pointer to node pointer out parameter to store the remainder.
 *
 * Returns:
 *  - A node pointer to head of the result linked list represents the division result of the two numbers.
 */

node* divide(node* l1, node* l2, node** remainder){

    int neg = 0;
    int neg_divisor = isNegative(l2);

    if ((isNegative(l1) && !isNegative(l2)) || (!isNegative(l1) && isNegative(l2))){
        neg = 1;
    }




    node* result_head = create_node(0);
    *remainder = create_node(0);
    node* result = result_head;

    node* p1 = l1->next;
    node* p2 = l2->next;

    l1->value = 0;
    l2->value = 0;

    if (p2->next == NULL && p2->value == 0){
        printf("MATH ERROR, You can't Divide By Zero!\n");
        return result_head;
    }


    while(p1->next != NULL){
        p1 = p1->next;
    }

    node* presult_head = create_node(0);

    while(1){

        if (p1->pre == NULL)
            break;

        node* new_digit = create_node(p1->value);

        insert_after(presult_head, new_digit);

        remove_trailing_zeros(presult_head);

        p1 = p1->pre;



        int i;
        node* i_list = create_node(0);
        for (i = 9 ; i >= 0 ; i--){
            insert_after(i_list, create_node(i));
            if (is_greater_or_equal(presult_head, multiply(i_list, l2))){
                insert_after(result_head, create_node(i));
                break;
            }
            else {
                node* temp = i_list->next;
                i_list->next = NULL;
                temp->pre = NULL;
                free(temp);
            }
        }


        node* new_presult_head = subtract(presult_head, multiply(i_list, l2));

        delete_node_list(presult_head->next);

        presult_head->next = new_presult_head->next;
        new_presult_head->next->pre = presult_head;

        *remainder = presult_head;


        free(new_presult_head);


    }

    remove_trailing_zeros(result_head);

    if (neg){
        result_head->value = -1;

        if (neg_divisor == 0){
            (*remainder)->value = -1;
            if((*remainder)->next->value == 0)
                (*remainder)->value = 0;
            else {
                node* rem = sum((*remainder), l2);
                *remainder = rem;
            }
        }
    }

    return result_head;
}


/*
 * Removes the trailing zeros to get the right mathematical format of representing a number. (e.g. 0023 -> 23)
 *
 * Parameters:
 *  - list: node pointer to the head of the linked list represents the number we want to remove trailing zeros from.
 *
 */

void remove_trailing_zeros(node* list){


    node* p = list;

    while(p->next != NULL)
        p = p->next;


    while(p->value == 0 && p != list->next){
        node* temp = p;
        p = p->pre;

        p->next = NULL;
        temp->pre = NULL;

        free(temp);

    }

}


/*
 * Printing number stored in a list to output file in the right order.
 *
 * Parameters:
 *  - list: node pointer to the head of the linked list represents the number we want to print.
 *
 */

void print_number(node* list) {

    int neg = (list->value == -1);


    list = list->next;

    if (list == NULL){
        return;
    }

    while(list->next != NULL)
        list = list->next;


    if (neg && !(list->value == 0 && list->pre->pre == NULL)) // checking if the number is negative and it's not -0
        fprintf(out ,"-");

    while(list->pre != NULL){
        fprintf(out, "%d", list->value);
        list = list->pre;
    }


    fflush(out);

}


void print_all_results(){

    expression* p = results->next;

    while(p != NULL){
        fprintf(out, "%s %c %s = ", numbers[p->index1], p->operation, numbers[p->index2]);
        print_number(p->result);
        fprintf(out,"\n");
        p = p->next;
    }


}


/*
 * Convert the string represents the number to a linked list of its digits.
 *
 * Parameters:
 *  - number: char pointer to the beginning of the string represents the number to be converted.
 *
 * Returns:
 *  - node pointer to the linked list we stored the number in.
 */

node* convert_to_linked_list(char* number){

    node* result_head = create_node(0);
    node* result = result_head;

    int neg = (number[0] == '-');

    if (neg)
        result_head->value = -1;

    int len = strlen(number);

    for (int i = len - 1 ; i >= (neg == 1 ? 1 : 0) ; i--){
        if (number[i] < '0' || number[i] > '9')
            continue;
        node* new_digit = create_node(number[i] - '0');
        insert_after(result, new_digit);
        result = result->next;
    }

    remove_trailing_zeros(result_head);

    return result_head;
}


/*
 * Display the options and the functionalities the user can choose.
 */

void Display_menu(){

    printf("please choose one of the operations:\n\n");
    printf("1. Read from the input file.\n");
    printf("2. Sum.\n");
    printf("3. Subtract.\n");
    printf("4. Multiply.\n");
    printf("5. Divide with remainder.\n");
    printf("6. Print to output file all results.\n");
    printf("7. Exit.\n");

}


/*
 * Reading the numbers from the input file and storing them in the numbers array of strings.
 */

void read_file(){

    file_size = 0;

    while(fscanf(in, "%s", numbers[file_size]) != -1){
        printf("%d: %s\n", file_size, numbers[file_size]);
        file_size++;
    }

    printf("Reading is finished successfully.\n");


}


/*
 * Driver function to call the right function to apply the operations the user chooses.
 */

int driver(){

    int option;

    scanf("%d", &option);

    if (option == 7)
        return -1;


    if (option == 1){
        read_file();
        return 0;
    }

    if (option == 6){
        print_all_results();
        return;
    }

    if (option > 8 || option < 1){
        printf("Invalid option, please try again.\n");
        return 0;
    }

    int idx1, idx2;

    while(1){
        printf("Please enter the indexes of two number to apply operation on them: (Zero Based)\n");
        scanf("%d %d", &idx1, &idx2);
        if (idx1 >= file_size || idx2 >= file_size || idx1 < 0 || idx2 < 0){
            printf("Please enter valid indexes!!\n");
        }
        else break;
    }



    node* l1 = convert_to_linked_list(numbers[idx1]);
    node* l2 = convert_to_linked_list(numbers[idx2]);



    if (option == 2){
        insert_expression_after(last_result, create_expression(idx1, idx2, '+', sum(l1,l2)));
    }
    else if (option == 3){
        insert_expression_after(last_result, create_expression(idx1, idx2, '-', subtract(l1,l2)));
    }
    else if (option == 4){
        insert_expression_after(last_result, create_expression(idx1, idx2, '*', multiply(l1,l2)));
    }
    else if (option == 5){
        node* rem;
        insert_expression_after(last_result, create_expression(idx1, idx2, '/', divide(l1,l2,&rem)));
        last_result = last_result->next;
        insert_expression_after(last_result, create_expression(idx1, idx2, '%', rem));
    }

    last_result = last_result->next;

    return 0;

}




int main()
{

    in = fopen("input.txt", "r");
    out = fopen("output.txt", "w");
    results = create_expression(-1, -1, '\0', NULL);
    last_result = results;

    do
    {
        Display_menu();
    }
    while(driver() != -1);

    delete_expression_list(results);
    fclose(in);
    fclose(out);

    return 0;
}
