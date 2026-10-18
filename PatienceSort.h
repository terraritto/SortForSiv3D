#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void PatienceSortSample()
{
	struct Data
	{
		double weight;
		ColorF color;

		void draw(Vec2 pos) const
		{
			pos.y = pos.y + (1.0 - weight) * RECT_SIZE_Y;
			RectF{ pos, Vec2{RECT_SIZE_X, weight * RECT_SIZE_Y} }.draw(color);
		}
	};

	Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });

	constexpr int DataNum = 130;
	constexpr Vec2 StartPos = Vec2{ 50.0, 50.0 };
	Array<Data> dataList;

	for (auto index : step(DataNum))
	{
		dataList.push_back(Data{ Random(0.0, 1.0), RandomColorF() });
	}

	Array<Array<Data>> result;
	result.push_back(dataList);

	auto Sort = [&]()
	{

		// 積み重ね保存用
		Array<Array<double>> piles;

		// まずは積み込み
		for (auto& data : dataList)
		{
			// まずないなら詰め込み
			if (piles.size() == 0)
			{
				piles.push_back({ data.weight });
				continue;
			}

			// すでにある場合
			bool isStack = false;
			auto pileValue = data.weight;
			for (auto& pile : piles)
			{
				// 小さいなら詰め込み
				if (pileValue <= pile.back())
				{
					isStack = true;
					pile.push_back(pileValue);
					break;
				}
			}

			// 詰め込みをしてないなら全ての一番上よりでかいので、横流し
			if (!isStack)
			{
				piles.push_back({ pileValue });
			}
		}

		// 積み込みデータを利用してソートしていく
		Array<Data> sortedList;
		while (true)
		{
			bool isSorted = true;
			double pileValue = std::numeric_limits<double>::infinity();
			int index = 0, count = 0;
			for (auto& pile : piles)
			{
				count++;
				if (pile.size() == 0) { continue; }
				isSorted = false;

				// 最後を比較していく
				double next = pile.back();
				bool checkPile = next < pileValue;
				if (checkPile)
				{
					// 条件を満たすならindex位置とその値を保持
					pileValue = next;
					index = count - 1;
				}
			}

			if (isSorted)
			{
				break; // ソートが終わってるなら終了
			}
			else
			{
				// 最小値を入れて、pileからは消す
				sortedList.push_back({pileValue, RandomColorF()});
				piles[index].pop_back();
				result.push_back(sortedList); // 記録
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
