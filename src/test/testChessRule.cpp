#ifdef _DEBUG1
module;
#include <catch2/catch_test_macros.hpp>

module chess.ChessRule;
import chess.SDLChessGameController;
import app.SDLAppViewFrame;
import foundation.Matrix;
import foundation.SharedPointer;



TEST_CASE("ChessRule::pickKing", "[vector]") {
	using namespace mbg;
	auto viewframe = SDLAppViewFrame::createPointer(60);
	viewframe->initialize();
	viewframe->hide();

	auto gameCtrler = std::static_pointer_cast<SDLChessGameController>(
		viewframe->getSceneStack()->getFront()
	);
	auto rule = std::static_pointer_cast<ChessRule>(gameCtrler->getGameModel()->getRule());
	SquareGroups groupsOfSquaresToMove;


	{
		INFO("pick King at (0, 4)");
		groupsOfSquaresToMove = rule->pickKing(0, 4);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (0, 7)");
		groupsOfSquaresToMove = rule->pickKing(0, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (0, 0)");
		groupsOfSquaresToMove = rule->pickKing(0, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}
	
	{
		INFO("pick King at (1, 7)");
		groupsOfSquaresToMove = rule->pickKing(1, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 2);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (2, 7)");
		groupsOfSquaresToMove = rule->pickKing(2, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 3);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (1, 0)");
		groupsOfSquaresToMove = rule->pickKing(1, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 2);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (2, 0)");
		groupsOfSquaresToMove = rule->pickKing(2, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 3);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (5, 0)");
		groupsOfSquaresToMove = rule->pickKing(5, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 3);
		CHECK(groupsOfSquaresToMove[1].size() == 2);
	}

	{
		INFO("pick King at (5, 7)");
		groupsOfSquaresToMove = rule->pickKing(5, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 3);
		CHECK(groupsOfSquaresToMove[1].size() == 2);
	}

	{
		INFO("pick King at (6, 0)");
		groupsOfSquaresToMove = rule->pickKing(6, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 2);
		CHECK(groupsOfSquaresToMove[1].size() == 3);
	}

	{
		INFO("pick King at (6, 7)");
		groupsOfSquaresToMove = rule->pickKing(6, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 2);
		CHECK(groupsOfSquaresToMove[1].size() == 3);
	}

	{
		INFO("pick King at (4, 4)");
		groupsOfSquaresToMove = rule->pickKing(4, 4);
		CHECK(groupsOfSquaresToMove[0].size() == 8);
		CHECK(groupsOfSquaresToMove[1].size() == 0);
	}

	{
		INFO("pick King at (5, 5)");
		groupsOfSquaresToMove = rule->pickKing(5, 5);
		CHECK(groupsOfSquaresToMove[0].size() == 5);
		CHECK(groupsOfSquaresToMove[1].size() == 3);
	}

	{
		INFO("pick King at (7, 4)");
		groupsOfSquaresToMove = rule->pickKing(7, 4);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 5);
	}

	{
		INFO("pick King at (7, 7)");
		groupsOfSquaresToMove = rule->pickKing(7, 7);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 3);
	}

	{
		INFO("pick King at (6, 5)");
		groupsOfSquaresToMove = rule->pickKing(6, 5);
		CHECK(groupsOfSquaresToMove[0].size() == 3);
		CHECK(groupsOfSquaresToMove[1].size() == 5);
	}

	{
		INFO("pick King at (7, 0)");
		groupsOfSquaresToMove = rule->pickKing(7, 0);
		CHECK(groupsOfSquaresToMove[0].size() == 0);
		CHECK(groupsOfSquaresToMove[1].size() == 3);
	}
	
}

#endif // DEBUG
