
#include "LoginViewController.h"

LoginViewController::LoginViewController()
{

}

LoginViewController::~LoginViewController()
{

}

void LoginViewController::viewDidLoad()
{
    // Do any additional setup after loading the view from its nib.
	CCRect winRect = this->getView()->getBounds();

    CAImageView* imageView = CAImageView::createWithImage(CAImage::create("r/login_head.png"));
    imageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
	int iw = imageView->getImage()->getPixelsWide();
	int ih = imageView->getImage()->getPixelsHigh();

	imageView->setFrame(CCRect((winRect.size.width - iw)*0.5, winRect.size.height*0.5 - ih, iw, ih));
    this->getView()->addSubview(imageView);

    CALabel* label = CALabel::createWithCenter(CCRect(winRect.size.width*0.5, winRect.size.height*0.5-270, winRect.size.width, 200));
    label->setTextAlignment(CATextAlignmentCenter);
    label->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    label->setFontSize(_px(72));
    label->setText("Hello World!");
    label->setColor(CAColor_yellow);
    this->getView()->insertSubview(label, 1);
    
    CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}

void LoginViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}