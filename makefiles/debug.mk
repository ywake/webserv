###############
# Debug rules #
###############

SAN_OBJDIR	:= $(BUILDDIR)debug/
SAN_OBJDIRS := $(SRCDIRS:$(SRCDIR)%=$(SAN_OBJDIR)%)
SAN_OBJS	:= $(SRCS:$(SRCDIR)%.cpp=$(SAN_OBJDIR)%.o)
SAN_DEPS	:= $(SRCS:$(SRCDIR)%.cpp=$(SAN_OBJDIR)%.d)
.PHONY: $(SAN_DEPS)

-include $(SAN_DEPS)

$(SAN_OBJDIR)%.o: $(SRCDIR)%.cpp
	@printf "$(CYAN)$(ITALIC)"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MMD -MP -c $< -o $@
	@printf "$(END)"

sani: CXXFLAGS += -fsanitize=address -g3
sani: $(SAN_OBJDIRS) $(SAN_OBJS)
	$(CXX) $(CXXFLAGS) $(SAN_OBJS) -o $(NAME) $(LIBS)

$(SAN_OBJDIRS):
	@mkdir -p $@

$(DSTRCTR):
	curl https://gist.githubusercontent.com/ywake/793a72da8cdae02f093c02fc4d5dc874/raw/destructor.c > $(DSTRCTR)

Darwin_leak: $(DSTRCTR) $(OBJDIRS) $(OBJS)
	$(CXX) -g -std=c++98 $(OBJS) $(DSTRCTR) -o $(NAME) $(LIBS)

.PHONY: Linux_leak
Linux_leak: sani

.PHONY: leak
leak: $(shell uname)_leak
