//
//  str_lookup_include.cpp
//  lh_play_string
//
//  Created by Liu,Han(ARL) on 2017/8/31.
//  Copyright © 2017年 Liu,Han(ARL). All rights reserved.
//

#include "str_lookup_include.hpp"
#include <string>


bool CLookInclude::sortlookup(const char* ca, const char* cb)
{
    std::string stra(ca);
    std::string strb(cb);
    std::sort(stra.begin(), stra.end());
    std::sort(stra.begin(), stra.end());
    size_t sizea = stra.size();
    size_t sizeb = strb.size();
    int ia = 0;
    int ib = 0;
    bool ret = true;
    while(ib < sizeb)
    {
        while(stra[ia] < strb[ib] && ia < sizea)
        {
            ia++;
        }
        
        if(ia > sizea || stra[ia] > strb[ib])
        {
            ret = false;
            break;
        }
        
        ib++;
    }

    return ret;
    //O(mlogm) + O(nlogn);
}

bool CLookInclude::hashlookup(const char* ca, const char* cb)
{
    int hash = 0;
    std::string stra(ca);
    std::string strb(cb);
    for(std::string::iterator itera = stra.begin();
        itera != stra.end();
        itera++)
    {
        hash |= (1<<(*itera - 'A'));
    }
    
    bool ret = true;
    for(std::string::iterator iterb = strb.begin();
        iterb != strb.end();
        iterb++)
    {
        if (!(hash & (1<<(*iterb - 'A'))))
        {
            ret = false;
            break;
        }
    }
    
    return ret;
    //O(m+n)
}




