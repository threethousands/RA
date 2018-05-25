#ifndef __RA_OBJECT__
#define __RA_OBJECT__

#include "cocos2d.h"
#include"RAPlayer.h"
#include"cocostudio\CocoStudio.h"
#include"ui\CocosGUI.h"

USING_NS_CC;
using namespace cocostudio;
using namespace cocos2d::ui;

class RAObject : public cocos2d::Sprite
{
public:
	RAObject(int hp,int power,int capital):
		hp_(hp), original_hp_(hp),power_cost_(power),capital_cost_(capital) {}
	~RAObject()override {}
	virtual bool initWithFile(const std::string& filename)override;
	virtual bool sufferAttack(int damage);//return survive or not
	virtual bool annihilation();
protected:
	const int power_cost_;
	const int capital_cost_;
	bool toBeOrNotToBe();
	int hp_;
	const int original_hp_;
};

template<typename T>
class RAConstructButton: public Node
{
public:
	RAConstructButton(Widget* UI, std::string& name) :
		button_((Button*)Helper::seekWidgetByName(UI, name)) {}
	static RAConstructButton* create(Widget* UI, std::string& name)
	{
		RAConstructButton* button = new RAConstructButton(UI,name);
		button->initButton();
		button->autorelease();
		return button;
	}
	void onTouchEnded(Touch* touch, Event* type)
	{
		button_->onTouchEnded(touch, type);
		auto point = touch->getLocation();
		auto object = T::create();
		object->setPosition(point);
		Director::getInstance()->getRunningScene()->getChildByTag(1)->addChild(object,object->kind());//0 means building,1 means soldier
	}
	bool onTouchBegan(Touch* touch, Event* event)
	{
		if (!button_->isBright())return false;
		return (button_->onTouchBegan(touch, event));
	}
	void initButton()
	{
		button_->setTouchEnabled(0);
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = CC_CALLBACK_2(RAConstructButton<T>::onTouchBegan, this);
		listener->onTouchEnded = CC_CALLBACK_2(RAConstructButton<T>::onTouchEnded, this);
		Director::getInstance()->getEventDispatcher()->
			addEventListenerWithSceneGraphPriority(listener, button_);
		//abandoned,now we use observer patter
		//schedule(schedule_selector(RAConstructButton::checkConstructable),0.05f);
		NotificationCenter::getInstance()->addObserver(this,
			callfuncO_selector(RAConstructButton<T>::checkConstructable),
			"RESOURCE_CHANGE",
			NULL);
	}
	//judge if capital and power can afford the object
	void checkConstructable(Ref* pSender)
	{
		if (button_->isBright())
		{
			if ((RAPlayer::getCapital() < T::capital_cost_) || (RAPlayer::getPower() < T::power_cost_))//���ܽ���
			{
				button_->setBright(false);
			}
		}
		else
		{
			if (!((RAPlayer::getCapital() < T::capital_cost_) || (RAPlayer::getPower() < T::power_cost_)))
				button_->setBright(true);
		}
	}
private:
	Button * button_;
};

namespace RA
{
	bool containsTouchLocation(Touch* touch, Event* event);
}

#endif // __RA_SOLDIER__
