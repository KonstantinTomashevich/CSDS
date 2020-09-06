#include "VigenereCommon.hpp"

bool ReadVigenereKey (IterableInputFile keyFile, std::vector <int> &keyOutput)
{
    if (keyFile.begin () == keyFile.end ())
    {
        return false;
    }
    else
    {
        for (char symbol : keyFile)
        {
            if (symbol >= 'a' && symbol <= 'z')
            {
                keyOutput.push_back (symbol - 'a');
            }
            else if (symbol >= 'A' && symbol <= 'Z')
            {
                keyOutput.push_back (symbol - 'A');
            }
        }

        return true;
    }
}

bool IsVigenereSupportedSymbol (char symbol, int &normalizationOffsetOutput)
{
    if (symbol >= 'a' && symbol <= 'z')
    {
        normalizationOffsetOutput = 'a';
        return true;
    }
    else if (symbol >= 'A' && symbol <= 'Z')
    {
        normalizationOffsetOutput = 'A';
        return true;
    }
    else
    {
        return false;
    }
}
