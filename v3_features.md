

 - replace bytes XXX with bytes YYY.
 - delete bytes x till y
 - insert bytes at cursor.
 - delete byte at cursor.
 - modify byte at cursor. 


╔═══════════════════════════════════════════════════╗
║ 
║ 
║ Apply changes: [ENTER]     Cancel: [ESCAPE]     Reset: [R] ║
╚═══════════════════════════════════════════════════╝


EXAMPLE COMMANDS

====== SAVE ======
save 
save copy

====== EXIT ======
exit
exit nosave

====== INSERT =======
insert x0 x93 x32 xFF
insert "These ascii chars"

======= FIND =======
find "this string"
find x

======= MOVE =======
move absolute 9999
move relative 123
move line 27
move top
move bottom