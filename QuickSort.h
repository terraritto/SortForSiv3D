#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void QuickSortSample()
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
		int n = dataList.size();

		auto quick = [&](auto quick, int left, int right) -> void
		{
			double pivot = dataList[(left+right)/2].weight;
			int l = left, r = right;

			// pivotの左に小さい値、右に大きい値となるように整列
			while (true)
			{
				// pivotより小さければ右にずらしていく
				while (dataList[left].weight < pivot)
				{
					left++;
				}

				// pivotより大きければ左にずらしていく
				while (pivot < dataList[right].weight)
				{
					right--;
				}

				if (right <= left) { break; }

				// 値が同じ位置でなければ入れ替え
				std::swap(dataList[right], dataList[left]);
				result.push_back(dataList);
			}

			// 元の位置に
			int p = left;
			left = l; right = r;

			if (left < p) { quick(quick, left, p - 1); }
			if (p < right) { quick(quick, p + 1, right); }
		};

		quick(quick, 0, n - 1);
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
