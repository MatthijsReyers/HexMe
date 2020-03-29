
- Cmdparser: Create new exception to be thrown at syntax error.
- Cmdparser: String does not have to be closed with a ".
- Viewer: Resizing may break cursor position.
- App: GUI should not be in charge of the file cursor position.
- Utils: clean up string escaping.

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