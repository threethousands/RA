#ifndef __RED_ALERT_H__
#define __RED_ALERT_H__
#include"RAObject\RABuilding.h"
#include"RAUtility.h"
#include"RAObject\RASoldier.h"
class RedAlert
{
public:
	static RedAlert* getInstance();
	void selectedSoldiersMove(Touch* touch);
	void initAll();
	bool onTouchBegan(Touch* touch, Event* event);
private:
	static RedAlert* instance_;
	RedAlert() {}
};


#endif // !__RED_ALERT_H__


