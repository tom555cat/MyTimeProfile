//
//  hookObjcMsgSend-arm64.s
//  MyTimeProfile
//
//  Created by tongleiming on 2021/6/28.
//

#ifdef __arm64__
#include <arm/arch.h>

/// 这部分以后再研究
.macro ENTRY /* name */
    .text
    .align 5
    .private_extern    $0
$0:
.endmacro

.macro END_ENTRY /* name */
LExit$0:
.endmacro

.macro COPY_STACK_FRAME
    

.endmacro COPY_STACK_FRAME

ENTRY _hook_msgSend
END_ENTRY _hook_msgSend

#endif
