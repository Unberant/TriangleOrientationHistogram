#include "_stl.h"
#define PI 3.14159265 

void file_3D::fill_up_data()
{
	std::cout << "Work with " << file_name << std::endl;

	std::ifstream in_file(file_name);
	file_data = get_data(in_file);
}

void file_3D::output_data() const
{
	std::ofstream file("data.txt");
	std::cout << "\nStart out data, time:  " << seconds.elapsed() << '\n';
	for (size_t i = 0; i < file_data.size() - 1; i += 2)
	{
		file << file_data.at(i) << ";" << file_data.at(i + 1) << std::endl;
	}
	file.close();
	std::cout << "  >end out data, time: " << seconds.elapsed() << '\n';
}

std::vector<double> file_3D::get_data(std::ifstream& file) const
{
	std::cout << "\nStart getting data, time:  " << seconds.elapsed() << '\n';
	std::vector<double> areas;
	std::vector<double> angels;
	if (file.is_open()) {
		switch (format_work(get_extension()))
		{
		case 1: {
			data_stl(file, areas, angels);
			break;
		}
		case 2: {
			data_obj(file, areas, angels);
			break;
		}
		case 3: {
			break;
		}
		case -1: {
			file.close();
			std::cout << "\nDon't know how work with such file: ." << get_extension();
			return { 0,0 };
		}
		}
		file.close();
		std::cout << "  >end getting data, time: " << seconds.elapsed() << '\n';
		return merge_data_into_groups(areas, angels);
	}
	else {
		std::cout << "File doesn't open.";
		return { 0,0 };
	}
}

void file_3D::data_stl(std::ifstream& file, std::vector<double>& areas, std::vector<double>& angels) const {
	std::string line;
	std::vector<double> coordinates;
	std::string word_search_1 = "normal";
	std::string word_search_2 = "vertex";
	while (std::getline(file, line))
	{
		if (line.find(word_search_1) != std::string::npos)
			angels.emplace_back(get_angle(get_coordinates(word_search_1, line)));

		if (line.find(word_search_2) != std::string::npos)
		{
			for (auto coordinate : get_coordinates(word_search_2, line))
				coordinates.emplace_back(coordinate);
		}
	}
	areas = get_areas(coordinates);
}

void file_3D::data_obj(std::ifstream& file, std::vector<double>& areas, std::vector<double>& angels) const
{
	areas = { 1,1 };
	angels = { 90,-90 };
}

std::vector<double> file_3D::get_areas(const std::vector<double>& coordinates) const
{
	std::vector<double> areas(coordinates.size() / 9);
	int j = 0;
	for (size_t i = 0; i < coordinates.size(); i += 9)
	{
		double a, b, c, P;
		a = side_length(coordinates.at(i), coordinates.at(i + 1), coordinates.at(i + 2),
			coordinates.at(i + 3), coordinates.at(i + 4), coordinates.at(i + 5));
		b = side_length(coordinates.at(i), coordinates.at(i + 1), coordinates.at(i + 2),
			coordinates.at(i + 6), coordinates.at(i + 7), coordinates.at(i + 8));
		c = side_length(coordinates.at(i + 3), coordinates.at(i + 4), coordinates.at(i + 5),
			coordinates.at(i + 6), coordinates.at(i + 7), coordinates.at(i + 8));
		P = (a + b + c) / 2.0f;
		areas.at(j++) = sqrt(P * (P - a) * (P - b) * (P - c));
	}
	return  areas;
}

double file_3D::get_angle(const std::vector<double>& coordinates) const
{
	return atan(coordinates[2] / (sqrt(pow(coordinates[0], 2) + pow(coordinates[1], 2.0f)))) * 180.0 / PI;
}

std::vector<double> file_3D::get_coordinates(const std::string& word, const std::string& line) const
{
	int start_position = line.find(word) + word.size();
	int spaces = 1;
	if (line.find("  ", start_position) != std::string::npos) spaces = 2;
	int end_position = 1;
	std::vector <double> coordinates{ 0,0,0 };
	int k = 0;
	while (end_position != std::string::npos)
	{
		end_position = line.find(" ", start_position + spaces);
		coordinates.at(k++) = std::stod(line.substr(start_position + spaces, end_position - start_position));
		start_position = end_position;
		spaces = 1;
	}
	return coordinates;
}

double file_3D::side_length(double x1, double y1, double z1, double x2, double y2, double z2) const 
{
	return sqrt(pow(x1 - x2, 2.0f) + pow(y1 - y2, 2.0f) + pow(z1 - z2, 2.0f));
}

std::vector<double> file_3D::merge_areas_unique(const std::vector<double>& areas, const std::vector<double>& angles) const 
{
	std::cout << "\nStart grouping data by unique, time:  " << seconds.elapsed() << '\n';

	std::vector<double> list{ 0,0.0 };

	int position = -1;
	for (size_t i = 0; i < angles.size(); i++) {
		for (size_t j = 1; j < list.size(); j += 2)
		{
			if (list.at(j) == angles.at(i))
			{
				position = j;
				break;
			}
			else position = -1;
		}
		if (position == -1)
		{
			list.emplace_back(areas.at(i));
			list.emplace_back(angles.at(i));
		}
		else
			list.at(position - 1) += areas.at(i);

	}
	std::cout << "  >end grouping data by unique, time: " << seconds.elapsed() << '\n';
	return list;
}

std::vector<double> file_3D::merge_data_into_groups(const std::vector<double>& areas, const std::vector<double>& angles) const
{
	std::cout << "\nStart grouping data by " << delta << ", time:  " << seconds.elapsed() << '\n';

	std::vector<double> list;
	double angle = -100;
	while (angle <= 100)
	{
		list.emplace_back(angle);
		list.emplace_back(0);
		angle += delta;
	}
	double border = delta / 2.0f;
	for (size_t i = 0; i < angles.size(); i++) {
		size_t left = 0, right = list.size() - 2;
		size_t j = (left + right) / 2;
		if (j % 2 != 0) --j;
		while (left < right)
		{
			j = (left + right) / 2;
			if (j % 2 != 0) --j;
			if (list.at(j) - border <= angles.at(i) && list.at(j) + border > angles.at(i))
			{
				list.at(j + 1) += areas.at(i);
				break;
			}
			if (list.at(j) - border < angles.at(i))
				left = j;
			else
				right = j;
		}
	}
	std::cout << "  >end grouping data by " << delta << ", time: " << seconds.elapsed() << '\n';
	return list;
}

std::string file_3D::get_extension() const
	int dot_position = file_name.find(".") + 1;
	return file_name.substr(dot_position);
}

int file_3D::format_work(std::string extension) const
{
	std::map<std::string, int> extensionbase;
	extensionbase["stl"] = 1;
	extensionbase["obj"] = 2;
	extensionbase["dae"] = 3;

	std::map <std::string, int> ::iterator index;
	index = extensionbase.find(extension);
	if (index == extensionbase.end())
		return -1;
	return index->second;
}

