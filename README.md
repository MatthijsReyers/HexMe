# HexMe
HexMe is an open source hex editor licensed under the MIT License. Hexme has a feature rich command line interface based on the nCurses library.

![Animated gif preview of HexMe running in terminal.](https://raw.githubusercontent.com/MatthijsReyers/HexMe/master/animation.gif)

## Features
- HexMe will give different colors to different types of bytes. Allowing the user to easily identify null bytes, bytes that translate to valid ascii characters, etc.
- HexMe can recognize a variety of file types by identifing the file header bytes, which it will give a unique color.

## Commands
When commands need a string, it can either be surrounded by quotation marks to enable support for C-style escape sequences or if the string does not contain any escape sequences or spaces the quotation marks may be left out.
| Syntax | Description |
| :----- | :---------- |
| `exit` | The `exit` command exits the HexMe application. |
| `open "path/to/file"` | The `open` command can be used to a different file. |
| `goto [hex/dec] [index]`<br>`goto [index]` | The `goto` command can be used to move the cursor to a specific byte by giving the index of the byte, either in hexadecimal or decimal format. (If no format is given HexMe will assume the index is given in hexadecimal format). |
| `find [first/next/last] "search query\n"` | The `find` command will move the curor to the first/next/last instance of the given search query. Where `next` in this case means: the first instance of the search query after the cursor. |
| `insert "ascii chars or \xFF\x42\x79\x74\x65\x73\x00"` | The `insert` command will insert the given string at the cursor. |
| `replace "ascii chars or \xFF\x42\x79\x74\x65\x73\x00"` | The `replace` command will replace the bytes after the cursor with the given string, extending the file if the replacement string is bigger than the space behind the cursor. |

## Upcomming features
Listed below is a list of features planned for the next version of HexMe:
- Addition of a `delete` command allowing users to delete bytes.
- Addition of a `previous` option to the `find` command effectively doing the same thing as the `next` option but backwards.

## Contributing
If you'd like to help please check out the CONTRIBUTE.md file.
