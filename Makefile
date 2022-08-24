MAKEFILES_DIR := makefiles/
-include $(MAKEFILES_DIR)color.mk

#############
# Variables #
#############

NAME	:= webserv
SRCDIR  := srcs/
SRCDIRS := $(shell find $(SRCDIR) -type d)
SRCS	:= $(shell find $(SRCDIR) -name "*.cpp" -type f)
INCLUDES := $(addprefix -I,$(SRCDIRS))

CXX		:= c++
CXXFLAGS:= -g -Wall -Werror -Wextra -std=c++98 -pedantic

BUILDDIR:= build/
OBJDIR	:= $(BUILDDIR)release/
OBJDIRS := $(SRCDIRS:$(SRCDIR)%=$(OBJDIR)%)
OBJS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.o)
DEPS	:= $(SRCS:$(SRCDIR)%.cpp=$(OBJDIR)%.d)

DSTRCTR	:= ./destructor.c

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJDIRS) $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)

clean: FORCE
	$(RM) -rf $(BUILDDIR)

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
	@mkdir -p $@

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	@printf "$(THIN)$(ITALIC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

-include $(DEPS)

.PHONY: FORCE
FORCE:

###############
# Debug rules #
###############
-include $(MAKEFILES_DIR)debug.mk

##############
# Test rules #
##############
-include $(MAKEFILES_DIR)test.mk
-include ./test/Makefile
