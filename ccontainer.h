#ifndef CCONTAINER_H_INCLUDED
#define CCONTAINER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

template <typename CElement>
class CContainerList
{
 protected:
  CElement *cElement_List;//�������� ����������
  CElement *cElement_ListEnd;//�������� ����������
 public:
  CContainerList(void);//�����������
  ~CContainerList();//����������
  void Release(void);//���������� ������
  CElement* AddElement(CElement cElement);//�������� �������
  void DeleteElement(CElement *cElement);//������� �������
  CElement* GetFirstElement(void);//�������� ������ �������
};


//----------------------------------------------------------------------------------------------------
template <typename CElement>
CContainerList<CElement>::CContainerList(void)
{
 cElement_List=NULL;
 cElement_ListEnd=NULL;
}
//----------------------------------------------------------------------------------------------------
template <typename CElement>
CContainerList<CElement>::~CContainerList()
{
 Release();
}
//----------------------------------------------------------------------------------------------------
template <typename CElement>
void CContainerList<CElement>::Release(void)
{
 CElement *cElementPtr=cElement_List;
 while(cElementPtr!=NULL)
 {
  CElement *cElementPtrOld=cElementPtr;
  cElementPtr=cElementPtr->Next;
  delete(cElementPtrOld);
 }
 cElement_List=NULL;
 cElement_ListEnd=NULL;
}
template <typename CElement>
CElement* CContainerList<CElement>::AddElement(CElement cElement)
{
 CElement *cElementPtr;
 if (cElement_List==NULL)
 {
  cElementPtr=new CElement;
  *cElementPtr=cElement;
  cElementPtr->Next=NULL;
  cElementPtr->Prior=NULL;
  cElement_List=cElementPtr;
  cElement_ListEnd=cElement_List;
  return(cElementPtr);
 }
 else
 {
  cElementPtr=cElement_ListEnd;
  cElementPtr->Next=new CElement;
  *cElementPtr->Next=cElement;
  cElementPtr->Next->Prior=cElementPtr;
  cElementPtr->Next->Next=NULL;
  cElement_ListEnd=cElementPtr->Next;
  return(cElementPtr->Next);
 }
}
template <typename CElement>
void CContainerList<CElement>::DeleteElement(CElement *cElement)
{
 CElement *cElementPtr;
 cElementPtr=cElement_List;
 while(cElementPtr!=NULL)
 {
  if (cElementPtr==cElement) break;
  cElementPtr=cElementPtr->Next;
 }
 if (cElementPtr==NULL) return;
 CElement *cElementPtr_Prior=cElementPtr->Prior;
 CElement *cElementPtr_Next=cElementPtr->Next;
 if (cElementPtr->Prior!=NULL) cElementPtr->Prior->Next=cElementPtr_Next;
 if (cElementPtr->Next!=NULL) cElementPtr->Next->Prior=cElementPtr_Prior;
 if (cElementPtr==cElement_List) cElement_List=cElementPtr_Next;
 if (cElementPtr==cElement_ListEnd) cElement_ListEnd=cElementPtr_Prior;
 delete(cElementPtr);
}
template <typename CElement>
CElement* CContainerList<CElement>::GetFirstElement(void)
{
 return(cElement_List);
}
#endif
