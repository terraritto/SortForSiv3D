#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void LibrarySortSample()
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
		// データサイズ
		int n = dataList.size();

		// 必要なデータを初期化
		int librarySize = 1, indexPos = 1, targetLibrary = 0;
		Array<double> gaps; gaps.resize(n + 1);
		Array<bool> numbered; numbered.resize(n + 1, false);
		auto sortedData = dataList;

		Array<Array<double>> library; library.resize(2); // 本の配列は2にしておく
		for (auto i : step(2)) { library[i].resize(n); }

		// Libraryの初期状態を形成
		library[targetLibrary][0] = dataList[0].weight;

		// データサイズよりもindexが小さいなら続ける
		while (indexPos < n)
		{
			// 二分探索でデータの指定の位置のものより大きい値を見つける
			// 大きい値がなければ最後のindexの部分に挿入される
			int insert = std::distance
			(
				library[targetLibrary].begin(),
				std::lower_bound(library[targetLibrary].begin(), library[targetLibrary].begin() + librarySize, dataList[indexPos].weight)
			);

			if (numbered[insert])
			{
				// gapsにinsertできないため、整理を行う(これがソートに繋がる)
				int provSize = 0, nextTargetLib = targetLibrary == 1 ? 0 : 1; // 操作するLibを切り替える

				for (int i = 0; i <= n; i++)
				{
					if (numbered[i])
					{
						// gapsに記録されているものを入れる
						library[nextTargetLib][provSize] = gaps[i];
						provSize++;
						numbered[i] = false; // numberedをfalseにして、gapにまた入れられるようにする
					}

					if (i <= librarySize)
					{
						// もう一つのライブラリには既にソート済みのものがあるので、
						// それは必要分だけ移す
						library[nextTargetLib][provSize] = library[targetLibrary][i];
						provSize++; // 移したところにはデータは来てほしくないので、ずらす
					}
				}

				targetLibrary = nextTargetLib; // 整理した方のlibraryで処理する
				librarySize = provSize - 1; // provサイズは整理した本の数にする

				for (auto i : step(n))
				{
					sortedData[i].weight = library[targetLibrary][i];
				}
				result.push_back(sortedData);
			}
			else
			{
				// gapに値がない場合は追加しておく(ここにinsertが可能)
				numbered[insert] = true;
				gaps[insert] = dataList[indexPos].weight;
				indexPos++;
			}
		}

		int indexPositionOutput = 0;
		for (int i = 0; indexPositionOutput < n; i++)
		{
			// 整理の部分を最後にやってるだけ,while抜け段階では
			// 最後の整理だけ行われていないのでここで行う
			if (numbered[i] == true)
			{
				sortedData[indexPositionOutput].weight = gaps[i];
				indexPositionOutput++;
			}

			if (i < librarySize)
			{
				sortedData[indexPositionOutput].weight = library[targetLibrary][i];
				indexPositionOutput++;
			}
		}

		result.push_back(sortedData);
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
