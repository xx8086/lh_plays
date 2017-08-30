//
//  head_to_tail.hpp
//  lh_play_string
//
//  Created by Liu,Han(ARL) on 2017/8/30.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#ifndef head_to_tail_hpp
#define head_to_tail_hpp

//将字符串的前n个字符移动到末尾，"abcdefg"前三个字符移到尾得"defgabc"
#include <stdio.h>
class CHeadToRail
{
public:
    bool movetail(char*, int, int);
    bool reversal(char*, int, int);
private:
    bool parameter_check(char*, int, int);
    void do_move_one(char*, int);
    void do_reversal(char*, int);
    
};

#endif /* head_to_tail_hpp */
