
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    char data;         
    struct Node* left;  
    struct Node* right; 
} Node;

// Stack used for conversion (stores nodes)
typedef struct Stack {
    Node* nodes[200]; 
    int top;          
} Stack;

// Initialize stack with top = -1
void initStack(Stack* stack) {
    stack->top = -1;
}

// Check if stack is empty
// Returns 1 if empty, 0 otherwise
int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

// Push node onto the stack
void push(Stack* stack, Node* node) {
    stack->nodes[++(stack->top)] = node; 
}

// Pop node from the top of the stack
Node* pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        return NULL; // Return NULL if stack is empty
    }
    return stack->nodes[(stack->top)--];
}

// Create a new node
Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Check if character is an operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

// Convert infix expression to postfix
void infixToPostfix(const char* infix, char* postfix) {
    Stack operatorStack;
    initStack(&operatorStack);
    int k = 0;

    for (int i = 0; infix[i] != '\0'; i++) {
        char c = infix[i];

        if (isspace(c)) {
            continue;
        } else if (isdigit(c)) {
            postfix[k++] = c;
        } else if (c == '(') {
            push(&operatorStack, createNode(c));
        } else if (c == ')') {
            while (!isStackEmpty(&operatorStack) && operatorStack.nodes[operatorStack.top]->data != '(') {
                postfix[k++] = pop(&operatorStack)->data;
            }
            pop(&operatorStack); // Remove '('
        } else if (isOperator(c)) {
            while (!isStackEmpty(&operatorStack) &&
                   isOperator(operatorStack.nodes[operatorStack.top]->data) &&
                   operatorStack.nodes[operatorStack.top]->data != '(') {
                postfix[k++] = pop(&operatorStack)->data;
            }
            push(&operatorStack, createNode(c));
        } else {
            printf("Error: Invalid character '%c'.\n", c);
            return;
        }
    }

    while (!isStackEmpty(&operatorStack)) {
        postfix[k++] = pop(&operatorStack)->data;
    }
    postfix[k] = '\0';
}

// Build expression tree from postfix expression
Node* buildTree(const char* postfix) {
    Stack nodeStack;
    initStack(&nodeStack);

    for (int i = 0; postfix[i] != '\0'; i++) {
        char c = postfix[i];

        Node* newNode = createNode(c);
        if (isOperator(c)) {
            newNode->right = pop(&nodeStack);
            newNode->left = pop(&nodeStack);
        }
        push(&nodeStack, newNode);
    }

    return pop(&nodeStack);
}

// Evaluate expression tree
int evaluate(Node* root) {
    if (root == NULL) {
        return 0;
    }
    if (!isOperator(root->data)) {
        return root->data - '0';
    }

    int left = evaluate(root->left);
    int right = evaluate(root->right);

    switch (root->data) {
        case '+': return left + right;
        case '-': return left - right;
        case '*': return left * right;
        case '/': return right != 0 ? left / right : 0;
        case '%': return right != 0 ? left % right : 0;
        default: return 0;
    }
}

// Preorder traversal (prefix notation)
void preorder(Node* root) {
    if (root != NULL) {
        printf("%c ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// Postorder traversal (postfix notation)
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
        printf("\nMenu:\n");
        printf("1. Enter expression\n");
        printf("2. Evaluate expression\n");
        printf("3. Show prefix form\n");
        printf("4. Show postfix form\n");
        printf("0. Exit\n");
        printf("Choice: ");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter infix expression: ");
                fgets(infix, 200, stdin);
                infixToPostfix(infix, postfix);
                root = buildTree(postfix);
                break;

            case 2:
                if (root) {
                    printf("Result: %d\n", evaluate(root));
                } else {
                    printf("No expression has been entered.\n");
                }
                break;

            case 3:
                if (root) {
                    printf("Prefix form: ");
                    preorder(root);
                    printf("\n");
                } else {
                    printf("No expression has been entered.\n");
                }
                break;

            case 4:
                if (root) {
                    printf("Postfix form: ");
                    postorder(root);
                    printf("\n");
                } else {
                    printf("No expression has been entered.\n");
                }
                break;

            case 0:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid option.\n");
        }
    }
}
