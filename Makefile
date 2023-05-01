CC=g++
CFLAGS=-Wall -std=c++17  -Wno-sign-compare
BINDIR=bin
OBJDIR=obj
SRCDIR=src

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/exec: $(OBJDIR)/main.o $(OBJDIR)/grammar.o 
	$(CC) -o $@ $^

test: $(BINDIR)/exec
	$(BINDIR)/exec

clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*