CC=g++
CFLAGS=-Wall -std=c++17  -Wno-sign-compare
BINDIR=bin
OBJDIR=obj
SRCDIR=src

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/ll1: $(OBJDIR)/LL1.o $(OBJDIR)/grammar.o 
	$(CC) -o $@ $^

$(BINDIR)/lr0: $(OBJDIR)/LR0.o $(OBJDIR)/grammar.o 
	$(CC) -o $@ $^

ll1: $(BINDIR)/ll1
	$(BINDIR)/ll1

lr0: $(BINDIR)/lr0
	$(BINDIR)/lr0
	
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*