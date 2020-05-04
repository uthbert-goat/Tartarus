CC      = gcc
PROF    = -O -g
NOCRYPT =
C_FLAGS =  -Wall $(PROF) $(NOCRYPT)
L_FLAGS =  $(PROF)

O_FILES = act_comm.o act_ente.o act_info.o act_move.o act_obj.o act_wiz.o \
          alias.o ban.o comm.o const.o db.o db2.o effects.o fight.o flags.o \
	  handler.o healer.o interp.o note.o lookup.o magic.o recycle.o\
	  save.o scan.o skills.o special.o tables.o update.o mob_cmds.o\
          mob_prog.o

tartarus: $(O_FILES)
	rm -f tartarus
	$(CC) $(L_FLAGS) -o tartarus $(O_FILES)

.c.o: merc.h
	$(CC) -c $(C_FLAGS) $<
