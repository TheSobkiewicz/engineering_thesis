CC := icpx -fsycl
CFLAGS := -std=c++17
WFLAGS := -Wall -Wextra

INPUT_ARGS := config.json

LIBS := libs
SRCDIR := src
BUILDDIR := build
TESTBUILDDIR := testbuild
DEVCLOUD_EXEC := exec
INCDIR := include
BINDIR := bin
EXEC := HelixSolver.out
TESTSRCDIR := test
TESTEXEC := Test.out
TARGET := $(BINDIR)/$(EXEC)
TESTTARGET := $(BINDIR)/$(TESTEXEC)
SRCEXT := cpp

LINKBOOST := -lboost_program_options

SRC := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJ := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRC:.$(SRCEXT)=.o))

INC := -I $(INCDIR)

TESTSRC := $(shell find $(TESTSRCDIR) -type f -name *.$(SRCEXT))
TESTOBJ := $(patsubst $(TESTSRCDIR)/%,$(TESTBUILDDIR)/%,$(TESTSRC:.$(SRCEXT)=.o))

ARRIA_FLAGS_FOR_REPORT := -fintelfpga -Xshardware -fsycl-link=early 
STRATIX_FLAGS_FOR_REPORT := -fintelfpga -Xshardware -fsycl-link=early 

ARRIA_FLAGS := -fintelfpga -Xshardware 
STRATIX_FLAGS := -fintelfpga -Xshardware 

fpga_hw:
	$(CC) $(CFLAGS) $(STRATIX_FLAGS) $(INC) $(SRCDIR)/*.cpp -o bin/fpga_out.fpga

report:
	$(CC) $(CFLAGS) $(STRATIX_FLAGS_FOR_REPORT) $(INC) $(SRCDIR)/*.cpp -o bin/fpga_compile_report.a

run: $(TARGET)
	./$< $(INPUT_ARGS)

build: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -fintelfpga -DFPGA_EMULATOR $(LINKBOOST) $^ -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -fintelfpga -DFPGA_EMULATOR $(INC) $< -c -o $@ $(WFLAGS)

test: $(TESTTARGET)
	./$<

$(TESTTARGET): $(TESTOBJ)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -pthread $(LIBS)/gtest-obj/*.o $^ -o $@

$(TESTBUILDDIR)/%.o: $(TESTSRCDIR)/%.$(SRCEXT)
	mkdir -p $(TESTBUILDDIR)
	$(CC) $(CFLAGS) $(INC) $< -c -o $@ $(WFLAGS)

clean:
	rm -rf $(BUILDDIR) $(BINDIR) $(TESTBUILDDIR)
	rm -rf $(DEVCLOUD_EXEC)/build.sh.*
	rm -rf $(DEVCLOUD_EXEC)/run.sh.*
	rm -rf *.sh.o*
	rm -rf *.sh.e*
	rm -rf *.png
	rm -rf detected-circles.txt

.PHONY: clean, run, build, test, report, fpga_hw
