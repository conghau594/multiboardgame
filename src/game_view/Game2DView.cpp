

module game_view.Game2DView;

namespace mbg {
  /****************************************************************************/
  Game2DView::Game2DView(
    Avatar::Pointer avatar,
    EntityView::Pointer headBackgroundView,
    EntityView::Pointer headTileMapView,
    EntityView::Pointer headTileView,
    EntityView::Pointer headElementView
  ) : EntityView(nullptr, nullptr, avatar)
  {
    constexpr Integer INITIAL_NUMBER_OF_ANCHORS = OTHER + 2;
    for (Integer i = 0; i < INITIAL_NUMBER_OF_ANCHORS; ++i) {
      anchors_.emplace_back(EntityView::createPointer());
      anchor_indices_.emplace_back(i);
    }

    if (headBackgroundView != nullptr) {
      anchors_[BACKGROUND]->setNext(headBackgroundView);
      headBackgroundView->setPrevious(anchors_[BACKGROUND]);
    }

    if (headTileMapView != nullptr) {
      anchors_[TILE_MAP]->setNext(headTileMapView);
      headTileMapView->setPrevious(anchors_[TILE_MAP]);
    }

    if (headTileView != nullptr) {
      anchors_[TILE]->setNext(headTileView);
      headTileView->setPrevious(anchors_[TILE]);
    }

    if (headElementView != nullptr) {
      anchors_[ITEM]->setNext(headElementView);
      headElementView->setPrevious(anchors_[ITEM]);
    }

    linkEntityViewsToAnchors();
  }

  /****************************************************************************/
  Game2DView::Game2DView(
    Avatar::Pointer avatar,
    EntityView::Pointer headBackgroundView,
    EntityView::Pointer headTileMapView,
    EntityView::Pointer headTileView,
    EntityView::Pointer headElementView,
    EntityView::Pointer headUIButtonView,
    EntityView::Pointer headTextInputView,
    EntityView::Pointer headDecorationView1,
    EntityView::Pointer headOtherView
  ) : EntityView(nullptr, nullptr, avatar)
  {
    constexpr Integer INITIAL_NUMBER_OF_ANCHORS = OTHER + 2;
    for (Integer i = 0; i < INITIAL_NUMBER_OF_ANCHORS; ++i) {
      anchors_.emplace_back(EntityView::createPointer());
      anchor_indices_.emplace_back(i);
    }

    if (headBackgroundView != nullptr) {
      anchors_[BACKGROUND]->setNext(headBackgroundView);
      headBackgroundView->setPrevious(anchors_[BACKGROUND]);
    }

    if (headTileMapView != nullptr) {
      anchors_[TILE_MAP]->setNext(headTileMapView);
      headTileMapView->setPrevious(anchors_[TILE_MAP]);
    }

    if (headTileView != nullptr) {
      anchors_[TILE]->setNext(headTileView);
      headTileView->setPrevious(anchors_[TILE]);
    }

    if (headElementView != nullptr) {
      anchors_[ITEM]->setNext(headElementView);
      headElementView->setPrevious(anchors_[ITEM]);
    }

    if (headUIButtonView != nullptr) {
      anchors_[UI_BUTTON]->setNext(headUIButtonView);
      headUIButtonView->setPrevious(anchors_[UI_BUTTON]);
    }

    if (headTextInputView != nullptr) {
      anchors_[TEXT_INPUT]->setNext(headTextInputView);
      headTextInputView->setPrevious(anchors_[TEXT_INPUT]);
    }

    if (headDecorationView1 != nullptr) {
      anchors_[DECORATION_1]->setNext(headDecorationView1);
      headDecorationView1->setPrevious(anchors_[DECORATION_1]);
    }

    if (headOtherView != nullptr) {
      anchors_[OTHER]->setNext(headOtherView);
      headOtherView->setPrevious(anchors_[OTHER]);
    }

    linkEntityViewsToAnchors();
  }
    
  /****************************************************************************/
  Game2DView::Game2DView(
    Avatar::Pointer avatar,
    EntityView::Pointer headBackgroundView,
    EntityView::Pointer headTileMapView,
    EntityView::Pointer headTileView,
    EntityView::Pointer headElementView,
    EntityView::Pointer headUIButtonView,
    EntityView::Pointer headTextInputView,
    EntityView::Pointer headDecorationView1,
    EntityView::Pointer headDecorationView2,
    EntityView::Pointer headDecorationView3,
    EntityView::Pointer headOtherView
  ) : EntityView(nullptr, nullptr, avatar)
  {
    constexpr Integer INITIAL_NUMBER_OF_ANCHORS = OTHER + 2;
    for (Integer i = 0; i < INITIAL_NUMBER_OF_ANCHORS; ++i) {
      anchors_.emplace_back(EntityView::createPointer());
      anchor_indices_.emplace_back(i);
    }

    if (headBackgroundView != nullptr) {
      anchors_[BACKGROUND]->setNext(headBackgroundView);
      headBackgroundView->setPrevious(anchors_[BACKGROUND]);
    }

    if (headTileMapView != nullptr) {
      anchors_[TILE_MAP]->setNext(headTileMapView);
      headTileMapView->setPrevious(anchors_[TILE_MAP]);
    }

    if (headTileView != nullptr) {
      anchors_[TILE]->setNext(headTileView);
      headTileView->setPrevious(anchors_[TILE]);
    }

    if (headElementView != nullptr) {
      anchors_[ITEM]->setNext(headElementView);
      headElementView->setPrevious(anchors_[ITEM]);
    }

    if (headUIButtonView != nullptr) {
      anchors_[UI_BUTTON]->setNext(headUIButtonView);
      headUIButtonView->setPrevious(anchors_[UI_BUTTON]);
    }

    if (headTextInputView != nullptr) {
      anchors_[TEXT_INPUT]->setNext(headTextInputView);
      headTextInputView->setPrevious(anchors_[TEXT_INPUT]);
    }

    if (headDecorationView1 != nullptr) {
      anchors_[DECORATION_1]->setNext(headDecorationView1);
      headDecorationView1->setPrevious(anchors_[DECORATION_1]);
    }

    if (headDecorationView2 != nullptr) {
      anchors_[DECORATION_2]->setNext(headDecorationView2);
      headDecorationView2->setPrevious(anchors_[DECORATION_2]);
    }

    if (headDecorationView3 != nullptr) {
      anchors_[DECORATION_3]->setNext(headDecorationView3);
      headDecorationView3->setPrevious(anchors_[DECORATION_3]);
    }

    if (headOtherView != nullptr) {
      anchors_[OTHER]->setNext(headOtherView);
      headOtherView->setPrevious(anchors_[OTHER]);
    }

    linkEntityViewsToAnchors();
  }

  /****************************************************************************/
  void Game2DView::linkEntityViewsToAnchors() {
    for (Integer i = 0; i < getNumberOfAnchors(); ++i) {
      EntityView::Pointer loopView = anchors_[i];
      while (loopView->getNext() != nullptr) {
        loopView = loopView->getNext();
      }

      loopView->setNext(anchors_[i + 1]);
      anchors_[i + 1]->setPrevious(loopView);
    }

#ifdef _DEBUG
    printAnchorInfo();
#endif // _DEBUG
  }

  /***************************************************************************
   * Delegates for all entity views of this scene to render this scene on 
   * window.
   * \sa EntityView, Window
   ***************************************************************************/
  void Game2DView::render(Window::Pointer window) {
    EntityView::Pointer entityView = (*this)[0];
#ifdef _DEBUG
    Integer count = 0;
#endif
    while (entityView != anchors_.back()) {
#ifdef _DEBUG1
      std::cout << "\n" << ++count << ") entityView = " << entityView << ", use_count = " << entityView.use_count();
#endif
      entityView->draw(window);
      entityView = entityView->getNext();
    }
  }

  /****************************************************************************
   * Does nothing if `i == k` or max(i, k) is greater than the number of anchors or min(i, k)
   * is lower than 1 (that is we cannot swap any view with the background view)
   ****************************************************************************/
  void Game2DView::swapAnchors(EntityViewType i, EntityViewType k) {
    if (i >= getNumberOfAnchors() || k >= getNumberOfAnchors() || i < 1 || k < 1)
      return;

    Integer indexOfAnchorI = getIndexOfAnchor(i);
    Integer indexOfAnchorK = getIndexOfAnchor(k);

    if (indexOfAnchorI > indexOfAnchorK)
      std::swap(indexOfAnchorI, indexOfAnchorK);

    EntityView::Pointer anchorI = (*this)[indexOfAnchorI];
    EntityView::Pointer anchorK = (*this)[indexOfAnchorK];
    EntityView::Pointer entityPrecedingAnchorI = anchorI->getPrevious();
    
    EntityView::Pointer anchorIPlus1 = (*this)[indexOfAnchorI + 1];  ///< `anchorIPlus1 == anchorK` if `i+1 == k`
    EntityView::Pointer anchorKPlus1 = (*this)[indexOfAnchorK + 1];
    EntityView::Pointer lastEntityOfAnchorI = anchorIPlus1->getPrevious();

#ifdef _DEBUG1
    std::cout << "\nanchorI = " << anchorI;
    std::cout << "\nanchorK = " << anchorK;
    std::cout << "\nentityPrecedingAnchorI = " << entityPrecedingAnchorI;

    std::cout << "\nanchorIPlus1 = " << anchorIPlus1;
    std::cout << "\nanchorKPlus1 = " << anchorKPlus1;
    std::cout << "\nlastEntityOfAnchorI = " << lastEntityOfAnchorI;
    //std::cout << "\nlastEntityOfAnchorK = " << lastEntityOfAnchorK;
#endif

    if (indexOfAnchorI < indexOfAnchorI + 1) {
      //RULE 1: Use var before changing//
      //RULE 2: if must change before using, must create intermediary variable//

      //(1) (2) links `anchor[k+1]->previous` (aka last entity of `anchor[k]`) with `anchor[i+1]`
      anchorKPlus1->getPrevious()->setNext(anchorIPlus1);     //NOTE[1]: use anchor[k+1]->previous        (aka lastEntityOfAnchorK)  
      anchorIPlus1->setPrevious(anchorKPlus1->getPrevious()); //NOTE[2]: anchor[i+1]->previous is changed ( != lastEntityOfAnchorI)    (use at NOTE[3])


      //(3) (4) links `anchor[i+1]->previous` (aka last entity of `anchor[i]`) with `anchor[k+1]`
      lastEntityOfAnchorI->setNext(anchorKPlus1);             //NOTE[3]: use anchor[i+1]->previous        (aka lastEntityOfAnchorI)
      anchorKPlus1->setPrevious(lastEntityOfAnchorI);         //NOTE[4]: anchor[k+1]->previous is changed ( != lastEntityOfAnchorK)    (use at NOTE[1])

      //(5) (6) links `anchor[k]->previous` with `anchor[i]`
      anchorK->getPrevious()->setNext(anchorI);               //NOTE[5]: use anchor[k]->previous          (aka entityPrecedingAnchorK)
      anchorI->setPrevious(anchorK->getPrevious());           //NOTE[6]: anchor[i]->previous is changed   ( != entityPrecedingAnchorI) (use at NOTE[7])

      //(7) (8) links `anchor[i]->previous` (aka `entityPrecedingAnchorI`) with `anchor[k]`
      entityPrecedingAnchorI->setNext(anchorK);               //NOTE[7]: use anchor[i]->previous          (aka entityPrecedingAnchorI)
      anchorK->setPrevious(entityPrecedingAnchorI);           //NOTE[8]: anchor[k]->previous is changed   ( != lastEntityOfAnchorI)    (use at NOTE[5])
    }
    else {//if i == k + 1
      //(11) (12) links `anchor[i]->previous` (aka `entityPrecedingAnchorI`) with `anchor[k]`
      entityPrecedingAnchorI->setNext(anchorK);            //NOTE[11]: use anchor[i]->previous            (aka entityPrecedingAnchorI)
      anchorK->setPrevious(entityPrecedingAnchorI);        //NOTE[12]: anchor[k]->previous is changed     ( != lastEntityOfAnchorI)    (no use)

      //(13) (14) links `anchor[k+1]->previous` (aka last entity of `anchor[k]`) with `anchor[i]`
      anchorKPlus1->getPrevious()->setNext(anchorI);       //NOTE[13]: use anchor[k+1]->previous          (aka lastEntityOfAnchorK)
      anchorI->setPrevious(anchorKPlus1->getPrevious());   //NOTE[14]: anchor[i]->previous is changed     ( != entityPrecedingAnchorI) (use at NOTE[11])

      //(15) (16) links `anchor[k]->previous` (aka last entity of `anchor[i]`) with `anchor[k+1]`
      lastEntityOfAnchorI->setNext(anchorKPlus1);          //NOTE[15]: use anchor[i]->previous            (aka lastEntityOfAnchorI)
      anchorKPlus1->setPrevious(lastEntityOfAnchorI);      //NOTE[16]: anchor[k+1]->previous is changed   ( != lastEntityOfAnchorK)    (use at NOTE[13]
    } 

    std::swap(anchors_[indexOfAnchorI], anchors_[indexOfAnchorK]);
    std::swap(anchor_indices_[i], anchor_indices_[k]);

#ifdef _DEBUG
    std::cout << "\n\n...void Game2DView::swapAnchors(EntityViewType i, EntityViewType k)...";
    printAnchorInfo();
#endif // _DEBUG
  }

  /****************************************************************************/
#ifdef _DEBUG
  void Game2DView::printAnchorInfo() {
    static const char* ANCHOR_NAMES[] = {
      "BACKGROUND",
      "DECORATION_1",

      "TILE_MAP",
      "DECORATION_2",

      "TILE",
      "DECORATION_3",

      "ITEM",
      "DECORATION_4",

      "UI_BUTTON",
      "DECORATION_5",

      "TEXT_INPUT",
      "DECORATION_6",

      "OTHER",
      "UNKNOWN_1",
      "UNKNOWN_2",
      "UNKNOWN_3",
      "UNKNOWN_4",
      "UNKNOWN_5",
      "UNKNOWN_6",
      "UNKNOWN_7",
      "UNKNOWN_8",
      "UNKNOWN_9",
      "UNKNOWN_10",
    };    
    
    static enum Unknown {
      UNKNOWN1 = OTHER + 1,
      UNKNOWN2,
      UNKNOWN3,
      UNKNOWN4,
      UNKNOWN5,
      UNKNOWN6,
      UNKNOWN7,
      UNKNOWN8,
      UNKNOWN9,
      UNKNOWN10
    };

    static constexpr Integer ANCHOR_VALUES[] = {
      BACKGROUND,
      DECORATION_1,

      TILE_MAP,
      DECORATION_2,

      TILE,
      DECORATION_3,

      ITEM,
      DECORATION_4,

      UI_BUTTON,
      DECORATION_5,

      TEXT_INPUT,
      DECORATION_6,

      OTHER,
      UNKNOWN1,
      UNKNOWN2,
      UNKNOWN3,
      UNKNOWN4,
      UNKNOWN5,
      UNKNOWN6,
      UNKNOWN7,
      UNKNOWN8,
      UNKNOWN9,
      UNKNOWN10,
    };

    std::cout << "\nAnchors information:";
    std::cout << "\n\t"
      << std::setw(18) << "Type" << "   "
      << std::setw(5) << "Value" << "   "
      << std::setw(5) << "Index" << "   "
      << std::setw(5) << "Views" << "   ";

    std::cout << "\n\t"
      << std::setfill('-') << std::setw(18 + 5 + 5 + 5 + 10) << "";
    std::cout << std::setfill(' ');
    
    Integer indexToTypeConverter[sizeof(ANCHOR_VALUES)];
    for (int i = 0; i < 1 + getNumberOfAnchors(); ++i) {
      indexToTypeConverter[getIndexOfAnchor(EntityViewType(i))] = i;
    }

    for (int i = 0; i < 1+getNumberOfAnchors(); ++i) {
      Integer type = indexToTypeConverter[i];
      EntityView::Pointer loopView = (*this)[i];
      EntityView::Pointer nextAnchor = (
        i == getNumberOfAnchors() ?
          nullptr  : 
          (*this)[i + 1]
        );
      Integer numAnchors = 0;

#ifdef _DEBUG1
      std::cout << "\n\t\t" << loopView << "\t" << loopView->getNext();
#endif

      while ( loopView->getNext() != nextAnchor) {
        ++numAnchors;
        loopView = loopView->getNext();

#ifdef _DEBUG1
        std::cout << "\n\t\t" << loopView << "\t" << loopView->getNext();
#endif
      }

      std::cout << "\n\t"
        << std::setw(18) << ANCHOR_NAMES[type] << "   "
        << std::setw(5) <<  ANCHOR_VALUES[type] << "   "
        << std::setw(5) << i << "   "
        << std::setw(5) << numAnchors;
      /*/std::cout << "\n\t"
        << std::setfill('-') << std::setw(18 + 5 + 5 + 5 + 10) << "";
      std::cout << std::setfill(' ');
      //*/
    }
#ifdef _DEBUG
    std::cout << "\n";
#endif
  }

#endif
}