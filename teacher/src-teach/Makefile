# 2010.2.22

#TAR_DIR = ./out/
DIR = ./out/

src = $(wildcard *.c)
dir = $(notdir $(src))
obj = $(patsubst %.c, $(DIR)%.o, $(dir))
target = main

CC = gcc
#CC = arm-linux-gcc
CFLAGS = -Wall -g -c -I../jpeg-8/ -I../freetype-2.3.11/include
LDFLAGS = -L../jpeg-8/.libs/ -ljpeg -L../freetype-2.3.11/objs/.libs -lfreetype -lpthread

all: $(target)

$(target): $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)
$(DIR)%.o:%.c
	$(CC) -o $@ $(CFLAGS) $< 

clean:
	-rm -f $(target)
	-rm -f $(obj)
	-rm -f *~

.PHONY: clean
#test:
#	@echo $(src)
#	@echo $(obj)
