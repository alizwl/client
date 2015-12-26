

#include "RootWindow.h"
#include "LoginViewController.h"
#include "CDNewsAboutController.h"
#include "MainViewController.h"

static RootWindow* _window = NULL;

RootWindow* RootWindow::getInstance()
{
    if (_window == NULL)
    {
        _window = new RootWindow();
        _window->init();
        _window->autorelease();
    }
    return _window;
}

RootWindow::RootWindow()
: m_pRootNavigationController(NULL)
{
    CAApplication::getApplication()->getKeypadDispatcher()->addDelegate(this);
}

RootWindow::~RootWindow()
{
	CAApplication::getApplication()->getKeypadDispatcher()->removeDelegate(this);
}

bool RootWindow::init()
{
    if (!CAWindow::init())
    {
        return false;
    }
    CAApplication::getApplication()->setNotificationView(CAView::createWithFrame(this->getBounds(), CAColor_green));

	CAViewController* viewController = m_pRootNavigationController ? m_pRootNavigationController->getViewControllerAtIndex(0) : NULL;

	MainViewController* mainview = new MainViewController();
	mainview->init();
	mainview->autorelease();
	//this->setRootViewController(mainview);

	m_pRootNavigationController = new CANavigationController();
	m_pRootNavigationController->initWithRootViewController(mainview);
	m_pRootNavigationController->setNavigationBarBackGroundImage(CAImage::create("common/navbg.jpg"));
	m_pRootNavigationController->setNavigationBarHidden(true, false);

	if (1)
	{
		LoginViewController* _viewController = new LoginViewController();
		_viewController->init();
		this->setRootViewController(_viewController);
		_viewController->release();
	}
	else
	{
		this->setRootViewController(m_pRootNavigationController);
	}

	CAApplication::getApplication()->setNotificationView(NULL);
    
    return true;
}

void RootWindow::keyBackClicked()
{
	CC_RETURN_IF(CAAlertView::hideWithDisplayed());

	if (this->getModalViewController())
	{
		this->dismissModalViewController(true);
	}
	else if (this->getDrawerController()->isShowLeftViewController())
	{
		this->getDrawerController()->hideLeftViewController(true);
	}
	else if (this->getRootNavigationController()->getViewControllerCount() > 1)
	{
		this->getRootNavigationController()->popViewControllerAnimated(true);
	}
	else{
		CAApplication::getApplication()->end();
	}
}