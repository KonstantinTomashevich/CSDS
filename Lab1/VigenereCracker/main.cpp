#include <iostream>
#include <cstdio>
#include <vector>
#include <cmath>

#include <queue>
#include <unordered_map>
#include <algorithm>

#include <Utility/File.hpp>
#include <Utility/VigenereCommon.hpp>

#define ERROR_INCORRECT_ARGUMENTS -1
#define ERROR_NO_INPUT -2
#define ERROR_NO_OUTPUT -3
#define UNABLE_TO_PREDICT_KEY_LENGTH -4

template <typename Function> void ForEachDivisor (std::size_t number, Function &callback)
{
    for (std::size_t divisor = 2; divisor <= sqrt (number); ++divisor)
    {
        if (number % divisor == 0)
        {
            std::size_t result = number / divisor;
            callback (divisor);

            if (number != result)
            {
                callback (result);
            }
        }
    }
}

std::unordered_map <size_t, size_t> FindDistances (IterableInputFile &inputFile, size_t subsequenceLength)
{
    std::unordered_map <std::string, std::size_t> occurrences {};
    std::unordered_map <std::size_t, std::size_t> distances {};
    std::string currentString {};
    std::size_t encodedPosition = 0;

    for (char symbol : inputFile)
    {
        int offset;
        if (IsVigenereSupportedSymbol (symbol, offset))
        {
            currentString.push_back (symbol - offset);
            ++encodedPosition;

            if (currentString.size () == subsequenceLength)
            {
                if (occurrences.count (currentString) > 0)
                {
                    distances[encodedPosition - occurrences[currentString]]++;
                }

                occurrences[currentString] = encodedPosition;
                currentString.erase (currentString.begin ());
            }
        }
        else
        {
            currentString.clear ();
        }
    }

    return distances;
}

std::unordered_map <std::size_t, std::size_t> CalculateDivisors (
    const std::unordered_map <std::size_t, std::size_t> &distances, std::size_t &total)
{
    struct
    {
        std::unordered_map <std::size_t, std::size_t> counts {};

        void operator() (std::size_t divisor)
        {
            counts[divisor]++;
        }
    } divisorCounter;

    total = 0;
    for (auto valueCountPair : distances)
    {
        ForEachDivisor (valueCountPair.first, divisorCounter);
        total += valueCountPair.second;
    }

    return divisorCounter.counts;
}

std::size_t PredictKeyLength (IterableInputFile &inputFile, std::size_t subsequenceLength)
{
    std::size_t total = 0;
    std::unordered_map <std::size_t, std::size_t> counts = CalculateDivisors (
        FindDistances (inputFile, subsequenceLength), total);

#define PURE_MAXIMIZATION_BARRIER 0.1f
    struct
    {
        std::size_t divisor;
        std::size_t count;
    } bestDivisor = { 0, 0 };

    for (auto divisor : counts)
    {
        if (divisor.first > bestDivisor.divisor && (float) divisor.second / total >= PURE_MAXIMIZATION_BARRIER)
        {
            bestDivisor = { divisor.first, divisor.second };
        }
    }

    return bestDivisor.divisor;
}

static const char *ENGLISH_SYMBOLS_FREQUENCY_ORDER = "ETAOINSRRHDLUCMFYWGPBVKXQJZ";

int main (int argCount, char **argValue)
{
    if (argCount != 3)
    {
        printf ("Expected arguments:\n"
                "    - text file to crack.\n"
                "    - output file name.\n");
        return ERROR_INCORRECT_ARGUMENTS;
    }

    IterableInputFile inputFile (argValue[1], false);
    if (inputFile.begin () == inputFile.end ())
    {
        printf ("Input file is empty or does not exists!");
        return ERROR_NO_INPUT;
    }

    FILE *outputFile = fopen (argValue[2], "w");
    if (outputFile == nullptr)
    {
        printf ("Unable to create output file!");
        return ERROR_NO_OUTPUT;
    }

    std::size_t keyLength = PredictKeyLength (inputFile, 3);
    if (keyLength == 0)
    {
        printf ("Unable to predict key length for given input file!");
        return UNABLE_TO_PREDICT_KEY_LENGTH;
    }

    printf ("Predicted key length: %d.\n", keyLength);
    std::vector <std::vector <std::size_t>> counters (
        keyLength, std::vector <std::size_t> (ALPHABET_SIZE, 0));

    std::size_t partIndex = 0;
    for (char input : inputFile)
    {
        int offset = -1;
        if (IsVigenereSupportedSymbol (input, offset))
        {
            counters[partIndex][input - offset]++;
            partIndex = (partIndex + 1) % keyLength;
        }
    }

    std::vector <std::vector <int>> replacementTable;
    for (partIndex = 0; partIndex < keyLength; ++partIndex)
    {
        // TODO: Remove debug outputs there?
        std::vector <int> orderedSymbols {};
        
        for (int fillerIndex = 0; fillerIndex < ALPHABET_SIZE; ++fillerIndex)
        {
            orderedSymbols.push_back (fillerIndex);
            printf ("%c %d ", fillerIndex + 'a', counters[partIndex][fillerIndex]);
        }

        printf ("\n");
        std::sort (orderedSymbols.begin (), orderedSymbols.end (),
                   [partIndex, &counters] (const int &first, const int &second)
                   {
                       if (counters[partIndex][first] != counters[partIndex][second])
                       {
                           return counters[partIndex][first] > counters[partIndex][second];
                       }
                       else
                       {
                           return first > second;
                       }
                   });

        for (int symbolIndex = 0; symbolIndex < ALPHABET_SIZE; ++symbolIndex)
        {
            printf ("%c", orderedSymbols[symbolIndex] + 'a');
        }

        printf ("\n");
        std::vector <int> partReplacements (ALPHABET_SIZE, 0);
        for (int symbolIndex = 0; symbolIndex < orderedSymbols.size (); ++symbolIndex)
        {
            printf ("%c = %c\n", orderedSymbols[symbolIndex] + 'a', ENGLISH_SYMBOLS_FREQUENCY_ORDER[symbolIndex]);
            partReplacements[orderedSymbols[symbolIndex]] = ENGLISH_SYMBOLS_FREQUENCY_ORDER[symbolIndex] - 'A';
        }

        replacementTable.emplace_back (partReplacements);
    }

    partIndex = 0;
    for (char input : inputFile)
    {
        int offset = -1;
        if (IsVigenereSupportedSymbol (input, offset))
        {
            fputc (replacementTable[partIndex][input - offset] + offset, outputFile);
            partIndex = (partIndex + 1) % keyLength;
        }
        else
        {
            fputc (input, outputFile);
        }
    }

    fclose (outputFile);
    return 0;
}
