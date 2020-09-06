#pragma once
#include <vector>
#include <Utility/File.hpp>

#define ALPHABET_SIZE ('z' - 'a' + 1)

bool ReadVigenereKey (IterableInputFile keyFile, std::vector <int> &output);
bool IsVigenereSupportedSymbol (char symbol, int &normalizationOffsetOutput);
