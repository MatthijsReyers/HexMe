

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

=
====== INSERT =======
insert x0 x93 x32 xFF
insert "These ascii chars"

======= FIND =======
find "this string"

======= MOVE =======
move absolute 9999
move line 27
move top
move bottom