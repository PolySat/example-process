include Make.rules.arm

override LDFLAGS+=-rdynamic -lproc -lsatpkt -lpolydrivers -lm -lrt -pthread
override CFLAGS+=-Wall -Werror -pedantic -std=gnu99 -g -pthread

SRC=payload.c example_pkt.c
OBJS=$(SRC:.c=.o)
EXECUTABLE=payload example-util
CMDS=
INSTALL_DEST=$(BIN_PATH)
CMD_FILE=payload.cmd.cfg

all: $(EXECUTABLE) $(CMDS)

payload: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@ -lz

example-util: example-util.o example_pkt.o
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@ -lz

example_pkt.c: example_schema.xp
	poly-xdrgen --target libproc --output example_pkt example_schema.xp

example_pkt.h: example_schema.xp
	poly-xdrgen --target libproc --output example_pkt example_schema.xp

install: $(EXECUTABLE) $(CMDS)
	cp $(EXECUTABLE) $(INSTALL_DEST)
	$(STRIP) $(INSTALL_DEST)/$(EXECUTABLE)
	cp $(CMD_FILE) $(ETC_PATH)

.PHONY: clean install

clean:
	rm -rf *.o $(EXECUTABLE) $(CMDS)

