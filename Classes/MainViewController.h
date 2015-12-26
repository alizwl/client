
#ifndef __Main__ViewController__
#define __Main__ViewController__

#include "RootWindow.h"

class MainViewController :
public CAViewController,
public CATableViewDelegate,
public CATableViewDataSource,
public CAPageViewDelegate,
public CACollectionViewDelegate,
public CAScrollViewDelegate,
public CADatePickerViewDelegate
{
    
public:

	MainViewController();
    
	virtual ~MainViewController();
    
public:
    
    void viewDidLoad();
    
    void viewDidUnload();
    
	void initMsgTableView();

	void initPageView();

	void showAlert();

	void refreshBtnCallBack(CAControl* btn, DPoint point);

	void onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	void onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json);

	void buttonCallBack(CAControl* btn, DPoint point);

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void pageViewDidBeginTurning(CAPageView* pageView);
	virtual void pageViewDidEndTurning(CAPageView* pageView);
    virtual void pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    virtual void pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point);
    
	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
private:

	std::vector<newsMsg>									m_msg;
	std::vector<newsPage>									m_page;

    CAPageView*                                             m_totalView;
    CAVector<CAView* >                                      m_totalViewList;
	CATableView*											m_msgTableView;
	CAPageView*												m_pageView;
	CAPageControl*											m_pageControl;
	CALabel*												m_pageViewTitle;
	CAView*													p_alertView;
	CAActivityIndicatorView*								p_pLoading;
	int														p_section;

	DSize													m_winSize;
};


#endif /* defined(__Login__ViewController__) */
