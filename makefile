CPP := g++
CPPFLAGS := -std=c++14
SRCS := ibmFloat.cpp project.cpp
OBJS := ibmFloat.o project.o

project: $(OBJS) ibmFloat.h
	$(CPP) $(CPPFLAGS) $(OBJS) -o $@

debug: $(OBJS) ibmFloat.h
	$(CPP) -g $(CPPFLAGS) $(OBJS) -o $@

%.o: %.cpp
	$(CPP) -c $(CPPFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm $(OBJS)
	@rm project debug