import std;

std::int64_t to_int(std::string_view str)
{
	std::int64_t val{};
	std::from_chars(str.data(), str.data() + str.size(), val);
	return val;
}

int main()
{
	std::ifstream input("input.txt");
	std::string line;
	std::int64_t sum = 0;
	std::unordered_map<std::size_t, std::size_t> scratchcards;
	std::size_t card_num = 1;
	while (std::getline(input, line))
	{
		scratchcards[card_num] += 1;
		auto nums_groups = line |
			std::views::drop_while([](char c) { return c != ':'; }) |
			std::views::drop(1) |
			std::views::split('|');
		auto itr = nums_groups.begin();
		auto winning_nums = *itr |
			std::views::split(' ') |
			std::views::transform([](auto&& range) { return to_int(std::string_view(range.begin(), range.end())); }) |
			std::views::filter([](std::int64_t val) { return val != 0; });
		++itr;
		auto drawn_numbers = *itr |
			std::views::split(' ') |
			std::views::transform([](auto&& range) { return to_int(std::string_view(range.begin(), range.end())); }) |
			std::views::filter([](std::int64_t val) { return val != 0; }) |
			std::ranges::to<std::vector>();
		auto count_if_func = [&winning_nums](std::int64_t val) { return std::ranges::contains(winning_nums, val); };
		auto num_winning = std::ranges::count_if(drawn_numbers, count_if_func);
		if (num_winning > 0)
		{
			sum += std::pow(2, num_winning - 1);
		}
		auto current_scratchcards = scratchcards[card_num];
		for (std::uint64_t i = 0; i < num_winning; ++i)
		{
			scratchcards[i + card_num + 1] += current_scratchcards;
		}
		++card_num;
	}
	std::cout << std::format("The answer to part 1 is: {}\n", sum);
	auto total_scratchcards = std::ranges::fold_left(scratchcards | std::views::values, 0, std::plus<std::int64_t>{});
	std::cout << std::format("The answer to part 2 is: {}\n", total_scratchcards);
}