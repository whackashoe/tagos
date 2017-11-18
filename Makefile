override DEPFLAGS += -MT $@ -MMD -MP -MF $(BUILD_DIR)/$*.d
#override DEPFLAGS += -MM
override CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -Werror=switch \
	-Wno-inconsistent-missing-override

override LDFLAGS += -lsfml-graphics -lsfml-window -lsfml-system \
	-Llib/Box2D/Build/gmake/bin/Debug -lBox2D \
	-lboost_system -lboost_thread \
	-pthread

BUILD_DIR := build
SRC_DIR := src

INCLUDES := -Iinclude -Ilib/Box2D
SOURCES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  := $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: tagos

release: CXXFLAGS += -O3
release: tagos
debug: CXXFLAGS += -DDEBUG -g
debug: tagos

tagos: $(OBJECTS) lib/Box2D/Build/gmake/bin/Debug/libBox2D.a
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

lib/Box2D/Build/gmake/bin/Debug/libBox2D.a:
	make -C lib/Box2D/Build/gmake

$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(INCLUDES) $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/%.d
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -c $@

clean:
	@rm -f build/*
	make -C lib/Box2D/Build/gmake clean

mostlyclean:
	@rm -f build/*

.PHONY: mostlyclean clean
.PRECIOUS: $(BUILD_DIR)/%.d

-include $(BUILD_DIR)/%.d
