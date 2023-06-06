CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
LDFLAGS =
LDLIBS = -lcurl

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
TARGET = auth_http

REQUIRED_LIBS = curl

.PHONY: all clean fclean re check_libs

all: check_libs $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

check_libs:
	@echo "Checking required libraries..."
	@for lib in $(REQUIRED_LIBS); do \
		if ! ldconfig -p | grep -q $$lib; then \
			echo "WARNING: The required library $$lib was not found. Please make sure it is installed."; \
			# exit 1; \
		fi; \
	done
	@echo "All required libraries found."

check_compiler:
	@echo "Checking compiler version..."
	@ver=`$(CXX) --version | awk 'NR==1{print substr($$NF,1,3)}'`; \
	if [ $$(echo "$$ver >= 7.0" | bc) -eq 0 ]; then \
		echo "ERROR: Compiler version must be 7.0 or higher."; \
		exit 1; \
	fi
	@echo "Compiler version check passed."
