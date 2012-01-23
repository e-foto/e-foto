#include "CommonMethods.h"
#include "CommonCVMethods.h"
#include "CommonQtMethods.h"
//#include "CommonSDLMethods.h"

CommonMethods* CommonMethods::commonInst = 0;

CM::ImplementationType CommonMethods::impType = CM::None;

CommonMethods* CommonMethods::instance(CM::ImplementationType type)
{
	if (type == CM::CVMethods)
	{
		if (!commonInst || (impType != CM::CVMethods && destroyInstance()))
		{
			commonInst = new CommonCVMethods();
		}
		impType = CM::CVMethods;
		return commonInst;
	}
	else if (type == CM::QtMethods)
	{
		if (!commonInst || (impType != CM::QtMethods && destroyInstance()))
		{
			commonInst = new CommonQtMethods();
		}
		impType = CM::QtMethods;
		return commonInst;
	}
	/*else if (type == CM::SDLMethods)
 {
  if (!commonInst || (impType != CM::SDLMethods && destroyInstance()))
  {
   commonInst = new CommonSDLMethods();
  }
  impType = CM::SDLMethods;
  return commonInst;
 }*/
	return NULL;
}

CommonMethods::~CommonMethods()
{
	if (impType != CM::None)
		destroyInstance();
}

bool CommonMethods::destroyInstance()
{
	if (commonInst)
	{
		if (impType == CM::CVMethods)
		{
			CommonCVMethods* aux = (CommonCVMethods*)commonInst;
			impType = CM::None;
			delete(aux);
		}
		else if (impType == CM::QtMethods)
		{
			CommonQtMethods* aux = (CommonQtMethods*)commonInst;
			impType = CM::None;
			delete(aux);
		}
		/*else if (impType == CM::SDLMethods)
  {
   CommonSDLMethods* aux = (CommonSDLMethods*)commonInst;
   impType = CM::None;
   delete(aux);
  }*/
		commonInst = 0;
		return true;
	}
	return false;
}
