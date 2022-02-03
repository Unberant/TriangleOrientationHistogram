#ifndef TEST_ENVISIONTEC_1_H
#define TEST_ENVISIONTEC_1_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <map>
#include <cmath>

class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1> >;

	std::chrono::time_point<clock_t> m_beg;

public:
	Timer() : m_beg(clock_t::now())
	{
	}

	void reset()
	{
		m_beg = clock_t::now();
	}

	double elapsed() const
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

class file_3D
{
public:
	explicit file_3D(std::string& NAME) : file_name(NAME), delta(1.8) {}
	explicit file_3D(std::string& NAME, double SCALE) : file_name(NAME), delta(SCALE) {}
	~file_3D() {}

	void output_data() const;

	void fill_up_data();

private:
	 std::vector<double> get_data(std::ifstream& file) const;

	void data_stl(std::ifstream& file, std::vector<double>& areas, std::vector<double>& angels) const;
	void data_obj(std::ifstream& file, std::vector<double>& areas, std::vector<double>& angels) const;

	std::vector<double> get_areas(const std::vector<double>& coordinates) const;
	double get_angle(const std::vector<double>& coordinates) const;

	std::vector<double> get_coordinates(const std::string& word, const std::string& line) const;
	double side_length(double x1, double y1, double z1, double x2, double y2, double z2) const;

	std::vector<double> merge_areas_unique(const std::vector<double>& areas, const std::vector<double>& angles) const;

	std::vector<double> merge_data_into_groups(const std::vector<double>& areas, const std::vector<double>& angles) const;

	std::string get_extension() const;
	int format_work(std::string extension) const;

private:
	std::vector<double> file_data;
	std::string file_name;
	Timer seconds;
	const double delta;
};

#endif 



