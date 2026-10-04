#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

void ShakerSortSample()
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
		int top = 0, bottom = dataList.size() - 1;

		while (true)
		{
			int last = -1;

			// 順方向
			last = top;

			for (int i = top; i < bottom; i++)
			{
				if (dataList[i+1].weight < dataList[i].weight)
				{
					std::swap(dataList[i].weight, dataList[i+1].weight);
					last = i;
				}
			}

			// 後方スキャンを狭める
			bottom = last;
			result.push_back(dataList);

			if (top == bottom) { break; }

			// 逆方向
			last = bottom;
			for (int i = bottom; top < i; i--)
			{
				if (dataList[i].weight < dataList[i - 1].weight)
				{
					std::swap(dataList[i].weight, dataList[i-1].weight);
					last = i;
				}
			}

			// 前方スキャンを狭める
			top = last;
			result.push_back(dataList);

			if (top == bottom) { break; }
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
