#ifdef _DEBUG1
module;
#include <catch2/catch_test_macros.hpp>

module chess.ChessRule;
import chess.SDLChessGameController;
import app.SDLAppViewFrame;
import foundation.Matrix;
import foundation.SharedPointer;
import foundation.String;
import chess.SDLChessGameTransceiver;


TEST_CASE("SDLChessGameController::submitMove", "[vector]") {
	using namespace mbg;
	auto viewframe = SDLAppViewFrame::createPointer(60);
	viewframe->initialize();
	viewframe->hide();

	auto gameCtrler = std::static_pointer_cast<SDLChessGameController>(
		viewframe->getSceneStack()->getFront()
	);
	SDLChessGameTransceiver::Pointer eTransceiver = SDLChessGameTransceiver::createPointer();
	gameCtrler->setEnemyTransceiver(eTransceiver);

	gameCtrler->submitMove(0, 1, 1, 3, 1);
	String str = eTransceiver->popData();

	CHECK(601 == eTransceiver->parseData<Integer>(str));
}

#endif // DEBUG
