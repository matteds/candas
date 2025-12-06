#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* file_content_to_buffer(char* file_path)
{
    char *buffer = NULL;
    FILE *file = fopen(file_path, "r");

    if (file != NULL) 
    {
        // Get file dimentions
        fseek(file, 0, SEEK_END); 
        long bufsize = ftell(file);
        
        // Allocate big enough capacity to store content
        buffer = malloc(sizeof(char) * (bufsize + 1));

        // Reset the file pointer at the file start and save
        // the content in the buffer
        fseek(file, 0, SEEK_SET);
        fread(buffer, sizeof(char), bufsize, file);

        fclose(file);

        return buffer;
    }
    else
    {
        fputs("Error opening file", stderr);
    }

}

int read_csv(char* file_path)
{   
    char* line;
    char* token;
    char* tmp_buffer;
    const char new_line_sep[2] = "\n";
    const char sep[2] = ",";

    // Save file content in a buffer
    char* buffer = file_content_to_buffer(file_path);

    // Split buffer for each new line
    line = strtok(buffer, new_line_sep);

    while (line != NULL) 
    {
        tmp_buffer = strdup(line);
        token = strtok(tmp_buffer, sep);

        while (token != NULL)
        {
            printf("%s\n", token);
            token = strtok(NULL, sep);
        }

        printf("%s\n", line);
        free(tmp_buffer);
        line = strtok(NULL, new_line_sep);
    }

    

    free(buffer);

}

int main()
{
    read_csv("customers_100.csv");
    return 0;
}