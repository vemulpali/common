include $(HOME)/nubos/src/etc/Makedefs.vtl

ODIR=obj
TARG=$(BINS)/brdcst_test.exe

default : $(TARG)
all : default

OBJS = $(patsubst %.c, $(ODIR)/%.o, $(wildcard *.c))
OBJS += $(patsubst %.cpp, $(ODIR)/%.o, $(wildcard *.cpp))


$(TARG) : $(ODIR) $(OBJS)
	$(LD) -static -o $(TARG) $(LDFLAGS) $(OBJS) $(LIBS) -lpthread -lvtlc

$(ODIR):
	rm -fr $(ODIR)
	mkdir obj

$(ODIR)/%.o : %.c 
	$(CC) -std=gnu99 -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o : %.cpp 
	$(CPP) -std=gnu++11 -c -o $@ $< $(CFLAGS)

clean:
	rm -f $(ODIR)/*
	rm -f $(TARG)
