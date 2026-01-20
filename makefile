CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

SRC := src/main.cc src/rng.cc src/market.cc src/trader.cc src/strategy.cc src/simulator.cc src/csv.cc
OUT := sim


all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
