CXX = g++
CXXFLAGS = -O2 -std=c++17

TARGET = GridGame
SRC = GridGame.cpp

STEPS = 00 03 06 09 12

all: images

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

jgr: $(TARGET)
	./$(TARGET)

images: jgr
	for s in $(STEPS); do \
	  jgraph step_$${s}.jgr > step_$${s}.ps; \
	  ps2pdf step_$${s}.ps step_$${s}.pdf; \
	  convert step_$${s}.ps step_$${s}.jpg; \
	done

clean:
	rm -f $(TARGET) step_*.jgr step_*.ps step_*.pdf step_*.jpg
