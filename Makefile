CC=g++
CFLAGS=-Wall -std=c++17
BINDIR=bin
OBJDIR=obj
SRCDIR=src

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(BINDIR)/test: $(OBJDIR)/main.o $(OBJDIR)/grammar.o 
	$(CC) -o $@ $^

test: $(BINDIR)/test
	$(BINDIR)/test

clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*