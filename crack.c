#include <cs50.h>
#include <stdio.h>
#include <crypt.h>
#include <string.h>
#include <math.h>
// Takes a hash and returns a corresponding password.

// Standard Linux DES Hash function that takes keypass and 2-char salt, returns 13-char hash.
char *crypt(const char *key, const char *salt);

int main(int argc, string argv[])
{
    // Make sure command line is correct.
    if (argc != 2 || strlen(argv[1]) != 13)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    // Obtain salt.
    char salt[3] = {argv[1][0], argv[1][1], '\0'};
    
    // Covering case "A" alone because log(possibilities) wouldn't start at possibilities = 0.
    if (strcmp(crypt("A", salt), argv[1]) == 0)
    {
        printf("A\n");
        return 0;
    }
    // Can now set possibilities = 1.
    int possibilities = 1;
    int remainder;
    int chars;
    // Iterates until done finding password.
    while (possibilities < 380204032) // set to 5 letter passwords, 52^5 cases)
    {
        remainder = possibilities;
        chars = log(possibilities) / log(52) + 1;
        char pass[chars];
        char array[chars];
        // Iterates log(52) times, creating an array of base 52.
        for (int i = 0; i < chars; i++)
        {
            // assign current divider (always a power of 52) set by which char of array.
            int divider = round(pow(52, chars - i - 1));
            // assign the array from left to right the possibilities remainder when divided - 1 (0-51).
            if (remainder > 51)
            {
                array[i] = (remainder / divider) - 1;
            }
            else
            {
                array[i] = (remainder / divider);
            }
            remainder = remainder % divider;
        } 
        // Now we have an array with 52 possibilities in each char ready to translate to letters.
        // Use the array[i] to find corresponding letter to be placed in pass[i].
        for (int i = 0; i < chars; i++)
        {
            // Line up with ascii. +65 for uppercase, -26 + 97 = +71 for lower.
            if (array[i] < 26)
            {
                pass[i] = array[i] + 65;
            }
            else if (array[i] > 25 && array[i] < 52)
            {
                pass[i] = array[i] + 71;
            }
            else // In case of mistranslation bug.
            {
                printf("BIG MISTAKE\n");
                return 1;
            }
        }
        // Debugging tool to see words being searched.
        // printf("Possibilities: %i, Pass: %s\n", possibilities, pass);
        // If given hash matches created hash, print.
        if (strcmp(crypt(pass, salt), argv[1]) == 0)
        {
            printf("%s\n", pass);
            return 0;
        }
        possibilities++;
    }
    printf("No password found in %i possibilities searched.\n", possibilities);
    return 0;
}
