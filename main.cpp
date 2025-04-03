//Jackson Rudnick

//Includes
#include <iostream>
#include <string>
#include <fstream>

// Custom Frame Register
struct frame_register {
	// Vars
	int curr_register;
	int num_frames;
	int num_page_faults = 0;
	int* frames;

	/*
	@brief Cycle through frames, for each frame, check distance between that frame and the next time the number in that frame would be put in, furthest gets replaced 
	@param in 		-- current reference list or all inputs
	@param len 		-- length of reference list
	@param depth	-- current index of reference list
	*/
	void update_current_register(int* in, int len, int depth) {
		// Vars
		int furthest_idx = 0;
		int old_reg = curr_register; // Keep old register in case of incomplete data

		// Cycle through frames in register
		for (int i = 0; i < num_frames; i++) {
			// Check if a frame has been initialized
			if (frames[i] == -1) {
				// Make current register the old register and return
				curr_register = old_reg;
				return;
			}

			// Cycle through input, starting at current search depth
			for (int j = depth+1; j < len; j++) {
				// if the ref input matches the frame
				if (in[j] == frames[i]) {
					// if the ref input if later on than the last furthest index
					if (j > furthest_idx) {
						// update the furthest idx and curr register
						furthest_idx = j;
						curr_register = i;
					}
					// break out for this frame since we found first occurance
					break;
				// we reach the end input
				} else if (j == len-1) {
					// number in curr register wasn't found so update furthest idx with length and current register
					furthest_idx = j;
					curr_register = i;
					return; // Premature optimization smh
				}
			}
		}
	};

	// Check if any of the frames contains the current ref input
	bool contains_input(int in) {
		// Cycle through frames
		for (int i = 0; i < num_frames; i++) {
			// One of the frames contains the input
			if (frames[i] == in) {
				return true;
			}
		}
		// Increment and post number of page faults that have occurred
		num_page_faults++;
		std::cout << "# of page faults :: " << num_page_faults << std::endl; 
		return false;
	};
};

// Open the file and return the first line within in
std::string get_string(std::string file_name) {
	// Vars
	std::string s;
	std::ifstream file(file_name);
	
	// If we successfully opened the fle
	if (file.is_open()) {
		//read the first line and close it
		std::string line;
		getline(file, line);
		s = line;
		file.close();
	} else {
		std::cout << "Error: Unable to open file" << std::endl;
	}

	return s;
}

// First in First out algorithm
void fifo(frame_register fr, int* in, int len) {
	// Loop through the input string
	for (int i = 0; i < len; i++) {
		// If the frame register doesn't contain the ref input
		if (!fr.contains_input(in[i])) {
			// Update the frame register based on current register
			fr.frames[fr.curr_register] = in[i];
			// Update current register
			fr.curr_register = (fr.curr_register+1)%fr.num_frames;
		}

		// Print out current frame register
		for (int j = 0; j < fr.num_frames; j++) {
			fr.frames[j] != -1 ? std::cout << fr.frames[j] << " " : std::cout << "- ";
		}
		std::cout << std::endl;
	}
}

// OPT Page Replacement algorithm
void opt(frame_register fr, int* in, int len) {
	// Cycle through the input string
	for (int i = 0; i < len; i++) {
		// If the frame register doesn't contain the ref input
		if (!fr.contains_input(in[i])) {
			// Update the current register
			fr.update_current_register(in, len, i);
			// Update the frame register based on current register
			fr.frames[fr.curr_register] = in[i];
			// Update the backup current register
			fr.curr_register = (fr.curr_register+1)%fr.num_frames;
		}

		// Print out current frame register
		for (int j = 0; j < fr.num_frames; j++) {
			fr.frames[j] != -1 ? std::cout << fr.frames[j] << " " : std::cout << "- ";
		}
		std::cout << std::endl;
	}
}

// Main Func
int main() {
	// Get file and input string
	std::cout << "Please enter the filename:\n";
	std::string file_name;
	std::cin >> file_name;
	
	std::string s = get_string(file_name);

	// Debug line (input string)
	std::cout << s << std::endl;

	// Option for algorithm
	char option = s[0];

	// Number of frames in the frame register
	int num_frames = s[2] - '0';

	// Overall ref input length
	int input_length = (s.length()-2)/2;
	int* input = new int[input_length];

	// Creating a proper input array
	for (int i = 0; i < (s.length()-2)/2; i++) {
		input[i] = s[2*i+4] - '0';
	}

	// Debug lines
	std::cout << "Option: " << option << std::endl;
	std::cout << "# of frames: " << num_frames << std::endl;
	std::cout << "# of inputs: " << (s.length()-2)/2 << std::endl;

	// Initializing the frame register
	frame_register fr;
	fr.curr_register = 0;
	fr.num_frames = num_frames;
	fr.frames = new int[num_frames];

	// Fill frame register with -1 
	for (int i = 0; i < num_frames; i++) {
		fr.frames[i] = -1;
	}

	// Determine algorithm choice based on option
	if (option == 'F') {
		fifo(fr, input, input_length);
	} else if (option == 'O') {
		opt(fr, input, input_length);
	} else {
		std::cout << "Error: Invalid input" << std::endl;
	}

	return 0;
}