#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void IntroSortSample()
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
		int maxDepth = floor(2 * log2(n));
		maxDepth = 2; // とりあえず入るようにしておく

		auto HeapSort = [&](int first, int last)
		{
			int nn = last;
			if (nn < first) { result.push_back(dataList); return; }

			auto adjustHeap = [&](auto adjustHeap, int i) -> void
			{
				int maxIndex = i;
				int ind = i;

				// 左のサブツリーが存在 + 左側のサブツリーの方が大きい -> 交換
				int li = i * 2 + first;
				if (li < nn && dataList[i + first].weight < dataList[li].weight)
				{
					maxIndex = li;
					ind = i * 2;
				}

				// 右のサブツリーが存在 + 右側のサブツリーの方が大きい -> 交換
				int ri = li + 1;
				if (ri < nn && dataList[maxIndex].weight < dataList[ri].weight)
				{
					maxIndex = ri;
					ind = i * 2 + 1;
				}

				if (ind != i)
				{
					// サブツリーが変更された場合は、それより下のノードも
					// 正しく最大値になってるかを並び変えてあげる
					std::swap(dataList[maxIndex], dataList[i + first]);
					adjustHeap(adjustHeap, ind);
				}
			};

			int medium = (last - first) / 2;
			for (int i = medium - 1; i >= 0; i--)
			{
				adjustHeap(adjustHeap, i);
			}

			result.push_back(dataList);

			// ヒープの位置を交換していく
			while (first < nn)
			{
				// 最大の位置と交換する(前回の処理で0の位置が最大の値のはず)
				std::swap(dataList[first], dataList[nn - 1]);
				result.push_back(dataList);

				// nの位置はもう弄らないものとする
				nn--;

				// 最大のヒープになるように調整
				adjustHeap(adjustHeap, 0);
				result.push_back(dataList);
			}
		};

		auto quick = [&](int& left, int& right)
		{
			double pivot = dataList[(left + right) / 2].weight;
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

			result.push_back(dataList);

			// 元の位置に
			int p = left;
			left = l; right = r;

			return p;
		};


		bool isSorted = false;
		auto introSort = [&](auto introSort, int first, int last, int depth) -> void
		{
			if (last <= first || isSorted) { return; }

			if (depth <= 0)
			{
				// depthが指定回数行われていたらHeapSort
				HeapSort(first, n);
				isSorted = true;
			}
			else
			{
				int l = first, r = last;

				// まだdepthに余裕があるならQuickSort
				int p = quick(l, r);

				// 再帰
				if (l < p) { introSort(introSort, l, p - 1, depth - 1); }
				if (p < r) { introSort(introSort, p + 1, r, depth - 1); }
			}
		};

		introSort(introSort, 0, n - 1, maxDepth);
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
