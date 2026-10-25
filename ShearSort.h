#pragma once
#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void ShearSortSample()
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
		// 配列の長さ
		const int length = dataList.size();

		int row = Sqrt(length);
		int col = length / row;

		// 行数*列数で並べた時に余る個数
		int remainder = length - row * col;
		if (remainder != 0) {
			// 余りがあり行数が3以上の奇数の場合、余りが偶数行目に並ぶので
			// 一行減らして余りが奇数行目に並ぶように行数/列数を変更する
			if (IsOdd(row) && row >= 3) {
				row--;
				col = length / row;
				remainder = length - row * col;
			}
		}
		// 2を底とする行数の対数
		// ただし行数が2の累乗ちょうどの場合には、さらに+1する
		int iteration = Ceil(Log2(Sqrt(col * row)) + 1);

		// 各部分でのソート
		auto oeTranSort = [&](int start, int end, int step, bool isInc) {
			// odd-evenソートで対応
			bool changed;
			do {
				changed = false;
				for (int i = start; i + step <= end; i += step * 2)
				{
					// 降順昇順を考慮しつつswap
					if ((isInc && (dataList[i].weight > dataList[i + step].weight))
						|| (!isInc && (dataList[i].weight < dataList[i + step].weight)))
					{
						std::swap(dataList[i], dataList[i + step]);
						changed = true;
					}
				}

				for (int i = start + step; i + step <= end; i += step * 2)
				{
					// 降順昇順を考慮しつつswap
					if ((isInc && (dataList[i].weight > dataList[i + step].weight))
						|| (!isInc && (dataList[i].weight < dataList[i + step].weight)))
					{
						std::swap(dataList[i], dataList[i + step]);
						changed = true;
					}
				}
			} while (changed);
		};

		// ソートする範囲の上限
		int max;
		for (int i = 0; i < iteration; i++) {
			// 行ごとに、奇数行目は左側が小さく、
			// 偶数行目は右側が小さくなるようにソートする
			for (int j = 0; j < row + (remainder > 0 ? 1 : 0); j++)
			{
				max = (j + 1) * col - 1; // 末尾

				if (max >= length)
				{
					// あまりの行は最終位置
					max = length - 1;
				}

				// 奇数偶数で昇順降順変えつつソート
				oeTranSort(j * col, max, 1, IsEven(j));
				result.push_back(dataList);

			}

			// 列ごとに、上が小さくなるようにソートする
			for (int k = 0; k < col; k++)
			{
				// 余りのある列は、最初に求めた行数よりもデータが一つ多い
				oeTranSort(k, (row - (k < remainder ? 0 : 1)) * col + k,
						col, true);
				result.push_back(dataList);
			}
		}

		// 規定回数分、行のソートを繰り返す
		// 行ごとに、左側が小さくなるようにソートする。 
		for (int j = 0; j < row + (remainder > 0 ? 1 : 0); j++) {
			max = (j + 1) * col - 1;
			if (max >= length) {
				max = length - 1;
			}

			// ここからは普通に行だけなので、単純にソートするだけ
			oeTranSort(j * col, max, 1, true);
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
