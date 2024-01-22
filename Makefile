NAME = webser

CXX = c++

CPPFLAG = -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

SRC_DIR = src
SRC_DIR1 = abouassi

OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cpp)
SRC1 = $(wildcard $(SRC_DIR1)/*.cpp)

OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))\
	$(patsubst $(SRC_DIR1)/%.cpp, $(OBJ_DIR)/%.o, $(SRC1))

all : $(NAME)

$(NAME) : $(OBJ)
	@echo "obj filse" $(OBJ)
	$(CXX) $(CPPFLAG) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAG) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR1)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CPPFLAG) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean:
	rm -rf $(OBJ_DIR) $(NAME)

re: fclean all