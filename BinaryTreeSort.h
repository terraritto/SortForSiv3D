#pragma once
#pragma once
#include <Siv3D.hpp>

#define RECT_SIZE_X 5
#define RECT_SIZE_Y 400

// 二分木ソート
void BinaryTreeSortSample()
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

	Array<double> result;

	// ソートするノード
	struct Node
	{
		double weight;
		std::shared_ptr<Node> left, right;
	};

	// ノードの生成
	auto MakeNewNode = [](double weight)
	{
		auto result = std::make_shared<Node>();
		result->weight = weight;
		return result;
	};

	// ソート順に取得していく
	auto Sorted = [&](auto Sorted, std::shared_ptr<Node> pNode, Array<double>& vList) -> void
	{
		if (pNode)
		{
			Sorted(Sorted, pNode->left, vList);
			vList.push_back(pNode->weight);
			Sorted(Sorted, pNode->right, vList);
		}
	};

	// 挿入をしていく
	auto Insert = [&](auto Insert, std::shared_ptr<Node> pNode, double weight) -> std::shared_ptr<Node>
	{
		// ノードがない場合は生成
		if (!pNode) { return MakeNewNode(weight); }

		if (weight < pNode->weight)
		{
			// 小さければ左に代入
			pNode->left = Insert(Insert, pNode->left, weight);
		}
		else
		{
			// 大きければ右へ代入
			pNode->right = Insert(Insert, pNode->right, weight);
		}

		return pNode;
	};

	auto Sort = [&]()
	{
		std::shared_ptr<Node> root;

		// 二分木を構築
		for (auto i : step(dataList.size()))
		{
			root = Insert(Insert, root, dataList[i].weight);
		}

		// ソート順に木構造から取り出し
		Sorted(Sorted, root, result);

		// resultからdataに入れて完了
		for (auto i : step(dataList.size()))
		{
			dataList[i].weight = result[i];
		}
	};

	// ソートを行う
	Sort();

	while (System::Update())
	{
		for (auto i : step(dataList.size()))
		{
			auto& d = dataList[i];

			// 描画
			d.draw(StartPos + Vec2::Right() * i * RECT_SIZE_X);
		}
	}
}

#undef RECT_SIZE_X
#undef RECT_SIZE_Y
