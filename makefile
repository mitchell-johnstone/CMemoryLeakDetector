all: deep_clean base_clean base_clean_bad

base_clean: base_clean.c
	gcc -Wall -Wextra -g -o base_clean base_clean.c mem_lib.c mem_lib.h

base_clean_bad: base_clean_bad.c
	gcc -Wall -Wextra -g -o base_clean_bad base_clean_bad.c mem_lib.c mem_lib.h

deep_clean: deep_clean.c
	gcc -Wall -Wextra -g -o deep_clean deep_clean.c mem_lib.c mem_lib.h

