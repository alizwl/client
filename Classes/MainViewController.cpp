
#include "MainViewController.h"
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"
#include "time.h"
#include "SessionsViewController.h"
#include "LoginViewController.h"
#include "SessionDetailViewController.h"
#include "MyStatusViewController.h"
#include "PrizeViewController.h"
#include "SiteViewController.h"
#include "VoteViewController.h"
#include "PhotoViewController.h"


MainViewController::MainViewController()
: m_pageView(NULL)
, m_msgTableView(NULL)
, p_alertView(NULL)
, p_pLoading(NULL)
, p_section(1)
{
    
}

MainViewController::~MainViewController()
{
    
}

void MainViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
    m_winSize = this->getView()->getBounds().size;
    
    if (m_msg.empty())
    {
        std::map<std::string, std::string> key_value;
        key_value["tag"] = mainViewTag[0];
        key_value["page"] = "1";
        key_value["limit"] = "20";
        key_value["appid"] = "10000";
        key_value["sign_method"] = "md5";
        key_value["sign"] = getSign(key_value);
        
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
        
        p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
        this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
        p_pLoading->setLoadingMinTime(0.5f);
        p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
    }
    else
    {
        this->initMsgTableView();
    }
    
}

void MainViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


void MainViewController::scrollViewHeaderBeginRefreshing(CrossApp::CAScrollView *view)
{
    std::map<std::string, std::string> key_value;
    key_value["tag"] = mainViewTag[0];
    key_value["page"] = "1";
    key_value["limit"] = "20";
    key_value["appid"] = "10000";
    key_value["sign_method"] = "md5";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
}

void MainViewController::scrollViewFooterBeginRefreshing(CAScrollView* view)
{
    p_section++;
    std::map<std::string, std::string> key_value;
    key_value["tag"] = mainViewTag[0];
    key_value["page"] = crossapp_format_string("%d", p_section);
    key_value["limit"] = "20";
    key_value["appid"] = "10000";
    key_value["sign_method"] = "md5";
    key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRefreshRequestFinished));
}

void MainViewController::initMsgTableView()
{
    if (m_page.empty())
    {
        showAlert();
        return;
    }
    if (m_msgTableView != NULL)
    {
        this->getView()->removeSubview(m_msgTableView);
        m_msgTableView = NULL;
    }
    
    m_msgTableView = CATableView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(170)));
    m_msgTableView->setTableViewDataSource(this);
    m_msgTableView->setTableViewDelegate(this);
    m_msgTableView->setScrollViewDelegate(this);
    m_msgTableView->setAllowsSelection(true);
    this->getView()->addSubview(m_msgTableView);
    CAPullToRefreshView *refreshDiscount = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeFooter);
    refreshDiscount->setLabelColor(CAColor_black);
    CAPullToRefreshView *refreshDiscount1 = CAPullToRefreshView::create(CAPullToRefreshView::CAPullToRefreshTypeHeader);
    refreshDiscount1->setLabelColor(CAColor_black);
    m_msgTableView->setFooterRefreshView(refreshDiscount);
    m_msgTableView->setHeaderRefreshView(refreshDiscount1);
    
    initPageView();
}

void MainViewController::initPageView()
{
    CAView* headView = CAView::create();
    m_pageView = CAPageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.width / 2), CAPageViewDirectionHorizontal);
    m_pageView->setPageViewDelegate(this);
    m_pageView->setTouchEnabled(true);
    CAVector<CAView* > viewList;
    for (int i = 0; i<m_page.size(); i++) {
        CommonUrlImageView* temImage = CommonUrlImageView::createWithFrame(DRect(m_winSize.width / 2, 100, m_winSize.width, m_winSize.width / 2));
        temImage->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
        temImage->setImage(CAImage::create("common/bg.png"));
        temImage->setUrl(m_page[i].m_imageUrl);
        viewList.pushBack(temImage);
    }
    m_pageView->setViews(viewList);
    m_pageView->setCurrPage(0, false);
    headView->addSubview(m_pageView);
    
    CAView* bg = CAView::createWithColor(ccc4(0, 0, 0, 100));
    bg->setFrame(DRect(0, m_winSize.width / 2 - 50, m_winSize.width, 50));
    headView->addSubview(bg);
    
    m_pageControl = CAPageControl::createWithCenter(DRect(m_winSize.width - 80, m_winSize.width / 2 - 25, 100, 50));
    m_pageControl->setNumberOfPages((int)m_page.size());
    m_pageControl->setPageIndicatorImage(CAImage::create("main/pagecontrol_selected.png"));
    m_pageControl->setCurrIndicatorImage(CAImage::create("main/pagecontrol_bg.png"));
    m_pageControl->setPageIndicatorTintColor(CAColor_gray);
    m_pageControl->setCurrentPageIndicatorTintColor(CAColor_green);
    m_pageControl->setTag(200);
    m_pageControl->addTarget(this, CAControl_selector(MainViewController::buttonCallBack));
    m_pageControl->setTouchEnabled(true);
    headView->addSubview(m_pageControl);
    
    if (m_page.size() > 0) {
        m_pageViewTitle = CALabel::createWithFrame(DRect(20, m_winSize.width / 2 - 40, m_winSize.width - 50, 50));
        m_pageViewTitle->setText(m_page[0].m_title);
        m_pageViewTitle->setColor(CAColor_white);
        m_pageViewTitle->setFontSize(_px(28));
        headView->addSubview(m_pageViewTitle);
    }
    for (int i = 0; i < 4; i++)
    {
        DRect r((m_winSize.width / 4) * i, m_winSize.width / 2, (m_winSize.width) / 4, m_winSize.width / 4);
        CAButton* btn = CAButton::createWithFrame(r, CAButtonTypeCustom);
        btn->setTag(300 + i);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/short_gift.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
        imageView->setFrame(DRect(0, 0, m_winSize.width / 4, m_winSize.width / 4));
        btn->setBackGroundViewForState(CAControlStateAll, imageView);
        btn->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
        headView->addSubview(btn);
    }
    
    CALabel* mc = CALabel::createWithFrame(DRect(20, m_winSize.width * 3 / 4, m_winSize.width, _px(60)));
    mc->setFontSize(_px(30));
    mc->setText("My Calender");
    mc->setColor(CAColor_gray);
    mc->setFontName("fonts/arial.ttf");
    
    headView->addSubview(mc);
    m_msgTableView->setTableHeaderView(headView);
    m_msgTableView->setTableHeaderHeight(m_winSize.width * 3 / 4 + _px(60));
    
    CAScale9ImageView* tabBg = CAScale9ImageView::createWithImage(CAImage::create("common/yellow_bg.png"));
    tabBg->setCapInsets(DRect(1, 1, 1, 1));
    tabBg->setFrame(DRect(_px(0), m_winSize.height - _px(170), m_winSize.width, _px(170)));
    this->getView()->addSubview(tabBg);
    
    for (int i = 0; i < 3; i++)
    {
        DRect r(i * (m_winSize.width / 3) + (m_winSize.width / 3 - _px(150)) / 2, m_winSize.height - _px(160), _px(150), _px(150));
        CAButton* btn = CAButton::createWithFrame(r, CAButtonTypeCustom);
        btn->setTag(400 + i);
        CAImageView* imageView = CAImageView::createWithImage(CAImage::create("main/short_gift.png"));
        imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
        btn->setBackGroundViewForState(CAControlStateAll, imageView);
        btn->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
        this->getView()->addSubview(btn);
    }
}

void MainViewController::showAlert()
{
    if (p_alertView) {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
    }
    
    p_alertView = CAView::createWithFrame(this->getView()->getBounds());
    this->getView()->addSubview(p_alertView);
    
    CAImageView* bg = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height));
    bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    bg->setImage(CAImage::create("common/bg.png"));
    
    CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
    btn5->setTag(100);
    btn5->setCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, m_winSize.width, m_winSize.height));
    btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
    btn5->setBackGroundViewForState(CAControlStateNormal, bg);
    btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
    btn5->addTarget(this, CAControl_selector(MainViewController::buttonCallBack), CAControlEventTouchUpInSide);
    p_alertView->addSubview(btn5);
    
    CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2,
                                                    m_winSize.height - 100,
                                                    m_winSize.width,
                                                    40));
    test->setColor(CAColor_gray);
    test->setTextAlignment(CATextAlignmentCenter);
    test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    test->setFontSize(_px(24));
    test->setText("Network cannot connected !");
    p_alertView->addSubview(test);
    
}

void MainViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
        m_page.clear();
        for (int i = 0; i<3; i++) {
            newsPage temp_page;
            temp_page.m_titleId = value[i]["til"].asInt();
            temp_page.m_title = value[i]["til"].asString();
            temp_page.m_imageUrl = value[i]["url"].asString();
            m_page.push_back(temp_page);
        }
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
            m_msg.push_back(temp_msg);
        }
    }
    
    {
        m_msg.clear();
        m_page.clear();
        for (int i = 0; i < 3; i++) {
            char title[8];
            sprintf(title, "title_%d", i);
            
            newsPage temp_page;
            temp_page.m_titleId = 100 + i;
            temp_page.m_title = title;
            temp_page.m_imageUrl = "http://img12.deviantart.net/88e2/i/2012/271/5/7/png_pond_by_paradise234-d5g6gky.png";
            m_page.push_back(temp_page);
        }
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
            temp_msg.m_imageUrl = "https://i.ytimg.com/vi/8fhry_eQt7g/maxresdefault.jpg";
            m_msg.push_back(temp_msg);
        }
    }
    
    if (p_pLoading)
    {
        p_pLoading->stopAnimating();
    }
    
    if (m_msgTableView)
    {
        m_msgTableView->reloadData();
    }
}

void MainViewController::onRefreshRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        const CSJson::Value& value = json["result"];
        int length = value.size();
        m_msg.clear();
        m_page.clear();
        for (int i = 0; i<3; i++) {
            newsPage temp_page;
            temp_page.m_titleId = value[i]["til"].asInt();
            temp_page.m_title = value[i]["til"].asString();
            temp_page.m_imageUrl = value[i]["url"].asString();
            m_page.push_back(temp_page);
        }
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
            m_msg.push_back(temp_msg);
        }
    }
    
    do
    {
        CC_BREAK_IF(p_pLoading == NULL);
        if (p_pLoading->isAnimating())
        {
            p_pLoading->stopAnimating();
        }
        else
        {
            m_msgTableView->reloadData();
        }
    } while (0);
}

void MainViewController::buttonCallBack(CAControl* btn, DPoint point)
{
    if (btn->getTag() == 100)
    {
        this->getView()->removeSubview(p_alertView);
        p_alertView = NULL;
        std::map<std::string, std::string> key_value;
        key_value["tag"] = mainViewTag[0];
        key_value["page"] = "1";
        key_value["limit"] = "20";
        key_value["appid"] = "10000";
        key_value["sign_method"] = "md5";
        key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewController::onRequestFinished));
        {
            p_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
            this->getView()->insertSubview(p_pLoading, CAWindowZOderTop);
            p_pLoading->setLoadingMinTime(0.5f);
            p_pLoading->setTargetOnCancel(this, callfunc_selector(MainViewController::initMsgTableView));
        }
    }
    else if (btn->getTag() == 200)
    {
        CAPageControl* button = (CAPageControl*)btn;
        m_pageView->setCurrPage(button->getCurrentPage(), true);
        if (m_page.size()>0) {
            m_pageViewTitle->setText(m_page[button->getCurrentPage()].m_title);
        }
    }
    else if (btn->getTag() == 300)
    {
        PhotoViewController* vc = new PhotoViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 301)
    {
        VoteViewController* vc = new VoteViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 302)
    {
        PrizeViewController* vc = new PrizeViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 303)
    {
        SiteViewController* vc = new SiteViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 400)
    {
		SessionsViewController* vc = new SessionsViewController();
		vc->init();
		RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    else if (btn->getTag() == 401)
    {
        
    }
    else if (btn->getTag() == 402)
    {
        MyStatusViewController* vc = new MyStatusViewController();
        vc->init();
        RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
    }
    
}

CATableViewCell* MainViewController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    MainViewTableCell* cell = dynamic_cast<MainViewTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = MainViewTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    cell->setModel(m_msg[row]);
    
    return cell;
}

unsigned int MainViewController::numberOfSections(CATableView *table)
{
    return 1;
}

unsigned int MainViewController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return m_msg.size();
}

unsigned int MainViewController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(170);
}


void MainViewController::pageViewDidBeginTurning(CAPageView* pageView)
{
    
}

void MainViewController::pageViewDidEndTurning(CAPageView* pageView)
{
    m_pageControl->setCurrentPage(pageView->getCurrPage());
    m_pageControl->updateCurrentPageDisplay();
    if (m_page.size() > 0) {
        m_pageViewTitle->setText(m_page[pageView->getCurrPage()].m_title);
    }
}

void MainViewController::pageViewDidSelectPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}

void MainViewController::pageViewDidSelectedPageAtIndex(CAPageView* pageView, unsigned int index, const DPoint& point)
{
    
}

void MainViewController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    int sessionId = m_msg[row].m_sessionId;
    
    SessionDetailViewController* vc = new SessionDetailViewController(sessionId);
    vc->init();
    RootWindow::getInstance()->getRootNavigationController()->pushViewController(vc, true);
}

void MainViewController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}