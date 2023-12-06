import std;

using namespace std::string_view_literals;

constexpr auto max_red = 12;
constexpr auto max_green = 13;
constexpr auto max_blue = 14;

int main()
{
	std::ifstream input("input.txt");
	std::string line;
	std::size_t game_id_sum = 0;
	std::size_t power_sum = 0;
	while (std::getline(input, line))
	{
		auto subsets = line | std::views::drop_while([](char c) { return c != ':'; }) | std::views::split(';');
		bool valid_game = true;
		int red = 0;
		int blue = 0;
		int green = 0;
		for (const auto& subset : subsets)
		{
			auto cube_sets = subset | std::views::split(',');
			bool valid_cube_set = true;
			for (const auto& cube_set : cube_sets)
			{
				auto num_view = cube_set | std::views::filter([](unsigned char c) { return std::isdigit(c); });
				std::string num_str(num_view.begin(), num_view.end());
				int num = 0;
				std::from_chars(num_str.data(), num_str.data() + num_str.size(), num);
				auto text_view = cube_set | std::views::filter([](unsigned char c) { return std::isalpha(c); });
				std::string text(text_view.begin(), text_view.end());
				if (text == "blue"sv)
				{
					if (num > max_blue)
					{
						valid_cube_set = false;
					}
					blue = std::max(num, blue);
				}
				else if (text == "green"sv)
				{
					if (num > max_green)
					{
						valid_cube_set = false;
					}
					green = std::max(num, green);
				}
				else if (text == "red"sv)
				{
					if (num > max_red)
					{
						valid_cube_set = false;
					}
					red = std::max(num, red);
				}
			}
			if (!valid_cube_set)
			{
				valid_game = false;
			}
		}
		if (valid_game)
		{
			auto num_view = line | std::views::drop(5);// std::views::take_while([](unsigned char c) { return std::isdigit(c); });
			std::string num_str(num_view.begin(), num_view.end());
			int game_id = 0;
			std::from_chars(num_str.data(), num_str.data() + num_str.size(), game_id);
			game_id_sum += game_id;
		}
		power_sum += (blue * red * green);
	}
	std::cout << "The sum is " << game_id_sum << "\n";
	std::cout << "The product sum is " << power_sum << "\n";
}