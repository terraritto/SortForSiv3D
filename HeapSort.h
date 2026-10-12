#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void HeapSortSample()
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
		int n = dataList.size();
		if (n < 1) { result.push_back(dataList); return; }

		auto adjustHeap = [&](auto adjustHeap, int i) -> void
		{
			int maxIndex = i;

			// 左のサブツリーが存在 + 左側のサブツリーの方が大きい -> 交換
			if (i * 2 < n && dataList[maxIndex].weight < dataList[i * 2].weight)
			{
				maxIndex = i * 2;
			}

			// 右のサブツリーが存在 + 右側のサブツリーの方が大きい -> 交換
			if (i * 2 + 1 < n && dataList[maxIndex].weight < dataList[i * 2 + 1].weight)
			{
				maxIndex = i * 2 + 1;
			}

			if (maxIndex != i)
			{
				// サブツリーが変更された場合は、それより下のノードも
				// 正しく最大値になってるかを並び変えてあげる
				std::swap(dataList[maxIndex], dataList[i]);
				adjustHeap(adjustHeap, maxIndex);
			}
		};

		// 最大ヒープの形成(この時index:0は最大値)
		for (int i = n / 2 - 1; i >= 0; i--)
		{
			adjustHeap(adjustHeap, i);
		}

		result.push_back(dataList);

		// ヒープの位置を交換していく
		while (0 < n)
		{
			// 最大の位置と交換する(前回の処理で0の位置が最大の値のはず)
			std::swap(dataList[0], dataList[n - 1]);
			result.push_back(dataList);

			// nの位置はもう弄らないものとする
			n--;

			// 最大のヒープになるように調整
			adjustHeap(adjustHeap, 0);
			result.push_back(dataList);
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
