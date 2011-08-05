#ifndef CBUTTON_H_
#define CBUTTON_H_

#include "../CBaseObject.h"
#include "../CBaseEntity.h"


#include <string>
using std::string;

class CButton : public CBaseObject
{
private:
	bool m_bIsPressed;
	string m_nLink;

public:
	CButton(string nLink = "-5234");
	~CButton(void);
	
	bool CheckCollision(IUnitInterface* pBase);
	void Update(float fDT);

};



#endif