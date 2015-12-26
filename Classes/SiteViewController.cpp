
#include "SiteViewController.h"
#include "MainViewTableCell.h"
#include "RootWindow.h"
#include "LoginViewController.h"
#include "utils/HttpConnect.h"

SiteViewController::SiteViewController()
{
	m_alertView = NULL;
	m_pLoading = NULL;
}

SiteViewController::~SiteViewController()
{

}

void SiteViewController::viewDidLoad()
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
	btn->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	CALabel* mc = CALabel::createWithFrame(DRect(_px(120), _px(62), _px(200), _px(50)));
	mc->setFontSize(_px(40));
	mc->setText("Site");
	mc->setColor(CAColor_white);
	mc->setFontName("fonts/arial.ttf");
	this->getView()->addSubview(mc);

	btn = CAButton::createWithFrame(DRect(m_winSize.width - 100, _px(130), _px(50), _px(50)), CAButtonTypeRoundedRect);
	imageView = CAImageView::createWithImage(CAImage::create("session/share_btn.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
	btn->setTag(200);
	btn->setBackGroundViewForState(CAControlStateAll, imageView);
	btn->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn);

	requestMsg();
	m_pLoading = CAActivityIndicatorView::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height / 2, 50, 50));
	m_pLoading->setLoadingMinTime(0.5f);
	this->getView()->addSubview(m_pLoading);

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void SiteViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void SiteViewController::requestMsg() // all(0) track(1) format(2) my_all(3)
{
	if (m_alertView) {
		this->getView()->removeSubview(m_alertView);
		m_alertView = NULL;
	}
	std::map<std::string, std::string> key_value;
	key_value["tag"] = mainViewTag[0];
	key_value["type"] = "0";
	key_value["section"] = "0";
	key_value["limit"] = "20";
	key_value["appid"] = "10000";
	key_value["sign_method"] = "md5";
	key_value["sign"] = getSign(key_value);
	CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(SiteViewController::onRequestFinished));
}

void SiteViewController::buttonCallBack(CAControl* btn, DPoint point)
{
	if (btn->getTag() == 100)
	{
		RootWindow::getInstance()->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else if (btn->getTag() == 200)
	{

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

void SiteViewController::showAlert()
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
	btn5->addTarget(this, CAControl_selector(SiteViewController::buttonCallBack), CAControlEventTouchUpInSide);
	m_alertView->addSubview(btn5);

	CALabel* test = CALabel::createWithCenter(DRect(m_winSize.width / 2, m_winSize.height - _px(400), m_winSize.width, _px(60)));
	test->setColor(CAColor_gray);
	test->setTextAlignment(CATextAlignmentCenter);
	test->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	test->setFontSize(_px(40));
	test->setText("Network cannot connected !");
	m_alertView->addSubview(test);

}

void SiteViewController::onRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
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
	}

	if (m_pLoading)
	{
		m_pLoading->stopAnimating();
		this->getView()->removeSubview(m_pLoading);
		m_pLoading = NULL;
	}
}


