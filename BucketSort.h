#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void BucketSortSample()
{
	struct Data
	{
		int weight;
		ColorF color;

		void draw(Vec2 pos) const
		{
			const double offset = static_cast<double>(weight) / 100.0;
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
		dataList.push_back(Data{ Random(0, 99), RandomColorF() });
	}

	Array<Array<Data>> result;
	result.push_back(dataList);

	auto Sort = [&]()
	{
		// 今回は[0,99]を仮定
		const int range = 100;

		// バケツを作る
		Array<Array<int>> buckets; buckets.resize(range);

		for (const auto& value : dataList)
		{
			// バケツのindexに値を放り込む
			buckets[value.weight].push_back(value.weight);
		}

		int index = 0;
		for (const auto& bucket : buckets)
		{
			for (const auto& value : bucket)
			{
				// 入ってる要素を順に取り出すだけ
				dataList[index++].weight = value;
				result.push_back(dataList);
			}
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
