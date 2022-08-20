#############
# Variables #
#############

NAME	:= webserv
SRCDIR := srcs/
SRCS	:= $(shell find $(SRCDIR:%/=%) -name "*.cpp" -type f)
VAR :=
INCLUDES = $(addprefix -I,$(shell find $(SRCDIR:%/=%) -type d))
TEST_CPP:=

CXX		:= c++
CXXFLAGS:= -g -Wall -Werror -Wextra -std=c++98 -pedantic #-fsanitize=address

OBJDIR	:= build/
OBJS	:= $(patsubst $(SRCDIR)%,$(OBJDIR)%,$(SRCS:%.cpp=%.o))
DEPS	:= $(OBJS:%.o=%.d)

DSTRCTR	:= ./destructor.c

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LIBS)

clean: FORCE
	$(RM) $(OBJS) $(DEPS)

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(NAME).dSYM

re: fclean all

run: $(NAME)
	./$(NAME)

norm: FORCE
	@printf "$(RED)"; norminette | grep -v ": OK!" \
	&& exit 1 \
	|| printf "$(GREEN)%s\n$(END)" "Norm OK!"

$(OBJDIRS):
	mkdir -p $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@mkdir -p $(OBJDIR)$(*D)
	@printf "$(THIN)$(ITALIC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

-include $(DEPS)

.PHONY: FORCE
FORCE:

###############
# Debug rules #
###############

$(DSTRCTR):
	curl https://gist.githubusercontent.com/ywake/793a72da8cdae02f093c02fc4d5dc874/raw/destructor.c > $(DSTRCTR)

sani: $(OBJDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) -fsanitize=address $(OBJS) -o $(NAME) $(LIBS)

Darwin_leak: $(DSTRCTR) $(OBJDIRS) $(OBJS)
	$(CXX) -g -std=c++98 $(OBJS) $(DSTRCTR) -o $(NAME) $(LIBS)

Linux_leak: sani

leak: $(shell uname)_leak

##############
# Test rules #
##############
TESTDIR		= gtest
GTESTDIR	= $(TESTDIR)/googletest
GTESTLIB	= $(GTESTDIR)/gtest.a
TESTCASE_DIR = $(TESTDIR)/testcases
TESTCASES	= $(wildcard $(TESTCASE_DIR)/*test.cpp)
TESTOBJS	= $(filter-out %main.o, $(OBJS))
TESTLIBS	= -lpthread
TESTER		= tester

$(GTESTLIB)	:
	$(MAKE) -C $(TESTDIR)

$(TESTER)	: $(GTESTLIB) $(TESTCASES) $(NAME)
	clang++ -std=c++11 -I$(GTESTDIR)/gtest $(INCLUDES) $(GTESTLIB) $(TESTCASES) $(TESTOBJS) $(TESTLIBS) -o $@

gtest    : $(TESTER) FORCE
	./$<

-include ./test/Makefile

##########
# Colors #
##########

END		= \e[0m
BOLD	= \e[1m
THIN	= \e[2m
ITALIC	= \e[3m
U_LINE	= \e[4m
BLACK	= \e[30m
RED		= \e[31m
GREEN	= \e[32m
YELLOW	= \e[33m
BLUE	= \e[34m
PURPLE	= \e[35m
CYAN	= \e[36m
WHITE	= \e[37m
