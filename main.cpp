#include "_stl.h"

int main()
{
	std::string file_name = "file_name";// change for your name

	file_3D file1(file_name,1.85);
	file1.fill_up_data();
	file1.output_data();
}
