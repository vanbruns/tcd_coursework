# From: https://gist.github.com/kristopherjohnson/f4eca9018c5085fc736b2f29a202b8f8

#!/usr/bin/env python3

import sys

letterValues = {
    'M': 1000,
    'D': 500,
    'C': 100,
    'L': 50,
    'X': 10,
    'V': 5,
    'I': 1
}

def value_of_roman_letter(ch):
    """Return the value associated with a single Roman numeral letter."""
    try:
        return letterValues[ch.upper()]
    except KeyError:
        print(f"error: invalid Roman numeral character '{ch}'\n",
              file=sys.stderr)
        sys.exit(1)

def int_from_roman_numeral(s):
    """Return the integer value of the given Roman numeral string."""
    result = 0
    lastValue = sys.maxsize
    for ch in s:
        value = value_of_roman_letter(ch)
        if value > lastValue:
            # We've found something like "IV" or "IX"
            # or "XC".  Need to undo the addition
            # of the previous value, then add
            # (value - lastValue) to the result.
            result += value - 2 * lastValue
        else:
            result += value
        lastValue = value
    return result

# From: https://www.w3resource.com/python-exercises/class-exercises/python-class-exercise-1.php

def int_to_roman_numeral(num):
        val = [
            1000, 900, 500, 400,
            100, 90, 50, 40,
            10, 9, 5, 4,
            1
            ]
        syb = [
            "M", "CM", "D", "CD",
            "C", "XC", "L", "XL",
            "X", "IX", "V", "IV",
            "I"
            ]
        roman_num = ''
        i = 0
        while  num > 0:
            for _ in range(num // val[i]):
                roman_num += syb[i]
                num -= val[i]
            i += 1
        return roman_num