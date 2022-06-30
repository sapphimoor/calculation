# calculation
This is a calculation program written in c with only `stdio.h`.

## How to Use
To use this program, run this in a terminal and input calculation formula you want to solve. At this time, you can insert spaces between numbers or operators. This internally handles numbers in fractional form so there is no calculation errors. If the answer is decimal then this shows you answer in `[fractions] ([decimals])` form, otherwise in `[integers]` form.

## Operators and Symbols
- unary operators
  - `+` : [prefix] make a number a positive
  - `-` : [prefix] make a number a negative
  - `!` : [suffix] calculate the factorial  
          &emsp;&emsp;(integer only)
- binary operators
  - `+` : [left-associative] calculate additions
  - `-` : [left-associative] calculate subtractions
  - `*` : [left-associative] calculate multiplications
  - `/` : [left-associative] calculate divisions  
          &emsp;&emsp;(2nd arg: except 0)
  - `^` : [right-associative] calculate powers  
          &emsp;&emsp;(2nd arg: integer only; 
          except `0 ^ [negative number]` form)
  - `P` : [right-associative] calculate permutations  
          &emsp;&emsp;(1st arg: integer more than or equal to 2nd arg; 
          2nd arg: not negative integer)
  - `C` : [right-associative] calculate combinations (both args: integer only)  
          &emsp;&emsp;(1st arg: integer more than or equal to 2nd arg; 
          2nd arg: not negative integer)
- brackets
  - `()` : calculate internals preferentially

|priority number|operators or symbols     |
|---------------|-------------------------|
|1              |`()`                     |
|2              |`!`                      |
|3              |`^` `P` `C`              |
|4              |`*` `/`                  |
|5              |`+` `-` (unary operator) |
|6              |`+` `-` (binary operator)|

## Options
- `-r` (or `--rp`) : show the reverse-polish notation you inputed formula
