#include "precompiled_header.h"
#include "CAnimationManager.h"
#include "CAnimation.h"
#include "CFrame.h"
#include "..\TinyXML\tinyxml.h"

CAnimationManager* CAnimationManager::instance = NULL;

CAnimationManager::CAnimationManager()
{

}
CAnimationManager::CAnimationManager(const CAnimationManager* c)
{

}
CAnimationManager::~CAnimationManager()
{
	m_vAnims.clear();
	m_vAnimPool.clear();
}

ostream& operator<<(ostream& os, const CAnimationManager& manager)
{
	for(int i = 0; i < manager.GetListSize(); i++)
	{
		CAnimation* anim = manager.GetListAnimation(i);
		if(anim == NULL)
			break;

		os << "Animation " << i << ": " << endl;
		os << *anim;
	}
	return os;
}

bool CAnimationManager::LoadBinary(std::string szFilename)
{
	ifstream in;
	in.open(szFilename, ios_base::in | ios_base::binary);
	if(!in.is_open())
		return false;

	//file is open
	if(in.good())
	{
		int nAnimCount, nFrameCount;
		std::vector<CAnimation*> list;
		CAnimation* anim;
		CFrame* frame;
		//Animation Count
		in.read((char *)&nAnimCount, sizeof(int));
		for(int i = 0; i < nAnimCount; i++)
		{
			anim = new CAnimation();
			//Frame Count
			in.read((char *)&nFrameCount, sizeof(int));
			for(int j = 0; j < nFrameCount; j++)
			{
			    //Anchor Point
				int nAPointX, nAPointY;
                in.read((char *)&nAPointX, sizeof(int));
                in.read((char *)&nAPointY, sizeof(int));
                //Draw Rect
				int nLeft, nTop, nRight, nBottom;
                in.read((char *)&nLeft, sizeof(int));
				in.read((char *)&nTop, sizeof(int));
				in.read((char *)&nRight, sizeof(int));
				in.read((char *)&nBottom, sizeof(int));
                //Collision Rect
                int nCLeft, nCTop, nCRight, nCBottom;
                in.read((char *)&nCLeft, sizeof(int));
				in.read((char *)&nCTop, sizeof(int));
				in.read((char *)&nCRight, sizeof(int));
				in.read((char *)&nCBottom, sizeof(int));
                //Duration
				float fDuration;
                in.read((char *)&fDuration, sizeof(float));
                //Trigger
				int nTrigLength;
				std::string szTriggerID;
                in.read((char *)&nTrigLength, sizeof(int));
                for (int h = 0; h < nTrigLength; h++) 
                {
					char szletter;
                    in.read((char *)&szletter, sizeof(char));
					szTriggerID.push_back(szletter);
                }
				frame = new CFrame(fDuration, szTriggerID, nTop, nLeft, nBottom, nRight);
				anim->AddFrame(frame);
			}//end frame loop
			//Path & Filename
			int nPathFileLength;
			std::string szPathFilename;
            in.read((char *)&nPathFileLength, sizeof(int));
            for (int n = 0; n < nPathFileLength; n++)
            {
                char szletter;
                in.read((char *)&szletter, sizeof(char));
			    szPathFilename.push_back(szletter);
            }
            //Filename 
            int nFileLength;
			std::string szFilename;
            in.read((char *)&nFileLength, sizeof(int));
            for (int k = 0; k < nFileLength; k++)
            {
                char szletter;
                in.read((char *)&szletter, sizeof(char));
			    szFilename.push_back(szletter);
            }
            //Name
            int nNameLength;
			std::string szName;
            in.read((char *)&nNameLength, sizeof(int));
            for (int k = 0; k < nNameLength; k++)
            {
                char szletter;
                in.read((char *)&szletter, sizeof(char));
			    szName.push_back(szletter);
            }
			anim->SetName(szName);
            //ImageID
			int nImageID; //-useless
            in.read((char *)&nImageID, sizeof(int));
            //Speed
			float fSpeed;
            in.read((char *)&fSpeed, sizeof(float));
			anim->SetSpeed(fSpeed);
            //Looping
			bool bIsLooping;
            in.read((char *)&bIsLooping, sizeof(bool));
			anim->SetLooping(bIsLooping);
            //Reversed
            bool bIsReversed;
            in.read((char *)&bIsReversed, sizeof(bool));
			anim->SetReversed(bIsReversed);
			list.push_back(anim);
		}//end animation loop
		//add them to the main list - in reverse order
		for(int size = (list.size() - 1); size >= 0; size--)
		{
			AddAnimation(list[size]);
		}
		
		in.close();
		return true;
	}
	return false;
}
bool CAnimationManager::LoadXML(std::string szFilename)
{
	TiXmlDocument doc;
	if( doc.LoadFile(szFilename.c_str()) == false)
		return false;

	TiXmlElement* pRoot = doc.RootElement();
	if(!pRoot)
		return false;

	TiXmlElement* pAnims = pRoot->FirstChildElement("CAnimation");
	TiXmlElement* pFrames, *pData, *pDataChild;
	std::vector<CAnimation*> list;
	CAnimation* anim;
	CFrame* frame;
	bool isSingle = false;
	//Check to see if the file has many animations
	if(!pAnims)
	{
		//The file must contain only 1 animation
		//problem is here
		pAnims = pRoot;
		isSingle = true;
	}

	if(pAnims)
	{
		//we have the CAnimation Node
		while(pAnims)
		{
			anim = new CAnimation();
			//list of frames
			pFrames = pAnims->FirstChildElement("frames");
			if(pFrames)
			{
				//CFrame Node
				pFrames = pFrames->FirstChildElement("CFrame");
				while(pFrames)
				{
					std::string x, y;
					std::string width, height;
					std::string dur;
					int nx, ny, nwidth, nheight;
					float fdur;
					pData = pFrames;
					//Anchor Point
					//pData = pFrames->FirstChildElement("PAnchor");
					//Draw Rect
					pDataChild = pData->FirstChildElement("RDrawRect");
					pDataChild = pDataChild->FirstChildElement("X");
					x = pDataChild->GetText();
					nx = atoi(x.c_str()); 
					pDataChild = pData->FirstChildElement("RDrawRect");
					pDataChild = pDataChild->FirstChildElement("Y");
					y = pDataChild->GetText();
					ny = atoi(y.c_str()); 
					pDataChild = pData->FirstChildElement("RDrawRect");
					pDataChild = pDataChild->FirstChildElement("Width");
					nwidth = atoi(width.c_str());
					pDataChild = pData->FirstChildElement("RDrawRect");
					pDataChild = pDataChild->FirstChildElement("Height");
					height = pDataChild->GetText();
					nheight = atoi(height.c_str());
					//Collision Rect
					//pDataChild = pData->FirstChildElement("RCollisionRect");
					//Duration
					pData = pFrames;
					pDataChild = pData->FirstChildElement("FDuration");
					dur = pDataChild->GetText();
					fdur = (float)atof(dur.c_str()); 
					//TriggerID
					std::string name;
					pDataChild = pData->FirstChildElement("SzTriggerID");
					if(pDataChild->GetText())
						name = pDataChild->GetText();
					//Add Frame
					frame = new CFrame(fdur, name, nx, ny, (ny + nheight), (nx + nwidth));
					anim->AddFrame(frame);
					pFrames = pFrames->NextSiblingElement("CFrame");
				}//end CFrame	
			}//end list of frames
			std::string speed;
			float fspeed;
			pData = pAnims;
			//ImageID
			//pAnims->FirstAttribute();
			//Animation name
			//pData->FirstChildElement("SzName")->GetText();
			std::string name = pData->FirstChildElement("SzName")->GetText();
			anim->SetName(name);
			//Filename
			//pData->NextSibling("SzFilename");
			//Filename&Path
			//pData->NextSibling("SzFilenameWithPath");
			//Speed
			speed = pAnims->FirstChildElement("FSpeed")->GetText();
			fspeed = (float)atof(speed.c_str());
			anim->SetSpeed(fspeed);
			//IsPlaying

			//IsLooping
			std::string text;
			text = pAnims->FirstChildElement("BIsLooping")->GetText();
			if(text == "true")
				anim->SetLooping(true);
			else if(text == "false")
				anim->SetLooping(false);

			//IsReversed
			text = pAnims->FirstChildElement("BIsReversed")->GetText();
			if(text == "true")
				anim->SetReversed(true);
			else if(text == "false")
				anim->SetReversed(false);

			list.push_back(anim);
			//to next Animation in file
			pAnims = pAnims->NextSiblingElement("CAnimation");

		}//End CAnimation
		//add them to the main list - in reverse order
		for(int size = (list.size() - 1); size >= 0; size--)
		{
			AddAnimation(list[size]);
		}
		return true;
	}

	return false;
}
bool CAnimationManager::LoadAnimation(std::string szFilename, bool bIsXML)
{
	if(bIsXML)
		return LoadXML(szFilename);
	else
		return LoadBinary(szFilename);
}
void CAnimationManager::AddAnimation(const CAnimation* anim)
{
	m_vAnims.push_back((CAnimation*)anim);
}
void CAnimationManager::AddPoolAnimation(const CAnimation* anim)
{
	m_vAnimPool.push_back((CAnimation*)anim);
}

CAnimationManager* CAnimationManager::GetInstance()
{
	if(!instance)
		instance = new CAnimationManager;

	return instance;
}
void CAnimationManager::DeleteInstance()
{
	if(instance != NULL)
	{
		delete instance;
		instance = NULL;
	}
}

bool CAnimationManager::UpdateAnimation(float fDelta, int nAnimID)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->Update(fDelta);
		return true;
	}
	return false;
}
bool CAnimationManager::Draw(int nAnimID, int nPosX, int nPosY)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->Draw(nPosX, nPosY);
		return true;
	}
	return false;
}

bool CAnimationManager::PlayAnimation(int nAnimID)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->Play();
		return true;
	}
	return false;
}
bool CAnimationManager::StopAnimation(int nAnimID)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->Stop();
		return true;
	}
	return false;
}
bool CAnimationManager::StopAnimationAtFrame(int nAnimIndex, int nFrameIndex)
{
	CAnimation* anim = GetAnimation(nAnimIndex);
	if(anim)
	{
		anim->StopAtFrame(nFrameIndex);
		return true;
	}
	return false;
}
bool CAnimationManager::ResumeAnimation(int nAnimID)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->Resume();
		return true;
	}
	return false;
}

int CAnimationManager::GetListSize() const
{
	return m_vAnims.size();
}
int CAnimationManager::GetPoolSize() const
{
	return m_vAnimPool.size();
}
int CAnimationManager::GetID(std::string szName) const
{
	//Find the animation by name
	unsigned int i;
	for(i = 0; i < m_vAnims.size(); i++)
	{
		if(m_vAnims[i]->GetName() == szName)
			 break;//return i;
	}

	//leave if we dont find it
	if(i >= m_vAnims.size())
		return -1;

	//the size before we add a new copy is the id
	int nID = m_vAnimPool.size();
	CAnimation* copy = new CAnimation(m_vAnims[i]);
	((CAnimationManager*)(this))->AddPoolAnimation(copy);
	return nID;
}
bool CAnimationManager::IsAnimationPlaying(int nAnimID) const
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
		return anim->IsPlaying();
	else
		return false;
}
bool CAnimationManager::SetAnimTexture(const int nAnimID, const int nImageID)
{
	CAnimation* anim = GetAnimation(nAnimID);
	if(anim)
	{
		anim->SetImageID(nImageID);
		return true;
	}
	return false;
}
CAnimation* CAnimationManager::GetAnimation(int nAnimID) const
{
	if(nAnimID < 0 || nAnimID >= (int)m_vAnimPool.size())
		return NULL;

	return m_vAnimPool[nAnimID];
}
CAnimation* CAnimationManager::GetAnimation(std::string szName) const
{
	//Find the animation by name
	for(unsigned int i = 0; i < m_vAnims.size(); i++)
	{
		if(m_vAnims[i]->GetName() == szName)
			 return m_vAnims[i];
	}
	return NULL;
}
CAnimation* CAnimationManager::GetListAnimation(int nAnimID) const
{
	if(nAnimID < 0 || nAnimID >= (int)m_vAnims.size())
		return NULL;

	return m_vAnims[nAnimID];
}
