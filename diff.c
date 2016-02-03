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
int count_lines_of_file( FILE *file);
char **read_words_from_file( FILE *file, int *word_amount);
Node *make_linked_list_from_array( char **words, int length);
void sort_str_array( char ***words, int length);
Node *generate_difflist_of_wordlists( Node *node1, Node *node2);
void count(Node *head);


main(int argc, char *argv[])
{
    //READING THE COMMANDLINE ARGUMENTS
    if (argc != 4) {
        printf("You need to give the names of 1st input, 2nd input, and output files in order.");
        return;
    }

    //printf("1: %s, 2: %s, o: %s\n", argv[1], argv[2], argv[3]);


    // READ THE WORDS FROM INPUT AND LOAD THEM INTO A 2D DYNAMIC ARRAY
    FILE *input_file = fopen(argv[1], "r");
    int word_amount = 0;
    char **words = read_words_from_file(input_file, &word_amount);
    printf("word amount: %d\n", word_amount);
    fclose(input_file);

    sort_str_array(&words, word_amount);


    //check whether they are really sorted or not
    /*int i;
    for(i = 0; i < word_amount; i++)
        printf( words[i] );
    */

    Node *printing_node = NULL;

    //making a linked list from that 2d pointer array
    Node *node1 = make_linked_list_from_array(words, word_amount);
    printf("node1\n-------------------------\n");
    for(printing_node = node1; printing_node != NULL; printing_node = printing_node->next)
        printf("%s\n", printing_node->word);
    printf("-------------------------\n");

    //GENERATE THE SECOND LINKED LIST OF WORDS FROM 2ND INPUT
    input_file = fopen(argv[2], "r");
    int word_amount2 = 0;
    char **words2 = read_words_from_file(input_file, &word_amount2);
    printf("2nd word amount: %d\n", word_amount2);
    fclose(input_file);

    sort_str_array(&words2, word_amount2);

    //making a linked list from that 2d pointer array
    Node *node2 = make_linked_list_from_array(words2, word_amount2);
    printf("node2\n-------------------------\n");
    for(printing_node = node2; printing_node != NULL; printing_node = printing_node->next)
        printf("%s\n", printing_node->word);
    printf("-------------------------\n");


    input_file = fopen(argv[3], "wb");
    Node *result_node = generate_difflist_of_wordlists(node1, node2);
    for(; result_node != NULL; result_node = result_node->next)
    {
        printf("result_node: %s\n", result_node->word);
        // write each word from difference list to output file
        write_to_file(result_node->word, input_file);
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
        curr->next = NULL;
        //printf("node added with %s", curr->word);

        //if we are not on head currently
        if(pre != NULL)
            pre->next = curr;
        else
            head = curr;

        pre = curr; //update the pre pointer

    }



    return head;
}

void count(Node *head)
{
    Node *temp;
    int length = 0;
    temp = head;
    while(temp!=NULL)
    {
        length++;
        temp=temp->next;
    }
    printf("nLength of Linked List : %d",length);
}


Node *generate_difflist_of_wordlists( Node *node1, Node *node2)
{
    // node pointers for result linked list
    Node *result_head = NULL;
    Node *result_curr = NULL;
    Node *result_prev = NULL;

    while( node1 != NULL && node2 != NULL )
    {
        /*
        The algorithm is as follows: starting from the first node of
        first list, compare each node to the second list's nodes until finding
        an equal word or a greater word within the second list.

        If an equal word is encountered, just pass to the next node in the
        first list.

        If a greater word is found in second list, that means the current node
        in first list doesn't have any equal word. Then, copy it to the difference
        list and continue comparisons with the next word in first list.

        */

        // when the 2nd list is iterated until a greater word is found
        // in the do-while loop; the actual node2 is lost. so we need to save it
        Node *node2_last = node2;

        int result;
        do {
            result = strcmp( node1->word, node2->word );
            //printf("comparing node1: %s to node2: %s", node1->word, node2->word);
            node2 = node2->next;

        } while ( result > 0 && node2 != NULL); // iterate unless a greater or an equal word is found in 2nd list

        //save back the node2
        node2 = node2_last;

        // if we found that words are equal, iterate both lists
        if ( result == 0)
        {
            node1 = node1->next;

            if( node2 != NULL)
                node2 = node2->next;
        }
        // if a greater word is encountered, that means node1 doesn't have any equal words in 2nd list
        else
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

    return result_head;
}

char **read_words_from_file( FILE *file, int *word_amount)
{
    if(file != NULL)
    {
        //read the word count
        /*char line_no_str[5];
        fgets(line_no_str, 5, file);
        int word_count = atoi(line_no_str);
        */

        int word_count = count_lines_of_file(file);
        *word_amount = word_count; //copy the word amount with pass-by-reference
        //printf("line amount: %d\n", word_count);

        char **words = (char **) malloc( word_count * sizeof(char *) );

        int i;
        char word[256];
        for ( i = 0; i < word_count; i++)
        {
            fgets(word, 256, file);

            //truncate the new line from word
            size_t ln = strlen(word) - 1;
            if( *word && word[ln] == '\n')
                word[ln] = '\0';

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

int count_lines_of_file( FILE *file)
{
    if(file != NULL)
    {
        int c;
        int lines = 0;
        while ( (c = getc(file) ) != EOF )
            if( c == '\n' )
                lines++;

        //move pointer back to the beginning of the file before exiting from function
        rewind(file);

        return lines;
    }
    else
    {
        printf("File not found");
        return 0;
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

//DISCARDED BECAUSE OF MISUNDERSTANDING THE PROBLEM
/*Node *generate_difflist_of_wordlists( Node *node1, Node *node2)
{

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


    // node pointers for result linked list
    Node *result_head = NULL;
    Node *result_curr = NULL;
    Node *result_prev = NULL;

    // until one of lists reach to the end
    while ( node1 != NULL && node2 != NULL)
    {
        if( node1->word == NULL )
            printf("node1word null");
        if( node2->word == NULL )
            printf("node2word null");

        printf("deneme");
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



*/
