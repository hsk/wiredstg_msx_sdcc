#! make -f
#
# makefile - start
#


# directory
#

# source file directory
SRCDIR			=	sources

# include file directory
INCDIR			=	sources

# object file directory
OBJDIR			=	objects

# binary file directory
BINDIR			=	bin

# output file directory
OUTDIR			=	disk

# vpath search directories
VPATH			=	$(SRCDIR):$(INCDIR):$(OBJDIR):$(BINDIR)

# assembler
#

# assembler command
AS				=	sdasz80

# assembler flags
ASFLAGS			=	-ls -I$(INCDIR) -I.

# c compiler
#

# c compiler command
CC				=	sdcc

# c compiler flags
CFLAGS			=	-mz80 --opt-code-speed -I$(INCDIR) -I.

# linker
#

# linker command
LD				=	sdcc

# linker flags
LDFLAGS			=	-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x82f0 --data-loc 0xd000

# suffix rules
#
.SUFFIXES:			.s .c .rel

# assembler source suffix
.s.rel:
	$(AS) $(ASFLAGS) -o $(OBJDIR)/$@ $<

# c source suffix
.c.rel:
	$(CC) $(CFLAGS) -o $(OBJDIR)/$@ -c $<

# project files
#

# target name
TARGET			=	WIREDSTG

# assembler source files
ASSRCS			=	crt0.s
				

# c source files
CSRCS			=	\
				main.c System.c \
				App.c \
				Game.c Ship.c \
				pattern.c

# object files
OBJS			=	$(ASSRCS:.s=.rel) $(CSRCS:.c=.rel)

1 2 3 4: $(TARGET).com
	openmsx -machine Sanyo_PHC-70FD2 -diska $(OUTDIR) -command "set speed 9999;after time 12.5 \"set speed $@00\""
# build project target
#
$(TARGET).com:		$(OBJDIR) $(BINDIR) $(OBJS) tools/ihx2bload
	$(LD) $(LDFLAGS) -o $(BINDIR)/$(TARGET).ihx $(foreach file,$(OBJS),$(OBJDIR)/$(file))
	tools/ihx2bload $(BINDIR)/$(TARGET).ihx -o $(OUTDIR)/$(TARGET).BIN
tools/ihx2bload:
	cd tools; make
$(OBJDIR):
	mkdir $(OBJDIR)
$(BINDIR):
	mkdir $(BINDIR)
# clean project
#
clean:
	rm -rf $(OBJDIR) $(BINDIR) tools/ihx2bload
##	@del /F /Q makefile.depend

# build depend file
#
depend:
##	ifneq ($(strip $(CSRCS)),)
##		$(CC) $(CFLAGS) -MM $(foreach file,$(CSRCS),$(SRCDIR)/$(file)) > makefile.depend
##	endif

# build resource file
#
resource:
	cd resources; python pattern.py > ../sources/pattern.c

# phony targets
#
.PHONY:				clean depend

# include depend file
#
-include makefile.depend


# makefile - end
