#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void MergeSortSample()
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

	auto Sort = [&]()
	{
		Array<Data> a = dataList, b = dataList;
		auto merge = [&](int left, int mid, int right)
		{
			int i = left, j = mid, k = 0;

			while (i < mid && j < right)
			{
				// 先頭から小さい順にbに放り込む
				if (a[i].weight <= a[j].weight)
				{
					b[k++].weight = a[i++].weight;
				}
				else
				{
					b[k++].weight = a[j++].weight;
				}
			}

			// 片方が出尽くしたら、もう片方は片っ端から入れるようにする
			if (i == mid)
			{
				while (j < right)
				{
					b[k++].weight = a[j++].weight;
				}
			}
			else
			{
				while (i < mid)
				{
					b[k++].weight = a[i++].weight;
				}
			}

			// kはマージするサイズになってるはず
			for (int l = 0; l < k; l++)
			{
				a[left + l].weight = b[l].weight;
			}

			result.push_back(a);
		};

		auto mergeSort = [&](auto mergeSort, int left, int right) -> void
		{
			if (left == right || left == right - 1) { return; }
			int mid = (left + right) / 2;
			mergeSort(mergeSort, left, mid);
			mergeSort(mergeSort, mid, right);
			merge(left, mid, right);
		};

		mergeSort(mergeSort, 0, dataList.size());
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
