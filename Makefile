DIRS=.:hud:particles:events:graphics:gameObjects:xml:utils
EMPTY= 
SPACE= $(EMPTY) $(EMPTY) 
#INCLUDEDIRS=$(addprefix -I, $(subst :, -I, $(DIRS)))
#INCLUDEDIRS=$(subst $(SPACE), -I, $(DIRS))
INCLUDEDIRS=$(subst :, -I, $(DIRS))

# Written by DJ Edmonson
# Warnings frequently signal eventual errors:
CXXFLAGS=`sdl-config --cflags` -g -W -Wall -Weffc++ -Wextra -pedantic -O0 -I$(INCLUDEDIRS)

# Linker flags for both OS X and Linux
LDFLAGS = `sdl-config --libs` -lSDL_ttf -lSDL_image -lexpat -lSDL_gfx -lSDL_mixer -I$(INCLUDEDIRS)

VPATH = hud

# Generates list of object files from all the
#   source files in directory
OBJS = $(addsuffix .o, $(basename $(shell find -name "*.cpp")))
DEPS = $(OBJS:.o=.d)

# Set executable name
EXEC = run

# Declare the phony targets
.PHONY: echo clean r clang gcc setclang setgcc vg

# Phony targets to run dependencies in order
gcc: | setgcc $(EXEC)
clang: | setclang $(EXEC)

# For use with the clang static analyzer by
#  using the environment variable for CXX
sb: | $(clean) $(EXEC)

vg: $(EXEC)
	valgrind ./$(EXEC)

# Run the executable
r:
	./$(EXEC)

clean:
	rm -rf $(OBJS)
	rm -rf $(DEPS)
	rm -rf $(EXEC)
	rm -rf *.d*

# Phony target to use clang for compile and linking
setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)

# Phony target to use g++ for compile and linking
setgcc:
	@echo "Setting g++"
	@echo "Objs: " $(OBJS)
	$(eval CXX = g++)
	$(eval CXX_LINK = g++)

# $< refers to the first dependency
# Uses static pattern rule to keep from compiling all
#   objects every time.
$(OBJS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DEPS): %.d: %.cpp
	@echo "Generating "$@
	@set -e; rm -f $@; \
      g++ -MM $(CXXFLAGS) $< > $@.$$$$; \
      sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
      rm -f $@.$$$$

# $@ refers to the target
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

include $(DEPS)
