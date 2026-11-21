#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void RadixSortSample()
{
	struct Data
	{
		int weight;
		ColorF color;

		void draw(Vec2 pos) const
		{
			const double offset = static_cast<double>(weight) / 999999999;
			pos.y = pos.y + (1.0 - offset) * RECT_SIZE_Y;
			RectF{ pos, Vec2{RECT_SIZE_X, offset * RECT_SIZE_Y} }.draw(color);
		}
	};

	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	constexpr int DataNum = 130;
	constexpr Vec2 StartPos = Vec2{ 50.0, 50.0 };
	Array<Data> dataList;

	for (auto index : step(DataNum))
	{
		dataList.push_back(Data{ Random(0, 999999999), RandomColorF() });
	}

	Array<Array<Data>> result;
	result.push_back(dataList);

	auto Sort = [&]()
	{
		int maxValue =
			(
				*std::max_element(dataList.begin(), dataList.end(),
					[](const Data& lhs, const Data& rhs) {return lhs.weight < rhs.weight; })
			).weight;

		// カウントソートで桁に関して並び替え
		auto countSort = [&](int exp)
		{
			Array<int> count{}; count.resize(10, 0);
			Array<Data> sorted{dataList.size()};

			// ある桁の数字を数える
			for (const auto& data : dataList)
			{
				count[(data.weight / exp) % 10]++;
			}

			// 桁の数からindex位置へ変換
			for (int i = 1; i < count.size(); i++)
			{
				count[i] += count[i-1];
			}

			// index位置に値を入れていく
			for (const auto& data : dataList.reverse())
			{
				auto digit = (data.weight / exp) % 10;
				sorted[count[digit] - 1] = data;
				count[digit]--; // indexをずらしておく
			}

			std::swap(sorted, dataList);
			result.push_back(dataList);
		};

		// 各桁に対してソートを実行
		for (int exp = 1; maxValue / exp > 0; exp *= 10)
		{
			countSort(exp);
		}
	};

	// ソートを行う
	Sort();

	int index = 0;

	while (System::Update())
	{
		if (0 < index && SimpleGUI::Button(U"Prev", Vec2{ Scene::Center().x - 350, 500 }))
		{
			index--;
		}

		if (SimpleGUI::Button(U"Start", Vec2{ Scene::Center().x - 350, 550 }))
		{
			index = 0;
		}

		if (SimpleGUI::Button(U"Last", Vec2{ Scene::Center().x + 200, 550 }))
		{
			index = result.size() - 1;
		}

		// 分割が必要なら実行
		if (index < result.size() - 1 && SimpleGUI::Button(U"Next", Vec2{ Scene::Center().x + 200, 500 }))
		{
			index++;
		}

		auto r = result[index];

		for (auto i : step(r.size()))
		{
			auto& d = r[i];

			// 描画
			d.draw(StartPos + Vec2::Right() * i * RECT_SIZE_X);
		}
	}
}

#undef RECT_SIZE_X
#undef RECT_SIZE_Y
