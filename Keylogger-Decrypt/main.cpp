#include <iostream>
#include <fstream>
#include <string>
#include "Base64.h"

int main(int argc, char *argv[]) {

	if (argc != 3)
		return std::cout << "Program needs TWO arguments, input and output!" << std::endl, 2;

	std::string in(argv[1]), out(argv[2]);
	std::ifstream fi(in);
	if(!fi)
		return std::cout << "Cannot read input file '" << in << "'" << std::endl, 3;

	std::string data;
	fi >> data;
	if(!fi)
		return std::cout << "Input file '" << in << "' corrupted!" << std::endl, 4;

	data = Base64::DecryptB64(data);

	std::ofstream fo(out);
	if(!fo)
		return std::cout << "Cannot write output file '" << out << "'" << std::endl, 5;

	fo << data;
	std::cout << "Decoding was successful" << std::endl;

	return 0;
}

