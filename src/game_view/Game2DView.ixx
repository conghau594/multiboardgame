module;
import <utility>;

#if _DEBUG
import <iostream>;
import <iomanip>;
#endif

export module game_view.Game2DView;

import foundation.basetypes;
import foundation.Vector;
import foundation.SharedPointer;
import core.SceneView;
import core.EntityView;
import core.Window;
import core.Avatar;

namespace mbg {
  ///A SDL view corresponding with a use case.
  /***************************************************************************
   * Each scene view contains some entity views for displaying and for user to
   * interact with. This conforms to MVC pattern.
   * \sa SDLSceneController, SDLSceneModel
   ***************************************************************************/
  export class Game2DView : public SceneView, public EntityView,
    public std::enable_shared_from_this<Game2DView> {
  public://types
    using Pointer = SharedPointer<Game2DView>;
    enum EntityViewType {
      BACKGROUND = 0,
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
      
      OTHER
    };

  public://member functions
    inline EntityView::Pointer& operator[](Integer i);
    inline Integer getIndexOfAnchor(EntityViewType i) const;
    inline SizeType getNumberOfAnchors() const;
    inline EntityView::Pointer getAnchor(Game2DView::EntityViewType type) const;
    inline EntityView::Pointer getNextAnchor(Game2DView::EntityViewType type) const;
    inline void clearAnchors();          ///< Clears all ...

    inline EntityView::Pointer getFirstTileView() const {
      EntityView::Pointer firstTargetedView = anchors_[getIndexOfAnchor(TILE)]->getNext();
      return (firstTargetedView == anchors_[1 + getIndexOfAnchor(TILE)]) ? nullptr : firstTargetedView;
    }

    inline EntityView::Pointer getFirstTileMapView() const {
      EntityView::Pointer firstTargetedView = anchors_[getIndexOfAnchor(TILE_MAP)]->getNext();
      return (firstTargetedView == anchors_[1 + getIndexOfAnchor(TILE_MAP)]) ? nullptr : firstTargetedView;
    }

    void swapAnchors(EntityViewType i, EntityViewType j);

#ifdef _DEBUG
    void printAnchorInfo();
#endif

    void addNewAnchor();
    inline virtual void addView(EntityView::Pointer entityView, EntityViewType viewType); ///Adds new entity view to this scene.
    inline virtual void addViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView, EntityViewType viewType); ///Adds collection of entity views to this scene.

    inline virtual void removeView(EntityView::Pointer entityView);
    inline virtual void removeViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView);

    inline virtual void insertViewBefore(EntityView::Pointer targetedView, EntityView::Pointer insertedView);
    inline virtual void insertViewSeriesBefore(EntityView::Pointer targetedView, EntityView::Pointer insertedFirstView, EntityView::Pointer insertedLastView);

    inline virtual void insertViewAfter(EntityView::Pointer targetedView, EntityView::Pointer insertedView);
    inline virtual void insertViewSeriesAfter(EntityView::Pointer targetedView, EntityView::Pointer insertedFirstView, EntityView::Pointer insertedLastView);

    inline virtual void clearViewsFromAnchor(EntityViewType viewType);

    virtual void render(Window::Pointer window);  ///< Renders screen by drawing all entity views belonging to it.

  public://static
    template <typename... ARGS>
    static inline Pointer createPointer(ARGS... args) {
      return Pointer(new Game2DView(std::forward<ARGS>(args)...));
    }

  protected://contructors
    Game2DView() = default;

    Game2DView(
      Avatar::Pointer avatar,
      EntityViewType viewType,
      EntityView::Pointer headView)
    {
      constexpr Integer INITIAL_NUMBER_OF_ANCHORS = OTHER + 2;
      for (Integer i = 0; i < INITIAL_NUMBER_OF_ANCHORS; ++i) {
        anchors_.emplace_back(EntityView::createPointer());
        anchor_indices_.emplace_back(i);
      }
    
      if (headView != nullptr) {
        anchors_[viewType]->setNext(headView);
        headView->setPrevious(anchors_[BACKGROUND]);
      }
    }

    Game2DView(
      Avatar::Pointer avatar,
      EntityView::Pointer headBackgroundView,
      EntityView::Pointer headTileMapView,
      EntityView::Pointer headTileView,
      EntityView::Pointer headElementView);

    Game2DView(
      Avatar::Pointer avatar,
      EntityView::Pointer headBackgroundView,
      EntityView::Pointer headTileMapView,
      EntityView::Pointer headTileView,
      EntityView::Pointer headElementView,
      EntityView::Pointer headUIButtonView,
      EntityView::Pointer headTextInputView,
      EntityView::Pointer headDecorationView,
      EntityView::Pointer headOtherView);

    Game2DView(
      Avatar::Pointer avatar,
      EntityView::Pointer headBackgroundView,
      EntityView::Pointer headTileMapView,
      EntityView::Pointer headTileView,
      EntityView::Pointer headElementView,
      EntityView::Pointer headUIButtonView,
      EntityView::Pointer headTextInputView,
      EntityView::Pointer headDecorationView_1,
      EntityView::Pointer headDecorationView_2,
      EntityView::Pointer headDecorationView_3,
      EntityView::Pointer headOtherView
    );

  private://member functions
    void linkEntityViewsToAnchors();

  private://data
    Vector<EntityView::Pointer> anchors_;
    Vector<Integer> anchor_indices_;
  };


  /****************************************************************************
   * \return The anchor of number i.
   ****************************************************************************/
  inline EntityView::Pointer& Game2DView::operator[](Integer i) { 
    return anchors_[i]; 
  }

  /****************************************************************************
   ****************************************************************************/
  inline EntityView::Pointer Game2DView::getAnchor(Game2DView::EntityViewType type) const {
    return anchors_[getIndexOfAnchor(type)];
  }

  /****************************************************************************
   ****************************************************************************/
  inline EntityView::Pointer Game2DView::getNextAnchor(Game2DView::EntityViewType type) const {
    return anchors_[1 + getIndexOfAnchor(type)];
  }


  /****************************************************************************
   ****************************************************************************/
  inline Integer Game2DView::getIndexOfAnchor(EntityViewType i) const { 
    return anchor_indices_[i]; 
  }


  /****************************************************************************
   ****************************************************************************/
  inline SizeType Game2DView::getNumberOfAnchors() const { 
    return anchors_.size() - 1; 
  }


  /****************************************************************************
   ****************************************************************************/
  inline void Game2DView::clearAnchors() { 
    anchors_.clear(); 
    anchor_indices_.clear();

    //always have anchors_ to contain at least 1 element at the end of the vector
    anchors_.emplace_back(EntityView::createPointer());
    anchor_indices_.emplace_back(0);

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::clearAnchor()";
    printAnchorInfo();
#endif // _DEBUG
  }

  /****************************************************************************
   * If the anchor of the `viewType` is not null, then replaces it from the
   * anchor by the entityView, else simply sets the anchor to the `entityView` and
   * links the previous entity view to this `entityView`.
   ****************************************************************************/
  inline void Game2DView::addView(EntityView::Pointer entityView, EntityViewType viewType) {
    //if (entityView == nullptr || viewType >= getNumberOfAnchors() || viewType < 0)
    //  return; //TODO.036 add log

    EntityView::Pointer theAnchor = (*this)[getIndexOfAnchor(viewType)];
    EntityView::Pointer entityFollowingTheAnchor = theAnchor->getNext();

    entityView->setPrevious(theAnchor);
    entityView->setNext(entityFollowingTheAnchor);
    theAnchor->setNext(entityView);
    if (entityFollowingTheAnchor != nullptr) {
      entityFollowingTheAnchor->setPrevious(entityView);
    }

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::addView(EntityView::Pointer entityView, EntityViewType viewType)";
    printAnchorInfo();
#endif // _DEBUG
  }


  /****************************************************************************/
  inline void Game2DView::removeView(EntityView::Pointer entityView) {
    //if (entityView == nullptr) // || viewType >= getNumberOfAnchors() || viewType < 0)
    //  return; //TODO.035 add log

    EntityView::Pointer prevView = entityView->getPrevious();
    EntityView::Pointer nextView = entityView->getNext();

    if (prevView != nullptr)
      prevView->setNext(nextView);

    if (nextView != nullptr)
      nextView->setPrevious(prevView);

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::removeView(EntityView::Pointer entityView)";
    printAnchorInfo();
#endif // _DEBUG
  }


  /****************************************************************************
   * \param headView The view that is the beginning of the collection of views
   *                 which you want to add to this game view. If this `headView`
   *                 is null then the function does nothing.
   * \param tailView The view that is the end of the collection of views. If
   *                 this `tailView` is null then the function will add only
   *                 the `headView` and change the next and previous view of it.
   ****************************************************************************/
  inline void Game2DView::addViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView, EntityViewType viewType) {
    //if (headView == nullptr || viewType >= getNumberOfAnchors() || viewType < 0)
    //  return; //TODO.036 add log

    EntityView::Pointer theAnchor = (*this)[getIndexOfAnchor(viewType)];
    EntityView::Pointer entityFollowingTheAnchor = theAnchor->getNext();

    headView->setPrevious(theAnchor);
    tailView->setNext(entityFollowingTheAnchor);
    theAnchor->setNext(headView);
    //if (entityFollowingTheAnchor != nullptr) {
      entityFollowingTheAnchor->setPrevious(tailView);
    //}

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::addViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView, EntityViewType viewType)";
    printAnchorInfo();
#endif // _DEBUG
  }
  
  /****************************************************************************/
  inline void Game2DView::insertViewBefore(EntityView::Pointer targetedView, EntityView::Pointer insertedView) {
    //if (targetedView->getPrevious() != nullptr) {
    targetedView->getPrevious()->setNext(insertedView);
    //}
    
    insertedView->setPrevious(targetedView->getPrevious());

    insertedView->setNext(targetedView);
    targetedView->setPrevious(insertedView);
  }

  /****************************************************************************/
  inline void Game2DView::insertViewSeriesBefore(EntityView::Pointer targetedView, EntityView::Pointer insertedFirstView, EntityView::Pointer insertedLastView) {
    //if (targetedView->getPrevious() != nullptr) {
    targetedView->getPrevious()->setNext(insertedFirstView);
    //}

    insertedFirstView->setPrevious(targetedView->getPrevious());
    insertedLastView->setNext(targetedView);
    targetedView->setPrevious(insertedLastView);
  }

  /****************************************************************************/
  inline void Game2DView::insertViewAfter(EntityView::Pointer targetedView, EntityView::Pointer insertedView) {
    //if (targetedView->getNext() != nullptr) {
    targetedView->getNext()->setPrevious(insertedView);
    //}

    insertedView->setNext(targetedView->getNext());
    insertedView->setPrevious(targetedView);
    targetedView->setNext(insertedView);
  }

  /****************************************************************************/
  inline void Game2DView::insertViewSeriesAfter(EntityView::Pointer targetedView, EntityView::Pointer insertedFirstView, EntityView::Pointer insertedLastView) {
    //if (targetedView->getNext() != nullptr) {
    targetedView->getNext()->setPrevious(insertedLastView);
    //}
    insertedLastView->setNext(targetedView->getNext());
    insertedFirstView->setPrevious(targetedView);
    targetedView->setNext(insertedFirstView);

  }

  /****************************************************************************/
  inline void Game2DView::removeViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView) {
    EntityView::Pointer prevHeadView = headView->getPrevious();
    EntityView::Pointer nextTailView = tailView->getNext();

    if (prevHeadView != nullptr)
      prevHeadView->setNext(nextTailView);

    if (nextTailView != nullptr)
      nextTailView->setPrevious(prevHeadView);

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::removeViewSeries(EntityView::Pointer headView, EntityView::Pointer tailView)";
    printAnchorInfo();
#endif // _DEBUG

  }


  /****************************************************************************/
  inline void Game2DView::clearViewsFromAnchor(EntityViewType viewType) {
    if (viewType >= getNumberOfAnchors() || viewType < 0)
      return; //TODO.036 add log

    EntityView::Pointer theAnchor = (*this)[getIndexOfAnchor(viewType)];
    EntityView::Pointer theNextAnchor = (*this)[getIndexOfAnchor(viewType) + 1];
    
    theAnchor->setNext(theNextAnchor);
    theNextAnchor->setPrevious(theAnchor);

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::clearViewsFromAnchor(EntityViewType viewType)";
    printAnchorInfo();
#endif // _DEBUG
  }

  /****************************************************************************/
  inline void Game2DView::addNewAnchor() {
    EntityView::Pointer prevLastAnchor = anchors_.back();

    anchors_.emplace_back();
    anchor_indices_.emplace_back(1 + getNumberOfAnchors());
    
    prevLastAnchor->setNext(anchors_.back());
    anchors_.back()->setPrevious(prevLastAnchor);

#ifdef _DEBUG1
    std::cout << "\n\n...void Game2DView::addNewAnchor()";
    printAnchorInfo();
#endif // _DEBUG
  }
}