/*Αγγελική Κλιάνη
  ΑΜ: 2022202300205
  dit23205@go.uop.gr 
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef struct Node {
    char data;         
    struct Node* left;  
    struct Node* right; 
} Node;

// Στοίβα για μετατροπή (χρησιμοποιείται για την αποθήκευση κόμβων)
typedef struct Stack {
    Node* nodes[200]; 
    int top;          
} Stack;

// Αρχικοποίηση στοίβας με -1
void initStack(Stack* stack) {
    stack->top = -1;
}

// Έλεγχος αν η στοίβα είναι άδεια
// Επιστρέφει 1 αν είναι άδεια, 0 αν δεν είναι
int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

// Προσθήκη κόμβου στην κορυφή της στοίβας
void push(Stack* stack, Node* node) {
    stack->nodes[++(stack->top)] = node; 
}

// Αφαίρεση κόμβου από την κορυφή της στοίβας
Node* pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        return NULL; // Αν είναι άδεια η στοίβα, επιστρέφει NULL
    }
    return stack->nodes[(stack->top)--]; // Αφαιρεί τον κόμβο και επιστρέφει το περιεχόμενό του
}

// Δημιουργία νέου κόμβου με δεδομένο χαρακτήρα
// Επιστρέφει τον νέο κόμβο που δημιουργήθηκε
Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node)); // Δημιουργία νέου κόμβου στη μνήμη
    newNode->data = data; // Ορίζουμε το δεδομένο του κόμβου
    newNode->left = NULL;  // Αρχικοποιούμε τους υποκόμβους στο NULL
    newNode->right = NULL;
    return newNode;
}

// Έλεγχος αν ο χαρακτήρας είναι μαθηματικός τελεστής (+, -, *, /, %)
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Μετατροπή ενθεματικής σε μεταθεματική (infix -> postfix) παράσταση
void infixToPostfix(const char* infix, char* postfix) {
    Stack operatorStack;
    initStack(&operatorStack);  // Αρχικοποιούμε τη στοίβα των τελεστών
    int k = 0;  // Δείκτης για την τελική μεταθεματική παράσταση

    // Διατρέχουμε την ενθεματική παράσταση
    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];  // Τρέχων χαρακτήρας

        if (isspace(c)) {
            continue; // Αγνοω τα κενά διαστήματα
        } else if (isdigit(c)) {
            postfix[k++] = c; // Προσθήκη αριθμού στην μεταθεματική παράσταση
        } else if (c == '(') {
            push(&operatorStack, createNode(c));  // Αν είναι άνοιγμα παρένθεσης, τοποθετούμε τον τελεστή στη στοίβα
        } else if (c == ')') {
            // Όταν συναντήσουμε κλείσιμο παρένθεσης, αφαιρούμε τους τελεστές μέχρι να φτάσουμε σε ανοιχτή παρένθεση
            while (!isStackEmpty(&operatorStack) && operatorStack.nodes[operatorStack.top]->data != '(') {
                postfix[k++] = pop(&operatorStack)->data;
            }
            pop(&operatorStack); // Αφαίρεση παρενθέσεων
        } else if (isOperator(c)) {
            // Αν είναι τελεστής, αφαιρούμε από τη στοίβα όλους τους τελεστές με μεγαλύτερη ή ίση προτεραιότητα
            while (!isStackEmpty(&operatorStack) &&
                   isOperator(operatorStack.nodes[operatorStack.top]->data) &&
                   operatorStack.nodes[operatorStack.top]->data != '(') {
                postfix[k++] = pop(&operatorStack)->data;
            }
            push(&operatorStack, createNode(c));  // Προσθήκη του τρέχοντος τελεστή στη στοίβα
        } else {
            printf("Σφάλμα: Μη έγκυρος χαρακτήρας '%c'.\n", c);  
            return;
        }
    }

    // Αφαιρούμε τους υπόλοιπους τελεστές από τη στοίβα και τους προσθέτουμε στην τελική παράσταση
    while (!isStackEmpty(&operatorStack)) {
        postfix[k++] = pop(&operatorStack)->data;
    }
    postfix[k] = '\0';  // Τερματισμός της μεταθεματικής παράστασης
}

// Κατασκευή δυαδικού δέντρου από μεταθεματική παράσταση
Node* buildTree(const char* postfix) {
    Stack nodeStack;
    initStack(&nodeStack);  // Αρχικοποιούμε τη στοίβα για τους κόμβους

    // Διατρέχουμε την μεταθεματική παράσταση
    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];  

        Node* newNode = createNode(c);  
        if (isOperator(c)) {
            // Αν είναι τελεστής, ορίζουμε τα δύο παιδιά του κόμβου
            newNode->right = pop(&nodeStack); 
            newNode->left = pop(&nodeStack);   
        }
        push(&nodeStack, newNode);  
    }

    return pop(&nodeStack);  // Ο τελευταίος κόμβος που μένει στη στοίβα είναι η ρίζα του δέντρου
}

// Υπολογισμός παράστασης 
int evaluate(Node* root) {
    if (root == NULL) {
        return 0;  // Αν ο κόμβος είναι NULL, επιστρέφουμε 0
    }
    if (!isOperator(root->data)) {
        return root->data - '0';  // Αν είναι αριθμός επιστρέφουμε την τιμή του
    }

    // Αν είναι τελεστής υπολογίζουμε αναδρομικά τα παιδιά του
    int left = evaluate(root->left);  
    int right = evaluate(root->right); 

    
    switch (root->data) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0; // Έλεγχος για διαίρεση με το 0
        case '%': return right != 0 ? left % right : 0; // Έλεγχος για mod
        default: return 0;
    }
}

// Διάσχιση preorder του δυαδικού δέντρου
// (Επισκέπτεται τον κόμβο πρώτα και μετά τα παιδιά του)
void preorder(Node* root) {
    if (root != NULL) {
        printf("%c ", root->data);  
        preorder(root->left);        
        preorder(root->right);      
    }
}

// Διάσχιση postorder του δυαδικού δέντρου
// (Επισκέπτεται τα παιδιά πρώτα και μετά τον κόμβο)
void postorder(Node* root) {
    if (root != NULL) {
        postorder(root->left);     
        postorder(root->right);     
        printf("%c ", root->data);  
    }
}


int main() {
    char infix[200];  
    char postfix[200]; 
    Node* root = NULL;  

    
    while (1) {
        printf("\nΜενού επιλογών:\n");
        printf("1. Είσοδος παράστασης\n");
        printf("2. Υπολογισμός παράστασης\n");
        printf("3. Εμφάνιση προθεματικής μορφής\n");
        printf("4. Εμφάνιση μεταθεματικής μορφής\n");
        printf("0. Έξοδος\n");
        printf("Επιλογή: ");

        int choice;
        scanf("%d", &choice); 
        getchar(); 

        switch (choice) {
            case 1:
                // Εισαγωγή παράστασης από τον χρήστη και μετατροπή σε μεταθεματική μορφή
                printf("Εισάγετε την ενθεματική παράσταση: ");
                fgets(infix, 200, stdin);  
                infixToPostfix(infix, postfix);  // Μετατρέπει την παράσταση σε μεταθεματική
                root = buildTree(postfix);  // Δημιουργεί το δυαδικό δέντρο από τη μεταθεματική
                break;

            case 2:
            
                if (root) {
                    printf("Αποτέλεσμα: %d\n", evaluate(root));  
                } else {
                    printf("Δεν έχει καταχωρηθεί δέντρο.\n");
                }
                break;

            case 3:
                
                if (root) {
                    printf("Προθεματική μορφή: ");
                    preorder(root);  
                    printf("\n");
                } else {
                    printf("Δεν έχει καταχωρηθεί δέντρο.\n");
                }
                break;

            case 4:
                
                if (root) {
                    printf("Μεταθεματική μορφή: ");
                    postorder(root);  
                    printf("\n");
                } else {
                    printf("Δεν έχει καταχωρηθεί δέντρο.\n");
                }
                break;

            case 0:
                
                printf("Έξοδος...\n");
                return 0;

            default:
            
                printf("Μη έγκυρη επιλογή.\n");
        }
    }
}
