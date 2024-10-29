/*
  Abdalhameed Maree - 1220775 - section 4
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*
  This is a linked list which is used to store the data from the file
  to the computer memory.
*/
typedef struct Node
{
  char name[50];
  struct Node *Next;
} Node;

/*
  This is a linked list which is used to handle the data and sort the way of
  adding them to the original list and making changes to them.
*/
typedef struct Queue
{
  char string[50];
  struct Queue *Next;
} Queue;
struct Queue *Front = NULL;
struct Queue *Rear = NULL;

/*
  Stack Implementation , for redo and undo operations
*/
typedef struct Stack
{
  char string[50];
  char operation[7];
  int index;
  struct Stack *Next;
} Stack;

/*
  The Functions prototype for Queue Linked List
*/
Queue *CreateQueue();
void MakeEmpty(Queue *queue);
void DisposeQueue(Queue *queue);
void Enqueue(char *string);
void Dequeue(int op, Node *list, Stack *stack);

/*
  The Functions prototype for Stack Linked List
*/
Stack *CreateStack();
void MakeEmptyStack(Stack *stack);
void Push(char *string, int index, char *operation, Stack *stack);
void Pop(Stack *stack);
Stack *Top(Stack *stack);
int IsEmpty(Stack *stack);
void DisposeStack(Stack *stack);

/*
  The Functions prototype for the General Linked List
*/
void Insert(char *name, int op, Node *head);
Node *Delete(char *name, Node *head, Stack *stack);
Node *DeleteSubstring(char *name, Node *head);
Node *Find(char *name, Node *head);

/*
  The Functions prototype for General Functions
*/
void menu();
void LoadInputFile(Node *head);
void printLoadedInformation(Node *head);
int getIndex(char *string, Node *head);
void outputFile(Node *head);

bool flag = false; // if flag is false then i use Delete Node else it will be used to delete a substring
/*
  The Main Function which is used to run the program
*/
int main()
{
  Node *List = (Node *)malloc(sizeof(Node)); // This is the Linked List that is used to store the text in
  List->Next = NULL;                         // Computers memory

  Stack *UndoStack = CreateStack(); // The Definition of Undo Stack
  Stack *RedoStack = CreateStack(); // The Definition of Redo Stack

  Queue *queue = CreateQueue(); // The Definition of the Queue

  menu();
  int choice;
  char string[100];    // an array of characters that will be used to store strings from the user
  int counter = 0;     // counter used to handle the program execution
  while (counter == 0) // Main program loop
  {
    printf("\n\nEnter your choice: ");
    scanf("%d", &choice); // Get user choice
    printf("\n");
    switch (choice)
    {
    case 1:
      LoadInputFile(List); // Load text from input file
      printf("\nInput file loaded.\n");
      break;
    case 2:
      printf("\nLoaded text: \n");
      printLoadedInformation(List); // Print the loaded text
      break;
    case 3:
    {
      printf("Enter string to insert: ");
      scanf(" %[^\n]", string);
      int op;

      printf("\nWhere Do You Want To Add The String ?\n"
             "1. The start of the sentence.\n"
             "2. The end of the sentence.\n"
             "3. At a specific index.\n");
      scanf("%d", &op);
      switch (op)
      {
      case 1:
      {
        Enqueue(string); // Enqueue string for insertion at the start
        while (Front != NULL)
        {
          Dequeue(1, List, UndoStack); // Dequeue and insert strings from the queue
        }
        break;
      }
      case 2:
      {
        Enqueue(string); // Enqueue string for insertion at the end
        while (Front != NULL)
        {
          Dequeue(-1, List, UndoStack); // Dequeue and insert strings from the queue
        }
        break;
      }

      case 3:
      {
        int index;
        printf("\nEnter the index to insert the string: ");
        scanf("%d", &index);
        Enqueue(string); // Enqueue string for insertion at a specific index
        while (Front != NULL)
        {
          Dequeue(index, List, UndoStack); // Dequeue and insert strings from the queue
        }
        break;
      }

      default:
        printf("\nInvalid choice. Please enter a valid option.\n");
        break;
      }
      break;
    }
    case 4:
    {
      printf("\nEnter a word to remove from the text: ");
      char rmString[100];
      scanf(" %[^\n]", rmString); // Get the string from the user
      printf("\n");
      Node *temp = Find(rmString, List);
      if (flag == true)
      {
        Push(rmString, getIndex(temp->name, List), "Remove", UndoStack); // push the string to the undo stack before change it
        List = DeleteSubstring(rmString, List);                          // Delete substring
      }
      else
      {
        Push(rmString, getIndex(rmString, List), "Remove", UndoStack);
        List = Delete(rmString, List, UndoStack); // Delete entire node
      }
      break;
    }

    case 5:
    {
      printf("\nPerforming Undo operation.\n");
      if (!IsEmpty(UndoStack))
      {
        Stack *temp = Top(UndoStack);
        Push(temp->string, temp->index, temp->operation, RedoStack);
        if (strcmp(temp->operation, "Insert") == 0)
        {
          List = Delete(temp->string, List, UndoStack);
          Pop(UndoStack);
          printf("\nUndo Operation Done.\n");
        }
        else if (strcmp(temp->operation, "Remove") == 0)
        {
          Insert(temp->string, temp->index, List);
          Pop(UndoStack);
          printf("\nUndo Operation Done.\n");
        }
      }
      else
        printf("\nUndo Stack is empty.\n");

      break;
    }

    case 6:
      printf("\nPerforming Redo operation.\n");
      if (!IsEmpty(RedoStack))
      {
        Push(Top(RedoStack)->string, Top(RedoStack)->index, Top(RedoStack)->operation, UndoStack);
        if (strcmp(Top(RedoStack)->operation, "Insert") == 0)
        {
          Insert(Top(RedoStack)->string, Top(RedoStack)->index, List);
          Pop(RedoStack);
          printf("\nRedo Operation Done.\n");
        }
        else if (strcmp(Top(RedoStack)->operation, "Remove") == 0)
        {
          List = Delete(Top(RedoStack)->string, List, RedoStack);
          Pop(RedoStack);
          printf("\nRedo Operation Done.\n");
        }
      }
      else
        printf("\nRedo Stack is empty.\n");
      break;

    case 7:
    {
      printf("\nUndo Stack: \n");
      Stack *tempUndoStack = CreateStack();
      while (!IsEmpty(UndoStack))
      {
        printf("%s\t\t%s\t\t%d\n", Top(UndoStack)->string, Top(UndoStack)->operation, Top(UndoStack)->index);
        Push(Top(UndoStack)->string, Top(UndoStack)->index, Top(UndoStack)->operation, tempUndoStack);
        Pop(UndoStack);
      }
      while (!IsEmpty(tempUndoStack))
      {
        Push(Top(tempUndoStack)->string, Top(tempUndoStack)->index, Top(tempUndoStack)->operation, UndoStack);
        Pop(tempUndoStack);
      }
      printf("\n");

      printf("\nRedo Stack: \n");
      Stack *tempRedoStack = CreateStack();
      while (!IsEmpty(RedoStack))
      {
        printf("%s\t\t%s\t\t%d\n", Top(RedoStack)->string, Top(RedoStack)->operation, Top(RedoStack)->index);
        Push(Top(RedoStack)->string, Top(RedoStack)->index, Top(RedoStack)->operation, tempRedoStack);
        Pop(RedoStack);
      }
      while (!IsEmpty(tempRedoStack))
      {
        Push(Top(tempRedoStack)->string, Top(tempRedoStack)->index, Top(tempRedoStack)->operation, RedoStack);
        Pop(tempRedoStack);
      }
      printf("\n");

      DisposeStack(tempUndoStack);
      DisposeStack(tempRedoStack);

      break;
    }

    case 8:
      outputFile(List);
      printf("\nData saved to the output file successfully!\n");
      break;

    case 9:
      printf("\nExiting program.\n");
      counter++;
      break;

    default:
      printf("\nInvalid choice. Please enter a valid option.\n");
    }
    if (counter > 0) // checks if the user has entered option 9
      continue;
    else
      menu();
  }
  // Free The Memory
  DisposeQueue(queue);
  DisposeStack(UndoStack);
  DisposeStack(RedoStack);
  free(List);
  return 0;
}

/*
  The Implementation of the General Functions and The General Linked List
*/
void menu()
{
  printf("\n1. Load the input file which contains the initial text.\n"
         "2. Print the loaded text.\n"
         "3. Insert strings to the text.\n"
         "4. Remove strings from the text.\n"
         "5. Perform Undo operation.\n"
         "6. Perform Redo operation.\n"
         "7. Print the Undo Stack and the Redo stack.\n"
         "8. Save the updated text to the output file.\n"
         "9. Exit\n");
}

void Insert(char *name, int op, Node *head)
{
  Node *currentNode = head;
  Node *tempNode = (Node *)malloc(sizeof(Node));
  if (tempNode == NULL)
  {
    printf("OUT OF MEMORY!\n");
    return;
  }
  else
  {
    strcpy(tempNode->name, name);
    tempNode->Next = NULL;
    if (op == -1) // insert last
    {
      while (currentNode->Next != NULL)
      {
        currentNode = currentNode->Next;
      }
      currentNode->Next = tempNode;
    }
    else if (op == 1) // insert first
    {
      tempNode->Next = currentNode->Next;
      currentNode->Next = tempNode;
    }
    else // insert at specific index
    {
      Node *currentNode = head;
      int counter = 0;

      while (currentNode->Next != NULL && counter < op - 1)
      {
        counter += strlen(currentNode->Next->name);
        currentNode = currentNode->Next;
      }

      tempNode->Next = currentNode->Next;
      currentNode->Next = tempNode;
    }
  }
}

Node *Delete(char *name, Node *head, Stack *stack)
{
  if (head == NULL) // checks if the list is empty
  {
    printf("EMPTY LIST!\n");
    return NULL;
  }
  // Removes the head if it was the wanted Node (يجب المراجعة - عالأغلب ما بلزم)
  if (strcmp(head->name, name) == 0)
  {
    Node *temp = head->Next;
    free(head);
    return temp;
  }

  Node *currentNode = Find(name, head);
  if (currentNode == NULL)
  {
    printf("Node not found!\n");
    return head;
  }
  Node *previous = head;
  while (previous->Next != currentNode)
  {
    previous = previous->Next;
  }
  previous->Next = currentNode->Next;
  free(currentNode);
  printf("String removed from the List.\n");
  return head;
}

Node *DeleteSubstring(char *name, Node *head) // a Function that Deletes a specific substring from a string
{
  if (head == NULL)
  {
    printf("EMPTY LIST!\n");
    return NULL;
  }
  Node *currentNode = Find(name, head);

  int length = strlen(currentNode->name);
  int subLength = strlen(name);
  int newLength = length - subLength;
  int counter = 0, newStringCounter = 0;
  char *newString = (char *)malloc(sizeof(char) * (newLength + 1));

  for (int i = 0; i < length; i++)
  {
    if (currentNode->name[i] == name[counter])
    {
      counter++;
    }

    else
    {
      newString[newStringCounter] = currentNode->name[i];
      newStringCounter++;
    }
  }
  newString[newLength] = '\0';
  printf("new string = %s\n", newString);
  strcpy(currentNode->name, newString);

  printf("String removed from the List.\n");
  return head;
}

Node *Find(char *name, Node *head) // searchs for a Node
{
  if (head->Next == NULL)
  {
    printf("EMPTY LIST!\n");
    return NULL;
  }
  Node *currentNode = head;
  while (currentNode->Next != NULL)
  {
    if (strcmp(currentNode->name, name) == 0)
    {
      flag = false;
      return currentNode;
    }

    if (strstr(currentNode->name, name) != NULL)
    {
      flag = true;
      return currentNode;
    }

    currentNode = currentNode->Next;
  }
  if (currentNode->Next == NULL && strcmp(currentNode->name, name) == 0)
    return currentNode;

  // printf("THE DATA DOESN'T EXIST IN THE LIST!\n");
  return NULL;
}

void LoadInputFile(Node *head) // loads the input data
{
  if (head == NULL)
  {
    printf("OUT OF MEMORY!\n");
    return;
  }
  FILE *input = fopen("input.txt", "r");
  if (input == NULL)
  {
    printf("Error: can't open the file .. \n");
    return;
  }

  char string[100];
  fgets(string, 100, input);
  char *str = strtok(string, " ");
  while (str != NULL)
  {
    Insert(str, -1, head);
    str = strtok(NULL, " ");
  }

  fclose(input);
}

void printLoadedInformation(Node *head) // prints the lst data
{
  if (head == NULL)
  {
    printf("Empty List!\n");
    return;
  }
  Node *currentNode = head->Next;
  while (currentNode != NULL)
  {
    printf("%s ", currentNode->name);
    currentNode = currentNode->Next;
  }
  printf("\n");
}

int getIndex(char *string, Node *head) // gets the index of the first occurence of a specific node
{
  Node *currentNode = head->Next;
  int counter = 0;

  while (currentNode != NULL)
  {
    if (strstr(currentNode->name, string) != NULL)
    {
      return counter;
    }
    counter += strlen(currentNode->name) + 1; // +1 for spaces
    currentNode = currentNode->Next;
  }
  return 0;
}

void outputFile(Node *head) // save the data on a text file
{
  FILE *output = fopen("newFile.txt", "w");
  Node *currentNode = head->Next;

  while (currentNode != NULL)
  {
    fprintf(output, "%s ", currentNode->name);
    currentNode = currentNode->Next;
  }

  fclose(output);
}

/*
  The Implementation of the Stacks Functions
*/
Stack *CreateStack()
{
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  if (stack == NULL)
  {
    printf("OUT OF MEMORY!\n");
    return NULL;
  }

  stack->Next = NULL;
  MakeEmptyStack(stack);
  return stack;
}

int IsEmpty(Stack *stack)
{
  return stack->Next == NULL;
}

void MakeEmptyStack(Stack *stack)
{
  if (stack == NULL)
    printf("OUT OF MEMORY! \n");

  else
    while (!IsEmpty(stack))
      Pop(stack);
}

void Pop(Stack *stack)
{
  Stack *firstNode;

  if (IsEmpty(stack))
    printf("EMPTY STACK\n");

  else
  {
    firstNode = stack->Next;
    stack->Next = stack->Next->Next;
    free(firstNode);
  }
}

void Push(char *string, int index, char *operation, Stack *stack)
{
  Stack *current = stack;
  Stack *newNode = (Stack *)malloc(sizeof(Stack));
  if (newNode == NULL)
    printf("Out of space !\n");

  strcpy(newNode->string, string);
  strcpy(newNode->operation, operation);
  newNode->index = index;
  if (current->Next == NULL)
  {
    newNode->Next = NULL;
    current->Next = newNode;
  }
  else
  {
    newNode->Next = current->Next;
    current->Next = newNode;
  }
}

Stack *Top(Stack *stack)
{
  if (!IsEmpty(stack))
    return stack->Next;

  printf("Empty stack\n");
  return NULL;
}

void DisposeStack(Stack *stack)
{
  if (!IsEmpty(stack))
  {
    MakeEmptyStack(stack);
    free(stack);
  }
}

/*
  The Implementation of the Queue Functions
*/
Queue *CreateQueue()
{
  Queue *queue = (Queue *)malloc(sizeof(Queue));
  if (queue == NULL)
  {
    printf("OUT OF MEMORY!\n");
    return NULL;
  }
  queue->Next = NULL;
  Front = NULL;
  Rear = NULL;
  return queue;
}

void MakeEmpty(Queue *queue)
{
  Queue *temp = queue->Next;
  while (temp != NULL)
  {
    Queue *next = temp->Next;
    free(temp);
    temp = next;
  }
  queue->Next = NULL;
  Front = NULL;
  Rear = NULL;
}

void DisposeQueue(Queue *queue)
{
  if (queue != NULL)
  {
    MakeEmpty(queue);
    free(queue);
  }
}

void Enqueue(char *string)
{
  char *newStr = strtok(string, " ");
  while (newStr != NULL)
  {
    Queue *newQueue = (Queue *)malloc(sizeof(Queue));
    if (newQueue == NULL)
    {
      printf("OUT OF MEMORY!\n");
      return;
    }
    strcpy(newQueue->string, newStr);
    newQueue->Next = NULL;
    if (Front == NULL)
    {
      Front = newQueue;
      Rear = newQueue;
    }
    else
    {
      Rear->Next = newQueue;
      Rear = newQueue;
    }
    newStr = strtok(NULL, " ");
  }
}

void Dequeue(int op, Node *list, Stack *stack)
{
  if (Front == NULL)
  {
    printf("Queue is empty\n");
    return;
  }

  Queue *tempNode = Front;                  // if op == 1 -> insert at first
  Insert(Front->string, op, list);          // if op == -1 -> insert at last
  Push(Front->string, op, "Insert", stack); // else insert at specific index
  Front = Front->Next;
  free(tempNode);
}