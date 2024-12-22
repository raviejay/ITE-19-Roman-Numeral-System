#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to process the file
int processFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Could not open file: %s\n", fileName);
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        Operation(line);
    }

    fclose(file);
    return 0;
}

// Function to process a single Roman numeral expression
void Operation(char *line) {
    char roman1[100], roman2[100], operator;
    int num1, num2, result;
    char resultInWords[500];

    // Parse the line for two Roman numerals and an operator
    if (sscanf(line, "%s %c %s", roman1, &operator, roman2) == 3) {
        num1 = convertRomanToDecimal(roman1);
        num2 = convertRomanToDecimal(roman2);

        if (num1 > 0 && num2 > 0) {
            // Perform the calculation based on the operator
            switch (operator) {
                case '+':
                    result = num1 + num2;
                    break;
                case '-':
                    result = num1 - num2;
                    break;
                case '*':
                    result = num1 * num2;
                    break;
                default:
                    printf("Invalid operator: %c\n", operator);
                    return;
            }

            // Convert the result to words
            numberToWords(result, resultInWords);

            // Print the result
            printf("Expression: %s %c %s = %d\n", roman1, operator, roman2, result);
            printf("In words: %s\n\n", resultInWords);
        } else {
            printf("Invalid Roman numeral(s) in: %s\n", line);
        }
    } else {
        printf("Invalid format in line: %s\n", line);
    }
}


// Convert to Roman numeral
int romanToDecimal(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0; // Invalid Roman numeral
    }
}

// Convert Roman numeral to decimal
int convertRomanToDecimal(char *roman) {
    int decimal = 0;
    int prevValue = 0;

    // Traverse the Roman numeral from right to left
    for (int i = strlen(roman) - 1; i >= 0; i--) {
        int currentValue = romanToDecimal(roman[i]);

        if (currentValue < prevValue) {
            // Subtractive case (e.g., IV, IX)
            decimal -= currentValue;
        } else {
            // Additive case
            decimal += currentValue;
        }

        prevValue = currentValue;
    }

    return decimal;
}

// Function to convert a number to words
void numberToWords(int num, char *words) {
    char *single_digits[] = { "", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
    char *two_digits[] = { "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety" };
    char *teen_digits[] = { "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen" };
    char *powers[] = { "", "thousand", "million", "billion" };

    if (num == 0) {
        strcpy(words, "zero");
        return;
    }

    int isNegative = 0;
    if (num < 0) {
        isNegative = 1;
        num = -num; // Make the number positive for further processing
    }

    int part[4] = {0}, i = 0;
    while (num > 0) {
        part[i++] = num % 1000;
        num /= 1000;
    }

    int idx = 0; // Track position in `words`

     if (isNegative) {
        char negativeWord[] = "negative ";
        int i = 0;
        while (negativeWord[i] != '\0') {
            words[idx++] = negativeWord[i++];
        }
    }


    for (int j = i - 1; j >= 0; j--) {
        int n = part[j];
        if (n == 0) continue;

        // Handle hundreds
        if (n / 100 > 0) {
            char *digit = single_digits[n / 100];
            while (*digit) words[idx++] = *digit++;
            words[idx++] = ' ';
            words[idx++] = 'h';
            words[idx++] = 'u';
            words[idx++] = 'n';
            words[idx++] = 'd';
            words[idx++] = 'r';
            words[idx++] = 'e';
            words[idx++] = 'd';
            words[idx++] = ' ';
        }

        n %= 100;

        // Handle tens and ones
        if (n >= 10 && n < 20) {
            char *teen = teen_digits[n - 10];
            while (*teen) words[idx++] = *teen++;
        } else {
            if (n / 10 > 0) {
                char *tens = two_digits[n / 10];
                while (*tens) words[idx++] = *tens++;
                words[idx++] = ' ';
            }
            if (n % 10 > 0) {
                char *unit = single_digits[n % 10];
                while (*unit) words[idx++] = *unit++;
            }
        }


        if (strlen(powers[j]) > 0) {
            words[idx++] = ' ';
            char *power = powers[j];
            while (*power) words[idx++] = *power++;
        }

        words[idx++] = ' ';
    }

    words[idx - 1] = '\0';
}


int main() {
    const char *fileName = "roman.txt";

    // Process the file
    if (processFile(fileName) != 0) {
        printf("Error processing the file: %s\n", fileName);
        return 1;
    }

    return 0;
}
