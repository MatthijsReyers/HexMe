
- Utils: clean up string escaping function.
- Cmdparser: use smart pointer for storing tokens.
- Textbox: command history with up/down keys.
- Create 'find' command
- Fix display issue on Raspbian
- Floating point exception when window becomes too small?
- Create logging thing to find out why 100% usage.

====== EXIT ======
exit

====== INSERT =======
insert "These ascii chars"

====== FIND ======
find first "this string"
find last "this string"
find next "this string" (<= default)

====== GOTO ======
goto start
goto end
goto hex 0a32
goto dec 10
goto FF0A5      (<= always assumes hex formating)

======= MOVE =======
move absolute 9999
move relative 123
move line 27
move top
move bottom