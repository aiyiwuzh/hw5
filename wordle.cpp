#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void makeWords(std::string& current,
               const std::string& floating,
               const std::set<std::string>& dict, 
               std::set<std::string>& results,
               size_t index,
               int blanks_left); 

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results; 
    std::string current = in; 

    int blanks = 0; 
    for (char c : in) 
    {
        if (c == '-') 
        {
            blanks++; 
        }
    }

    makeWords(current, floating, dict, results, 0, blanks);
    return results; 
}

// Define any helper functions here
void makeWords(std::string& current, const std::string& floating,
               const std::set<std::string>& dict, 
               std::set<std::string>& results, size_t index,
               int blanks_left)
{
    // base case: word is filled
    if (index == current.size())
    {
        // word is valid: floating letters all used & in dictionary
        if (floating.empty() && dict.find(current) != dict.end())
        {
            results.insert(current); 
        }
        return; 
    }

    // recursive case
    if (current[index] != '-')
    {
        // current index is already filled, call recursive call for next index
        makeWords(current, floating, dict, results, index + 1, blanks_left);
        return;
    }

    // first, try placing each floating letter in the blank 
    for (size_t i = 0; i < floating.size(); ++i)
    {
        char c = floating[i];
        current[index] = c; 
        std::string new_floating = floating; 
        new_floating.erase(i, 1);
        makeWords(current, new_floating, dict, results, index + 1, blanks_left - 1);
    }
    
    // if there are more blanks than floating letters, try entire alphabet 
    if ((int)floating.size() < blanks_left)
    {
        for (char c = 'a'; c <= 'z'; ++c)
        {
            // don't use floating letters again 
            if (floating.find(c) != std::string::npos) continue; 
            current[index] = c; 
            makeWords(current, floating, dict, results, index + 1, blanks_left - 1);
        }
    }
    
        // backtrack to reset the current position to -
        current[index] = '-';
}
