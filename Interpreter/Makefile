C=gcc
CXX=g++
RM=rm -f


SRCS=interp.cc driver.cc
OBJS=$(subst .cc,.o,$(SRCS))


All: driver

driver: $(OBJS)
	$(CXX) $(LDFLAGS) -o driver  $(OBJS)  $(LDLIBS) $(clean)


driver.o: driver.cc

interp.o: interp.h interp.cc

clean:
	$(RM) $(OBJS) 

distclean: clean
	   $(RM) tool
