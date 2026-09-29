# Makefile da TUI-lib
#
#   make            compila a biblioteca estática (build/libtui.a)
#   make examples   compila os exemplos em build/
#   make test       compila e roda os testes
#   make run-hello  roda o exemplo 01_hello (idem run-bounce, run-input)
#   make clean      remove o diretório build/

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
CPPFLAGS += -Iinclude

BUILD    := build
LIB      := $(BUILD)/libtui.a
SRCS     := $(wildcard src/*.cpp)
OBJS     := $(SRCS:src/%.cpp=$(BUILD)/%.o)
EXAMPLES := 01_hello 02_bounce 03_input
TEST     := $(BUILD)/test_tui

all: $(LIB)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

$(LIB): $(OBJS)
	ar rcs $@ $^

examples: $(EXAMPLES:%=$(BUILD)/%)

$(BUILD)/%: examples/%.cpp $(LIB) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ $(LIB)

$(TEST): tests/test_tui.cpp $(LIB) | $(BUILD)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $< -o $@ $(LIB)

test: $(TEST)
	./$(TEST)

run-hello: $(BUILD)/01_hello
	./$(BUILD)/01_hello

run-bounce: $(BUILD)/02_bounce
	./$(BUILD)/02_bounce

run-input: $(BUILD)/03_input
	./$(BUILD)/03_input

clean:
	rm -rf $(BUILD)

.PHONY: all examples test run-hello run-bounce run-input clean
