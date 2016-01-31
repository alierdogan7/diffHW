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
    FILE *input_file = fopen("/home/burak/Documents/CS342/input1.txt", "r");
    int word_amount = 0;
    char **words = read_words_from_file(input_file, &word_amount);
    printf("word amount: %d\n", word_amount);

    //printing the words array
    //int i;
    //for ( i = 0; i < word_amount; i++)
    //    printf( words[i] );

    //making a linked list from that 2d pointer array
    Node *node = make_linked_list_from_array(words, word_amount);
    for(; node->next != NULL; node = node->next)
        printf("node: %s", node->word);

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
        char word[100];
        for ( i = 0; i < word_count; i++)
        {
            fgets(word, 100, file);
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


