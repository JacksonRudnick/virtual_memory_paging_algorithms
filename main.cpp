#include <iostream>
#include <string>
#include <fstream>

struct frame_register {
	int curr_register;
	int num_frames;
	int* frames;
};


std::string get_string(std::string file_name) {
	std::string s;
	std::ifstream file(file_name);
	
	if (file.is_open()) {
		std::string line;
		getline(file, line);
		s = line;
		file.close();
	}
	else {
		std::cout << "Error: Unable to open file" << std::endl;
	}

	return s;
}

void fifo(frame_register fr, int* in, int len) {
	for (int i = 0; i < len; i++) {
		
	}
	
}

void opt(frame_register fr, int* in, int len) {

}

int main() {
	std::string file_name = "FIFO.txt";
	
	std::string s = get_string(file_name);

	// Debug line
	std::cout << s << std::endl;

	char option = s[0];

	int num_frames = s[2] - '0';

	int input_length = (s.length()-2)/2;
	int* input = new int[input_length];

	for (int i = 0; i < (s.length()-2)/2; i++) {
		input[i] = s[2*i+4] - '0';
	}

	// Debug lines
	std::cout << "Option: " << option << std::endl;
	std::cout << "# of frames: " << num_frames << std::endl;
	std::cout << "# of inputs: " << (s.length()-2)/2 << std::endl;

	frame_register fr;
	fr.curr_register = 0;
	fr.num_frames = num_frames;
	fr.frames = new int[num_frames];

	if (option == 'F') {
		fifo(fr, input, input_length);
	} else if (option == 'O') {
		opt(fr, input, input_length);
	} else {
		std::cout << "Error: Invalid input" << std::endl;
	}

	return 0;
}