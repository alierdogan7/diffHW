#include <stdio.h>
#include <string.h>

#define TURKISH_HELLO "selamun aleykum ey dunya"
#define MAX_LENGTH 1000


//char c = getchar(); //gets the char from keyboard input
//putchar(c); //prints the char to output

/*int c;

while( ( c = getchar()" ) != EOF )
    putchar(c);
*/

typedef struct {
    struct Node *next;
    char *word;
} Node;

int getln( char line[] );
void println( char line[] );
void write_to_file( char *str, FILE *file);
char **read_words_from_file( FILE *file, int *word_amount);
Node *make_linked_list_from_array( char **words, int length);
void sort_str_array( char ***words, int length);
Node *generate_difflist_of_wordlists( Node *node1, Node *node2);

main()
{
    //READING INPUT AND WRITING TO FILE
    /*char my_line[MAX_LENGTH];

    int length = getln( my_line );
    printf("length: %d\n", length);
    printf( my_line);
    FILE *my_file;

    char *path = "/home/burak/Documents/CS342/output.txt";
    my_file = fopen(path, "a");
    write_to_file(my_line, my_file);*/

    //char *command = "cat ";
    //strcat(command, path);
    //system(command);
    //char **words = (char **) malloc( 10 * sizeof(char *) );

    // READ THE WORDS FROM INPUT AND LOAD THEM INTO A 2D DYNAMIC ARRAY
    FILE *input_file = fopen("input1.txt", "r");
    int word_amount = 0;
    char **words = read_words_from_file(input_file, &word_amount);
    printf("word amount: %d\n", word_amount);
    fclose(input_file);

    sort_str_array(&words, word_amount);

    /*
    //check whether they are really sorted or not
    int i;
    for(i = 0; i < word_amount; i++)
        printf( words[i] );
    */

    //making a linked list from that 2d pointer array
    Node *node1 = make_linked_list_from_array(words, word_amount);
    //for(; node1 != NULL; node1 = node1->next)
      //  printf("node1: %s", node1->word);

    //GENERATE THE SECOND LINKED LIST OF WORDS FROM 2ND INPUT
    input_file = fopen("input2.txt", "r");
    int word_amount2 = 0;
    char **words2 = read_words_from_file(input_file, &word_amount2);
    printf("2nd word amount: %d\n", word_amount2);
    fclose(input_file);

    sort_str_array(&words2, word_amount2);

    //making a linked list from that 2d pointer array
    Node *node2 = make_linked_list_from_array(words2, word_amount2);
    //for(; node2 != NULL; node2 = node2->next)
    //    printf("node2: %s", node2->word);

    //input_file = fopen("output.txt", "wb");
    Node *result_node = generate_difflist_of_wordlists(node1, node2);
    /*for(; result_node != NULL; result_node = result_node->next)
    {
        printf("result_node: %s", result_node->word);
        // write each word from difference list to output file
      //  write_to_file(result_node->word, input_file);
    }*/

}

Node *generate_difflist_of_wordlists( Node *node1, Node *node2)
{
    /*
    Iterating the linked lists meanwhile elements of them
    are being compared.

    This algorithm's key rule is that after each comparison,
    little one is connected to linked listed of difference.
    Then, little one gives the pointer to its successor
    in its own linked list.

    If the result of comparison is equivalence,
    both nodes give their pointers to their own
    successors and they are not linked to difference list
    since they are excluded from difference list.
    */

    // node pointers for result linked list
    Node *result_head = NULL;
    Node *result_curr = NULL;
    Node *result_prev = NULL;

    // until one of lists reach to the end
    while ( node1 != NULL && node2 != NULL)
    {
        int result = strcmp( node1->word, node2->word );

        // if node1 is the little one, copy it to diff list
        // and pass the pointer to its successor
        if ( result < 0 )
        {
            //copying the node to the result list
            result_curr = (Node*) malloc( sizeof(Node) );
            result_curr->word = node1->word;

            //if not first item in list
            if( result_prev != NULL)
                result_prev->next = result_curr;
            // if we're on head
            else
                result_head = result_curr;

            result_prev = result_curr;

            //copying finished
            //pass the pointer to its successor
            node1 = node1->next;
        }

        // if we found same elements between lists
        // just continue iterating the nodes and ignore them
        else if ( result == 0)
        {
            node1 = node1->next;
            node2 = node2->next;
        }

        // if node2 is the little one, copy it to result list
        // and iterate it
        else
        {
            //copying the node to the result list
            result_curr = (Node*) malloc( sizeof(Node) );
            result_curr->word = node2->word;

            //if not first item in list
            if( result_prev != NULL)
                result_prev->next = result_curr;
            // if we're on head
            else
                result_head = result_curr;

            result_prev = result_curr;

            //copying finished
            //pass the pointer to its successor
            node2 = node2->next;
        }
    }

    // if only one of the lists ended, connect
    // the other list's remaining nodes to result list
    if ( node1 == NULL && node2 != NULL)
    {
        while( node2 != NULL)
        {
            //copying the node to the result list
            result_curr = (Node*) malloc( sizeof(Node) );
            result_curr->word = node2->word;

            //if not first item in list
            if( result_prev != NULL)
                result_prev->next = result_curr;
            // if we're on head
            else
                result_head = result_curr;

            result_prev = result_curr;

            //copying finished
            //pass the pointer to its successor
            node2 = node2->next;
        }
    }
    else if (node1 != NULL && node2 == NULL)
    {
        while( node1 != NULL)
        {
            //copying the node to the result list
            result_curr = (Node*) malloc( sizeof(Node) );
            result_curr->word = node1->word;

            //if not first item in list
            if( result_prev != NULL)
                result_prev->next = result_curr;
            // if we're on head
            else
                result_head = result_curr;

            result_prev = result_curr;

            //copying finished
            //pass the pointer to its successor
            node1 = node1->next;
        }
    }


    //after result linked list is complete return its head node
    return result_head;
}

// implements an insertion sort for ordering the words lexicographically
void sort_str_array( char ***words, int length)
{
    int i;
    for( i = 1; i < length ; i++)
    {
        int j = i;

        //if j-1th word is greater than jth word keep swapping it
        while( j > 0 && strcmp( (*words)[j-1], (*words)[j]) > 0)
        {
            // swap pointer's addresses not the actual string because it's hard
            char *tmp = (*words)[j];
            (*words)[j] = (*words)[j-1];
            (*words)[j-1] = tmp;

            j--;
        }
    }
}

Node *make_linked_list_from_array( char **words, int length)
{

    Node *head = NULL;
    Node *curr = NULL;
    Node *pre = NULL;

    int i;
    for( i = 0; i < length; i++)
    {
        curr = ( Node*) malloc( sizeof( Node));
        curr->word = words[i];

        //if we are not on head currently
        if(pre != NULL)
            pre->next = curr;

        pre = curr; //update the pre pointer

        if( i == 0)
            head = curr;

    }

    return head;
}

char **read_words_from_file( FILE *file, int *word_amount)
{
    if(file != NULL)
    {
        //read the word count
        char line_no_str[5];
        fgets(line_no_str, 5, file);
        int word_count = atoi(line_no_str);
        *word_amount = word_count; //copy the word amount with pass-by-reference
        //printf("line amount: %d\n", word_count);

        char **words = (char **) malloc( word_count * sizeof(char *) );

        int i;
        char word[256];
        for ( i = 0; i < word_count; i++)
        {
            fgets(word, 256, file);
            //printf("the word is %s and length is %d\n", word, strlen(word));
            words[i] = (char * ) malloc( strlen(word) * sizeof(char) );
            strcpy(words[i], word);
        }

        //while ( fgets(line, 50, file) != NULL)
        return words;
    }
    else
    {
        printf("File cannot be found");
    }
}

void write_to_file( char *str, FILE *file)
{
    if(file != NULL )
    {
        putc( '\n', file);
        int i;
        int c;
        //read all the chars from char array and put it to file
        for(i = 0; (c = *(str + i) ) != '\0'; i++)
            putc( c, file );
    }
    else
    {
        printf("File cannot be found");
    }
}

int getln( char line[] )
{
    int i = 0;
    int c;

    while( (c = getchar() ) != EOF && c != '\n' )
    {
        line[i] = c;
        i++;
    }

    line[i] = '\0';

    return i; // return the length of the line
}

void println( char line[] )
{
    int i;
    for ( i = 0; line[i] != '\0'; i++ )
    {
        putchar(line[i]);
    }

}


