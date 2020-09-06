#include <iostream>
#include <cstdio>
#include <vector>

#include <Utility/File.hpp>
#include <Utility/VigenereCommon.hpp>

#define ERROR_INCORRECT_ARGUMENTS -1
#define ERROR_NO_INPUT -2
#define ERROR_NO_KEY -2
#define ERROR_NO_OUTPUT -3

int main (int argCount, char **argValue)
{
    if (argCount != 4)
    {
        printf ("Expected arguments:\n"
                "    - text file to decode.\n"
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

    std::vector <int> key;
    if (!ReadVigenereKey ({ argValue[2], false }, key))
    {
        printf ("Key file is empty or does not exists!");
        return ERROR_NO_KEY;
    }

    FILE *outputFile = fopen (argValue[3], "w");
    if (outputFile == nullptr)
    {
        printf ("Unable to create output file!");
        return ERROR_NO_OUTPUT;
    }

    std::size_t keyIndex = 0;
    for (char input : inputFile)
    {
        int offset = -1;
        if (IsVigenereSupportedSymbol (input, offset))
        {
            int decoded = (input - offset - key[keyIndex]);
            if (decoded < 0)
            {
                decoded = ALPHABET_SIZE + decoded;
            }

            decoded %= ALPHABET_SIZE;
            fputc (offset + decoded, outputFile);

            // TODO: Or increase key index anyway?
            keyIndex = (keyIndex + 1) % key.size ();
        }
        else
        {
            fputc (input, outputFile);
        }
    }

    fclose (outputFile);
    return 0;
}
