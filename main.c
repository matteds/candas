#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    char* content;
    int lenght;

} String;

typedef struct
{
    String* content;
    int capacity;
    int lenght;

} String_array;

typedef struct
{
    String content;
    int x_id;
    int y_id;
    
} Cell;

typedef struct
{
    Cell* content;
    int capacity;
    int lenght;

} Dataframe;


void free_string(String str)
{
    free(str.content);
}

void free_string_array(String_array str_array)
{
    for (int i = 0; i < str_array.lenght; i++)
    {
        free_string(str_array.content[i]);
    }
}

String file_content_to_buffer(char* file_path)
{
    String buffer = {0};
    FILE *file = fopen(file_path, "r");

    if (file != NULL) 
    {
        // Get file dimentions
        fseek(file, 0, SEEK_END); 
        buffer.lenght = ftell(file) + 1;
        
        // Allocate big enough capacity to store content
        buffer.content = malloc(sizeof(char) * (buffer.lenght));

        // Reset the file pointer at the file start and save
        // the content in the buffer
        fseek(file, 0, SEEK_SET);
        fread(buffer.content, sizeof(char), buffer.lenght, file);

        fclose(file);

        return buffer;
    }
    else
    {
        fputs("Error opening file ", stderr);
    }

}

String_array str_split_to_array(String input_string, const char* sep)
{

    String_array output = {0};

    char* token;
    output.content = malloc(2 * sizeof(String));
    output.capacity = 2;

    int count = 0;

    token = strtok(input_string.content, sep);
    while (token != NULL)
    {
        String tmp = {0};
        tmp.content = token;
        tmp.lenght = strlen(token) + 1;

        if (count >= output.capacity)
        {
            output.capacity *= 2;
            output.content = realloc(output.content, output.capacity * sizeof(String));
        }

        output.content[count] = tmp;
        output.lenght = count;
        token = strtok(NULL, sep);
        count++;
    }

    return output;
    
}

Dataframe read_csv(char* file_path, const char* sep)
{   
    Dataframe output = {0};
    int count = 0;
    output.content = malloc(2 * sizeof(Cell));
    output.capacity = 2;

    // Save file content in a buffer
    String buffer = file_content_to_buffer(file_path);

    // Split saved content by new line
    String_array lines = str_split_to_array(buffer, "\n");

    // Iterate on rows to extract cells
    for (int row_id = 0; row_id < lines.lenght; row_id++) 
    {
        String_array row = {0};
        row = str_split_to_array(lines.content[row_id], sep);

        for (int col_id = 0; col_id < row.lenght; col_id++)
        {
            Cell tmp = {0};
            tmp.content = row.content[col_id];
            tmp.x_id = row_id;
            tmp.y_id = col_id;

            if (count >= output.capacity)
            {
                output.capacity *= 2;
                output.content = realloc(output.content, output.capacity * sizeof(Cell));
            }

            output.content[count] = tmp;
            count++;
        }
    }

    output.lenght = count;

    //free_string(buffer);
    //free_string_array(lines);

    return output;

}

int main()
{
    Dataframe df = read_csv("customers_100.csv", ",");
    printf("%s\n", df.content[0].content.content);
    printf("%d\n", df.content[0].x_id);
    printf("%d\n", df.content[0].y_id);

    return 0;
}