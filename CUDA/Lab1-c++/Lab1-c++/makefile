CXX = clang++
CXXFLAGS = -O2 -std=c++17 -fno-vectorize -fno-tree-vectorize -fno-slp-vectorize -Rpass=loop-vectorize -Rpass-missed=loop-vectorize -Rpass-analysis=loop-vectorize -Wall -Iinclude -march=native -fopenmp
LDFLAGS = -O2 -lpapi -lomp -fopenmp

SRCDIR = src
INCDIR = include
BUILDDIR = build
BINDIR = bin

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

all: create_dirs $(BINDIR)/matrix_multiply

create_dirs:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)

$(BINDIR)/matrix_multiply: $(OBJECTS)
	@echo "Linking: $@"
	@$(CXX) $(LDFLAGS) $^ -o $@
	@echo "Build completed!"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "Compiling: $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILDDIR) $(BINDIR)
	@echo "Clean completed!"

rebuild: clean all

.PHONY: all create_dirs clean rebuild