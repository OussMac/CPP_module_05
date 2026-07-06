CPP     = c++ -std=c++98
FLAGS   = -Wall -Werror -Wextra
NAME    = Intern
HEADERS = Bureaucrat.hpp AForm.hpp ShrubberyCreationForm.hpp \
          RobotomyRequestForm.hpp PresidentialPardonForm.hpp Intern.hpp
SRC     = main.cpp Bureaucrat.cpp AForm.cpp \
          ShrubberyCreationForm.cpp RobotomyRequestForm.cpp \
          PresidentialPardonForm.cpp Intern.cpp
OBJ     = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAGS) $^ -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CPP) $(FLAGS) -c $< -o $@

re: fclean all

fclean: clean
	rm -f $(NAME)

clean:
	rm -f $(OBJ)

.PHONY: clean fclean re all