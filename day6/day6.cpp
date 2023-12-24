import std;

std::int64_t compute_times(std::int64_t time, std::int64_t distance)
{
	auto det = std::sqrt(std::pow(time, 2) - 4 * (distance + 1));
	auto x1 = std::ceil(0.5 * (time - det));
	auto x2 = std::floor(0.5 * (time + det));
	return static_cast<std::int64_t>(x2 - x1 + 1);
}

int main()
{
	std::ifstream input("input.txt");
	std::string line;
	std::getline(input, line);
	auto not_digit = [](char c) { return !std::isdigit(c); };
	auto reduced_line = line | std::views::drop_while(not_digit);
	std::ispanstream iss1(reduced_line);
	std::vector<std::int64_t> times;
	std::ranges::copy(std::views::istream<std::int64_t>(iss1), std::back_inserter(times));
	std::int64_t single_time = 0;
	for (auto c : reduced_line)
	{
		if (std::isdigit(c))
		{
			single_time *= 10;
			single_time += c - '0';
		}
	}

	std::getline(input, line);
	reduced_line = line | std::views::drop_while(not_digit);
	std::ispanstream iss2(reduced_line);
	std::vector<std::int64_t> distances;
	std::ranges::copy(std::views::istream<std::int64_t>(iss2), std::back_inserter(distances));
	std::int64_t single_distance = 0;
	for (auto c : reduced_line)
	{
		if (std::isdigit(c))
		{
			single_distance *= 10;
			single_distance += c - '0';
		}
	}

	auto ans = std::ranges::fold_left(std::views::zip(times, distances), 1,
									  [](std::int64_t acc, const auto& zipped) { return acc * compute_times(std::get<0>(zipped), std::get<1>(zipped)); });
	std::cout << std::format("The answer to part 1 is: {}\n", ans);

	std::cout << std::format("The answer to part 2 is: {}\n", compute_times(single_time, single_distance));
}