#include "gltf.h"
#include "json.hpp"
#include <iostream>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
using nlohmann::json;
void 
Loader::load_file(const char* path){
	FILE* file = fopen(path,"r");
	if(file == NULL){
		throw std::runtime_error("failed to load gltf file");

	}
	std::ifstream file_opened(path);
	if(!file_opened){
		throw std::runtime_error("failed to load gltf file from fstream");
	}
	std::string str((std::istreambuf_iterator<char>(file_opened)),std::istreambuf_iterator<char>());
	//std::cout << str << std::endl;

	std::vector<unsigned char> out;
	file_opened.seekg(0,file_opened.end);
	size_t file_size = static_cast<size_t>(file_opened.tellg());
	file_opened.seekg(0, file_opened.end);

	//read binary
	std::cout << "file size " <<file_size << std::endl;
	out.resize(file_size);
	file_opened.read(reinterpret_cast<char*> (&out.at(0)),
	static_cast<std::streamsize>(file_size));
	file_opened.close();

	for(auto c : out){
		//std::cout << c << std::endl;
	}
	


	loader::gltf file_struct;
	load_from_string(file_struct,str.c_str(), 
		static_cast<unsigned int>(out.size())
		);

}

void Loader::load_from_string(loader::gltf gltf, const char* str, unsigned int length){
	json parser;

	parser = json::parse(str, str + length);

	if (!parser.is_object()) {
		throw "error was parsing file";
	}

	
}