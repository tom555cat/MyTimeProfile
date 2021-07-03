//
//  TPCallTrace.h
//  MyTimeProfile
//
//  Created by tongleiming on 2021/7/1.
//

#ifndef TPCallTrace_h
#define TPCallTrace_h

#include <stdio.h>

//void hookObjcMsgSend(void);

void startTrace(char *featureName);
void stopTrace(void);

#endif /* TPCallTrace_h */
