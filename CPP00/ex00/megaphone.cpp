#include <iostream>
#include <cctype>

int main(int argc, char **argv) {

		if(argc == 1) {
			std::cout << "* LOUD AND UNBEARABLE FEEDBACK NOISE *" << std::endl;
			return 1;
		}
	
		for(int i = 1; i < argc; i++) {
			for(int j = 0; argv[i][j] != '\0'; j++) {
				char c = argv[i][j];
        	std::cout << (char)std::toupper(c);
			}
		}
		std::cout << std::endl;
    return 0;
}