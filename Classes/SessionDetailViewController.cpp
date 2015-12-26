
#include "SessionDetailViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"

SessionDetailViewController::SessionDetailViewController(int sessionId)
{
	m_alertView = NULL;
	m_pLoading = NULL;
    m_sessionId = sessionId;
}

SessionDetailViewController::~SessionDetailViewController()
{

}

void SessionDetailViewController::viewDidLoad()
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
	btn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CALabel* mc = CALabel::createWithFrame(DRect(_px(120), _px(62), _px(200), _px(50)));
	mc->setFontSize(_px(40));
	mc->setText("Details");
	mc->setColor(CAColor_white);
	mc->setFontName("fonts/arial.ttf");
	this->getView()->addSubview(mc);

	btn = CAButton::createWithFrame(DRect(m_winSize.width - 100, _px(130), _px(50), _px(50)), CAButtonTypeRoundedRect);
	imageView = CAImageView::createWithImage(CAImage::create("session/share_btn.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(200);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	requestMsg();
	m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
	m_pLoading->setLoadingMinTime(0.5f);
	this->getView()->addSubview(m_pLoading);

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SessionDetailViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SessionDetailViewController::initDetailView()
{
    DSize _size = m_winSize;
    
    int offY = _px(240);
    m_urlImageView = CommonUrlImageView::createWithFrame(DRect(20, 20 + offY, 100, 100));
    m_urlImageView->setTag(200);
    m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("main/head_1.png"));
    this->getView()->addSubview(m_urlImageView);
    
    m_titleLabel = CALabel::createWithFrame(DRect(140, 20 + offY, _size.width / 2, 40));
    m_titleLabel->setColor(CAColor_black);
    m_titleLabel->setTextAlignment(CATextAlignmentLeft);
    m_titleLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_titleLabel->setFontSize(_px(32));
    m_titleLabel->setTag(201);
    this->getView()->addSubview(m_titleLabel);
    
    m_lectureLabel = CALabel::createWithFrame(DRect(140, 65 + offY, _size.width / 2, 40));
    m_lectureLabel->setColor(CAColor_gray);
    m_lectureLabel->setTextAlignment(CATextAlignmentLeft);
    m_lectureLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_lectureLabel->setFontSize(_px(26));
    m_lectureLabel->setTag(202);
    this->getView()->addSubview(m_lectureLabel);
    
    CAView* tmpView = CAView::create();
    CALabel* tLabel = CALabel::createWithFrame(DRect(140, 105 + offY, _size.width / 2, 40));
    tLabel->setColor(CAColor_gray);
    tLabel->setTextAlignment(CATextAlignmentLeft);
    tLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    tLabel->setFontSize(_px(24));
    tLabel->setTag(203);
    tLabel->setText("Track : ");
    tmpView->addSubview(tLabel);
    
    m_trackLabel = CALabel::createWithFrame(DRect(220, 103 + offY, _size.width / 2, 40));
    m_trackLabel->setColor(CAColor_gray);
    m_trackLabel->setTextAlignment(CATextAlignmentLeft);
    m_trackLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_trackLabel->setFontSize(_px(24));
    m_trackLabel->setTag(204);
    m_trackLabel->setFontName("fonts/arial.ttf");
    tmpView->addSubview(m_trackLabel);
    this->getView()->addSubview(tmpView);
    
    CAView* tmpFView = CAView::create();
    CALabel* fLabel = CALabel::createWithFrame(DRect(360, 105 + offY, _size.width / 2, 40));
    fLabel->setColor(CAColor_gray);
    fLabel->setTextAlignment(CATextAlignmentLeft);
    fLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    fLabel->setFontSize(_px(24));
    fLabel->setTag(205);
    fLabel->setText("Format : ");
    tmpFView->addSubview(fLabel);
    
    m_formatLabel = CALabel::createWithFrame(DRect(460, 103 + offY, _size.width / 2, 40));
    m_formatLabel->setColor(CAColor_gray);
    m_formatLabel->setTextAlignment(CATextAlignmentLeft);
    m_formatLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_formatLabel->setFontSize(_px(24));
    m_formatLabel->setTag(206);
    m_formatLabel->setFontName("fonts/arial.ttf");
    tmpFView->addSubview(m_formatLabel);
    this->getView()->addSubview(tmpFView);
    
    
    m_startTimeLabel = CALabel::createWithFrame(DRect(_size.width - 200, 20 + offY, 200, 40));
    m_startTimeLabel->setColor(CAColor_gray);
    m_startTimeLabel->setTextAlignment(CATextAlignmentRight);
    m_startTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_startTimeLabel->setFontSize(_px(24));
    m_startTimeLabel->setTag(207);
    this->getView()->addSubview(m_startTimeLabel);
    
    m_durationLabel = CALabel::createWithFrame(DRect(_size.width - 200, 65 + offY, 200, 40));
    m_durationLabel->setColor(CAColor_gray);
    m_durationLabel->setTextAlignment(CATextAlignmentRight);
    m_durationLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_durationLabel->setFontSize(_px(24));
    m_durationLabel->setTag(208);
    this->getView()->addSubview(m_durationLabel);
    
    m_likeNumLabel = CALabel::createWithFrame(DRect(_size.width / 2 + 50, 65 + offY, 200, 40));
    m_likeNumLabel->setColor(CAColor_gray);
    m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
    m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_likeNumLabel->setFontSize(_px(24));
    m_likeNumLabel->setTag(209);
    this->getView()->addSubview(m_likeNumLabel);
    
    CAImageView* likeImg = CAImageView::createWithImage(CAImage::create("common/like.png"));
    likeImg->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    likeImg->setFrame(DRect(_size.width / 2 + 15, 58 + offY, 30, 30));
    likeImg->setTag(210);
    this->getView()->addSubview(likeImg);
    
    DRect r(0, 0, 50, 50);
    m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/store.png"));
    m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    m_storeBtnImage->setFrame(r);
    CAButton* storeBtn = CAButton::createWithFrame(DRect(_size.width - 60, 100 + offY, 50, 50), CAButtonTypeRoundedRect);
    storeBtn->setAllowsSelected(true);
    storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
    storeBtn->addTarget(this, CAControl_selector(MainViewTableCell::storeBtnCallBack), CAControlEventTouchUpInSide);
    this->getView()->addSubview(storeBtn);
    
    
    m_titleLabel->setText(m_msg.m_title);
    m_lectureLabel->setText(m_msg.m_lecturer);
    m_trackLabel->setText(m_msg.m_track);
    m_formatLabel->setText(m_msg.m_format);
    m_startTimeLabel->setText(crossapp_format_string("%lld", m_msg.m_startTime));
    m_durationLabel->setText(crossapp_format_string("%d", m_msg.m_duration));
    m_likeNumLabel->setText(crossapp_format_string("%d", m_msg.m_likeNum));
    
    m_isStore = m_msg.m_stored;
    m_canStore = true;
    m_storeBtnImage->setImage(CAImage::create("common/store.png"));
    
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    m_urlImageView->setUrl(m_msg.m_imageUrl);
}

void SessionDetailViewController::requestMsg() // all(0) track(1) format(2) my_all(3)
{
	if (m_alertView) {
		this->getView()->removeSubview(m_alertView);
		m_alertView = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
    key_value["uid"] = crossapp_format_string("%d", RootWindow::getInstance()->getUserId());
    key_value["sid"] = crossapp_format_string("%d", m_sessionId);
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SessionDetailViewController::onRequestFinished));
}

void SessionDetailViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 100)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 200)
	{

	}
    else if (btn->getTag() == 400)
    {
        if (m_canStore)
        {
            std::map<std::string, std::string> key_value;
            key_value["sid"] = crossapp_format_string("%d", m_msg.m_sessionId);
            key_value["uid"] = crossapp_format_string("%d", RootWindow::getInstance()->getUserId());
            key_value["sto"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
            key_value["sign"] = getSign(key_value);
            CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));
            
            m_canStore = false;
        }
    }
	else if (btn->getTag() == 500)
	{
		requestMsg();
		if (m_pLoading == NULL)
		{
			m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, 50, 50));
			this->getView()->insertSubview(m_pLoading, CAWindowZOderTop);
			m_pLoading->setLoadingMinTime(0.5f);
		}
	}
}

void SessionDetailViewController::showAlert()
{
	if (m_alertView) {
		this->getView()->removeSubview(m_alertView);
		m_alertView = NULL;
	}

	m_alertView = CAView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	this->getView()->addSubview(m_alertView);

	CAImageView* bg = CAImageView::createWithFrame(DRect(0, 0, m_winSize.width, m_winSize.height - _px(200)));
	bg->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	bg->setImage(CAImage::create("common/bg.png"));

	CAButton* btn5 = CAButton::create(CAButtonTypeSquareRect);
	btn5->setTag(500);
	btn5->setCenter(DRect(m_winSize.width / 2, (m_winSize.height - _px(200)) / 2, m_winSize.width, m_winSize.height - _px(200)));
	btn5->setTitleColorForState(CAControlStateNormal, CAColor_white);
	btn5->setBackGroundViewForState(CAControlStateNormal, bg);
	btn5->setBackGroundViewForState(CAControlStateHighlighted, bg);
	btn5->addTarget(this, CAControl_selector(SessionDetailViewController::buttonCallBack), CAControlEventTouchUpInSide);
	m_alertView->addSubview(btn5);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(60)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	test->setFontSize(_px(40));
	test->setText("Network cannot connected !");
	m_alertView->addSubview(test);

}

void SessionDetailViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	if (status == HttpResponseSucceed)
	{
		const CSJson::Value& value = json["result"];

		m_msg.m_sessionId = value["sid"].asInt();
		m_msg.m_title = value["title"].asString();
		m_msg.m_lecturer = value["lct"].asString();
		m_msg.m_track = value["trk"].asString();
		m_msg.m_format = value["fmt"].asString();
		m_msg.m_startTime = value["trk"].asInt64();
		m_msg.m_duration = value["drt"].asInt();
		m_msg.m_likeNum = value["lkn"].asInt();
		m_msg.m_imageUrl = value["image"].asString();
        m_msg.m_stored = value["store"].asBool();
        
        initDetailView();
	}
	else
	{
		//showAlert(id);
	}

	{
		m_msg.m_sessionId = 200;
		m_msg.m_title = "Customer Success";
		m_msg.m_lecturer = "Lisa Chen";
		m_msg.m_track = "Customer";
		m_msg.m_format = "Dev Faire";
		cc_timeval ct;
		CCTime::gettimeofdayCrossApp(&ct, NULL);
		m_msg.m_startTime = ct.tv_sec + 3500;
		m_msg.m_likeNum = 20;
		m_msg.m_imageUrl = "http://www.qq745.com/uploads/allimg/141220/1-141220161Z4.jpg";
        m_msg.m_stored = true;
        initDetailView();
	}
    
    

	if (m_pLoading)
	{
		m_pLoading->stopAnimating();
		this->getView()->removeSubview(m_pLoading);
		m_pLoading = NULL;
	}
}

void SessionDetailViewController::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
    if (status == HttpResponseSucceed)
    {
        m_isStore = !m_isStore;
        if (m_isStore)
        {
            m_storeBtnImage->setImage(CAImage::create("common/store.png"));
        }
        else
        {
            m_storeBtnImage->setImage(CAImage::create("common/unstore.png"));
        }
    }
    m_canStore = true;
}


