import std;

struct Gear
{
	std::int64_t row = -1;
	std::int64_t col_start = -1;
	std::int64_t col_end = -1;
	std::int64_t val = -1;
};

struct Star
{
	std::int64_t row = -1;
	std::int64_t col = -1;
};

int main()
{
	std::ifstream input("input.txt");
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(input, line))
	{
		lines.push_back(std::move(line));
	}

	auto is_special = [&lines](std::int64_t row, std::int64_t col)
	{
		if (row < 0 || col < 0)
		{
			return false;
		}
		if (row >= lines.size() || col >= lines.at(row).size())
		{
			return false;
		}
		auto c = lines.at(row).at(col);
		return !std::isdigit(c) && c != '.';
	};

	auto is_gear_ratio = [&lines](std::int64_t row, std::int64_t col)
	{
		if (row < 0 || col < 0)
		{
			return false;
		}
		if (row >= lines.size() || col >= lines.at(row).size())
		{
			return false;
		}
		return lines.at(row).at(col) == '*';
	};

	std::int64_t sum = 0;
	std::vector<Gear> gears;
	std::vector<Star> stars;
	for (std::int64_t row = 0; row < lines.size(); ++row)
	{
		std::int64_t number = 0;
		bool valid_number = false;
		bool gear_ratio_number = false;
		Gear gear;
		for (std::int64_t col = 0; col < lines[row].size(); ++col)
		{
			bool can_be_valid = is_special(row, col) ||
				is_special(row - 1, col) || 
				is_special(row + 1, col) ||
				is_special(row + 1, col - 1) ||
				is_special(row + 1, col + 1) ||
				is_special(row - 1, col - 1) ||
				is_special(row - 1, col + 1) ||
				is_special(row, col - 1) ||
				is_special(row, col + 1);

			bool can_be_gear_ratio = is_gear_ratio(row, col) ||
				is_gear_ratio(row - 1, col) ||
				is_gear_ratio(row + 1, col) ||
				is_gear_ratio(row + 1, col - 1) ||
				is_gear_ratio(row + 1, col + 1) ||
				is_gear_ratio(row - 1, col - 1) ||
				is_gear_ratio(row - 1, col + 1) ||
				is_gear_ratio(row, col - 1) ||
				is_gear_ratio(row, col + 1);
			
			auto c = lines.at(row).at(col);
			if (std::isdigit(c))
			{
				valid_number = valid_number || can_be_valid;
				gear_ratio_number = gear_ratio_number || can_be_gear_ratio;
				if (number == 0)
				{
					gear.col_start = col;
				}
				number *= 10;
				number += c - '0';
			}
			else if (number != 0)
			{
				if (valid_number)
				{
					sum += number;
				}
				if (gear_ratio_number)
				{
					gear.col_end = col - 1;
					gear.row = row;
					gear.val = number;
					gears.push_back(gear);
				}
				number = 0;
				valid_number = false;
				gear_ratio_number = false;
				gear = Gear{};
			}
			if (c == '*')
			{
				stars.emplace_back(row, col);
			}
		}
		if (number != 0)
		{
			if (valid_number)
			{
				sum += number;
			}
			if (gear_ratio_number)
			{
				gear.col_end = lines[row].size() - 1;
				gear.row = row;
				gear.val = number;
				gears.push_back(gear);
			}
		}
	}
	std::cout << std::format("The answer to part 1 is: {}\n", sum);
	std::int64_t gear_ratios = 0;
	for (const auto& star : stars)
	{
		auto is_diag = [&star](const Gear& gear)
		{
			if (star.row == gear.row)
			{
				return ((star.col == (gear.col_start - 1)) ||
					    (star.col == (gear.col_end + 1)));
			}
			if (star.row == (gear.row - 1) || star.row == (gear.row + 1))
			{
				return ((star.col >= (gear.col_start - 1)) &&
						(star.col <= (gear.col_end + 1)));
			}
			return false;
		};
		auto res = std::views::filter(gears, is_diag) | std::ranges::to<std::vector>();
		if (res.size() == 2)
		{
			gear_ratios += (res.front().val * res.back().val);
		}
	}
	std::cout << std::format("The answer to part 2 is: {}\n", gear_ratios);
}