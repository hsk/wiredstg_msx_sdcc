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
LDFLAGS			=	-mz80 --no-std-crt0 --nostdinc --nostdlib --code-loc 0x8420 --data-loc 0xd000

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
				Title.c \
				Game.c Ground.c Star.c Ship.c Shot.c Bullet.c \
				Enemy.c EnemyBomb.c EnemyFans.c EnemyRugal.c EnemyGarun.c EnemyDee01.c EnemyDucker.c EnemyBigCore.c EnemyBeam.c \
				pattern.c

# object files
OBJS			=	$(ASSRCS:.s=.rel) $(CSRCS:.c=.rel)

r: $(TARGET).com
	openmsx -machine Sanyo_PHC-70FD2 -diska $(OUTDIR) -command "set speed 9999;after time 12.5 \"set speed 400\""

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
	@bin2s -n patternTable -o sources\pattern.s resources\pattern.chr

# phony targets
#
.PHONY:				clean depend

# include depend file
#
-include makefile.depend


# makefile - end
