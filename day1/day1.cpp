import std;

using namespace std::string_view_literals;

std::size_t part1(std::span<std::string> input);
std::size_t part2(std::span<std::string> input);

int main()
{
	std::ifstream input("input.txt");
	std::string line{};
	std::vector<std::string> input_lines;
	while (std::getline(input, line))
	{
		input_lines.push_back(std::move(line));
	}
	std::cout << std::format("The answer to part 1 is {}\nThe answer to part 2 is {}\n",
		                     part1(input_lines), part2(input_lines));
	return 0;
}

std::size_t part1(std::span<std::string> input)
{
	std::size_t sum = 0;
	for (const auto& line : input)
	{
		auto numbers = line | std::views::filter([](unsigned char c) { return std::isdigit(c) != 0; });
		auto tens = (*numbers.begin() - '0') * 10;
		auto ones = *(--numbers.end()) - '0';
		sum += tens + ones;
	}
	return sum;
}

std::size_t part2(std::span<std::string> input)
{
	constexpr std::array digit_names =
	{
		"one"sv,
		"two"sv,
		"three"sv,
		"four"sv,
		"five"sv,
		"six"sv,
		"seven"sv,
		"eight"sv,
		"nine"sv
	};
	std::size_t sum = 0;
	for (auto line : input)
	{
		std::size_t tens = 0;
		auto i = 0;
		while (tens == 0)
		{
			auto view = line | std::views::drop(i);
			if (std::isdigit(*view.begin()))
			{
				tens = (*view.begin() - '0') * 10;
			}
			else
			{
				for (auto j = 1; auto name : digit_names)
				{
					auto temp = std::string_view(view | std::views::take(name.size()));
					if (temp == name)
					{
						tens = j * 10;
						break;
					}
					++j;
				}
			}
			++i;
		}
		std::size_t ones = 0;
		i = 0;
		while (ones == 0)
		{
			auto view = line | std::views::reverse | std::views::drop(i);
			if (std::isdigit(*view.begin()))
			{
				ones = (*view.begin() - '0');
			}
			else
			{
				for (auto j = 1; auto name : digit_names)
				{
					auto temp_view = view | std::views::take(name.size());
					std::string temp(temp_view.begin(), temp_view.end());
					auto reversed_name_view = name | std::views::reverse;
					std::string reversed_name(reversed_name_view.begin(), reversed_name_view.end());
					if (temp == reversed_name)
					{
						ones = j;
						break;
					}
					++j;
				}
			}
			++i;
		}
		sum += tens + ones;
	}
	return sum;
}