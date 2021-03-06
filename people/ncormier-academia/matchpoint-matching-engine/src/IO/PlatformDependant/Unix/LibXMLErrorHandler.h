/******************************************************************************
******************************************************************************
* Copyright (c) 2007 MatchPoint, All rights reserved
*
* File    :    LibXMLErrorHandler.h
* Desc    :    XML Error Handler Unix header
* Author  :    mat
******************************************************************************/

#ifndef __LIBXMLERRORHANDLER_H__
# define __LIBXMLERRORHANDLER_H__

/*****************************************************************************/
/* Header files                                                              */
/*****************************************************************************/
# include "XMLErrorHandler.h"
/*****************************************************************************/
/* Default namespace                                                         */
/*****************************************************************************/
# include "DefaultNamespace.h"

DNSPACE_OPEN
/*****************************************************************************/
/* Forward definitions                                                       */
/*****************************************************************************/
/*****************************************************************************/
/* Types Definition                                                          */
/*****************************************************************************/

class LibXMLErrorHandler : public XMLErrorHandler
{
  /***************************************************************************/
  /* Methods                                                                 */
  /***************************************************************************/
protected:
public:
  LibXMLErrorHandler();
  virtual ~LibXMLErrorHandler(void);
private:
  /***************************************************************************/
  /* Properties                                                              */
  /***************************************************************************/
protected:
private:
};
/*****************************************************************************/
/* Default namespace close                                                   */
/*****************************************************************************/
DNSPACE_CLOSE

#endif /* __LIBXMLERRORHANDLER_H__ */
