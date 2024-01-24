A utility used in Windows batch scripts that displays a simple menu that can be interacted with keyboard arrow keys and returns selection value, which can be used instead of "select /p" command.

Usage: `menu <menuitem> [[menuitem] menuitem ...]`, Up/Down to select, Enter to select, ESC to cancel, returns number of menu rows you selected, starting from 1, or 0 if canceled, or -1 in case of error. Use `errorlevel` to check return value in batch script.

The display effect is as follows:

```
    WoW Launcher
--------------------------------------------------
    1. Turtle WoW CN
 >  2. Turtle WoW EN
    3. World of Warcraft 1.12.1.5875 enUS
    4. World of Warcraft 1.12.1.5875 zhCN
    5. World of Warcraft 3.3.5a.12340 enUS
```
