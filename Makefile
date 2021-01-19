#OPT = -m32
fs_comp: fs_comp.c
	gcc $(OPT) -o fs_comp fs_comp.c -I.