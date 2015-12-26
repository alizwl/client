
#include "SessionsViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "SessionsSearchViewController.h"
#include "utils/HttpConnect.h"
#include "SessionDetailViewController.h"

SessionsViewController::SessionsViewController()
{
	for (int i = 0; i < 3; i++)
	{
		m_alertView[i] = NULL;
		m_pLoading[i] = NULL;
	}
}

SessionsViewController::~SessionsViewController()
{

}

void SessionsViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;

	CAScale9ImageView* tabBg = CAScale9ImageView::createWithImage(CAImage::create("common/yellow_bg.png"));
	tabBg->setCapInsets(DRect(1, 1, 1, 1));
	tabBg->setFrame(DRect(_px(0), _px(0), m_winSize.width, _px(200)));
	this->getView()->addSubview(tabBg);

	CAButton* btn = CAButton::createWithFrame(DRect(_px(20), _px(50), _px(64), _px(64)), CAButtonTypeRoundedRect);
	CAImageView* imageView = CAImageView::createWithImage(CAImage::create("source_material/btn_left_white.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(100);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CALabel* mc = CALabel::createWithFrame(DRect(_px(120), _px(62), _px(200), _px(50)));
	mc->setFontSize(_px(40));
	mc->setText("Agenda");
	mc->setColor(CAColor_white);
	mc->setFontName("fonts/arial.ttf");
	this->getView()->addSubview(mc);

	btn = CAButton::createWithFrame(DRect(m_winSize.width - _px(136), _px(50), _px(64), _px(64)), CAButtonTypeRoundedRect);
	imageView = CAImageView::createWithImage(CAImage::create("session/search_btn.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(200);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CAPageControl* pageControl = CAPageControl::createWithFrame(DRect(0, _px(100), m_winSize.width, _px(100)));
	pageControl->setNumberOfPages(3);
	pageControl->setPageIndicatorImage(CAImage::create("main/pagecontrol_selected.png"));
	pageControl->setCurrIndicatorImage(CAImage::create("main/pagecontrol_bg.png"));
	pageControl->setPageIndicatorTintColor(CAColor_gray);
	pageControl->setCurrentPageIndicatorTintColor(CAColor_green);
	pageControl->setTag(300);
	pageControl->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack));
	pageControl->setTouchEnabled(true);
	this->getView()->addSubview(pageControl);

	for (int i = 0; i < 3; i++)
	{
		CALabel* label = CALabel::createWithCenter(DRect(m_winSize.width * (2 * i + 1) / 6, _px(70), _px(200), _px(40)));
		label->setFontSize(_px(30));
		label->setColor(CAColor_gray);
		label->setTextAlignment(CATextAlignmentCenter);
		label->setFontName("fonts/arial.ttf");
		pageControl->addSubview(label);

		CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/pagecontrol_selected.png"));
		imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
		pageControl->addSubview(imageView);

		m_tabItemView[i] = new TabItemView;
		m_tabItemView[i]->lab = label;
		m_tabItemView[i]->imgView = imageView;
	}
	m_tabItemView[0]->lab->setColor(CAColor_white);
	m_tabItemView[0]->lab->setText("All sessions");
	m_tabItemView[1]->lab->setText("Track");
	m_tabItemView[2]->lab->setText("Format");
	m_tabItemView[0]->imgView->setFrame(DRect(m_winSize.width / 6 + _px(100), _px(70), _px(30), _px(30)));
	m_tabItemView[1]->imgView->setFrame(DRect(m_winSize.width / 2 + _px(60), _px(70), _px(30), _px(30)));
	m_tabItemView[2]->imgView->setFrame(DRect(m_winSize.width * 5 / 6 + _px(70), _px(70), _px(30), _px(30)));
	m_tabItemView[1]->imgView->setVisible(false);
	m_tabItemView[2]->imgView->setVisible(false);


	m_pageView = CAPageView::createWithFrame(DRect(0, _px(200), m_winSize.width, m_winSize.height - _px(200)), CAPageViewDirectionHorizontal);
    m_pageView->setScrollEnabled(false);
	CAVector<CAView* > viewList;
	for (int i = 0; i < 3; i++) {
		CATableView* tableView = CATableView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
		tableView->setTableViewDataSource(this);
		tableView->setTableViewDelegate(this);
		tableView->setScrollViewDelegate(this);
		tableView->setAllowsSelection(true);
		tableView->setTag(400 + i);
		CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
		refreshDiscount->setTag(400 + i);
		refreshDiscount->setLabelColor(CAColor_black);
		CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
		refreshDiscount1->setTag(400 + i);
		refreshDiscount1->setLabelColor(CAColor_black);
		tableView->setFooterRefreshView(refreshDiscount);
		tableView->setHeaderRefreshView(refreshDiscount1);
		m_msgTableView[i] = tableView;
		m_pageViewCell[i] = CAView::createWithFrame(DRect(0, _px(200), m_winSize.width, m_winSize.height - _px(200)));
		m_pageViewCell[i]->addSubview(tableView);
		viewList.pushBack(m_pageViewCell[i]);
	}
	m_pageView->setViews(viewList);
	m_pageView->setCurrPage(0, false);
	this->getView()->addSubview(m_pageView);


	if (m_msg[0].empty())
	{
		requestMsg(0);

		m_pLoading[0] = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
		m_pageViewCell[0]->insertSubview(m_pLoading[0], CAWindowZOderTop);
		m_pLoading[0]->setLoadingMinTime(0.5f);
	}

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionsViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionsViewController::requestMsg(int page) // all(0) track(1) format(2) my_all(3)
{
	if (m_alertView[page]) {
		m_pageViewCell[page]->removeSubview(m_alertView[page]);
		m_alertView[page] = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = crossapp_format_string("%d", page);
	key_value["section"] = "0";
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	if (page == 0)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished0));
	}
	else if (page == 1)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished1));
	}
	else if (page == 2)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished2));
	}
}

void SessionsViewController::requestApendMsg(int page)
{
	if (m_alertView[page]) {
		m_pageViewCell[page]->removeSubview(m_alertView[page]);
		m_alertView[page] = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = crossapp_format_string("%d", page);
	key_value["section"] = crossapp_format_string("%d", m_section[page]);
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	if (page == 0)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished0));
	}
	else if (page == 1)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished1));
	}
	else if (page == 2)
	{
		CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionsViewController::onRequestFinished2));
	}
}

void SessionsViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
	int id = view->getTag() - 400;
	requestMsg(id);
}

void SessionsViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
	int id = view->getTag() - 400;
	requestApendMsg(id);
}

void SessionsViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 100)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 200)
	{
		SessionsSearchViewController* vc = new SessionsSearchViewController();
		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
	}
	else if (btn->getTag() == 300)
	{
		CAPageControl* button = (CAPageControl*)btn;
		int i = button->getCurrentPage();

		m_tabItemView[i]->lab->setColor(CAColor_white);
		m_tabItemView[i]->imgView->setVisible(true);
		m_tabItemView[(i + 1) % 3]->lab->setColor(CAColor_gray);
		m_tabItemView[(i + 1) % 3]->imgView->setVisible(false);
		m_tabItemView[(i + 2) % 3]->lab->setColor(CAColor_gray);
		m_tabItemView[(i + 2) % 3]->imgView->setVisible(false);

		m_pageView->setCurrPage(i, true);

		if (m_msg[i].empty())
		{
			requestMsg(i);
			if (m_pLoading[i] == NULL)
			{
				m_pLoading[i] = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, 50, 50));
				m_pageViewCell[i]->insertSubview(m_pLoading[i], CAWindowZOderTop);
				m_pLoading[i]->setLoadingMinTime(0.5f);
			}
			
		}
	}
	else if (btn->getTag() >= 500 && btn->getTag() <= 502)
	{
		int id = btn->getTag() - 500;
		requestMsg(id);
		if (m_pLoading[id] == NULL)
		{
			m_pLoading[id] = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, 50, 50));
			m_pageViewCell[id]->insertSubview(m_pLoading[id], CAWindowZOderTop);
			m_pLoading[id]->setLoadingMinTime(0.5f);
		}
	}
}

void SessionsViewController::showAlert(int id)
{
	if (m_alertView[id]) {
		m_pageViewCell[id]->removeSubview(m_alertView[id]);
		m_alertView[id] = NULL;
	}

	m_alertView[id] = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	m_pageViewCell[id]->addSubview(m_alertView[id]);

	CAImageView* bg = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	bg->setImage(CAImage::create("common/bg.png"));

	CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
	btn5->setTag(500 + id);
	btn5->setCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, m_winSize.width, m_winSize.height - _px(200)));
	btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
	btn5->setBackGroundViewForState(CAControlStateNormal, bg);
	btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
	btn5->addTarget(this, CAControl_selector(SessionsViewController::buttonCallBack), CAControlEventTouchUpInSide);
	m_alertView[id]->addSubview(btn5);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(60)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	test->setFontSize(_px(40));
	test->setText("Network cannot connected !");
	m_alertView[id]->addSubview(test);

}

void SessionsViewController::onRequestFinished0(const HttpResponseStatus& status, const CSJson::Value& json)
{
	int id = 0;
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];
		int length = value.size();
		m_msg[id].clear();

		for (int index = 3; index < length; index++)
		{
			newsMsg temp_msg;
			temp_msg.m_sessionId = value[index]["sid"].asInt();
			temp_msg.m_title = value[index]["title"].asString();
			temp_msg.m_lecturer = value[index]["lct"].asString();
			temp_msg.m_track = value[index]["trk"].asString();
			temp_msg.m_format = value[index]["fmt"].asString();
			temp_msg.m_startTime = value[index]["trk"].asInt64();
			temp_msg.m_duration = value[index]["drt"].asInt();
			temp_msg.m_likeNum = value[index]["lkn"].asInt();
			temp_msg.m_imageUrl = value[index]["image"].asString();
			m_msg[id].push_back(temp_msg);
		}
	}
	else if (m_msg[id].empty())
	{
		//showAlert(id);
	}

	{
		m_msg[id].clear();
		for (int i = 0; i < 7; i++)
		{
			newsMsg temp_msg;
			temp_msg.m_sessionId = 200 + i;
			temp_msg.m_title = "Customer Success";
			temp_msg.m_lecturer = "Lisa Chen";
			temp_msg.m_track = "Customer";
			temp_msg.m_format = "Dev Faire";
			cc_timeval ct;
			CCTime::gettimeofdayCrossApp(&ct, NULL);
            
			temp_msg.m_startTime = ct.tv_sec + 3500;
			temp_msg.m_likeNum = 20;
			temp_msg.m_imageUrl = "http://img12.deviantart.net/88e2/i/2012/271/5/7/png_pond_by_paradise234-d5g6gky.png";
			m_msg[id].push_back(temp_msg);
		}
	}

	if (m_pLoading[id])
	{
		m_pLoading[id]->stopAnimating();
		m_pageViewCell[id]->removeSubview(m_pLoading[id]);
		m_pLoading[id] = NULL;
	}

	if (m_msgTableView[id])
	{
		m_msgTableView[id]->reloadData();
	}
}

void SessionsViewController::onRequestFinished1(const HttpResponseStatus& status, const CSJson::Value& json)
{
	int id = 1;
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];
		int length = value.size();
		m_msg[id].clear();

		for (int index = 3; index < length; index++)
		{
			newsMsg temp_msg;
			temp_msg.m_sessionId = value[index]["sid"].asInt();
			temp_msg.m_title = value[index]["title"].asString();
			temp_msg.m_lecturer = value[index]["lct"].asString();
			temp_msg.m_track = value[index]["trk"].asString();
			temp_msg.m_format = value[index]["fmt"].asString();
			temp_msg.m_startTime = value[index]["trk"].asInt64();
			temp_msg.m_duration = value[index]["drt"].asInt();
			temp_msg.m_likeNum = value[index]["lkn"].asInt();
			temp_msg.m_imageUrl = value[index]["image"].asString();
			m_msg[id].push_back(temp_msg);
		}
	}
	else if(m_msg[id].empty())
	{
		showAlert(id);
	}

	if (m_pLoading[id])
	{
		m_pLoading[id]->stopAnimating();
		m_pageViewCell[id]->removeSubview(m_pLoading[id]);
		m_pLoading[id] = NULL;
	}

	if (m_msgTableView[id])
	{
		m_msgTableView[id]->reloadData();
	}
}

void SessionsViewController::onRequestFinished2(const HttpResponseStatus& status, const CSJson::Value& json)
{
	int id = 2;
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];
		int length = value.size();
		m_msg[id].clear();

		for (int index = 3; index < length; index++)
		{
			newsMsg temp_msg;
			temp_msg.m_sessionId = value[index]["sid"].asInt();
			temp_msg.m_title = value[index]["title"].asString();
			temp_msg.m_lecturer = value[index]["lct"].asString();
			temp_msg.m_track = value[index]["trk"].asString();
			temp_msg.m_format = value[index]["fmt"].asString();
			temp_msg.m_startTime = value[index]["trk"].asInt64();
			temp_msg.m_duration = value[index]["drt"].asInt();
			temp_msg.m_likeNum = value[index]["lkn"].asInt();
			temp_msg.m_imageUrl = value[index]["image"].asString();
			m_msg[id].push_back(temp_msg);
		}
	}
	else if (m_msg[id].empty())
	{
		showAlert(id);
	}

	if (m_pLoading[id])
	{
		m_pLoading[id]->stopAnimating();
		m_pageViewCell[id]->removeSubview(m_pLoading[id]);
		m_pLoading[id] = NULL;
	}

	if (m_msgTableView[id])
	{
		m_msgTableView[id]->reloadData();
	}
}

unsigned int SessionsViewController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    return _px(70);
}

CAView* SessionsViewController::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = NULL;
    int id = table->getTag() - 400;
    if (id == 0)
    {
        std::string head;
        if (section == 0)
        {
            head = crossapp_format_string("All Sessions - ");
        }
        view = CAView::create();
        
        DSize _size = viewSize;
        CALabel* header = CALabel::createWithFrame(DRect(40, _px(20), _size.width/2, _size.height));
        header->setText(head);
        header->setFontSize(_px(30));
        header->setColor(CAColor_gray);
        header->setTextAlignment(CATextAlignmentLeft);
        header->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
        view->addSubview(header);
    }
    
    
    return view;
}

CATableViewCell* SessionsViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
	int id = table->getTag() - 400;

	DSize _size = cellSize;
	MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
	if (cell == NULL)
	{
		cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
		cell->initWithCell();
	}
	cell->setModel(m_msg[id][row]);
	return cell;
}

unsigned int SessionsViewController::numberOfSections(CATableView *table)
{
	return 2;
}

unsigned int SessionsViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	int id = table->getTag() - 400;

    return m_msg[id].size();
}

unsigned int SessionsViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return _px(170);
}

void SessionsViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	int id = table->getTag() - 400;
    
    int sessionId = m_msg[id][row].m_sessionId;
    
    SessionDetailViewController* vc = new SessionDetailViewController(sessionId);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void SessionsViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	int id = table->getTag() - 400;
}


