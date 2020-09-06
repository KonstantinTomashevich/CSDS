#include <iostream>
#include <cstdio>
#include <Utility/File.hpp>
#include <vector>

#define ERROR_INCORRECT_ARGUMENTS -1
#define ERROR_NO_INPUT -2
#define ERROR_NO_KEY -2
#define ERROR_NO_OUTPUT -3

int main (int argCount, char **argValue)
{
    if (argCount != 4)
    {
        printf ("Expected arguments:\n"
                "    - text file to encode.\n"
                "    - text file with key.\n"
                "    - output file name.\n");
        return ERROR_INCORRECT_ARGUMENTS;
    }

    IterableInputFile inputFile (argValue[1], false);
    if (inputFile.begin () == inputFile.end ())
    {
        printf ("Input file is empty or does not exists!");
        return ERROR_NO_INPUT;
    }

    IterableInputFile keyFile (argValue[2], false);
    if (keyFile.begin () == keyFile.end ())
    {
        printf ("Key file is empty or does not exists!");
        return ERROR_NO_KEY;
    }

    std::vector <int> key;
    for (char symbol : keyFile)
    {
        if (symbol >= 'a' && symbol <= 'z')
        {
            key.push_back(symbol - 'a');
        }
        else if (symbol >= 'A' && symbol <= 'Z')
        {
            key.push_back(symbol - 'A');
        }
    }

    FILE *outputFile = fopen (argValue [3], "w");
    if (outputFile == nullptr)
    {
        printf ("Unable to create output file!");
        return ERROR_NO_OUTPUT;
    }

    std::size_t keyIndex = 0;
    for (char input : inputFile)
    {
        int offset = -1;
        if (input >= 'a' && input <= 'z')
        {
            offset = 'a';
        }
        else if (input >= 'A' && input <= 'Z')
        {
            offset = 'A';
        }

        if (offset == -1)
        {
            fputc (input, outputFile);
        }
        else
        {
            fputc (offset + (input - offset + key[keyIndex]) % ('z' - 'a'), outputFile);
            // TODO: Or increase key index anyway?
            keyIndex = (keyIndex + 1) % key.size();
        }
    }

    fclose (outputFile);
    return 0;
}
