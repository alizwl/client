
#include "MainViewTableCell.h"
#include "utils/HttpConnect.h"

MainViewTableCell::MainViewTableCell()
: m_titleLabel(NULL),
m_lectureLabel(NULL),
m_isStore(true),
m_urlImageView(NULL)
{
	this->setAllowsSelected(false);
}

MainViewTableCell::~MainViewTableCell()
{

}

MainViewTableCell* MainViewTableCell::create(const std::string& identifier, const DRect& _rect)
{
	MainViewTableCell* tableViewCell = new MainViewTableCell();
	if (tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
	{
		tableViewCell->setFrame(_rect);
		tableViewCell->autorelease();
		return tableViewCell;
	}
	CC_SAFE_DELETE(tableViewCell);
	return NULL;
}

void MainViewTableCell::highlightedTableViewCell()
{
	this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void MainViewTableCell::selectedTableViewCell()
{
	this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void MainViewTableCell::initWithCell()
{
	DSize _size = this->getFrame().size;
    
    m_urlImageView = CommonUrlImageView::createWithFrame(DRect(20, 20, 100, 100));
    m_urlImageView->setTag(200);
    m_urlImageView->setImageViewScaleType(CAImageViewScaleTypeFitImageCrop);
    m_urlImageView->setImage(CAImage::create("main/head_1.png"));
    this->getContentView()->addSubview(m_urlImageView);
    
	m_titleLabel = CALabel::createWithFrame(DRect(140, 20, _size.width / 2, 40));
	m_titleLabel->setColor(CAColor_black);
	m_titleLabel->setTextAlignment(CATextAlignmentLeft);
	m_titleLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	m_titleLabel->setFontSize(_px(32));
	m_titleLabel->setTag(201);
	this->getContentView()->addSubview(m_titleLabel);

	m_lectureLabel = CALabel::createWithFrame(DRect(140, 65, _size.width / 2, 40));
	m_lectureLabel->setColor(CAColor_gray);
	m_lectureLabel->setTextAlignment(CATextAlignmentLeft);
	m_lectureLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	m_lectureLabel->setFontSize(_px(26));
	m_lectureLabel->setTag(202);
	this->getContentView()->addSubview(m_lectureLabel);
    
    CAView* tmpView = CAView::create();
    CALabel* tLabel = CALabel::createWithFrame(DRect(140, 105, _size.width / 2, 40));
    tLabel->setColor(CAColor_gray);
    tLabel->setTextAlignment(CATextAlignmentLeft);
    tLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    tLabel->setFontSize(_px(24));
    tLabel->setTag(203);
    tLabel->setText("Track : ");
    tmpView->addSubview(tLabel);
    
    m_trackLabel = CALabel::createWithFrame(DRect(220, 103, _size.width / 2, 40));
    m_trackLabel->setColor(CAColor_gray);
    m_trackLabel->setTextAlignment(CATextAlignmentLeft);
    m_trackLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_trackLabel->setFontSize(_px(24));
    m_trackLabel->setTag(204);
    m_trackLabel->setFontName("fonts/arial.ttf");
    tmpView->addSubview(m_trackLabel);
    this->getContentView()->addSubview(tmpView);
    
    CAView* tmpFView = CAView::create();
    CALabel* fLabel = CALabel::createWithFrame(DRect(360, 105, _size.width / 2, 40));
    fLabel->setColor(CAColor_gray);
    fLabel->setTextAlignment(CATextAlignmentLeft);
    fLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    fLabel->setFontSize(_px(24));
    fLabel->setTag(205);
    fLabel->setText("Format : ");
    tmpFView->addSubview(fLabel);
    
    m_formatLabel = CALabel::createWithFrame(DRect(460, 103, _size.width / 2, 40));
    m_formatLabel->setColor(CAColor_gray);
    m_formatLabel->setTextAlignment(CATextAlignmentLeft);
    m_formatLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_formatLabel->setFontSize(_px(24));
    m_formatLabel->setTag(206);
    m_formatLabel->setFontName("fonts/arial.ttf");
    tmpFView->addSubview(m_formatLabel);
    this->getContentView()->addSubview(tmpFView);
    
    
    m_startTimeLabel = CALabel::createWithFrame(DRect(_size.width - 200, 20, 200, 40));
    m_startTimeLabel->setColor(CAColor_gray);
    m_startTimeLabel->setTextAlignment(CATextAlignmentRight);
    m_startTimeLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_startTimeLabel->setFontSize(_px(24));
    m_startTimeLabel->setTag(207);
    this->getContentView()->addSubview(m_startTimeLabel);
    
    m_durationLabel = CALabel::createWithFrame(DRect(_size.width - 200, 65, 200, 40));
    m_durationLabel->setColor(CAColor_gray);
    m_durationLabel->setTextAlignment(CATextAlignmentRight);
    m_durationLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_durationLabel->setFontSize(_px(24));
    m_durationLabel->setTag(208);
    this->getContentView()->addSubview(m_durationLabel);
    
    m_likeNumLabel = CALabel::createWithFrame(DRect(_size.width / 2 + 50, 65, 200, 40));
    m_likeNumLabel->setColor(CAColor_gray);
    m_likeNumLabel->setTextAlignment(CATextAlignmentLeft);
    m_likeNumLabel->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
    m_likeNumLabel->setFontSize(_px(24));
    m_likeNumLabel->setTag(209);
    this->getContentView()->addSubview(m_likeNumLabel);
    
    CAImageView* likeImg = CAImageView::createWithImage(CAImage::create("common/like.png"));
    likeImg->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    likeImg->setFrame(DRect(_size.width / 2 + 15, 55, 30, 30));
    likeImg->setTag(210);
    this->getContentView()->addSubview(likeImg);
    
    DRect r(0, 0, 50, 50);
    m_storeBtnImage = CAImageView::createWithImage(CAImage::create("common/store.png"));
    m_storeBtnImage->setImageViewScaleType(CAImageViewScaleTypeFitViewByHorizontal);
    m_storeBtnImage->setFrame(r);
    CAButton* storeBtn = CAButton::createWithFrame(DRect(_size.width - 60, 100, 50, 50), CAButtonTypeRoundedRect);
    storeBtn->setAllowsSelected(true);
    storeBtn->setBackGroundViewForState(CAControlStateAll, m_storeBtnImage);
    storeBtn->addTarget(this, CAControl_selector(MainViewTableCell::storeBtnCallBack), CAControlEventTouchUpInSide);
    this->getContentView()->addSubview(storeBtn);
}

void MainViewTableCell::setModel(const newsMsg &cellmodel)
{
    m_msgInfo = cellmodel;
    
    m_titleLabel->setText(cellmodel.m_title);
    m_lectureLabel->setText(cellmodel.m_lecturer);
    m_trackLabel->setText(cellmodel.m_track);
    m_formatLabel->setText(cellmodel.m_format);
    m_startTimeLabel->setText(crossapp_format_string("%lld", cellmodel.m_startTime));
    m_durationLabel->setText(crossapp_format_string("%d", cellmodel.m_duration));
    m_likeNumLabel->setText(crossapp_format_string("%d", cellmodel.m_likeNum));
    
    m_isStore = true;
    m_canStore = true;
    m_storeBtnImage->setImage(CAImage::create("common/store.png"));
    
    m_urlImageView->setImage(CAImage::create("common/bg.png"));
    m_urlImageView->setUrl(cellmodel.m_imageUrl);
}

void MainViewTableCell::storeBtnCallBack(CAControl* btn, DPoint point)
{
    if (m_canStore)
    {
        std::map<std::string, std::string> key_value;
        key_value["sid"] = crossapp_format_string("%d", m_msgInfo.m_sessionId);
        key_value["uid"] = crossapp_format_string("%d", RootWindow::getInstance()->getUserId());
        key_value["sto"] = crossapp_format_string("%d", m_isStore ? 1 : 0);
        key_value["sign"] = getSign(key_value);
        CommonHttpManager::getInstance()->send_post(httpUrl, key_value, this, CommonHttpJson_selector(MainViewTableCell::onStoreRequestFinished));
        
        m_canStore = false;
    }
}

void MainViewTableCell::onStoreRequestFinished(const HttpResponseStatus& status, const CSJson::Value& json)
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