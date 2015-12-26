
#include "LoginViewController.h"
#include "CommonHttpManager.h"
#include "utils/HttpConnect.h"
#include "RootWindow.h"

LoginViewController::LoginViewController()
{

}

LoginViewController::~LoginViewController()
{

}

void LoginViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
	DSize winSize = this->getView()->getBounds().size;

    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("login/login_head.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	int iw = imageView->getImage()->getPixelsWide();
	int ih = imageView->getImage()->getPixelsHigh();

	imageView->setFrame(DRect((winSize.width - iw)*0.5, winSize.height*0.5 - ih, iw, ih));
    this->getView()->addSubview(imageView);
	
	imageView = CAImageView::createWithImage(CAImage::create("login/login_name.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	iw = imageView->getImage()->getPixelsWide();
	ih = imageView->getImage()->getPixelsHigh();
	imageView->setFrame(DRect(winSize.width * 0.5 - iw - _px(220), winSize.height * 0.7, iw, ih));
	this->getView()->addSubview(imageView);

	imageView = CAImageView::createWithImage(CAImage::create("login/login_passwd.png"));
	imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	imageView->setFrame(DRect(winSize.width * 0.5 - iw - _px(220), winSize.height * 0.7 + _px(100), iw, ih));
	this->getView()->addSubview(imageView);

	m_pAccount = CATextField::createWithFrame(DRect(winSize.width * 0.5 - _px(200), winSize.height * 0.7, _px(400), _px(80)));
	m_pAccount->setTextColor(CAColor_orange);

	m_pAccount->setPlaceHolder(UTF8("Account(email address)"));
	this->getView()->addSubview(m_pAccount);

	m_pPassword = CATextField::createWithFrame(DRect(winSize.width * 0.5 - _px(200), winSize.height * 0.7 + _px(100), _px(400), _px(80)));
	m_pPassword->setPlaceHolder(UTF8("password"));
	m_pPassword->setInputType(KEY_BOARD_INPUT_PASSWORD);
	this->getView()->addSubview(m_pPassword);

	CAButton* btn3 = CAButton::createWithFrame(DRect(winSize.width * 0.5 - _px(100), winSize.height * 0.7 + _px(200), _px(200), _px(80)), CAButtonTypeRoundedRect);
	btn3->setTitleForState(CAControlStateAll, UTF8("Login"));
	btn3->setTitleColorForState(CAControlStateAll, CAColor_orange);
	btn3->addTarget(this, CAControl_selector(LoginViewController::loginBtnCallBack), CAControlEventTouchUpInSide);
	this->getView()->addSubview(btn3);

    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void LoginViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void LoginViewController::loginBtnCallBack(CAControl* btn, DPoint point)
{
	//point = btn->convertToWorldSpace(point);
	//CAButton* button = (CAButton*)btn;

	string accout = m_pAccount->getText();
	string passwd = m_pPassword->getText();

	if (accout.length() == 0 || passwd.length() == 0) {
		CAAlertView *alertView = CAAlertView::createWithText("Waining !", "Account or password cannot be null !", "OK", NULL);
		alertView->show();
		return;
	}
    
    std::map<std::string,std::string> key_value;
    key_value["tag"] = loginTag;
    key_value["page"]= "1";
    key_value["limit"]= "20";
    key_value["appid"]="10000";
    key_value["sign_method"]="md5";
	key_value["sign"] = getSign(key_value);
    CommonHttpManager::getInstance()->send_get(httpUrl, key_value, this,
                                               CommonHttpJson_selector(LoginViewController::onRequestLoginFinished));
}

void LoginViewController::onRequestLoginFinished(const HttpResponseStatus& status, const CSJson::Value& json)
{
	RootWindow::getInstance()->setRootViewController(RootWindow::getInstance()->getRootNavigationController());
}