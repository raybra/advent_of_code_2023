import std;

std::int64_t to_int(std::string_view str)
{
	std::int64_t val{};
	std::from_chars(str.data(), str.data() + str.size(), val);
	return val;
}

struct Range
{
	std::int64_t source;
	std::int64_t dest;
	std::int64_t span;
};

std::istream& operator>>(std::istream& is, Range& range)
{
	is >> range.dest >> range.source >> range.span;
	return is;
}

int main()
{
	std::ifstream input("input.txt");
	std::vector<std::int64_t> seeds;
	std::vector<Range> seed_to_soil;
	std::vector<Range> soil_to_fertilizer;
	std::vector<Range> fertilizer_to_water;
	std::vector<Range> water_to_light;
	std::vector<Range> light_to_temperature;
	std::vector<Range> temperature_to_humidity;
	std::vector<Range> humidity_to_location;
	std::vector<Range>* map_to_update = nullptr;
	std::string line;
	std::getline(input, line);
	// first line is seed line
	std::ranges::copy(line | std::views::drop(7) | std::views::split(' ') | std::views::transform([](auto&& range) { return to_int(std::string_view{ range }); }), std::back_inserter(seeds));

	// Compute part 2 seeds
	std::vector<Range> seeds_part_2;
	for (auto i = 0; i < seeds.size(); i += 2)
	{
		seeds_part_2.emplace_back(seeds.at(i), seeds.at(i), seeds.at(i + 1));
	}
	while (std::getline(input, line))
	{
		if (line.empty())
		{
			continue;
		}

		if (line.contains("seed-to-soil"))
		{
			map_to_update = &seed_to_soil;
			continue;
		}
		else if (line.contains("soil-to-fertilizer"))
		{
			map_to_update = &soil_to_fertilizer;
			continue;
		}
		else if (line.contains("fertilizer-to-water"))
		{
			map_to_update = &fertilizer_to_water;
			continue;
		}
		else if (line.contains("water-to-light"))
		{
			map_to_update = &water_to_light;
			continue;
		}
		else if (line.contains("light-to-temperature"))
		{
			map_to_update = &light_to_temperature;
			continue;
		}
		else if (line.contains("temperature-to-humidity"))
		{
			map_to_update = &temperature_to_humidity;
			continue;
		}
		else if (line.contains("humidity-to-location"))
		{
			map_to_update = &humidity_to_location;
			continue;
		}

		std::istringstream iss(line);
		Range r;
		iss >> r;
		map_to_update->push_back(r);
	}
	std::int64_t smallest_loc = std::numeric_limits<std::int64_t>::max();
	for (auto seed : seeds)
	{
		auto get_map_value = [](std::int64_t value, const std::vector<Range>& map)
		{
			for (const auto& range : map)
			{
				if (value >= range.source && (value < range.source + range.span))
				{
					return range.dest + value - range.source;
				}
			}
			return value;
		};
		auto temp = get_map_value(seed, seed_to_soil);
		temp = get_map_value(temp, soil_to_fertilizer);
		temp = get_map_value(temp, fertilizer_to_water);
		temp = get_map_value(temp, water_to_light);
		temp = get_map_value(temp, light_to_temperature);
		temp = get_map_value(temp, temperature_to_humidity);
		temp = get_map_value(temp, humidity_to_location);
		smallest_loc = std::min(smallest_loc, temp);
	}
	std::cout << std::format("The answer to part 1 is: {}\n", smallest_loc);
	smallest_loc = std::numeric_limits<std::int64_t>::max();
	for (auto seed_range : seeds_part_2)
	{
		auto get_map_value = [](std::int64_t value, const std::vector<Range>& map)
		{
			for (const auto& range : map)
			{
				if (value >= range.source && (value < range.source + range.span))
				{
					return range.dest + value - range.source;
				}
			}
			return value;
		};
		for (auto seed : std::views::iota(seed_range.source, seed_range.source + seed_range.span))
		{
			auto temp = get_map_value(seed, seed_to_soil);
			temp = get_map_value(temp, soil_to_fertilizer);
			temp = get_map_value(temp, fertilizer_to_water);
			temp = get_map_value(temp, water_to_light);
			temp = get_map_value(temp, light_to_temperature);
			temp = get_map_value(temp, temperature_to_humidity);
			temp = get_map_value(temp, humidity_to_location);
			smallest_loc = std::min(smallest_loc, temp);
		}
	}
	std::cout << std::format("The answer to part 2 is: {}\n", smallest_loc);
}