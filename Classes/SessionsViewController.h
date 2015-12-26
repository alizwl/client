
#ifndef __Sessions__ViewController__
#define __Sessions__ViewController__

#include <iostream>
#include "CrossApp.h"
#include "RootWindow.h"

USING_NS_CC;

typedef struct
{
	CALabel* lab;
	CAImageView* imgView;
} TabItemView;

class SessionsViewController :
	public CAViewController,
	public CATableViewDelegate,
	public CATableViewDataSource,
	public CAScrollViewDelegate
{
    
public:

	SessionsViewController();
    
	virtual ~SessionsViewController();
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

	void buttonCallBack(CAControl* btn, DPoint point);

	void showAlert(int id);

	void refreshBtnCallBack(CAControl* btn, DPoint point);

	void requestMsg(int page);
	void requestApendMsg(int page);

	void onRequestFinished0(const HttpResponseStatus& status, const CSJson::Value& json);
	void onRequestFinished1(const HttpResponseStatus& status, const CSJson::Value& json);
	void onRequestFinished2(const HttpResponseStatus& status, const CSJson::Value& json);

	virtual void scrollViewHeaderBeginRefreshing(CAScrollView* view);
	virtual void scrollViewFooterBeginRefreshing(CAScrollView* view);

    unsigned int tableViewHeightForHeaderInSection(CATableView* table, unsigned int section);
    CAView* tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section);
	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	virtual unsigned int numberOfSections(CATableView *table);
	virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);

	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
    
private:

	DSize													m_winSize;

    CAPageView*												m_pageView;
	CAView*													m_alertView[3];
	CAActivityIndicatorView*								m_pLoading[3];
    
	TabItemView*											m_tabItemView[3];

	std::vector<newsMsg>									m_msg[3];
	CATableView*											m_msgTableView[3];
	CAView*													m_pageViewCell[3];
	int														m_section[3];
	
};


#endif /* defined(__HelloCpp__ViewController__) */
