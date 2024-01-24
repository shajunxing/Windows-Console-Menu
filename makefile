COMPILER=cl.exe /nologo /c /O2 /MD /wd4819
LINKER=link.exe /nologo

menu.exe: menu.c
    $(COMPILER) menu.c
    $(LINKER) menu.obj