C=gcc
CXX=g++
RM=rm -f


SRCS=admin.cc driver.cc scanner.cc symtable.cc
OBJS=$(subst .cc,.o,$(SRCS))


All: driver

driver: $(OBJS)
	$(CXX) $(LDFLAGS) -o driver  $(OBJS)  $(LDLIBS)

driver.o: driver.cc

scanner.o: scanner.h scanner.cc

symtable.o: symtable.h symtable.cc

token.o: token.h 

admin.o: admin.h admin.cc

symbol.o: symbol.h

clean:
	$(RM) $(OBJS)

distclean: clean
	   $(RM) tool
