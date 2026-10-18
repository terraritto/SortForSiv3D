#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void StrandSortSample()
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
		Array<Data> sorted;
		while (true)
		{
			if (dataList.size() == 0) { break; }

			// まずサブツリーに分けていく
			Array<Data> subTree; subTree.push_back(dataList[0]);
			dataList = dataList.removed_at(0);

			int index = 0;
			while (true)
			{
				if (index == dataList.size()) { break; }

				if (subTree.back().weight <= dataList[index].weight)
				{
					// 基準より大きいなら追加
					subTree.push_back(dataList[index]);
					dataList = dataList.removed_at(index);
				}
				else
				{
					index++;
				}
			}

			// 最小のものを選択していくだけ
			Array<Data> temp;
			auto treeIterator = subTree.begin();
			auto targetIterator = sorted.begin();
			while (true)
			{
				bool isTreeFinished = treeIterator == subTree.end();
				bool isTargetFinished = targetIterator == sorted.end();

				if (isTreeFinished && isTargetFinished) { break; }

				if (!isTreeFinished && isTargetFinished)
				{
					temp.push_back(*treeIterator); treeIterator++; continue;

				}
				if (isTreeFinished && !isTargetFinished)
				{
					temp.push_back(*targetIterator); targetIterator++; continue;
				}

				auto& iter = (*treeIterator).weight < (*targetIterator).weight ?
					treeIterator : targetIterator;
				temp.push_back(*iter); iter++;
			}
			sorted = temp;

			// 結果を積み込んでおく
			result.push_back(sorted);
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
